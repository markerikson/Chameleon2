using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Westwind.Tools;
using System.IO;
using Chameleon.Features;

namespace Chameleon
{
	public class App
	{
		public static Options Configuration;

		static App()
		{
			Configuration = (Options)Options.ReadKeysFromFile(Options.OptionsPath, typeof(Options));
		}
	}

	public class Options : wwAppConfiguration
	{
		private static string m_dataFolder;

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

		public ChameleonFeatures PermittedFeatures = ChameleonFeatures.NoFeature;

		public string LastHostname = "";
		public string LastUsername = "";
		
	}
}
