using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;
using System.Threading;

using Routrek.Crypto;
using Routrek.SSHC;
using Routrek.SSHCV2;
using Routrek.Toolkit;
using Routrek.PKI;

using Pipestream;
using System.IO;
using Granados.Extensions;
using System.Net.Sockets;

namespace Granados
{
	#region SSH enums
	public enum SSH_FXP : byte
	{
		INIT = 1,
		VERSION = 2,
		OPEN = 3,
		CLOSE = 4,
		READ = 5,
		WRITE = 6,
		LSTAT = 7,
		FSTAT = 8,
		SETSTAT = 9,
		FSETSTAT = 10,
		OPENDIR = 11,
		READDIR = 12,
		REMOVE = 13,
		MKDIR = 14,
		RMDIR = 15,
		REALPATH = 16,
		STAT = 17,
		RENAME = 18,
		READLINK = 19,
		SYMLINK = 20,
		STATUS = 101,
		HANDLE = 102,
		DATA = 103,
		NAME = 104,
		ATTRS = 105,
		EXTENDED = 200,
		EXTENDED_REPLY = 201,
	}

	public enum SSH_FXF
	{
		READ = 0x00000001,
		WRITE = 0x00000002,
		APPEND = 0x00000004,
		CREAT = 0x00000008,
		TRUNC = 0x00000010,
		EXCL = 0x00000020,
	}

	public enum SSH_FILEXFER : uint
	{
		ATTR_SIZE = 0x00000001,
		ATTR_UIDGID = 0x00000002,
		ATTR_PERMISSIONS = 0x00000004,
		ATTR_ACMODTIME = 0x00000008,
		ATTR_EXTENDED = 0x80000000,
	}

	public enum SSH_FX
	{
		OK = 0,
		EOF = 1,
		NO_SUCH_FILE = 2,
		PERMISSION_DENIED = 3,
		FAILURE = 4,
		BAD_MESSAGE = 5,
		NO_CONNECTION = 6,
		CONNECTION_LOST = 7,
		OP_UNSUPPORTED = 8,
	}

	public enum SFTPTransferMode
	{
		OVERWRITE = 0,
		RESUME = 1,
		APPEND = 2,
	}

	#endregion

	#region Extra classes
	internal class Header
	{
		public int length;
		public int type;
		public int rid;
	}

	public abstract class SftpProgressMonitor
	{
		public static int PUT = 0;
		public static int GET = 1;
		public abstract void init(int op, String src, String dest, long max);
		public abstract bool count(long count);
		public abstract void end();
	}
	#endregion

	public class SFTPConnection : ISSHConnectionEventReceiver, ISSHChannelEventReceiver
	{
		#region fields
		private SSH2Connection m_conn;
		
		private SSHChannel m_channel;

		private bool m_errorEstablishingConnection;
		private bool m_ready;
		private bool m_initializedSuccessfully;

		private PipeStream m_pipe;

		private static int MAX_MSG_LENGTH = 256 * 1024;

		private Thread m_thread;

		private string file_separator = Path.PathSeparator.ToString();
		private char file_separatorc = Path.PathSeparator;

		private Buffer buf;

		private int[] ackid = new int[1];

		internal int seq = 1;
		private int server_version;
		private string home, cwd, lcwd;

		private SFTPTransferMode mode;
		
		#endregion

		#region properties

		public string Lcwd
		{
			get { return lcwd; }
		}
		public string Cwd
		{
			get { return cwd; }
		}
		public string Home
		{
			get { return home; }
		}
		
		public bool Ready
		{
			get { return m_ready; }
			set
			{
				m_ready = value;
			}
		}

		public bool InitializedSuccessfully
		{
			get { return m_initializedSuccessfully; }
			set { m_initializedSuccessfully = value; }
		}

		public bool IsConnected
		{
			get
			{
				if(m_conn != null)
				{
					return !m_conn.IsClosed;
				}

				return false;
			}
		}

		public void SetConnection(SSH2Connection conn)
		{
			m_conn = conn;
		}

		public Granados.SFTPTransferMode TransferMode
		{
			get { return mode; }
			set { mode = value; }
		}

		#endregion

		#region Constructor / startup

		public SFTPConnection()
		{
			m_pipe = new PipeStream();
			buf = new Buffer(32768);
			m_thread = new Thread(DoSFTPInit);
			Ready = false;

			TransferMode = SFTPTransferMode.RESUME;
		}

		public void Connect(string host, SSHConnectionParameter parameters)
		{
			Socket s = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
			s.Blocking = false;
			s.Connect(host, 22);

			parameters.Protocol = SSHProtocol.SSH2;
			parameters.AuthenticationType = AuthenticationType.Password;

			DateTime connectionStartTime = DateTime.Now;
			DateTime maxWaitTime = connectionStartTime.AddSeconds(10);

			m_conn = (SSH2Connection)SSHConnection.Connect(parameters, this, s);

			StartSFTPSession();
			
			while(!InitializedSuccessfully)
			{
				Thread.Sleep(100);

				if(DateTime.Now.CompareTo(maxWaitTime) > 0)
				{
					m_conn.Close();
				}
			}

		}
		

		public void StartSFTPSession()
		{
			m_thread.Start();
		}

		private void DoSFTPInit()
		{
			if(m_conn == null || m_conn.IsClosed)
			{
				InitializedSuccessfully = false;
			}

			m_errorEstablishingConnection = false;
			Ready = false;

			m_channel = m_conn.EstablishSFTP(this);

			while(!Ready)
			{
				Thread.Sleep(50);
			}

			if(m_errorEstablishingConnection)
			{
				InitializedSuccessfully = false;
				return;
			}

			sendINIT();

			Header header = ReadHeader();
			int length = header.length;

			if(length > MAX_MSG_LENGTH)
			{
				string message = String.Format("ERROR! Type = {0}.  Reason: Received message is too long ({1}",
												SSH_FX.FAILURE.ToString(), length);
				throw new Exception(message);
			}
			int type = header.type;             // 2 -> SSH_FXP_VERSION
			int server_version = header.rid;

			skip(length);

			sendREALPATH(Encoding.ASCII.GetBytes("."));

			header = ReadHeader();

			length = header.length;
			type = header.type;					// 104 -> SSH_FXP_NAME

			byte[] buffer = new byte[1024];

			fill(buffer, 0, length);

			SSH2DataReader reader = new SSH2DataReader(buffer);
			int count = reader.ReadInt32();

			home = Encoding.ASCII.GetString(reader.ReadString());
			cwd = home;

			lcwd = Path.GetFullPath(".");

			InitializedSuccessfully = true;
		}

