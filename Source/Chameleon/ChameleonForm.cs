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

			menuEditUndo.ShortcutKeyDisplayString = "Ctrl+Z";
			menuEditCopy.ShortcutKeyDisplayString = "Ctrl+C";
			menuEditPaste.ShortcutKeyDisplayString = "Ctrl+V";
		}

		void OnNewFile(object sender, EventArgs e)
		{
			m_editors.NewFile();
		}

		private void menuHelpAbout_Click(object sender, EventArgs e)
		{
			MessageBox.Show("Chameleon 2.0 alpha 0.0001");
		}

		private void ChameleonForm_FormClosing(object sender, FormClosingEventArgs e)
		{
			m_appClosing = true;
			if(!m_editors.CloseAllFiles())
			{
				e.Cancel = true;
			}

			m_appClosing = false;
		}


		#region File Menu handlers
		private void menuFileOpenLocal_Click(object sender, EventArgs e)
		{
			m_editors.OpenFile(FileLocation.Local);
		}

		private void menuFileOpenRemote_Click(object sender, EventArgs e)
		{
			m_editors.OpenFile(FileLocation.Remote);
		}

		private void menuFileSave_Click(object sender, EventArgs e)
		{
			ChameleonEditor editor = m_editors.CurrentEditor;
			m_editors.SaveFile(editor, editor.FileLocation, false, true);
		}

		private void menuFileSaveAsLocal_Click(object sender, EventArgs e)
		{
			ChameleonEditor editor = m_editors.CurrentEditor;
			m_editors.SaveFile(editor, FileLocation.Local, true, false);
		}

		private void menuFileSaveAsRemote_Click(object sender, EventArgs e)
		{
			ChameleonEditor editor = m_editors.CurrentEditor;
			m_editors.SaveFile(editor, FileLocation.Remote, true, false);
		}

		private void menuFileClose_Click(object sender, EventArgs e)
		{
			m_editors.CloseFile(m_editors.CurrentEditor);
		}

		private void menuFileCloseAll_Click(object sender, EventArgs e)
		{
			m_editors.CloseAllFiles();
		}
		#endregion

		#region Edit Menu handlers
		private void menuEditUndo_Click(object sender, EventArgs e)
		{
			m_editors.CurrentEditor.UndoRedo.Undo();
		}

		private void menuEditRedo_Click(object sender, EventArgs e)
		{
			m_editors.CurrentEditor.UndoRedo.Redo();
		}

		private void menuEditCut_Click(object sender, EventArgs e)
		{
			m_editors.CurrentEditor.Clipboard.Cut();
		}

		private void menuEditCopy_Click(object sender, EventArgs e)
		{
			m_editors.CurrentEditor.Clipboard.Copy();
		}

		private void menuEditPaste_Click(object sender, EventArgs e)
		{
			m_editors.CurrentEditor.Clipboard.Paste();
		}


		#endregion

		

		

	}
}
