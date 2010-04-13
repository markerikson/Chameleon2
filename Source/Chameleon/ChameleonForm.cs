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
			
			editor1.SetDefaultEditorStyles();

			string testSourcePath = "d:\\projects\\temp\\fizzbuzz.cpp";
			if(File.Exists(testSourcePath))
			{
				string source = File.ReadAllText(testSourcePath);
				editor1.Text = source;
			}
		}

		
	}
}