		#endregion

		#region ISSHConnectionEventReceiver Members

		public void OnDebugMessage(bool always_display, byte[] msg)
		{
			Debug.WriteLine("DEBUG: " + Encoding.ASCII.GetString(msg));
		}

		public void OnIgnoreMessage(byte[] msg)
		{
			Debug.WriteLine("Ignore: " + Encoding.ASCII.GetString(msg));
		}

		public void OnUnknownMessage(byte type, byte[] data)
		{
			Debug.WriteLine("Unknown Message " + type);
		}

		public void OnError(Exception error, string msg)
		{
			Debug.WriteLine("ERROR: " + msg);
		}

		public void OnConnectionClosed()
		{
			Debug.WriteLine("Connection closed");
		}

		public void OnAuthenticationPrompt(string[] prompts)
		{
			Debug.WriteLine("Auth Prompt " + prompts[0]);
		}

		public PortForwardingCheckResult CheckPortForwardingRequest(string remote_host, int remote_port, string originator_ip, int originator_port)
		{
			PortForwardingCheckResult r = new PortForwardingCheckResult();
			r.allowed = false;
			r.channel = this;
			return r;
		}

		public void EstablishPortforwarding(ISSHChannelEventReceiver receiver, SSHChannel channel)
		{
			// nothing
		}

		#endregion

		#region ISSHChannelEventReceiver Members

		public void OnData(byte[] data, int offset, int length)
		{
			m_pipe.Write(data, offset, length);
		}

		public void OnExtendedData(int type, byte[] data)
		{
			string message = String.Format("EXTENDED DATA: type = {0}, data = {1}", type,
											Encoding.ASCII.GetString(data));
			Debug.WriteLine(message);
		}

		public void OnChannelClosed()
		{
			Debug.WriteLine("Channel closed");
			m_conn.Disconnect("");
		}

		public void OnChannelEOF()
		{
			Debug.WriteLine("Channel EOF");
			m_channel.Close();
		}

		public void OnChannelError(Exception error, string msg)
		{
			m_errorEstablishingConnection = true;
		}

		public void OnChannelReady()
		{
			m_ready = true;
		}

		public void OnMiscPacket(byte packet_type, byte[] data, int offset, int length)
		{
			string message = String.Format("MiscPacket: type = {0}, data = {1}", packet_type,
											Encoding.ASCII.GetString(data));
			Debug.WriteLine(message);
		}

		#endregion

		#region SFTP network functions
		private void sendINIT()
		{
			SSH2DataWriter writer = new SSH2DataWriter();

			putHEAD(writer, SSH_FXP.INIT, 5);
			writer.Write(3);				// version 3

			m_conn.TransmitPacket(writer.ToByteArray());
		}

		private void putHEAD(SSH2DataWriter writer, SSH_FXP type, int length)
		{ //throws Exception{
			writer.Write((byte)PacketType.SSH_MSG_CHANNEL_DATA);
			writer.Write(m_channel.RemoteChannelID);
			writer.Write(length + 4);
			writer.Write(length);
			writer.Write((byte)type);
		}

		private void sendREALPATH(byte[] path)
		{ //throws Exception{
			sendPacketPath(SSH_FXP.REALPATH, path);
		}

		private void sendOPENDIR(byte[] path)
		{ //throws Exception{
			sendPacketPath(SSH_FXP.OPENDIR, path);
		}

		private void sendREADDIR(byte[] path)
		{ //throws Exception{
			sendPacketPath(SSH_FXP.READDIR, path);
		}

		private void sendREMOVE(byte[] path)
		{ //throws Exception{
			sendPacketPath(SSH_FXP.REMOVE, path);
		}

		private void sendRENAME(byte[] p1, byte[] p2)
		{ //throws Exception{
			sendPacketPath(SSH_FXP.RENAME, p1, p2);
		}

		private void sendRMDIR(byte[] path)
		{ 
			sendPacketPath(SSH_FXP.RMDIR, path);
		}

		internal bool _sendCLOSE(byte[] handle, Header header)
		{
			sendCLOSE(handle);
			return checkStatus(null);
		}

		private void sendCLOSE(byte[] path)
		{ //throws Exception{
			sendPacketPath(SSH_FXP.CLOSE, path);
		}

		private void sendSTAT(byte[] path)
		{ //throws Exception{
			sendPacketPath(SSH_FXP.STAT, path);
		}

		private void sendSETSTAT(byte[] path, SftpATTRS attr)
		{
			SSH2DataWriter writer = new SSH2DataWriter();
			putHEAD(writer, SSH_FXP.SETSTAT, 9 + path.Length + attr.Length());
			writer.Write(seq++);
			writer.Write(path);
			attr.dump(buf);
			m_conn.TransmitPacket(writer.ToByteArray());
		}

		private void sendMKDIR(byte[] path, SftpATTRS attr)
		{
			SSH2DataWriter writer = new SSH2DataWriter();
			putHEAD(writer, SSH_FXP.MKDIR, 9 + path.Length + (attr != null ? attr.Length() : 4));
			writer.Write(seq++);
			writer.Write(path);             // path
			if(attr != null)
			{
				attr.dump(buf);
			}
			else
			{
				writer.Write(0);
			}
			//session.write(packet, this, 9 + path.Length + (attr != null ? attr.Length() : 4) + 4);
			m_conn.TransmitPacket(writer.ToByteArray());
		}

		private void sendPacketPath(SSH_FXP fxp, byte[] path)
		{
			SSH2DataWriter writer = new SSH2DataWriter();
			putHEAD(writer, fxp, 9 + path.Length);
			writer.Write(seq++);
			writer.WriteAsString(path);
			m_conn.TransmitPacket(writer.ToByteArray());
		}

		private void sendPacketPath(SSH_FXP fxp, byte[] p1, byte[] p2)
		{
			SSH2DataWriter writer = new SSH2DataWriter();
			putHEAD(writer, fxp, 13 + p1.Length + p2.Length);
			writer.Write(seq++);
			writer.WriteAsString(p1);
			writer.WriteAsString(p2);
			m_conn.TransmitPacket(writer.ToByteArray());
		}

