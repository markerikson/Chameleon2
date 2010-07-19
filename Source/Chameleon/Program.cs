using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using Chameleon.Network;
using Chameleon.Features;
using CodeLite;
using DevInstinct.Patterns;

namespace Chameleon
{
	static class Program
	{
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main()
		{
			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);

			//string featurePermission = Networking.Instance.GetFeaturePermissions();			

			//ChameleonFeatures dummyFeatures = ChameleonFeatures.Feature1 | ChameleonFeatures.DragDropSnippets;
			//App.Configuration.PermittedFeatures = dummyFeatures;
			Form f = new ChameleonForm();
			Application.Run(f);

			Singleton<CtagsManagerWrapper>.Instance.CodeLiteParserEnd();
		}
	}
}
