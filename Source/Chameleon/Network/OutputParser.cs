using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
//using WalburySoftware;
using libVT100;

namespace Chameleon.Network
{
	public class OutputParser
	{
		private List<OutputCollectingReader> m_readers;
		private IAnsiDecoder vt;
		private AnsiDecoderClient client;
		private StringBuilder sb;

		static readonly object _locker = new object();


		private OutputParser()
		{
			m_readers = new List<OutputCollectingReader>();
			vt = new AnsiDecoder();
			client = new AnsiDecoderClient();

			sb = new StringBuilder();

			vt.Subscribe(client);
			client.Characters += new libVT100.AnsiDecoderClient.CharactersDelegate(VT100CharactersReceived);
		}

		public void AddReader(OutputCollectingReader r)
		{
			m_readers.Add(r);
		}

		public bool CheckOutput(OutputCollectingReader r)
		{
			bool outputComplete = false;

			if(r.state == OutputState.Executing) 
			{
				int startIndex = r.textReceived.IndexOf(Networking.StartToken + "\r");

				if(startIndex != -1)
				{					
					// skip past the start token and the "\r" that should be after it
					r.textReceived = r.textReceived.Substring(startIndex + Networking.StartToken.Length + 2);
					r.state = OutputState.Busy;
				}
			}

			if(r.state == OutputState.Busy) 
			{
				int endIndex = r.textReceived.IndexOf(Networking.EndToken);

				if(endIndex != -1) 
				{
					// remove the end token and everything after it
					r.textReceived = r.textReceived.Substring(0, endIndex);
					r.state = OutputState.Finished;
				}
			}

			// if we're ready to finish, send the received text back to whoever
			// supplied the callback function
			if(r.state == OutputState.Finished && !r.finished)
			{
				if(r.textReceived != "")
				{
					lock(_locker)
					{
						sb.Clear();

						byte[] bytes = Encoding.UTF8.GetBytes(r.textReceived);
						vt.Input(bytes);

						r.finalOutput = sb.ToString();
					}

					if(r.callback != null)
					{
						r.callback(r.finalOutput);
					}
				}

				r.finished = true;				
				
				m_readers.Remove(r);
				outputComplete = true;
			}

			return outputComplete;
		}

		public void VT100CharactersReceived(AnsiDecoderClient _client, char[] _chars)
		{
			string s = new string(_chars);
			sb.Append(s);
			//Console.WriteLine(s);
		}
		/*
		void ucp_ParserEvent(object Sender, TerminalEmulator.ParserEventArgs e)
		{
			Console.WriteLine(e.CurSequence);
		}
		*/

	}
}