		private void sendOPENR(byte[] path)
		{ 
			sendOPEN(path, SSH_FXF.READ);
		}
		private void sendOPENW(byte[] path)
		{ 
			sendOPEN(path, SSH_FXF.WRITE | SSH_FXF.CREAT | SSH_FXF.TRUNC);
		}
		private void sendOPENA(byte[] path)
		{ 
			sendOPEN(path, SSH_FXF.WRITE |/*SSH_FXF_APPEND|*/SSH_FXF.CREAT);
		}
		private void sendOPEN(byte[] path, SSH_FXF mode)
		{
			SSH2DataWriter writer = new SSH2DataWriter();
			putHEAD(writer, SSH_FXP.OPEN, 17 + path.Length);
			writer.Write(seq++);
			writer.WriteAsString(path);
			writer.Write((int)mode);
			writer.Write(0);
			m_conn.TransmitPacket(writer.ToByteArray());
		}

		private void sendREAD(byte[] handle, long offset, int length)
		{
			SSH2DataWriter writer = new SSH2DataWriter();
			putHEAD(writer, SSH_FXP.READ, 21 + handle.Length);
			writer.Write(seq++);
			writer.WriteAsString(handle);
			writer.WriteLong(offset);
			writer.Write(length);
			m_conn.TransmitPacket(writer.ToByteArray());
		}

		internal int sendWRITE(byte[] handle, long offset,
			byte[] data, int start, int length)
		{
			SSH2DataWriter writer = new SSH2DataWriter();

			/*
			int _length = length;

			if(buf.buffer.Length < buf.index + 13 + 21 + handle.Length + length
				+ 32 + 20  // padding and mac
				)
			{
				_length = buf.buffer.Length - (buf.index + 13 + 21 + handle.Length
					+ 32 + 20);  // padding and mac					
			}
			*/

			putHEAD(writer, SSH_FXP.WRITE, 21 + handle.Length + length);       // 14
			writer.Write(seq++);                                    //  4
			writer.WriteAsString(handle);                                  //  4+handle.length
			writer.WriteLong(offset);                                    //  8

			writer.WriteAsString(data, start, length);                    //  4+_length
			/*
			if(buf.buffer != data)
			{
				writer.WriteAsString(data, start, length);                    //  4+_length
			}
			else
			{
				writer.Write(_length);
				buf.skip(_length);
			}
			*/

			m_conn.TransmitPacket(writer.ToByteArray());
			return length;
		}




		#endregion

		#region SFTP file manipulation functions
		public void cd(String path)
		{
			//throws SftpException{
			try
			{
				path = remoteAbsolutePath(path);

				List<String> v = glob_remote(path);
				if(v.Count != 1)
				{
					throw new SftpException((int)SSH_FX.FAILURE, v.ToString());
				}

				path = v[0];
				sendREALPATH(path.GetBytes());

				Header _header = new Header();
				_header = ReadHeader();
				int length = _header.length;
				int type = _header.type;

				buf.rewind();
				fill(buf.buffer, 0, length);

				if(type != 101 && type != 104)
				{
					throw new SftpException(SSH_FX.FAILURE, "");
				}
				int i;
				if(type == 101)
				{
					i = buf.getInt();
					throwStatusError(buf, i);
				}
				i = buf.getInt();
				byte[] str = buf.getString();
				if(str != null && str[0] != '/')
				{
					str = (cwd + "/" + str.GetString()).GetBytes();
				}
				str = buf.getString();         // logname
				i = buf.getInt();              // attrs

				string newpwd = str.GetString();
				SftpATTRS attr = _stat(newpwd);
				if((attr.getFlags() & SftpATTRS.SSH_FILEXFER_ATTR_PERMISSIONS) == 0)
				{
					throw new SftpException(SSH_FX.FAILURE,
											"Can't change directory: " + path);
				}
				if(!attr.isDir())
				{
					throw new SftpException(SSH_FX.FAILURE,
											"Can't change directory: " + path);
				}
				cwd = newpwd;
			}
			catch(Exception e)
			{
				if(e is SftpException) throw (SftpException)e;
				throw new SftpException(SSH_FX.FAILURE, "");
			}
		}

		public void lcd(String path)
		{
			path = localAbsolutePath(path);
			if(Directory.Exists(path))
			{
				try
				{
					path = Path.GetFullPath(path);
				}
				catch(Exception) { }

				lcwd = path;
				return;
			}
			throw new SftpException(SSH_FX.NO_SUCH_FILE, "No such directory");
		}

		public List<LsEntry> ls()
		{
			return ls(cwd);
		}

		public List<LsEntry> ls(String path)
		{
			try
			{
				path = remoteAbsolutePath(path);

				String dir = path;
				byte[] pattern = null;
				SftpATTRS attr = null;
				if(isPattern(dir) ||
					((attr = stat(dir)) != null && !attr.isDir()))
				{
					int foo = path.LastIndexOf('/');
					dir = path.Substring(0, ((foo == 0) ? 1 : foo));
					pattern = path.Substring(foo + 1).GetBytes();
				}

				sendOPENDIR(dir.GetBytes());

				Header _header = ReadHeader();
				int length = _header.length;
				SSH_FXP type = (SSH_FXP)_header.type;

				buf.rewind();
				fill(buf.buffer, 0, length);

				if(type != SSH_FXP.STATUS && type != SSH_FXP.HANDLE)
				{
					throw new SftpException(SSH_FX.FAILURE, "");
				}

				if(type == SSH_FXP.STATUS)
				{
					int i = buf.getInt();
					throwStatusError(buf, i);
				}

				byte[] handle = buf.getString();         // filename

				List<LsEntry> v = new List<LsEntry>();
				while(true)
				{
					sendREADDIR(handle);

					_header = ReadHeader();
					length = _header.length;
					type = (SSH_FXP)_header.type;

					if(type != SSH_FXP.STATUS && type != SSH_FXP.NAME)
					{
						throw new SftpException(SSH_FX.FAILURE, "");
					}

					if(type == SSH_FXP.STATUS)
					{
						buf.rewind();

						fill(buf.buffer, 0, length);
						int i = buf.getInt();
						if(i == (int)SSH_FX.EOF)
							break;
						throwStatusError(buf, i);
					}

					buf.rewind();
					fill(buf.buffer, 0, 4); length -= 4;
					int count = buf.getInt();

					byte[] str;
					//int flags;

					buf.reset();
					while(count > 0)
					{
						if(length > 0)
						{
							buf.shift();
							int j = (buf.buffer.Length > (buf.index + length)) ? length : (buf.buffer.Length - buf.index);
							int i = fill(buf.buffer, buf.index, j);
							buf.index += i;
							length -= i;
						}
						byte[] filename = buf.getString();
						str = buf.getString();
						string longname = str.GetString();

						SftpATTRS attrs = SftpATTRS.getATTR(buf);
						if(pattern == null || Util.glob(pattern, filename))
						{
							v.Add(new LsEntry(filename.GetString(), longname, attrs));
						}

						count--;
					}
				}
				_sendCLOSE(handle, _header);
				return v;
			}
			catch(Exception e)
			{
				if(e is SftpException) throw (SftpException)e;
				throw new SftpException(SSH_FX.FAILURE, "");
			}
		}

