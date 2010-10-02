using System;
using DevInstinct.Patterns;
using Piccolo.Common;
using Routrek.SSHC;
using Chameleon.Extensions.Networking;
using System.Threading;
using Microsoft.VisualBasic;
using System.Windows.Forms;
using Chameleon.GUI;

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
			// We start by assuming the current login name will be in the server,
			// which it should be if this is a campus box.  
			string studentID = Environment.UserName;
			bool studentIsCustom = false;

			// However, if we previously had a custom name entered, we use that instead
			if(App.Configuration.CustomStudentID != "")
			{
				studentID = App.Configuration.CustomStudentID;
				studentIsCustom = true;
			}

			string baseURL = App.Configuration.FeaturePermissionsURL;
			string featureText = "";

			HttpHelper http = new HttpHelper();

			bool validStudentFound = false;
			

			// retry three times
			for(int i = 0; i < 3 && !validStudentFound; i++)
			{
				string escapedStudentID = System.Uri.EscapeDataString(studentID);
				string permissionsURL = baseURL + "?student=" + escapedStudentID;
				
				try
				{
					featureText = http.HttpStringGet(permissionsURL);

					if(featureText == "studentDoesNotExist")
					{
						string prompt = string.Format("The ID '{0}' was not found on the server.  If this is"
													  + " a campus computer, click \"Cancel\" and please contact your professor."
													  + " Otherwise, enter your network login ID:", studentID);

						SplashForm sf = Splasher.MySplashForm;
						int x = sf.Location.X + 70;
						int y = sf.Location.Y + sf.Height + 5;

						string input = Interaction.InputBox(prompt, "Student ID Not Found", "", x, y);

						if(input == "")
						{
							// they either clicked "Cancel" or hit enter with an empty box - bail out
							Application.Exit();
						}
						else
						{
							studentID = input;
							studentIsCustom = true;
						}
					}
					else
					{
						if(featureText.IndexOf("Error") != -1)
						{
							return null;
						}
						else
						{
							validStudentFound = true;
						}						
					}					
				}
				catch(Exception e)
				{
					// do nothing, just return an empty string
				}
			}

			if(validStudentFound && studentIsCustom)
			{
				App.Configuration.CustomStudentID = studentID;
			}
			else
			{
				App.Configuration.CustomStudentID = "";
			}

			return featureText;
		}
	}
}
