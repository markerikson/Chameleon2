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
using CustomControls.TaskDialog;

namespace Chameleon.GUI
{
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
				HandleModifiedFile(editor);
			}
		}

		private void HandleModifiedFile(ChameleonEditor editor)
		{
			string message = string.Format("The file '{0}' has unsaved changes. Do you want to save "
										+ "the file before it is closed?", editor.Filename);

			TaskDialog td = new TaskDialog();

			td.WindowTitle = "Save Changes?";
			td.MainInstruction = message;
			td.UseCommandLinks = true;

			TaskDialogButton saveButton = new TaskDialogButton();
			saveButton.ButtonId = 101;
			saveButton.ButtonText = "Save and close this file";

			TaskDialogButton noSaveButton = new TaskDialogButton();
			noSaveButton.ButtonId = 102;
			noSaveButton.ButtonText = "Close this file, but don't save";

			TaskDialogButton cancelButton = new TaskDialogButton();
			cancelButton.ButtonId = 103;
			cancelButton.ButtonText = "Don't close this file";

			td.Buttons = new TaskDialogButton[] { saveButton, noSaveButton, cancelButton };

			int result = td.Show(this);

			switch(result)
			{
				case 101:
				{
					MessageBox.Show("Save clicked");
					break;
				}
				case 102:
				{
					MessageBox.Show("Don't save clicked");
					break;
				}
				case 103:
				{
					MessageBox.Show("Cancel clicked");
					break;
				}
				default:
				{
					MessageBox.Show("Something else clicked");
					break;
				}
			}
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


			FATabStripItem tabItem = new FATabStripItem(newFileTitle, editor);
			m_editorsToTabs[editor] = tabItem;
			m_tabsToEditors[tabItem] = editor;

			m_tabStrip.AddTab(tabItem, true);
		}


	}
}
