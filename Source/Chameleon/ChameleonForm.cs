using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using ScintillaNet;
using ScintillaNet.Configuration;
using System.IO;
using Chameleon.GUI;

namespace Chameleon
{
	public partial class ChameleonForm : Form
	{
		private static bool m_appClosing = false;

		public static bool AppClosing
		{
			get { return m_appClosing; }
		}

		public ChameleonForm()
		{
			InitializeComponent();

			btnNewFile.Click += new EventHandler(OnNewFile);


			string testSourcePath = "d:\\projects\\temp\\fizzbuzz.cpp";
			if(File.Exists(testSourcePath))
			{
				string source = File.ReadAllText(testSourcePath);
				m_editors.CurrentEditor.Text = source;
			}		
		}

		void OnNewFile(object sender, EventArgs e)
		{
			m_editors.NewFile();
		}

		private void menuHelpAbout_Click(object sender, EventArgs e)
		{
			MessageBox.Show("Chameleon 2.0 alpha 0.0001");
		}

		private void menuFileOpenLocal_Click(object sender, EventArgs e)
		{
			m_editors.OpenFile(FileLocation.Local);
		}

		private void menuFileOpenRemote_Click(object sender, EventArgs e)
		{
			m_editors.OpenFile(FileLocation.Remote);
		}



		

		
	}
}
