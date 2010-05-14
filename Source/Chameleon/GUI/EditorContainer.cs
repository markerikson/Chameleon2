using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Text;
using System.Windows.Forms;
using FarsiLibrary.Win;
using PSTaskDialog;
using ScintillaNet.Configuration;
using Chameleon.Util;
using System.Drawing;

namespace Chameleon.GUI
{
	
	public enum FileType
	{
		SourceFiles = 0,
		HeaderFiles = 1,
		TextFiles = 2,
		AllFiles = 3,
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
		private bool m_closingTab;

		private ToolTip m_tooltip;
		
		private static string m_fileFilter;

		private Dictionary<ChameleonEditor, FATabStripItem> m_editorsToTabs;
		private Dictionary<FATabStripItem, ChameleonEditor> m_tabsToEditors;

		static EditorContainer()
		{
			string filterCPPFiles = "C++ source files (*.cpp, *.c)|*.cpp;*.c";
			string filterHeaderFiles = "C++ header files (*.h, *.hpp)|*.h;*.hpp";
			string filterTextFiles = "Text files (*.txt)|*.txt";
			string filterAllFiles = "All files (*.*)|*.*";

			StringBuilder sb = new StringBuilder();

			sb.Append(filterCPPFiles);
			sb.Append("|");
			sb.Append(filterHeaderFiles);
			sb.Append("|");
			sb.Append(filterTextFiles);
			sb.Append("|");
			sb.Append(filterAllFiles);

			m_fileFilter = sb.ToString();
		}

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

			m_closingTab = false;

			m_tooltip = new ToolTip();
			m_tooltip.SetToolTip(m_tabStrip, "Testing");
			m_tooltip.Popup += new PopupEventHandler(OnTooltipPopup);

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
			if(m_closingTab)
			{
				m_closingTab = false;
				return;
			}

			// we want to handle the closing ourselves
			m_closingTab = true;
			e.Cancel = true;

			ChameleonEditor editor = m_tabsToEditors[e.Item];
			CloseFile(editor);

			m_closingTab = false;
		}

		void OnTooltipPopup(object sender, PopupEventArgs e)
		{
			Point translated = m_tabStrip.PointToClient(Cursor.Position);
			FATabStripItem tab = m_tabStrip.GetTabItemByPoint(translated);

			if(tab != null)
			{
				ChameleonEditor editor = m_tabsToEditors[tab];
				m_tooltip.SetToolTip(m_tabStrip, editor.Filename);
			}
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

			string newFileTitle = string.Format("<untitled> {0}", m_fileNum);

			ChameleonEditor editor = new ChameleonEditor();

			// load up our saved CPP config from the embedded resource and
			// tell the editor to use those settings
			string cppConfigXML = GetResource("Chameleon.ConfigCPP.xml");
			TextReader tr = new StringReader(cppConfigXML);
			Configuration config = new Configuration(tr, "cpp");
			editor.ConfigurationManager.Configure(config);
			
			editor.SetDefaultEditorStyles();
			editor.Dock = DockStyle.Fill;

			FATabStripItem tabItem = new FATabStripItem("empty tab title", editor);
			m_editorsToTabs[editor] = tabItem;
			m_tabsToEditors[tabItem] = editor;
			editor.ParentTab = tabItem;

			editor.Filename = newFileTitle;

			m_tabStrip.AddTab(tabItem, true);
		}
		

		public bool CloseFile(ChameleonEditor editor)
		{
			if(editor == null)
			{
				editor = CurrentEditor;
			}

			ModifiedFileResult fileHandled = HandleModifiedFile(editor, ModifiedFileAction.Close);

			if(fileHandled == ModifiedFileResult.Cancel)
			{
				return false;
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

				editor.ResetEditor();

				string newFileTitle = string.Format("<untitled> {0}", m_fileNum);
				editor.Filename = newFileTitle;				
			}

			return true;
		}

		public bool CloseAllFiles()
		{
			int numEditors = m_editorsToTabs.Count;

			for(int i = 0; i < numEditors; i++)
			{
				if(!CloseFile(m_currentEditor))
				{
					return false;
				}
			}

			return true;
		}


