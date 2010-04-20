using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ScintillaNet;
using System.Drawing;
using System.IO;
using FarsiLibrary.Win;

namespace Chameleon.GUI
{
	public enum FileLocation
	{
		Local,
		Remote,
		Unknown
	}

	public class FileInformation
	{
		private FileLocation m_location;		
		private string m_filename;

		public string Filename
		{
			get { return m_filename; }
			set { m_filename = value; }
		}
		public Chameleon.GUI.FileLocation Location
		{
			get { return m_location; }
			set { m_location = value; }
		}

		public FileInformation()
		{
			m_location = FileLocation.Unknown;
			m_filename = "";
		}
	}

	public class ChameleonEditor : Scintilla
	{
		private FileInformation m_fileInfo;
		private FATabStripItem m_parentTab;

		public FATabStripItem ParentTab
		{
			get { return m_parentTab; }
			set { m_parentTab = value; }
		}		

		public Chameleon.GUI.FileLocation FileLocation
		{
			get { return m_fileInfo.Location; }
			set { m_fileInfo.Location = value; }
		}

		public string Filename
		{
			get { return m_fileInfo.Filename; }
			set { m_fileInfo.Filename = value; }
		}

		public ChameleonEditor()
		{
			//SetDefaultEditorStyles();
			m_fileInfo = new FileInformation();
			Filename = "";
			FileLocation = FileLocation.Unknown;
			
		}

		public void SetDefaultEditorStyles()
		{
			this.ConfigurationManager.Language = "cpp";

			Styles.ClearAll();

			IsBraceMatching = true;

			Margins.Margin0.Type = MarginType.Number;
			Margins.Margin0.Width = 40;


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

		public void ResetEditor()
		{
			Text = string.Empty;
			Filename = string.Empty;
			FileLocation = FileLocation.Unknown;
			this.IsReadOnly = false;
			this.UndoRedo.EmptyUndoBuffer();
		}

		public void LoadFileText(FileInformation fileInfo, string text)
		{
			ResetEditor();
			this.Text = text;
			this.Filename = fileInfo.Filename;
			this.FileLocation = fileInfo.Location;

			string filename = Path.GetFileName(this.Filename);

			string indicator = (this.FileLocation == FileLocation.Local) ? "L" : "R";

			string tabText = string.Format("({0}) {1}", indicator, filename);
			m_parentTab.Title = tabText;


			// do stuff with EOL here?

			UndoRedo.EmptyUndoBuffer();

		}

		public bool HasBeenSaved()
		{
			bool realFilename = (m_fileInfo.Filename.IndexOf("<untitled>") == -1);
			bool realLocation = (m_fileInfo.Location != FileLocation.Unknown);

			return realFilename && realLocation;
		}
	}
}
