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

		private static string m_appName = "Chameleon";
		private static string m_companyName = "ISquared Software";
		private static string m_contactEmail = "mark@isquaredsoftware.com";
		private static string m_reportFromAddress = "crashreport@chameleon.isquaredsoftware.com";
		private static string m_reportSmtpServer = "mail.chameleon.isquaredsoftware.com";
		private static string m_reportSmtpUsername = "crashreport@chameleon.isquaredsoftware.com";
		private static string m_reportSmtpPassword = "DUMMYPASS";

		private static bool m_closeOnException = true;
		
		#endregion

		#region Properties
		public static string AppName
		{
			get { return m_appName; }
			set { m_appName = value; }
		}

		public static string CompanyName
		{
			get { return m_companyName; }
			set { m_companyName = value; }
		}

		public static string ContactEmail
		{
			get { return m_contactEmail; }
			set { m_contactEmail = value; }
		}

		public static string ReportFromAddress
		{
			get { return m_reportFromAddress; }
			set { m_reportFromAddress = value; }
		}

		public static string ReportSmtpServer
		{
			get { return m_reportSmtpServer; }
			set { m_reportSmtpServer = value; }
		}

		public static string ReportSmtpUsername
		{
			get { return m_reportSmtpUsername; }
			set { m_reportSmtpUsername = value; }
		}

		public static string ReportSmtpPassword
		{
			get { return m_reportSmtpPassword; }
			set { m_reportSmtpPassword = value; }
		}

		public static bool CloseOnException
		{
			get { return m_closeOnException; }
			set { m_closeOnException = value; }
		}

		#endregion

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

		public Options()
		{
		}

		public void SaveSettings()
		{
			WriteKeysToFile(OptionsPath);
		}

		public string FeaturePermissionsURL = "http://www.isquaredsoftware.com/iu/chameleonfeatures.php";

		public string StudentID = "s1278644";

		public ChameleonFeatures PermittedFeatures = ChameleonFeatures.DragDropSnippets;

		public string LastHostname = "";
		public string LastUsername = "";
		
	}
}
