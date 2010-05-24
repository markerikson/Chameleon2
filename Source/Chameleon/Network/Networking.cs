using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Windows.Forms;

using Routrek.Crypto;
using Routrek.SSHC;
using Routrek.SSHCV1;
using Routrek.SSHCV2;
using Routrek.Toolkit;
using Routrek.PKI;
using System.Diagnostics;
using DevInstinct.Patterns;
using Granados;
using Chameleon.Util;
using System.IO;

namespace Chameleon.Network
{
	class Reader : ISSHConnectionEventReceiver, ISSHChannelEventReceiver
	{
		public SSHConnection _conn;
		public bool _ready;

		public void OnData(byte[] data, int offset, int length)
		{
			System.Console.Write(Encoding.ASCII.GetString(data, offset, length));
		}
		public void OnDebugMessage(bool always_display, byte[] data)
		{
			Debug.WriteLine("DEBUG: " + Encoding.ASCII.GetString(data));
		}
		public void OnIgnoreMessage(byte[] data)
		{
			Debug.WriteLine("Ignore: " + Encoding.ASCII.GetString(data));
		}
		public void OnAuthenticationPrompt(string[] msg)
		{
			Debug.WriteLine("Auth Prompt " + msg[0]);
		}

		public void OnError(Exception error, string msg)
		{
			Debug.WriteLine("ERROR: " + msg);
		}
		public void OnChannelClosed()
		{
			Debug.WriteLine("Channel closed");
			_conn.Disconnect("");
			//_conn.AsyncReceive(this);
		}
		public void OnChannelEOF()
		{
			_pf.Close();
			Debug.WriteLine("Channel EOF");
		}
		public void OnExtendedData(int type, byte[] data)
		{
			Debug.WriteLine("EXTENDED DATA");
		}
		public void OnConnectionClosed()
		{
			Debug.WriteLine("Connection closed");
		}
		public void OnUnknownMessage(byte type, byte[] data)
		{
			Debug.WriteLine("Unknown Message " + type);
		}
		public void OnChannelReady()
		{
			_ready = true;
		}
		public void OnChannelError(Exception error, string msg)
		{
			Debug.WriteLine("Channel ERROR: " + msg);
		}
		public void OnMiscPacket(byte type, byte[] data, int offset, int length)
		{
		}

		public PortForwardingCheckResult CheckPortForwardingRequest(string host, int port, string originator_host, int originator_port)
		{
			PortForwardingCheckResult r = new PortForwardingCheckResult();
			r.allowed = true;
			r.channel = this;
			return r;
		}
		public void EstablishPortforwarding(ISSHChannelEventReceiver rec, SSHChannel channel)
		{
			_pf = channel;
		}

		public SSHChannel _pf;
	}

	class Networking
	{
		private string m_host;
		private string m_username;
		private string m_password;

		private SSH2Connection m_conn;
		private SFTPConnection m_sftp;

		private static List<string> m_dirsToSkip;

		static Networking()
		{
			m_dirsToSkip = new List<string>();

			m_dirsToSkip.Add(".");
			m_dirsToSkip.Add("..");
		}

		private Dictionary<string, SSHChannel> m_shells;

		public bool IsConnected
		{
			get
			{
				return m_conn != null && !m_conn.IsClosed;
			}
		}

		public static Networking Instance
		{
			get
			{
				return Singleton<Networking>.Instance;
			}
		}


		private Networking()
		{
			m_host = "";
			m_username = "";
			m_password = "";

			m_shells = new Dictionary<string, SSHChannel>();
		}


		public bool Connect(string host, string username, string password)
		{
			if(IsConnected)
			{
				return false;
			}

			Socket s;

			try
			{
				s = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
				s.Connect(host, 22);

				SSHConnectionParameter f = new SSHConnectionParameter();

				f.UserName = username;
				f.Password = password;
				f.Protocol = SSHProtocol.SSH2;
				f.AuthenticationType = AuthenticationType.Password;
				f.WindowSize = 0x1000;

				Reader r = new Reader();

				m_conn = (SSH2Connection)SSHConnection.Connect(f, r, s);				
			}
			catch(Exception e)
			{
				MessageBox.Show("SSH connection exception: " + e.Message);
				return false;
			}			

			return true;
		}

		public void Disconnect()
		{
			if(m_conn == null)
			{
				return;
			}

			m_conn.Disconnect("");
			m_conn = null;

			m_sftp = null;

			m_shells.Clear();
		}

		public void GetFileContents(FilePath file, ref string contents)
		{
			EnsureSFTPConnection();

			string filename = file.GetFullPath();

			MemoryStream ms = new MemoryStream();

			m_sftp.get(filename, ms);

			contents = Encoding.ASCII.GetString(ms.ToArray());
		}

		public void SendFileContents(FilePath destination, string contents)
		{
			EnsureSFTPConnection();

			string destString = destination.GetFullPath();

			MemoryStream ms = new MemoryStream();
			byte[] bytes = Encoding.ASCII.GetBytes(contents);
			ms.Write(bytes, 0, bytes.Length);
			ms.Position = 0;

			m_sftp.put(ms, destString);
		}

		public FilePath GetUserHomeDirectory()
		{
			EnsureSFTPConnection();

			FilePath home = new FilePath();
			home.AssignDir(m_sftp.Home, PathFormat.Unix);

			return home;
		}


		public DirectoryListing GetDirectoryListing(FilePath folderToList, bool includeHidden)
		{
			EnsureSFTPConnection();

			string path = folderToList.GetFullPath();
			List<LsEntry> files = m_sftp.ls(path);

			DirectoryListing dl = new DirectoryListing();

			foreach(LsEntry lse in files)
			{
				string filename = lse.getFilename();

				if(filename.StartsWith(".") && !includeHidden)
				{
					continue;
				}

				if(lse.getAttrs().isDir() && !m_dirsToSkip.Contains(filename))
				{
					dl.dirNames.Add(filename);
				}
				else
				{
					dl.fileNames.Add(filename);
				}
			}


			return dl;
		}

		private void EnsureSFTPConnection()
		{
			if(!IsConnected)
			{
				throw new Exception("Can't use SFTP if not connected");
			}

			if(m_sftp == null || !m_sftp.IsConnected)
			{
				m_sftp = new SFTPConnection(m_conn);
				m_sftp.StartSFTPSession();
			}
		}

		public SSHChannel StartShell(ISSHChannelEventReceiver receiver)
		{
			return m_conn.OpenShell(receiver);
		}


	}
}