		public SftpATTRS stat(String path)
		{
			//throws SftpException{
			try
			{
				path = remoteAbsolutePath(path);

				List<String> v = glob_remote(path);
				if(v.Count != 1)
				{
					throw new SftpException(SSH_FX.FAILURE, v.ToString());
				}
				path = v[0];
				return _stat(path);
			}
			catch(Exception e)
			{
				if(e is SftpException) throw (SftpException)e;
				throw new SftpException(SSH_FX.FAILURE, "");
			}
			//return null;
		}

		private SftpATTRS _stat(String path)
		{
			try
			{
				sendSTAT(path.GetBytes());

				Header _header = ReadHeader();

				int length = _header.length;
				SSH_FXP type = (SSH_FXP)_header.type;

				buf.rewind();
				fill(buf.buffer, 0, length);

				if(type != SSH_FXP.ATTRS)
				{
					if(type == SSH_FXP.STATUS)
					{
						int i = buf.getInt();
						throwStatusError(buf, i);
					}
					throw new SftpException(SSH_FX.FAILURE, "");
				}
				SftpATTRS attr = SftpATTRS.getATTR(buf);
				return attr;
			}
			catch(Exception e)
			{
				if(e is SftpException) throw (SftpException)e;
				throw new SftpException(SSH_FX.FAILURE, "");
			}
			//return null;
		}

		private void _setStat(String path, SftpATTRS attr)
		{
			try
			{
				sendSETSTAT(path.GetBytes(), attr);

				Header _header = ReadHeader();
				int length = _header.length;
				SSH_FXP type = (SSH_FXP)_header.type;

				buf.rewind();
				fill(buf.buffer, 0, length);

				if(type != SSH_FXP.STATUS)
				{
					throw new SftpException(SSH_FX.FAILURE, "");
				}
				int i = buf.getInt();
				if(i != (int)SSH_FX.OK)
				{
					throwStatusError(buf, i);
				}
			}
			catch(Exception e)
			{
				if(e is SftpException) throw (SftpException)e;
				throw new SftpException(SSH_FX.FAILURE, "");
			}
		}

		public void mkdir(String path)
		{
			try
			{
				path = remoteAbsolutePath(path);

				sendMKDIR(path.GetBytes(), null);

				Header _header = ReadHeader();
				int length = _header.length;
				SSH_FXP type = (SSH_FXP)_header.type;
				buf.rewind();
				fill(buf.buffer, 0, length);

				if(type != SSH_FXP.STATUS)
				{
					throw new SftpException(SSH_FX.FAILURE, "");
				}

				int i = buf.getInt();
				if(i == (int)SSH_FX.OK) return;
				throwStatusError(buf, i);
			}
			catch(Exception e)
			{
				if(e is SftpException) throw (SftpException)e;
				throw new SftpException(SSH_FX.FAILURE, "");
			}
		}

		public void rename(String oldpath, String newpath)
		{
			//throws SftpException{
			if(server_version < 2)
			{
				throw new SftpException(SSH_FX.FAILURE,
										"The remote sshd is too old to support rename operation.");
			}
			try
			{
				oldpath = remoteAbsolutePath(oldpath);
				newpath = remoteAbsolutePath(newpath);

				List<String> v = glob_remote(oldpath);
				int vsize = v.Count;
				if(vsize != 1)
				{
					throw new SftpException(SSH_FX.FAILURE, v.ToString());
				}
				oldpath = v[0];

				v = glob_remote(newpath);
				vsize = v.Count;
				if(vsize >= 2)
				{
					throw new SftpException(SSH_FX.FAILURE, v.ToString());
				}
				if(vsize == 1)
				{
					newpath = v[0];
				}
				else
				{  // vsize==0
					if(isPattern(newpath))
						throw new SftpException(SSH_FX.FAILURE, newpath);
					newpath = Util.Unquote(newpath);
				}

				sendRENAME(oldpath.GetBytes(), newpath.GetBytes());

				Header _header = ReadHeader();
				int length = _header.length;
				SSH_FXP type = (SSH_FXP)_header.type;

				buf.rewind();
				fill(buf.buffer, 0, length);

				if(type != SSH_FXP.STATUS)
				{
					throw new SftpException(SSH_FX.FAILURE, "");
				}

				int i = buf.getInt();
				if(i == (int)SSH_FX.OK) return;
				throwStatusError(buf, i);
			}
			catch(Exception e)
			{
				if(e is SftpException) throw (SftpException)e;
				throw new SftpException(SSH_FX.FAILURE, "");
			}
		}

		public void rm(String path)
		{
			try
			{
				path = remoteAbsolutePath(path);
				List<String> v = glob_remote(path);
				int vsize = v.Count;
				Header _header = new Header();

				for(int j = 0; j < vsize; j++)
				{
					path = v[j];
					sendREMOVE(path.GetBytes());

					_header = ReadHeader();
					int length = _header.length;
					SSH_FXP type = (SSH_FXP)_header.type;

					buf.rewind();
					fill(buf.buffer, 0, length);

					if(type != SSH_FXP.STATUS)
					{
						throw new SftpException(SSH_FX.FAILURE, "");
					}
					int i = buf.getInt();
					if(i != (int)SSH_FX.OK)
					{
						throwStatusError(buf, i);
					}
				}
			}
			catch(Exception e)
			{
				if(e is SftpException) throw (SftpException)e;
				throw new SftpException(SSH_FX.FAILURE, "");
			}
		}

