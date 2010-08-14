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

		public static string OptionsPath
		{
			get
			{
				string appDataFolder = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData);
				string optionsFile = "Chameleon.xml";
				string optionsPath = Path.Combine(appDataFolder, optionsFile);

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
