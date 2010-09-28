using System;
using System.Threading;
using System.Windows.Forms;

namespace Chameleon.GUI
{
	public class Splasher
	{
		public static SplashForm MySplashForm = null;
		static Thread MySplashThread = null;

		//	internally used as a thread function - showing the form and
		//	starting the messageloop for it
		static void ShowThread() 
		{
			MySplashForm = new SplashForm();
			Application.Run(MySplashForm);
		}

		//	public Method to show the SplashForm
		static public void Show() 
		{
			if (MySplashThread != null)
				return;

			MySplashThread = new Thread(new ThreadStart(Splasher.ShowThread));
			MySplashThread.IsBackground = true;
			MySplashThread.SetApartmentState(ApartmentState.STA);
			MySplashThread.Start();
		}

		//	public Method to hide the SplashForm
		static public void Close() 
		{
			if (MySplashThread == null) return;
			if (MySplashForm == null) return;

			try 
			{
				MySplashForm.Invoke(new MethodInvoker(MySplashForm.Close));
			}
			catch (Exception) 
			{
			}
			MySplashThread = null;
			MySplashForm = null;
		}

		//	public Method to set or get the loading Status
		static public string Status 
		{
			set 
			{
				if (MySplashForm == null) 
				{
					return;
				}

				MySplashForm.StatusInfo = value;
			}
			get 
			{
				if (MySplashForm == null) 
				{
					throw new InvalidOperationException("Splash Form not on screen");
				}
				return MySplashForm.StatusInfo;
			}
		}
	}
}
