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
	public class OutputCollectingReader : ISSHConnectionEventReceiver, ISSHChannelEventReceiver
	{
		public SSHConnection _conn;
		public bool _ready;
		public string textReceived;
		public string finalOutput;
		public Action<string> callback;
		public OutputState state;
		public SSHChannel chan;
		public bool finished;

		public OutputCollectingReader()
		{
			state = OutputState.Ready;
			finished = false;
		}

		public void OnData(byte[] data, int offset, int length)
		{
			byte[] bytes = new byte[length];
			Buffer.BlockCopy(data, offset, bytes, 0, length);
			string text = Encoding.UTF8.GetString(bytes);
			textReceived += text;

			if(state == OutputState.Ready)
			{
				state = OutputState.Executing;
			}

			// if the parser says we've received everything, close this channel
			if(Singleton<OutputParser>.Instance.CheckOutput(this))
			{
				//chan.Close();
			}
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
			/*
			if(_conn != null)
			{
				_conn.Disconnect("");
			}
			*/
		}
		public void OnChannelEOF()
		{
			if(_pf != null)
			{
				_pf.Close();
			}

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
