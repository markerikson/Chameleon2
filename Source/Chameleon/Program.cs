using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using Chameleon.Network;
using Chameleon.Features;
using CodeLite;
using DevInstinct.Patterns;
using System.IO;
using ExceptionReporting;
using ExceptionReporting.Core;
using ExceptionReporting.Mail;
using ExceptionReporting.WinForms;
using System.Threading;


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

			Application.ThreadException += ApplicationThreadException;


			string appDataFolder = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData);
			Options.DataFolder = Path.Combine(appDataFolder, "Chameleon");

			if(!Directory.Exists(Options.DataFolder))
			{
				Directory.CreateDirectory(Options.DataFolder);
			}

			//string featurePermission = Networking.Instance.GetFeaturePermissions();			

			//ChameleonFeatures dummyFeatures = ChameleonFeatures.Feature1 | ChameleonFeatures.DragDropSnippets;
			//App.Configuration.PermittedFeatures = dummyFeatures;
			Form f = new ChameleonForm();
			Application.Run(f);

			Singleton<CtagsManagerWrapper>.Instance.CodeLiteParserEnd();
		}

		public static void ApplicationThreadException(object sender, ThreadExceptionEventArgs e)
		{
			ExceptionReporter er = new ExceptionReporter();

			ExceptionReporting.Core.ExceptionReportInfo conf = er.Config;

			conf.AppName = "Chameleon";
			conf.CompanyName = "ISquared Software";

			conf.MailMethod = ExceptionReportInfo.EmailMethod.SMTP;
			conf.ContactEmail = "mark@isquaredsoftware.com";
			conf.EmailReportAddress = "mark.erikson@gmail.com";
			conf.SmtpFromAddress = "crashreport@chameleon.isquaredsoftware.com";
			conf.ContactMessageTop = "Contact message top";
			conf.SmtpServer = "mail.chameleon.isquaredsoftware.com";
			conf.SmtpUsername = "crashreport@chameleon.isquaredsoftware.com";
			conf.SmtpPassword = "DUMMYPASS";
			
			conf.ShowLessMoreDetailButton = true;
			conf.ShowFullDetail = false;

			er.Show(e.Exception);

			Application.Exit();
		}

	}
}
