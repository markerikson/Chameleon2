using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Routrek.Crypto;
using Routrek.SSHC;
using Routrek.SSHCV1;
using Routrek.SSHCV2;
using Routrek.Toolkit;
using Routrek.PKI;
using System.Diagnostics;
using DevInstinct.Patterns;

namespace Chameleon.Network
{
	public enum OutputState
	{
		Ready,
		Executing,
		Busy,
		Finished,
	}

	public class ConsoleReader : ISSHConnectionEventReceiver, ISSHChannelEventReceiver
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

	public class NullReader : ISSHConnectionEventReceiver, ISSHChannelEventReceiver
	{
		public SSHConnection _conn;
		public bool _ready;

		public void OnData(byte[] data, int offset, int length)
		{
		}
		public void OnDebugMessage(bool always_display, byte[] data)
		{
		}
		public void OnIgnoreMessage(byte[] data)
		{
		}
		public void OnAuthenticationPrompt(string[] msg)
		{
		}

		public void OnError(Exception error, string msg)
		{
		}
		public void OnChannelClosed()
		{
			_conn.Disconnect("");
			//_conn.AsyncReceive(this);
		}
		public void OnChannelEOF()
		{
			_pf.Close();
		}
		public void OnExtendedData(int type, byte[] data)
		{
		}
		public void OnConnectionClosed()
		{
		}
		public void OnUnknownMessage(byte type, byte[] data)
		{
		}
		public void OnChannelReady()
		{
			_ready = true;
		}
		public void OnChannelError(Exception error, string msg)
		{
		}
		public void OnMiscPacket(byte type, byte[] data, int offset, int length)
		{
		}

		public PortForwardingCheckResult CheckPortForwardingRequest(string host, int port, string originator_host, int originator_port)
		{
			PortForwardingCheckResult r = new PortForwardingCheckResult();
			r.allowed = false;
			r.channel = null;
			return r;
		}
		public void EstablishPortforwarding(ISSHChannelEventReceiver rec, SSHChannel channel)
		{
			_pf = channel;
		}

		public SSHChannel _pf;
	}

	
}