		public void rmdir(String path)
		{
			try
			{
				path = remoteAbsolutePath(path);

				List<String> v = glob_remote(path);
				int vsize = v.Count;
				Header _header = new Header();

				for(int j = 0; j < vsize; j++)
				{
					path = v[j];
					sendRMDIR(path.GetBytes());

					_header = ReadHeader();
					int length = _header.length;
					SSH_FXP type = (SSH_FXP)_header.type;

					buf.rewind();
					fill(buf.buffer, 0, length);

					if(type != SSH_FXP.STATUS)
					{
						throw new SftpException(SSH_FX.FAILURE, "");
					}

					int i = buf.getInt();
					if(i != (int)SSH_FX.OK)
					{
						throwStatusError(buf, i);
					}
				}
			}
			catch(Exception e)
			{
				if(e is SftpException) throw (SftpException)e;
				throw new SftpException(SSH_FX.FAILURE, "");
			}
		}

		public void setMtime(String path, int mtime)
		{ 
			try
			{
				path = remoteAbsolutePath(path);

				List<String> v = glob_remote(path);

				int vsize = v.Count;
				for(int j = 0; j < vsize; j++)
				{
					path = v[j];

					SftpATTRS attr = _stat(path);

					attr.setFLAGS(0);
					attr.setACMODTIME(attr.getATime(), mtime);
					_setStat(path, attr);
				}
			}
			catch(Exception e)
			{
				if(e is SftpException) throw (SftpException)e;
				throw new SftpException(SSH_FX.FAILURE, "");
			}
		}

		#endregion

		#region SFTP file transfer functions

		#region GET functions
		public void get(String src, String dst)
		{ 
			get(src, dst, null, SFTPTransferMode.OVERWRITE);
		}
		public void get(String src, String dst,
			SftpProgressMonitor monitor)
		{
			get(src, dst, monitor, SFTPTransferMode.OVERWRITE);
		}
		public void get(String src, String dst,
			SftpProgressMonitor monitor, SFTPTransferMode mode)
		{
			src = remoteAbsolutePath(src);
			dst = localAbsolutePath(dst);
			try
			{
				List<String> v = glob_remote(src);
				int vsize = v.Count;
				if(vsize == 0)
				{
					throw new SftpException(SSH_FX.NO_SUCH_FILE, "No such file");
				}

				bool isDstDir = Directory.Exists(dst); ;
				StringBuilder dstsb = null;
				if(isDstDir)
				{
					if(!dst.EndsWith(file_separator))
					{
						dst += file_separator;
					}
					dstsb = new StringBuilder(dst);
				}
				else if(vsize > 1)
				{
					throw new SftpException(SSH_FX.FAILURE, "Copying multiple files, but destination is missing or a file.");
				}

				for(int j = 0; j < vsize; j++)
				{
					String _src = v[j];

					SftpATTRS attr = _stat(_src);
					if(attr.isDir())
					{
						throw new SftpException(SSH_FX.FAILURE, "not supported to get directory " + _src);
					}

					String _dst = null;
					if(isDstDir)
					{
						int i = _src.LastIndexOf('/');
						if(i == -1) dstsb.Append(_src);
						else dstsb.Append(_src.Substring(i + 1));
						_dst = dstsb.ToString();
						dstsb.Remove(dst.Length, _dst.Length);
					}
					else
					{
						_dst = dst;
					}

					if(mode == SFTPTransferMode.RESUME)
					{
						long size_of_src = attr.getSize();
						long size_of_dst = new FileInfo(_dst).Length;
						if(size_of_dst > size_of_src)
						{
							throw new SftpException(SSH_FX.FAILURE, "failed to resume for " + _dst);
						}
						if(size_of_dst == size_of_src)
						{
							return;
						}
					}

					if(monitor != null)
					{
						monitor.init(SftpProgressMonitor.GET, _src, _dst, attr.getSize());
						if(mode == SFTPTransferMode.RESUME)
						{
							monitor.count(new FileInfo(_dst).Length);
						}
					}
					FileStream fs = null;
					if(mode == SFTPTransferMode.OVERWRITE)
					{
						//fos = new FileOutputStream(_dst);
						fs = new FileStream(_dst, FileMode.Create);
					}
					else
					{
						//fos = new FileOutputStream(_dst, true); // append
						fs = new FileStream(_dst, FileMode.Append);
					}

					_get(_src, fs, monitor, mode, new FileInfo(_dst).Length);
					fs.Close();
				}
			}
			catch(Exception e)
			{
				if(e is SftpException) throw (SftpException)e;
				throw new SftpException(SSH_FX.FAILURE, "");
			}
		}
		public void get(String src, FileStream dst)
		{ 
			get(src, dst, null, SFTPTransferMode.OVERWRITE, 0);
		}
		public void get(String src, FileStream dst,
			SftpProgressMonitor monitor)
		{ 
			get(src, dst, monitor, SFTPTransferMode.OVERWRITE, 0);
		}
		public void get(String src, FileStream dst,
			SftpProgressMonitor monitor, SFTPTransferMode mode, long skip)
		{
			try
			{
				src = remoteAbsolutePath(src);
				List<String> v = glob_remote(src);
				if(v.Count != 1)
				{
					throw new SftpException(SSH_FX.FAILURE, v.ToString());
				}
				src = v[0];

				if(monitor != null)
				{
					SftpATTRS attr = _stat(src);
					monitor.init(SftpProgressMonitor.GET, src, "??", attr.getSize());
					if(mode == SFTPTransferMode.RESUME)
					{
						monitor.count(skip);
					}
				}
				_get(src, dst, monitor, mode, skip);
			}
			catch(Exception e)
			{
				if(e is SftpException) throw (SftpException)e;
				throw new SftpException(SSH_FX.FAILURE, "");
			}
		}


