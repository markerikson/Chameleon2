using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ScintillaNet;
using System.Drawing;

namespace Chameleon.GUI
{
	class ChameleonEditor : Scintilla
	{
		public ChameleonEditor()
		{
			//SetDefaultEditorStyles();
		}

		public void SetDefaultEditorStyles()
		{
			this.ConfigurationManager.Language = "cpp";

			IsBraceMatching = true;


			Styles.Default.FontName = "Courier New";
			Styles.Default.Size = 10.0f;
			Styles.BraceLight.ForeColor = Color.FromArgb(0, 0, 0);
			Styles.BraceLight.Bold = true;
			Styles.BraceLight.BackColor = Color.FromArgb(0, 255, 255);

			
			// comments are mid-green
			Styles[1].ForeColor = Color.FromArgb(0, 128, 0);
			Styles[2].ForeColor = Color.FromArgb(0, 128, 0);
			Styles[3].ForeColor = Color.FromArgb(0, 128, 0);

			// numbers are red
			Styles[4].ForeColor = Color.FromArgb(255, 0, 0);
			Styles[8].ForeColor = Color.FromArgb(255, 0, 0);

			// keywords are blue
			Styles[5].ForeColor = Color.FromArgb(0, 0, 255);
			Styles[5].Bold = true;

			// strings are teal
			Styles[6].ForeColor = Color.FromArgb(0, 128, 128);
			Styles[7].ForeColor = Color.FromArgb(0, 128, 128);
			Styles[12].ForeColor = Color.FromArgb(0, 128, 128);
			
			// preprocessor is purple
			Styles[9].ForeColor = Color.FromArgb(128, 0, 128);

			// operators are black
			Styles[10].ForeColor = Color.FromArgb(0, 0, 0);
			Styles[10].Bold = true;

			// identifiers are yellow for now
			//Styles[11].ForeColor = Color.FromArgb(255, 255, 0);

			// secondary keywords are bright green for now
			Styles[16].ForeColor = Color.FromArgb(0, 255, 0);
		}
	}
}
