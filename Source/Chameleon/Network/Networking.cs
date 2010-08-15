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
using Piccolo.Common;
using System.Threading;

namespace Chameleon.Network
{
	

	public class Networking
	{
		private SSH2Connection m_conn;		
		private SFTPConnection m_sftp;

		private OutputParser m_outputParser;

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


		private Networking()
		{
			m_shells = new Dictionary<string, SSHChannel>();

			m_outputParser = Singleton<OutputParser>.Instance;
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

		public void ExecuteRemoteCommand(string command, Action<string> callback)
		{
			OutputCollectingReader r = new OutputCollectingReader();
			r.callback = callback;

			m_outputParser.AddReader(r);

			SSHChannel chan = StartShell(r);
			r.chan = chan;

			// TODO This whole function probably ought to spin off a new thread or something
			while(!r._ready)
			{
				Thread.Sleep(50);
			}

			//chan.Transmit("bash");

			string formattedCommand = string.Format("echo {0}; {1}; echo {2};exit;\r", StartToken, command, EndToken);
			chan.Transmit(formattedCommand);

		}

		public string GetFeaturePermissions()
		{
			string studentID = App.Configuration.StudentID;
			string baseURL = App.Configuration.FeaturePermissionsURL;

			string permissionsURL = baseURL + "?student=" + studentID.ToLower();

			HttpHelper http = new HttpHelper();
			string featureText = http.HttpStringGet(permissionsURL);

			if(featureText.IndexOf("Error") != -1)
			{
				return null;
			}

			return featureText;
		}


	}
}
