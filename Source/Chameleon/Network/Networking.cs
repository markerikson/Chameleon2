using System;
using System.Collections.Generic;
using System.IO;
using System.Net.Sockets;
using System.Text;
using System.Windows.Forms;
using Chameleon.Util;
using DevInstinct.Patterns;
using Granados;
using Routrek.SSHC;
using Routrek.SSHCV2;

namespace Chameleon.Network
{
	// courtesy of http://www.codeproject.com/KB/IP/SetKeepAliveValues.aspx
	[System.Runtime.InteropServices.StructLayout(System.Runtime.InteropServices.LayoutKind.Explicit)]
	unsafe struct TcpKeepAlive
	{
		[System.Runtime.InteropServices.FieldOffset(0)]
		[
			System.Runtime.InteropServices.MarshalAs
			(
				System.Runtime.InteropServices.UnmanagedType.ByValArray,
				SizeConst = 12
			)
		]
		public fixed byte Bytes[12];

		// non-zero = on
		[System.Runtime.InteropServices.FieldOffset(0)]
		public uint On_Off;

		// timeout, in milliseconds, with no activity until the first keep-alive packet is sent
		[System.Runtime.InteropServices.FieldOffset(4)]
		public uint KeepAliveTime;

		// interval, in milliseconds, between when successive keep-alive packets are sent
		// if no acknowledgment is received
		[System.Runtime.InteropServices.FieldOffset(8)]
		public uint KeepAliveInterval;
	}

	public static class SocketUtil
	{
		public static int SetKeepAliveValues(System.Net.Sockets.Socket Socket, bool On_Off, 
												uint KeepAliveTime, uint KeepAliveInterval)
		{
			int Result = -1;

			unsafe
			{
				TcpKeepAlive KeepAliveValues = new TcpKeepAlive();

				KeepAliveValues.On_Off = Convert.ToUInt32(On_Off);
				KeepAliveValues.KeepAliveTime = KeepAliveTime;
				KeepAliveValues.KeepAliveInterval = KeepAliveInterval;

				byte[] InValue = new byte[12];

				for(int I = 0; I < 12; I++)
					InValue[I] = KeepAliveValues.Bytes[I];

				Result = Socket.IOControl(IOControlCode.KeepAliveValues, InValue, null);
			}

			return Result;
		}

		public static bool IsConnected(Socket socket)
		{
			try
			{
				return !(socket.Poll(1, SelectMode.SelectRead) && socket.Available == 0);
			}
			catch(SocketException) { return false; }
		}


	}


	public class Networking
	{
		#region Private fields
		private SSH2Connection m_conn;
		private SFTPConnection m_sftp;		
		
		public static readonly string StartToken = "St_Ar_Tt_oK_eN";
		public static readonly string EndToken = "En_Dt_oK_eN";

		private static List<string> m_dirsToSkip;

		private Dictionary<string, SSHChannel> m_shells;

		private Socket m_socket;
		private Timer m_timer;

		#endregion

		#region Properties

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

		public Routrek.SSHCV2.SSH2Connection Connection
		{
			get { return m_conn; }
			set { m_conn = value; }
		}

		public SFTPConnection Sftp
		{
			get { return m_sftp; }
			set { m_sftp = value; }
		}

		#endregion

		#region Constructors
		static Networking()
		{
			m_dirsToSkip = new List<string>();

			m_dirsToSkip.Add(".");
			m_dirsToSkip.Add("..");
		}

		protected Networking()
		{
			m_shells = new Dictionary<string, SSHChannel>();
			
		}

		#endregion

		public event EventHandler SocketClosed;

		public bool Connect(string host, string username, string password)
		{
			if(IsConnected)
			{
				return false;
			}

			try
			{
				m_socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

				// send TCP keep-alive packets after 10 seconds of inactivity, then every 5 seconds
				SocketUtil.SetKeepAliveValues(m_socket, true, 10 * 1000, 5 * 1000);

				m_socket.Connect(host, 22);

				SSHConnectionParameter f = new SSHConnectionParameter();

				f.UserName = username;
				f.Password = password;
				f.Protocol = SSHProtocol.SSH2;
				f.AuthenticationType = AuthenticationType.Password;
				f.WindowSize = 0x1000;

				NullReader r = new NullReader();

				if(m_timer != null)
				{
					m_timer.Stop();
					m_timer.Dispose();
				}

				m_conn = (SSH2Connection)SSHConnection.Connect(f, r, m_socket);

				m_timer = new Timer();
				// check connection every 10 milliseconds
				m_timer.Interval = 10;

				m_timer.Tick += new EventHandler(OnTimerTick);
				
	
			}
			catch(Exception e)
			{
				if(e.Message == "User authentication failed")
				{
					MessageBox.Show("This username and/or password were not accepted.  Check them and try again",
									"Login Error", MessageBoxButtons.OK, MessageBoxIcon.Warning);
				}
				else
				{
					MessageBox.Show("SSH connection exception: " + e.Message);
				}
				
				return false;
			}			

			return true;
		}

		void OnTimerTick(object sender, EventArgs e)
		{
			if(!SocketUtil.IsConnected(m_socket))
			{
				if(SocketClosed != null)
				{
					SocketClosed(this, new EventArgs());
				}
			}
		}

		public void Disconnect()
		{
			if(m_conn == null)
			{
				return;
			}

			try
			{
				m_conn.Disconnect("");
			}
			// ignore these
			catch(SocketException) { }
			catch(ObjectDisposedException) { }
			
			m_conn = null;

			Sftp = null;

			m_shells.Clear();
		}

		public void GetFileContents(FilePath file, ref string contents)
		{
			EnsureSFTPConnection();

			string filename = file.GetFullPath();

			MemoryStream ms = new MemoryStream();

			Sftp.get(filename, ms);

			contents = Encoding.UTF8.GetString(ms.ToArray());
		}

		public void SendFileContents(FilePath destination, string contents)
		{
			EnsureSFTPConnection();

			string destString = destination.GetFullPath();

			MemoryStream ms = new MemoryStream();
			byte[] bytes = Encoding.UTF8.GetBytes(contents);
			ms.Write(bytes, 0, bytes.Length);
			ms.Position = 0;

			Sftp.put(ms, destString);
		}

		public FilePath GetUserHomeDirectory()
		{
			EnsureSFTPConnection();

			FilePath home = new FilePath();
			home.AssignDir(Sftp.Home, PathFormat.Unix);

			return home;
		}


		public DirectoryListing GetDirectoryListing(FilePath folderToList, bool includeHidden)
		{
			EnsureSFTPConnection();

			string path = folderToList.GetFullPath();
			List<LsEntry> files = Sftp.ls(path);

			DirectoryListing dl = new DirectoryListing();

			dl.directoryName = folderToList;

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

		public void DeleteFile(FilePath fileToDelete)
		{
			EnsureSFTPConnection();

			string filename = fileToDelete.GetFullPath();
			m_sftp.rm(filename);
		}

		private void EnsureSFTPConnection()
		{
			if(!IsConnected)
			{
				throw new Exception("Can't use SFTP if not connected");
			}

			if(Sftp == null || !Sftp.IsConnected)
			{
				Sftp = new SFTPConnection(m_conn);
				Sftp.StartSFTPSession();
			}
		}

		public SSHChannel StartShell(ISSHChannelEventReceiver receiver)
		{
			return m_conn.OpenShell(receiver);
		}

	}
}
