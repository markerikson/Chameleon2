using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Chameleon;
using FarsiLibrary.Win;
using PSTaskDialog;

namespace Chameleon.GUI
{
	[Flags]
	public enum FileType
	{
		SourceFiles = 1 << 0,
		HeaderFiles = 1 << 1,
		AllFiles 	= 1 << 2,
		AllTypes	= 1 << 3,
	}

	public enum ModifiedFileAction
	{
		Close,
		Reload,
		Compile,
	}

	public enum ModifiedFileResult
	{
		Save,
		NoSave,
		Cancel,
	}

	public partial class EditorContainer : UserControl
	{
		private int m_fileNum;
		private ChameleonEditor m_currentEditor;

		private Dictionary<ChameleonEditor, FATabStripItem> m_editorsToTabs;
		private Dictionary<FATabStripItem, ChameleonEditor> m_tabsToEditors;

		public ChameleonEditor CurrentEditor
		{
			get { return m_currentEditor; }
		}


		public EditorContainer()
		{
			InitializeComponent();

			m_editorsToTabs = new Dictionary<ChameleonEditor, FATabStripItem>();
			m_tabsToEditors = new Dictionary<FATabStripItem, ChameleonEditor>();

			m_tabStrip.TabStripItemSelectionChanged += new TabStripItemChangedHandler(OnSelectedTabChanged);
			m_tabStrip.TabStripItemClosing += new TabStripItemClosingHandler(OnTabClosing);

			m_fileNum = 0;
			NewFile();
		}

		void OnTabClosing(TabStripItemClosingEventArgs e)
		{
			ChameleonEditor editor = m_tabsToEditors[e.Item];

			if(editor.Modified)
			{
				e.Cancel = !HandleModifiedFile(editor, ModifiedFileAction.Close);				
			}
		}

		// returns true if things were handled successfully, and the task should continue
		private bool HandleModifiedFile(ChameleonEditor editor, ModifiedFileAction fileAction)
		{
			string messageEnd = "";
			string saveButtonText = "";
			string noSaveButtonText = "";
			string cancelButtonText = "";

			List<string> buttons = new List<string>();
			Dictionary<int, ModifiedFileResult> buttonActions = new Dictionary<int, ModifiedFileResult>();

			switch(fileAction)
			{
				case ModifiedFileAction.Close:
				{
					messageEnd = "Do you want to save the file before it is closed?";
					saveButtonText = "Save this file, then close it";
					noSaveButtonText = "Don't save this file, but still close it";
					cancelButtonText = "Cancel closing, and leave this file open";

					buttons.Add(saveButtonText);
					buttons.Add(noSaveButtonText);				

					buttonActions[0] = ModifiedFileResult.Save;
					buttonActions[1] = ModifiedFileResult.NoSave;
	
					break;
				}
				case ModifiedFileAction.Reload:
				{
					if(editor.Location == FileLocation.Unknown)
					{
						return false;
					}

					messageEnd = "Do you want to revert to what was last saved?";
					noSaveButtonText = "Lose changes and reload this file";
					cancelButtonText = "Cancel reloading, and return to editing";

					buttons.Add(noSaveButtonText);

					buttonActions[0] = ModifiedFileResult.NoSave;
					break;
				}
				case ModifiedFileAction.Compile:
				{
					messageEnd = "Do you want to save this file and compile it?";
					saveButtonText = "Save and compile this file";
					cancelButtonText = "Cancel compiling, and return to editing";

					buttons.Add(saveButtonText);
					buttonActions[0] = ModifiedFileResult.Save;
					break;
				}
			}
			string message = string.Format("The file '{0}' has unsaved changes. {1}", editor.Filename, messageEnd);

			buttons.Add(cancelButtonText);
			buttonActions[buttonActions.Count] = ModifiedFileResult.Cancel;

			string commandButtonString = string.Join("|", buttons);

			int selectedButtonID = cTaskDialog.ShowCommandBox("Unsaved Changes", message, "",
									commandButtonString, false);
			ModifiedFileResult selectedAction = buttonActions[selectedButtonID];

			bool returnResult = false;

			switch(selectedAction)
			{
				case ModifiedFileResult.Save:
				{
					returnResult = SaveFile(editor, false, true, FileType.AllTypes);
					break;
				}
				case ModifiedFileResult.NoSave:
				{
					returnResult = true;
					break;
				}
				case ModifiedFileResult.Cancel:
				{
					// returnResult is already false, no actions needed
					break;
				}
			}

			return returnResult;
		}

		private void OnSelectedTabChanged(TabStripItemChangedEventArgs e)
		{
			m_currentEditor = m_tabsToEditors[e.Item];
		}

		public void NewFile()
		{
			m_fileNum++;

			string newFileTitle = string.Format("(?) <untitled> {0}", m_fileNum);

			ChameleonEditor editor = new ChameleonEditor();
			editor.ConfigurationManager.CustomLocation = "ConfigCPP.xml";
			editor.SetDefaultEditorStyles();
			editor.Dock = DockStyle.Fill;
			editor.Filename = newFileTitle;
			editor.ModifiedChanged += new EventHandler(OnEditorModifiedChanged);


			FATabStripItem tabItem = new FATabStripItem(newFileTitle, editor);
			m_editorsToTabs[editor] = tabItem;
			m_tabsToEditors[tabItem] = editor;

			m_tabStrip.AddTab(tabItem, true);
		}

		void OnEditorModifiedChanged(object sender, EventArgs e)
		{
			ChameleonEditor editor = sender as ChameleonEditor;

			if(m_editorsToTabs.ContainsKey(editor))
			{
				FATabStripItem tab = m_editorsToTabs[editor];

				string tabText = tab.Title;

				if(editor.Modified)
				{
					if(!tabText.EndsWith(" *"))
						tabText += " *";
				}
				else
				{
					if(tabText.EndsWith(" *"))
						tabText = tabText.Substring(0, tabText.Length - 2);
				}

				tab.Title = tabText;
			}
		}

		void CloseFile(ChameleonEditor editor)
		{
			if(editor == null)
			{
				editor = CurrentEditor;
			}

			bool continueClosing = HandleModifiedFile(editor, ModifiedFileAction.Close);

			if(!continueClosing)
			{
				return;
			}

			FATabStripItem tab = m_editorsToTabs[editor];

			if(m_tabStrip.Items.Count > 1 || ChameleonForm.AppClosing)
			{
				
				m_tabStrip.RemoveTab(tab);

				m_editorsToTabs.Remove(editor);
				m_tabsToEditors.Remove(tab);
				tab.Controls.Remove(editor);
				editor.Dispose();
			}
			// closing out the last buffer, reset it to act as a new one
			else
			{
				m_fileNum = 1;

				string newFileTitle = string.Format("(?) <untitled> {0}", m_fileNum);
				editor.Filename = newFileTitle;
				tab.Title = newFileTitle;
				editor.ResetEditor();
			}
		}

		public bool SaveFile(ChameleonEditor editor, bool saveAs, bool askLocalRemote, 
							FileType filterType)
		{
			return true;
		}


	}
}
