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

			conf.AppName = Options.AppName;
			conf.CompanyName = Options.CompanyName;

			conf.MailMethod = ExceptionReportInfo.EmailMethod.SMTP;
			conf.ContactEmail = Options.ContactEmail;
			conf.SmtpFromAddress = Options.ReportFromAddress;
			conf.SmtpServer = Options.ReportSmtpServer;
			conf.SmtpUsername = Options.ReportSmtpUsername;
			conf.SmtpPassword = Options.ReportSmtpPassword;
			
			conf.ShowLessMoreDetailButton = true;
			conf.ShowFullDetail = false;

			er.Show(e.Exception);

			if(Options.CloseOnException)
			{
				Application.Exit();
			}
			else
			{
				// we presumably set this to false because it was something
				// we threw ourselves - set it back to true for future
				// unexpected exceptions
				Options.CloseOnException = true;
			}
			
		}

	}
}
