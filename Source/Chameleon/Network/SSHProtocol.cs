using System;
using Routrek.SSHC;
using System.Net.Sockets;
using Chameleon.Network;
using Chameleon.GUI;
using WalburySoftware;

namespace SSHClient
{
	public enum AuthMethod { Host, Password, PublicKey, KeyboardInteractive };
	/// <summary>
	/// Description of Protocol.
	/// </summary>
	public class SSHProtocol : ISSHConnectionEventReceiver, ISSHChannelEventReceiver
	{
		#region Public Properties
		public string Username
		{
			set
			{
				_params.UserName = value;
			}
		}
		public string Password
		{
			set
			{
				_params.Password = value;
			}
		}
		public AuthenticationType AuthenticationType
		{
			set
			{
				_params.AuthenticationType = value;
			}
		}
        public string Key
        {
            set
            {
            	// tunnel this through to modified key classes!
            	// rule will be if password is set but IdentifyFile
            	// is null
            	// then password is a base64 ProtectedData key
            	_params.Password = value;
            }
        }
        public Routrek.SSHC.SSHProtocol Protocol
        {
            set
            {
            	_params.Protocol = value;
            }
        }
        public string TerminalName
        {
            set
            {
            	_params.TerminalName = value;
            }
        }
        public int TerminalWidth
        {
            set
            {
            	_params.TerminalWidth = value;
            }
        }
        public int TerminalHeight
        {
            set
            {
            	_params.TerminalHeight = value;
            }
        }
		#endregion
		#region Public Enums
		#endregion
		#region Public Fields
		#endregion
		#region Public Delegates
		public delegate void DataIndicate(byte[] data);
        #endregion
		#region Public Events
	    public event DataIndicate OnDataIndicated;
        public event Action OnDisconnect;
		#endregion

		#region private fields
		TerminalEmulator m_term;

		#endregion
		#region Public Constructors
		public SSHProtocol(TerminalEmulator term)
		{

			m_term = term;
		}
		#endregion
		#region Public Methods
		/*
		public void setTerminalParams(string type, int rows, int cols)
		{
			_params = new SSHConnectionParameter();
			TerminalName = type;
            TerminalHeight = rows;
            TerminalWidth = cols;
		}

		public void setProtocolParams(
			AuthMethod authMethod,
			string userName,
			string pass,
			string key,
			bool SSH1)
		{
            if(userName==null)
            	userName="";
            if (userName == "") // can't do auto login without username
                authMethod = AuthMethod.KeyboardInteractive;
        	Username=userName;
            
            if(authMethod==AuthMethod.Password)
            {
                if(pass==null)
            	    pass="";
                if(pass=="")
                {
            	    AuthenticationType=AuthenticationType.KeyboardInteractive;
                }
                else
                {
            	    AuthenticationType=AuthenticationType.Password;
            	    Password=pass;
                }
            }
            if (SSH1)
            {
                Protocol = Routrek.SSHC.SSHProtocol.SSH1;
            }
            else
            {
                Protocol = Routrek.SSHC.SSHProtocol.SSH2;
            }
		}
		*/
		public void RequestData (byte[] data)
		{
			_pf.Transmit(data, 0, data.Length);
		}
		/*
		public void Connect (Socket s)
		{
			_params.WindowSize = 0x1000;
			_conn = SSHConnection.Connect(_params, this, s);
			_pf = _conn.OpenShell(this);
			SSHConnectionInfo ci = _conn.ConnectionInfo;
		}
		*/
		public void Connect()
		{
			_conn = ChameleonNetworking.Instance.Connection;

			this.OnDataIndicated += m_term.IndicateData;
			m_term.OnDataRequested += this.RequestData;

			m_term.Enabled = true;
			_pf = _conn.OpenShell(this);
		}

		public void Disconnect()
		{
			_pf.Close();

			m_term.Enabled = false;
			this.OnDataIndicated -= m_term.IndicateData;
			m_term.OnDataRequested -= this.RequestData;
		}
		
		public void OnData(byte[] data, int offset, int length)
		{
			if(OnDataIndicated!=null)
			{
				byte[] obuf = new byte[length];
				System.Array.Copy(data, offset, obuf, 0, obuf.Length);
				OnDataIndicated(obuf);
			}
		}

		public void OnDebugMessage(bool always_display, byte[] data) 
		{
            //Debug.WriteLine("DEBUG: "+ Encoding.Default.GetString(data));
		}

		public void OnIgnoreMessage(byte[] data)
		{
            //Debug.WriteLine("Ignore: "+ Encoding.Default.GetString(data));
		}

		public void OnAuthenticationPrompt(string[] msg)
		{
			//Debug.WriteLine("Auth Prompt "+msg[0]);
		}

		public void OnError(Exception error) 
		{
			//Debug.WriteLine("ERROR: "+ msg);
		}
		public void OnChannelError(Exception error)
		{
			//Debug.WriteLine("Channel ERROR: "+ error.Message);
		}
		public void OnChannelClosed()
		{
			//Debug.WriteLine("Channel closed");
			//_conn.Disconnect("");
			//_conn.AsyncReceive(this);

			Disconnect();
		}

		public void OnChannelEOF()
		{
			_pf.Close();
			//Debug.WriteLine("Channel EOF");
		}
		public void OnExtendedData(int type, byte[] data) 
		{
			//Debug.WriteLine("EXTENDED DATA");
		}
		public void OnConnectionClosed() 
		{
			//Debug.WriteLine("Connection closed");.
            if (OnDisconnect != null) OnDisconnect();
		}
		public void OnUnknownMessage(byte type, byte[] data) 
		{
			//Debug.WriteLine("Unknown Message " + type);
		}
		public void OnChannelReady() 
		{
			_ready = true;
		}

		public void OnChannelError(Exception error, string msg) 
		{
			//Debug.WriteLine("Channel ERROR: "+ msg);
		}

        public void OnError(Exception error, string msg)
        {
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
		#endregion
		#region Public Overrides
		#endregion
		#region Private Enums
		#endregion
		#region Private Fields
		private SSHConnectionParameter _params;
		private SSHConnection _conn;
		private bool _ready;
		public SSHChannel _pf;
		#endregion
    }
}
