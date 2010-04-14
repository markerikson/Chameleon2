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

namespace Chameleon
{
	public partial class ChameleonForm : Form
	{
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
			/*
			editor1.SetDefaultEditorStyles();

			
			*/
			
		}

		void OnNewFile(object sender, EventArgs e)
		{
			m_editors.NewFile();
		}



		

		
	}
}
