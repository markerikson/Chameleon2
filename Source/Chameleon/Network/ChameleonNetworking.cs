using System;
using DevInstinct.Patterns;
using Piccolo.Common;
using Routrek.SSHC;
using Chameleon.Extensions.Networking;
using System.Threading;

namespace Chameleon.Network
{
	public class ChameleonNetworking : Networking
	{
		private OutputParser m_outputParser;

		private ChameleonNetworking()
		{
			m_outputParser = Singleton<OutputParser>.Instance;
		}

		public new static ChameleonNetworking Instance
		{
			get
			{
				return Singleton<ChameleonNetworking>.Instance;
			}
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
