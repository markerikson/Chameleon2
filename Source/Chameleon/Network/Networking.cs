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
	

	public class Networking
	{
		private SSH2Connection m_conn;		
		private SFTPConnection m_sftp;
		
		public static readonly string StartToken = "St_Ar_Tt_oK_eN";
		public static readonly string EndToken = "En_Dt_oK_eN";

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

		public Routrek.SSHCV2.SSH2Connection Connection
		{
			get { return m_conn; }
			set { m_conn = value; }
		}


		protected Networking()
		{
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

				NullReader r = new NullReader();

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

			try
			{
				m_conn.Disconnect("");
			}
			// ignore these
			catch(SocketException) { }
			catch(ObjectDisposedException) { }
			
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