		private void _get(String src, FileStream dst,
			SftpProgressMonitor monitor, SFTPTransferMode mode, long skip)
		{ 
			try
			{
				sendOPENR(src.GetBytes());

				Header _header = ReadHeader();
				int length = _header.length;
				SSH_FXP type = (SSH_FXP)_header.type;

				buf.rewind();

				fill(buf.buffer, 0, length);

				if(type != SSH_FXP.STATUS && type != SSH_FXP.HANDLE)
				{
					throw new SftpException(SSH_FX.FAILURE, "Type is " + type);
				}

				if(type == SSH_FXP.STATUS)
				{
					int i = buf.getInt();
					throwStatusError(buf, i);
				}

				byte[] handle = buf.getString();         // filename

				long offset = 0;
				if(mode == SFTPTransferMode.RESUME)
				{
					offset += skip;
				}

				int request_len = 0;
			
				while(true)
				{
					request_len = buf.buffer.Length - 13;
					if(server_version == 0) { request_len = 1024; }
					sendREAD(handle, offset, request_len);

					_header = ReadHeader();
					length = _header.length;
					type = (SSH_FXP)_header.type;

					int i;
					if(type == SSH_FXP.STATUS)
					{
						buf.rewind();
						fill(buf.buffer, 0, length);
						i = buf.getInt();
						if(i == (int)SSH_FX.EOF)
						{
							goto BREAK;
						}
						throwStatusError(buf, i);
					}

					if(type != SSH_FXP.DATA)
					{
						goto BREAK;
					}

					buf.rewind();
					fill(buf.buffer, 0, 4); length -= 4;
					i = buf.getInt();   // length of data 
					int foo = i;
					while(foo > 0)
					{
						int bar = foo;
						if(bar > buf.buffer.Length)
						{
							bar = buf.buffer.Length;
						}

						i = m_pipe.Read(buf.buffer, 0, bar);

						if(i < 0)
						{
							goto BREAK;
						}
						int data_len = i;
						dst.Write(buf.buffer, 0, data_len);

						offset += data_len;
						foo -= data_len;

						if(monitor != null)
						{
							if(!monitor.count(data_len))
							{
								while(foo > 0)
								{
									i = m_pipe.Read(buf.buffer,
										0,
										(buf.buffer.Length < foo ? buf.buffer.Length : foo));
									if(i <= 0) break;
									foo -= i;
								}
								goto BREAK;
							}
						}
					}
				}
			BREAK:
				dst.Flush();

				if(monitor != null) monitor.end();
				_sendCLOSE(handle, _header);
			}
			catch(Exception e)
			{
				if(e is SftpException) throw (SftpException)e;
				throw new SftpException(SSH_FX.FAILURE, "");
			}
		}

		#endregion


		#region PUT functions
		private void _put(FileStream src, String dst,
			SftpProgressMonitor monitor, SFTPTransferMode mode)
		{
			try
			{
				long skip = 0;
				if(mode == SFTPTransferMode.RESUME || mode == SFTPTransferMode.APPEND)
				{
					try
					{
						SftpATTRS attr = _stat(dst);
						skip = attr.getSize();
					}
					catch(Exception)
					{
						//System.err.println(eee);
					}
				}
				if(mode == SFTPTransferMode.RESUME && skip > 0)
				{
					long skipped = src.Skip(skip);
					if(skipped < skip)
					{
						throw new SftpException(SSH_FX.FAILURE, "failed to resume for " + dst);
					}
				}
				if(mode == SFTPTransferMode.OVERWRITE) { sendOPENW(dst.GetBytes()); }
				else { sendOPENA(dst.GetBytes()); }

				Header _header = ReadHeader();
				int length = _header.length;
				SSH_FXP type = (SSH_FXP)_header.type;

				buf.rewind();
				fill(buf.buffer, 0, length);

				if(type != SSH_FXP.STATUS && type != SSH_FXP.HANDLE)
				{
					throw new SftpException(SSH_FX.FAILURE, "invalid type=" + type);
				}
				if(type == SSH_FXP.STATUS)
				{
					int i = buf.getInt();
					throwStatusError(buf, i);
				}
				byte[] handle = buf.getString();         // filename
				byte[] data = new byte[buf.buffer.Length
								  - (5 + 13 + 21 + handle.Length
									+ 32 + 20 // padding and mac
								   )
						];

				/*
				bool dontcopy = true;

				if(!dontcopy)
				{
					data = new byte[buf.buffer.Length
								  - (5 + 13 + 21 + handle.Length
									+ 32 + 20 // padding and mac
								   )
						];
				}
				*/
				long offset = 0;
				if(mode == SFTPTransferMode.RESUME || mode == SFTPTransferMode.APPEND)
				{
					offset += skip;
				}

				int startid = seq;
				int _ackid = seq;
				int ackcount = 0;
				while(true)
				{
					int nread = 0;
					int s = 0;
					int datalen = 0;
					int count = 0;

					datalen = data.Length - s;
					/*
					if(!dontcopy)
					{
						datalen = data.Length - s;
					}
					else
					
					{
						data = buf.buffer;
						s = 5 + 13 + 21 + handle.Length;
						datalen = buf.buffer.Length - s
								- 32 - 20; // padding and mac
					}
					*/
					do
					{
						nread = src.Read(data, s, datalen);
						if(nread > 0)
						{
							s += nread;
							datalen -= nread;
							count += nread;
						}
					}
					while(datalen > 0 && nread > 0);
					if(count <= 0) break;

					int _i = count;
					while(_i > 0)
					{
						_i -= sendWRITE(handle, offset, data, 0, _i);
						if((seq - 1) == startid ||
						  m_pipe.BytesAvailable >= 1024)
						{
							while(m_pipe.BytesAvailable > 0)
							{
								if(checkStatus(ackid))
								{
									_ackid = ackid[0];
									if(startid > _ackid || _ackid > seq - 1)
									{
										if(_ackid == seq)
										{
											Console.WriteLine("ack error: startid=" + startid + " seq=" + seq + " _ackid=" + _ackid);
										}
										else
										{
											throw new SftpException(SSH_FX.FAILURE, "ack error: startid=" + startid + " seq=" + seq + " _ackid=" + _ackid);
										}
									}
									ackcount++;
								}
								else
								{
									break;
								}
							}
						}
					}
					offset += count;
					if(monitor != null && !monitor.count(count))
					{
						break;
					}
				}
				int _ackcount = seq - startid;
				while(_ackcount > ackcount)
				{
					if(!checkStatus(null))
					{
						break;
					}
					ackcount++;
				}
				if(monitor != null) monitor.end();
				_sendCLOSE(handle, _header);
				//System.err.println("start end "+startid+" "+endid);
			}
			
			catch(Exception e)
			{
				if(e is SftpException) throw;
				throw new SftpException(SSH_FX.FAILURE, e.ToString());
			}
			
		}