		// returns true if things were handled successfully, and the task should continue
		private ModifiedFileResult HandleModifiedFile(ChameleonEditor editor, ModifiedFileAction fileAction)
		{
			if(!editor.Modified)
			{
				return ModifiedFileResult.NoSave;
			}

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
					bool saveResult = SaveFile(editor, editor.FileLocation, false, true);

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

		public bool SaveFile(ChameleonEditor editor, FileLocation suggestedLocation,
								bool saveAs, bool askLocalRemote)
		{
			bool doSaveAs;
			FileLocation location = FileLocation.Unknown;

			if(suggestedLocation == FileLocation.Unknown)
			{
				doSaveAs = true;
			}
			else
			{
				doSaveAs = saveAs || !m_currentEditor.HasBeenSaved();
			}
						
			string filename = "";

			if(doSaveAs && suggestedLocation == FileLocation.Unknown)
			{
				string message = "Where should this file be saved?";

				List<string> buttons = new List<string>();
				buttons.Add("Locally (on the computer you're using)");
				buttons.Add("Remotely (on the server)");
				buttons.Add("Cancel");

				int button = cTaskDialog.ShowCommandBox("Save File Location", message, "",
					string.Join("|", buttons), false);
				location = (FileLocation)button;
			}
			else
			{
				location = suggestedLocation;
			}

			if(location == FileLocation.Unknown)
			{
				return false;
			}

			string fileContents = m_currentEditor.Text;

			if(location == FileLocation.Local)
			{
				if(doSaveAs)
				{
					SaveFileDialog sfd = new SaveFileDialog();
					sfd.Title = "Save File As";
					sfd.Filter = m_fileFilter;

					if(sfd.ShowDialog() != DialogResult.OK)
					{
						return false;
					}

					// SFD automatically appends the appropriate extension if necessary
					filename = sfd.FileName;					
				}
				else
				{
					filename = m_currentEditor.Filename;
				}

				if(filename == "")
				{
					return false;
				}

				File.WriteAllText(filename, fileContents);
			}
			else if(location == FileLocation.Remote)
			{
				MessageBox.Show("Remote saving not implemented yet!");
				return false;
			}
			// shouldn't be Unknown by this point

			editor.SetFileSaved(filename, location);

			return true;
		}

		public bool OpenFile(FileLocation location)
		{
			if(location == FileLocation.Unknown)
			{
				return false;
			}

			FileInformation info = GetFilenameToOpen(location);

			if(info == null)
			{
				return false;
			}

			return OpenSourceFile(info);
		}

		public FileInformation GetFilenameToOpen(FileLocation location)
		{			
			if(m_currentEditor == null || location == FileLocation.Unknown)
			{
				return null;
			}

			FileInformation fileInfo = new FileInformation();

			if(location == FileLocation.Local)
			{
				OpenFileDialog ofd = new OpenFileDialog();
				ofd.CheckFileExists = true;
				ofd.Multiselect = false;
				ofd.Filter = m_fileFilter;

				if(ofd.ShowDialog() == DialogResult.OK)
				{
					fileInfo.Location = location;

					fileInfo.Filename.Assign(ofd.FileName, PathFormat.Windows);
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

			ChameleonEditor alreadyOpenedFile = GetEditorByFilename(fileInfo.Filename.GetFullPath());

			if(alreadyOpenedFile != null)
			{
				ModifiedFileResult mfr = HandleModifiedFile(alreadyOpenedFile, ModifiedFileAction.Reload);

				// should only have these two choices returned for the Reload case
				if(mfr == ModifiedFileResult.Cancel)
				{
					return false;
				}
				else if(mfr == ModifiedFileResult.NoSave)
				{
					SelectEditor(alreadyOpenedFile);
					return true;
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
					string fullFilename = fileInfo.Filename.GetFullPath();
					if(File.Exists(fullFilename))
					{
						fileContents = File.ReadAllText(fullFilename);
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
