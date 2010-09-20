using System;
using System.Collections.Generic;
using System.Text;

namespace de.mud.terminal
{
	public class SSHBuffer : vt320
	{
		public delegate void DataRequest(byte[] data);
		public event DataRequest OnDataRequested;

		public SSHBuffer()
		{
			this.TerminalID = "xterm";
		}

		public override void write(sbyte[] b)
		{
			if(OnDataRequested != null)
			{
				byte[] bytes = new byte[b.Length];
				Buffer.BlockCopy(b, 0, bytes, 0, b.Length);

				OnDataRequested(bytes);
			}
		}

		public void InputReceived(byte[] data)
		{
			string text = Encoding.UTF8.GetString(data);

			putString(text);
		}
	}
}
