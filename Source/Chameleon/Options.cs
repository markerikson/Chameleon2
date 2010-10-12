using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Westwind.Tools;
using System.IO;
using Chameleon.Features;
using System.Reflection;
using Chameleon.Network;
using DevInstinct.Patterns;

namespace Chameleon
{
	public class App
	{
		private static Options m_config;

		public static Options Configuration
		{
			get
			{
				return m_config;
			}
		}

		public static Networking Networking
		{
			get
			{
				return Singleton<ChameleonNetworking>.Instance;
			}
		}

		static App()
		{
			m_config = (Options)Options.ReadKeysFromFile(Options.OptionsPath, typeof(Options));
		}
	}

	public class Options : wwAppConfiguration
	{
		#region Private static fields
		private static string m_dataFolder;

		private static Dictionary<string, string> m_reportSettings;

		static Options()
		{
			m_reportSettings = new Dictionary<string, string>();
			m_reportSettings["appName"] = "Chameleon";
			m_reportSettings["companyName"] = "ISquared Software";
			m_reportSettings["contactEmail"] = "mark@isquaredsoftware.com";
			m_reportSettings["reportFromAddress"] = "crashreport@chameleon.isquaredsoftware.com";
			m_reportSettings["reportSmtpServer"] = "mail.chameleon.isquaredsoftware.com";
			m_reportSettings["reportSmtpUsername"] = "crashreport@chameleon.isquaredsoftware.com";
			m_reportSettings["reportSmtpPassword"] = "DUMMYPASS";
		}

		

		private static bool m_closeOnException = true;
		
		#endregion

		#region Properties
		public static Dictionary<string, string> ReportSettings
		{
			get
			{
				return m_reportSettings;
			}
		}

		public static bool CloseOnException
		{
			get { return m_closeOnException; }
			set { m_closeOnException = value; }
		}

		public static string DataFolder
		{
			get
			{
				if(string.IsNullOrWhiteSpace(m_dataFolder))
				{
					return Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData);
				}

				return m_dataFolder;
			}
			set
			{
				m_dataFolder = value;
			}
		}

		public static string OptionsPath
		{
			get
			{
				//string appDataFolder = ChameleonForm.DataFolder;
				string optionsFile = "Chameleon.xml";
				string optionsPath = Path.Combine(DataFolder, optionsFile);

				return optionsPath;
			}
		}

		#endregion

		

		public Options()
		{
		}

		public void SaveSettings()
		{
			WriteKeysToFile(OptionsPath);
		}

		// The URL that Chameleon tries to get permissions data from
		public string FeaturePermissionsURL = "";//"http://www.isquaredsoftware.com/chameleon/chameleonfeatures.php";

		// The alternate ID to use when checking permissions (if they're not on a campus box, for example)
		public string CustomStudentID = "";

		// Assume this is allowed by default
		public ChameleonFeatures PermittedFeatures = ChameleonFeatures.None;

		public string LastHostname = "";
		public string LastUsername = "";
		
	}
}