		/*
		put foo
		c->s OPEN
		s->c HANDLE
		c->s WRITE
		s->c STATUS
		c->s CLOSE
		s->c STATUS
		*/
		public void put(String src, String dst)
		{ //throws SftpException{
			put(src, dst, null, SFTPTransferMode.OVERWRITE);
		}
		public void put(String src, String dst, SFTPTransferMode mode)
		{ //throws SftpException{
			put(src, dst, null, mode);
		}
		public void put(String src, String dst,
			SftpProgressMonitor monitor)
		{ //throws SftpException{
			put(src, dst, monitor, SFTPTransferMode.OVERWRITE);
		}
		public void put(String src, String dst,
			SftpProgressMonitor monitor, SFTPTransferMode mode)
		{
			//throws SftpException{
			src = localAbsolutePath(src);
			dst = remoteAbsolutePath(dst);

			//System.err.println("src: "+src+", "+dst);
			try
			{
				List<String> v = glob_remote(dst);
				int vsize = v.Count;
				if(vsize != 1)
				{
					if(vsize == 0)
					{
						if(isPattern(dst))
							throw new SftpException(SSH_FX.FAILURE, dst);
						else
							dst = Util.Unquote(dst);
					}
					throw new SftpException(SSH_FX.FAILURE, v.ToString());
				}
				else
				{
					dst = v[0];
				}

				//System.err.println("dst: "+dst);

				bool _isRemoteDir = isRemoteDir(dst);

				v = glob_local(src);
				//System.err.println("glob_local: "+v+" dst="+dst);
				vsize = v.Count;

				StringBuilder dstsb = null;
				if(_isRemoteDir)
				{
					if(!dst.EndsWith("/"))
					{
						dst += "/";
					}
					dstsb = new StringBuilder(dst);
				}
				else if(vsize > 1)
				{
					throw new SftpException(SSH_FX.FAILURE, "Copying multiple files, but destination is missing or a file.");
				}

				for(int j = 0; j < vsize; j++)
				{
					String _src = v[j];
					String _dst = null;
					if(_isRemoteDir)
					{
						int i = _src.LastIndexOf(file_separatorc);
						if(i == -1) dstsb.Append(_src);
						else dstsb.Append(_src.Substring(i + 1));
						_dst = dstsb.ToString();
						dstsb.Remove(dst.Length, _dst.Length);
					}
					else
					{
						_dst = dst;
					}
					//System.err.println("_dst "+_dst);

					long size_of_dst = 0;
					if(mode == SFTPTransferMode.RESUME)
					{
						try
						{
							SftpATTRS attr = _stat(_dst);
							size_of_dst = attr.getSize();
						}
						catch(Exception)
						{
							//System.err.println(eee);
						}
						long size_of_src = new FileInfo(_src).Length;
						if(size_of_src < size_of_dst)
						{
							throw new SftpException(SSH_FX.FAILURE, "failed to resume for " + _dst);
						}
						if(size_of_src == size_of_dst)
						{
							return;
						}
					}

					if(monitor != null)
					{
						monitor.init(SftpProgressMonitor.PUT, _src, _dst,
									 (new FileInfo(_src)).Length);
						if(mode == SFTPTransferMode.RESUME)
						{
							monitor.count(size_of_dst);
						}
					}
					FileStream fs = null;
					try
					{
						//fis = new FileStream(_src);
						fs = new FileStream(_src, FileMode.Open);
						_put(fs, _dst, monitor, mode);
					}
					finally
					{
						if(fs != null)
						{
							//	    try{
							fs.Close();
							//	    }catch(Exception ee){};
						}
					}
				}
			}
			catch(Exception e)
			{
				if(e is SftpException) throw;// (SftpException)e;
				throw new SftpException(SSH_FX.FAILURE, e.ToString());
			}
		}

		

		#endregion

		#endregion

		#region SFTP helper functions
		private String remoteAbsolutePath(String path)
		{
			if(path[0] == '/') return path;
			if(cwd.EndsWith("/")) return cwd + path;
			return cwd + "/" + path;
		}

		private String localAbsolutePath(String path)
		{
			if(Path.IsPathRooted(path)) return path;
			if(lcwd.EndsWith(file_separator)) return lcwd + path;
			return lcwd + file_separator + path;
		}

		private bool isRemoteDir(String path)
		{
			try
			{
				sendSTAT(path.GetBytes());

				Header _header = ReadHeader();
				int length = _header.length;
				SSH_FXP type = (SSH_FXP)_header.type;
				buf.rewind();
				fill(buf.buffer, 0, length);

				if(type != SSH_FXP.ATTRS)
				{
					return false;
				}
				SftpATTRS attr = SftpATTRS.getATTR(buf);
				return attr.isDir();
			}
			catch(Exception) { }
			return false;
		}

		private bool isPattern(String path)
		{
			return path.IndexOf("*") != -1 || path.IndexOf("?") != -1;
		}

		private bool isPattern(byte[] path)
		{
			int i = path.Length - 1;
			while(i >= 0)
			{
				if(path[i] == '*' || path[i] == '?')
				{
					if(i > 0 && path[i - 1] == '\\')
					{
						i--;
					}
					else
					{
						break;
					}
				}
				i--;
			}
			return !(i < 0);
		}


