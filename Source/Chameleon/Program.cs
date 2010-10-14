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
using Chameleon.GUI;
using System.Diagnostics;

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

			//	show the splash form
			Splasher.Show();

			while(Splasher.MySplashForm == null)
			{
				Thread.Sleep(250);
			}

			Splasher.Status = "Starting up...";

			if(File.Exists("wyUpdate.exe"))
			{
				Splasher.Status = "Checking for updates...";

				ProcessStartInfo psi = new ProcessStartInfo();
				psi.FileName = "wyupdate.exe";
				psi.Arguments = "-quickcheck -justcheck -noerr";
				psi.CreateNoWindow = true;
				psi.UseShellExecute = true;

				Process proc = Process.Start(psi);

				proc.WaitForExit();

				int code = proc.ExitCode;
				proc.Close();

				// updates are available
				if(code == 2)
				{
					Splasher.Status = "Updates available.  Launching updater...";
					Thread.Sleep(1000);
					Splasher.Close();

					psi.Arguments = "-filetoexecute=Chameleon.exe";
					proc = Process.Start(psi);
					Environment.Exit(1);
				}
			}

			string appDataFolder = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData);
			Options.DataFolder = Path.Combine(appDataFolder, "Chameleon");

			// initialize the singleton
			Options options = App.GlobalSettings;

			

			if(!Directory.Exists(Options.DataFolder))
			{
				Directory.CreateDirectory(Options.DataFolder);
			}


			Splasher.Status = "Checking for new features...";

			string permissionsText = ChameleonNetworking.Instance.GetFeaturePermissions();

			if(permissionsText == null)
			{
				// do nothing - leave PermittedFeatures at default/last saved
			}
			else if(!String.IsNullOrWhiteSpace(permissionsText))
			{
				ChameleonFeatures enabledFeatures = Permissions.ParsePermissions(permissionsText);
				App.UserSettings.PermittedFeatures = enabledFeatures;
			}
			
			Splasher.Status = "Starting Chameleon...";

			Thread.Sleep(1000);
		
			Form f = new ChameleonForm();

			//	if the form is still shown...
			Splasher.Close();

			Application.Run(f);

			Singleton<CtagsManagerWrapper>.Instance.CodeLiteParserEnd();
		}

		public static void ApplicationThreadException(object sender, ThreadExceptionEventArgs e)
		{
			ExceptionReporter er = new ExceptionReporter();

			ExceptionReporting.Core.ExceptionReportInfo conf = er.Config;

			Dictionary<string, string> rs = Options.ReportSettings;

			conf.AppName = rs["appName"];
			conf.CompanyName = rs["companyName"];

			conf.MailMethod = ExceptionReportInfo.EmailMethod.SMTP;
			conf.ContactEmail = rs["contactEmail"];
			conf.SmtpFromAddress = rs["reportFromAddress"];
			conf.SmtpServer = rs["reportSmtpServer"];
			/*
			
			
			conf.SmtpUsername = Options.ReportSmtpUsername;
			conf.SmtpPassword = Options.ReportSmtpPassword;
			*/

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
