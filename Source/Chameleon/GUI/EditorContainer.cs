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
using ScintillaNet;
using ScintillaNet.Configuration;
using System.Reflection;
using System.Text;
using System.IO;
using System.Xml;

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

			
			//System.Reflection.Assembly mscorlibAssembly = typeof(int).Assembly;
			//m_isDesignTime = mscorlibAssembly.FullName.ToUpper().EndsWith("B77A5C561934E089");

			m_fileNum = 0;
			NewFile();


		}

		public static string GetResource(string resourceName)
		{
			Assembly assembly = Assembly.GetExecutingAssembly();
			TextReader textReader = new StreamReader(assembly.GetManifestResourceStream(resourceName));
			string result = textReader.ReadToEnd();
			textReader.Close();

			return result;
		}

		void OnTabClosing(TabStripItemClosingEventArgs e)
		{
			ChameleonEditor editor = m_tabsToEditors[e.Item];

			if(editor.Modified)
			{
				if(HandleModifiedFile(editor, ModifiedFileAction.Close) == ModifiedFileResult.Cancel)
				{
					e.Cancel = true;
				}
			}
		}

		// returns true if things were handled successfully, and the task should continue
		private ModifiedFileResult HandleModifiedFile(ChameleonEditor editor, ModifiedFileAction fileAction)
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
					if(editor.FileLocation == FileLocation.Unknown)
					{
						return ModifiedFileResult.Cancel;
					}

					if(!editor.Modified)
					{
						return ModifiedFileResult.NoSave;
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

			ModifiedFileResult returnResult = ModifiedFileResult.Cancel;

			switch(selectedAction)
			{
				case ModifiedFileResult.Save:
				{
					bool saveResult = SaveFile(editor, false, true, FileType.AllTypes);

					returnResult = saveResult ? ModifiedFileResult.Save : ModifiedFileResult.Cancel;
					break;
				}
				case ModifiedFileResult.NoSave:
				{
					returnResult = ModifiedFileResult.NoSave;
					break;
				}
				case ModifiedFileResult.Cancel:
				{
					// returnResult is already cancel, no actions needed
					break;
				}
			}

			return returnResult;
		}

		private void OnSelectedTabChanged(TabStripItemChangedEventArgs e)
		{
			m_currentEditor = m_tabsToEditors[e.Item];
		}

		private void SelectEditor(ChameleonEditor editor)
		{
			FATabStripItem tab = m_editorsToTabs[editor];
			m_tabStrip.SelectedItem = tab;
			editor.Focus();
			m_currentEditor = editor;
		}

		public void NewFile()
		{
			m_fileNum++;

			string newFileTitle = string.Format("(?) <untitled> {0}", m_fileNum);

			ChameleonEditor editor = new ChameleonEditor();

			// load up our saved CPP config from the embedded resource and
			// tell the editor to use those settings
			string cppConfigXML = GetResource("Chameleon.ConfigCPP.xml");
			TextReader tr = new StringReader(cppConfigXML);
			Configuration config = new Configuration(tr, "cpp");
			editor.ConfigurationManager.Configure(config);
			
			editor.SetDefaultEditorStyles();
			editor.Dock = DockStyle.Fill;
			editor.Filename = newFileTitle;
			editor.ModifiedChanged += new EventHandler(OnEditorModifiedChanged);


			FATabStripItem tabItem = new FATabStripItem(newFileTitle, editor);
			m_editorsToTabs[editor] = tabItem;
			m_tabsToEditors[tabItem] = editor;
			editor.ParentTab = tabItem;

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

			ModifiedFileResult fileHandled = HandleModifiedFile(editor, ModifiedFileAction.Close);

			if(fileHandled == ModifiedFileResult.Cancel)
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

		public bool OpenFile(FileLocation location)
		{
			if(location == FileLocation.Unknown)
			{
				return false;
			}

			FileInformation info = GetFilenameToOpen(FileType.AllTypes, location);

			if(info == null)
			{
				return false;
			}

			return OpenSourceFile(info);
		}

		public FileInformation GetFilenameToOpen(FileType fileTypes, FileLocation location)
		{			
			if(m_currentEditor == null || location == FileLocation.Unknown)
			{
				return null;
			}

			FileInformation fileInfo = new FileInformation();
			string filter = ConstructFilterString(fileTypes);

			if(location == FileLocation.Local)
			{
				OpenFileDialog ofd = new OpenFileDialog();
				ofd.CheckFileExists = true;
				ofd.Multiselect = false;
				ofd.Filter = filter;

				if(ofd.ShowDialog() == DialogResult.OK)
				{
					fileInfo.Location = location;
					fileInfo.Filename = ofd.FileName;
				}
			}
			else if(location == FileLocation.Remote)
			{
				MessageBox.Show("Remote opening not implemented yet!");
				return null;
			}

			return fileInfo;
		}

		bool OpenSourceFile(FileInformation fileInfo)
		{
			string fileContents = "";
			string locationPrefix = (fileInfo.Location == FileLocation.Local) ? "(L)" : "(R)";

			ChameleonEditor alreadyOpenedFile = GetEditorByFilename(fileInfo.Filename);

			if(alreadyOpenedFile != null)
			{
				ModifiedFileResult mfr = HandleModifiedFile(alreadyOpenedFile, ModifiedFileAction.Reload);

				if(mfr == ModifiedFileResult.Cancel)
				{
					return false;
				}
				else if(mfr == ModifiedFileResult.NoSave)
				{
					SelectEditor(alreadyOpenedFile);
				}
			}

			if(!GetFileContents(fileInfo, ref fileContents))
			{
				return false;
			}

			ChameleonEditor destinationEditor = null;

			if(alreadyOpenedFile != null)
			{
				destinationEditor = alreadyOpenedFile;
			}
			else
			{
				// current buffer is empty and untouched, so load the file into it
				if( !m_currentEditor.Modified && 
					!m_currentEditor.HasBeenSaved() &&
					m_currentEditor.Text.Length == 0)
				{
					destinationEditor = m_currentEditor;
				}
				// need to create a new buffer for the file;
				else
				{
					NewFile();
					FATabStripItem newTab = m_tabStrip.Items[m_tabStrip.Items.Count - 1];
					destinationEditor = m_tabsToEditors[newTab];
				}
			}

			destinationEditor.LoadFileText(fileInfo, fileContents);
			SelectEditor(destinationEditor);
			destinationEditor.Modified = false;

			return true;
		}



		private string ConstructFilterString(FileType fileTypes)
		{
			string filterCPPFiles = "C++ source files (*.cpp, *.c)|*.cpp;*.c";
			string filterHeaderFiles = "C++ header files (*.h, *.hpp)|*.h;*.hpp";
			string filterAllFiles = "All files (*.*)|*.*";

			StringBuilder sb = new StringBuilder();

			if(fileTypes.HasFlag(FileType.AllTypes))
			{
				sb.Append(filterCPPFiles);
				sb.Append("|");
				sb.Append(filterHeaderFiles);
				sb.Append("|");
				sb.Append(filterAllFiles);
			}
			else if(fileTypes.HasFlag(FileType.SourceFiles))
			{
				sb.Append(filterCPPFiles);
			}
			else if(fileTypes.HasFlag(FileType.HeaderFiles))
			{
				sb.Append(filterHeaderFiles);
			}
			else if(fileTypes.HasFlag(FileType.AllFiles))
			{
				sb.Append(filterAllFiles);
			}

			return sb.ToString();
		}

		public ChameleonEditor GetEditorByFilename(string filename)
		{
			foreach(ChameleonEditor editor in m_editorsToTabs.Keys)
			{
				if(editor.Filename == filename)
				{
					return editor;
				}
			}

			return null;
		}

		private bool GetFileContents( FileInformation fileInfo,  ref string fileContents )
		{
			switch(fileInfo.Location)
			{
				case FileLocation.Unknown:
				{
					return false;
				}
				case FileLocation.Local:
				{
					if(File.Exists(fileInfo.Filename))
					{
						fileContents = File.ReadAllText(fileInfo.Filename);
					}
					break;
				}
				case FileLocation.Remote:
				{
					MessageBox.Show("Remote file opening not implemented yet!");
					return false;
				}
			}

			return true;
		}
	}
}