		private List<String> glob_remote(string _path)
		{
			//throws Exception{
			//System.err.println("glob_remote: "+_path);
			List<String> v = new List<String>();
			byte[] path = _path.GetBytes();

			if(!isPattern(path))
			{
				//v.addElement(Util.unquote(_path)); return v;
				v.Add(Util.Unquote(_path));
				return v;
			}

			int i = path.Length - 1;

			while(i >= 0)
			{
				if(path[i] == '/')
				{
					break;
				}

				i--;
			}

			if(i < 0)
			{
				v.Add(Util.Unquote(_path));
				return v;
			}

			byte[] dir;

			if(i == 0)
			{
				dir = new byte[] { (byte)'/' };
			}
			else
			{
				dir = new byte[i];
				Array.Copy(path, 0, dir, 0, i);
			}
			//System.err.println("dir: "+new String(dir));

			byte[] pattern = new byte[path.Length - i - 1];
			Array.Copy(path, i + 1, pattern, 0, pattern.Length);
			//System.err.println("file: "+new String(pattern));

			sendOPENDIR(dir);

			Header _header = ReadHeader();
			//_header = header(buf, _header);
			int length = _header.length;
			//int type = _header.type;
			SSH_FXP type = (SSH_FXP)_header.type;

			//byte[] buffer = new byte[length];

			fill(buf.buffer, 0, length);

			//SSH2DataReader reader = new SSH2DataReader(buffer);

			if(type != SSH_FXP.STATUS && type != SSH_FXP.HANDLE)
			{
				throw new SftpException((int)SSH_FX.FAILURE, "");
			}
			if(type == SSH_FXP.STATUS)
			{
				i = buf.getInt();// reader.ReadInt32();
				throwStatusError(buf.buffer, i);
			}

			byte[] handle = buf.getString();//reader.ReadString();         // filename

			while(true)
			{
				sendREADDIR(handle);

				_header = ReadHeader();
				length = _header.length;
				type = (SSH_FXP)_header.type;

				if(type != SSH_FXP.STATUS && type != SSH_FXP.NAME)
				{
					throw new SftpException((int)SSH_FX.FAILURE, "");
				}
				if(type == SSH_FXP.STATUS)
				{
					//buffer = new byte[length];
					buf.rewind();
					fill(buf.buffer, 0, length);
					break;
				}

				buf.rewind();
				fill(buf.buffer, 0, 4); length -= 4;
				int count = buf.getInt();

				//reader = new SSH2DataReader(buffer);
				count = buf.getInt();//reader.ReadInt32();

				byte[] str;
				//int flags;

				buf.reset();
				while(count > 0)
				{
					if(length > 0)
					{
						buf.shift();
						int j = (buf.buffer.Length > (buf.index + length)) ? length : (buf.buffer.Length - buf.index);
						//i = io.ins.read(buf.buffer, buf.index, j);
						i = m_pipe.Read(buf.buffer, buf.index, j);
						if(i <= 0) break;
						buf.index += i;
						length -= i;
					}

					byte[] filename = buf.getString();
					//System.err.println("filename: "+new String(filename));
					str = buf.getString();
					SftpATTRS attrs = SftpATTRS.getATTR(buf);

					if(Util.glob(pattern, filename))
					{
						//v.addElement(new String(dir) + "/" + new String(filename));
						string element = dir.GetString() + "/" + filename.GetString();
						v.Add(element);
					}
					count--;
				}
			}
			if(_sendCLOSE(handle, _header))
				return v;
			return null;
		}

		private List<String> glob_local(String _path)
		{ //throws Exception{
			//System.out.println("glob_local: "+_path);
			List<String> v = new List<String>();
			byte[] path = _path.GetBytes();

			int i = path.Length - 1;

			while(i >= 0) 
			{ 
				if(path[i] == '*' || path[i] == '?')
				{
					break;
				}
				i--; 
			}

			if(i < 0) 
			{ 
				v.Add(_path);
				return v; 
			}

			while(i >= 0) 
			{
				if(path[i] == file_separatorc)
				{
					break;
				}
				i--; 
			}

			if(i < 0) 
			{ 
				v.Add(_path); 
				return v; 
			}

			byte[] dir;

			if(i == 0) 
			{ 
				dir = new byte[] { (byte)file_separatorc }; 
			}
			else
			{
				dir = new byte[i];
				Array.Copy(path, 0, dir, 0, i);
			}

			byte[] pattern = new byte[path.Length - i - 1];
			Array.Copy(path, i + 1, pattern, 0, pattern.Length);
			//System.out.println("dir: "+new String(dir)+" pattern: "+new String(pattern));

			try
			{
				string[] children = Util.ListFiles(dir.GetString());
				for(int j = 0; j < children.Length; j++)
				{
					//System.out.println("children: "+children[j]);
					if(Util.glob(pattern, children[j].GetBytes()))
					{
						v.Add(dir.GetString() + file_separator + children[j]);
					}
				}
			}
			catch(Exception)
			{
			}
			return v;
		}


		#endregion

		#region Network helper functions

		public void Disconnect()
		{
			m_channel.Close();
		}

		internal Header ReadHeader()
		{
			Header header = new Header();
			byte[] buffer = new byte[32];
			//MemoryStream ms = new MemoryStream(buffer);
			//BinaryReader br = new BinaryReader(ms);
			SSH2DataReader reader = new SSH2DataReader(buffer);
			int i = fill(buffer, 0, 9);
			header.length = reader.ReadInt32() - 5;//br.ReadInt32() - 5;
			header.type = reader.ReadByte() & 0xff;
			header.rid = reader.ReadInt32();

			return header;
		}

		private int fill(byte[] buf, int s, int len)
		{
			int i = 0;
			int foo = s;
			while(len > 0)
			{
				i = m_pipe.Read(buf, s, len);
				//i = io.ins.read(buf, s, len);
				if(i <= 0)
				{
					throw new System.IO.IOException("inputstream is closed");
					//return (s-foo)==0 ? i : s-foo;
				}
				s += i;
				len -= i;
			}
			return s - foo;
		}

		private void skip(long foo)
		{
			byte[] buffer = new byte[1024];
			while(foo > 0)
			{
				//long bar = io.ins.skip(foo);
				long bar = m_pipe.Read(buffer, 0, (int)foo);

				if(bar <= 0)
					break;
				foo -= bar;
			}
		}

		private void throwStatusError(byte[] buf, int i)
		{
			if(server_version >= 3)
			{
				throw new SftpException(i, buf.GetString());
			}
			else
			{
				throw new SftpException(i, "Failure");
			}
		}

		private void throwStatusError(Buffer buf, int i)
		{
			if(server_version >= 3)
			{
				byte[] str = buf.getString();
				throw new SftpException(i, str.GetString());
			}
			else
			{
				throw new SftpException(i, "Failure");
			}
		}

		internal bool checkStatus(int[] ackid)
		{
			Header _header = ReadHeader();
			int length = _header.length;
			SSH_FXP type = (SSH_FXP)_header.type;

			if(ackid != null)
				ackid[0] = _header.rid;

			buf.rewind();
			fill(buf.buffer, 0, length);

			if(type != SSH_FXP.STATUS)
			{
				throw new SftpException((int)SSH_FX.FAILURE, "");
			}
			int i = buf.getInt();
			if(i != (int)SSH_FX.OK)
			{
				throwStatusError(buf, i);
			}
			return true;
		}





		#endregion
	}
}
