using System.Text;
using Routrek.SSHC;

namespace Chameleon.Extensions.Networking
{	
	public static class SSHChannelExtensions
	{
		public static void Transmit(this SSHChannel chan, string text)
		{
			byte[] data = Encoding.Default.GetBytes(text);
			chan.Transmit(data, 0, data.Length);
		}
	}
}