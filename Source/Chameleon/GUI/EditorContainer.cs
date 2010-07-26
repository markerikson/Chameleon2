using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Text;
using System.Windows.Forms;
using FarsiLibrary.Win;
using PSTaskDialog;
using ScintillaNet.Configuration;
using CU = Chameleon.Util;
using Chameleon.Util;
using Chameleon.Network;
using System.Drawing;
using ScintillaNet;
using Chameleon.Features.CodeRules;
using DevInstinct.Patterns;
using CodeLite;
using System.Threading;
using Chameleon.Parsing;

namespace Chameleon.GUI
{
	#region Enumerations
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
	#endregion

	public class TransparentPanel : Panel
	{
		protected override CreateParams CreateParams
		{
			get
			{
				CreateParams createParams = base.CreateParams;
				createParams.ExStyle |= 0x00000020; // WS_EX_TRANSPARENT
				return createParams;
			}
		}

		protected override void OnPaintBackground(PaintEventArgs e)
		{
			// Do not paint background.
		}
	}

	public partial class EditorContainer : UserControl
	{
		private int m_fileNum;
		private ChameleonEditor m_currentEditor;
		private bool m_closingTab;

		private ToolTip m_tooltip;
		private TransparentPanel m_transPanel;
		private Label label1;

		private bool m_draggingItem;
		private bool m_dragInitialized;
		
		private static string m_fileFilter;

		private Dictionary<ChameleonEditor, FATabStripItem> m_editorsToTabs;
		private Dictionary<FATabStripItem, ChameleonEditor> m_tabsToEditors;

		private static Dictionary<string, string> m_snippets;

		private CodeRuleManager m_ruleManager;
		private CtagsManagerWrapper cmw;

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

			m_snippets = new Dictionary<string, string>();
			m_snippets["for"] = "f";
			m_snippets["while"] = "w";
			m_snippets["if"] = "if";
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
			m_draggingItem = false;
			m_dragInitialized = false;

			m_tooltip = new ToolTip();
			m_tooltip.SetToolTip(m_tabStrip, "Testing");
			m_tooltip.Popup += new PopupEventHandler(OnTooltipPopup);

			m_transPanel = new TransparentPanel();
			label1 = new Label();
			
			m_transPanel.Parent = this;
			m_transPanel.AllowDrop = true;
			m_transPanel.BackColor = System.Drawing.Color.Maroon;
			m_transPanel.Controls.Add(this.label1);
			m_transPanel.Location = new System.Drawing.Point(12, 234);
			m_transPanel.Name = "panel1";
			m_transPanel.Size = new System.Drawing.Size(110, 58);
			m_transPanel.TabIndex = 2;
			m_transPanel.DragDrop += new System.Windows.Forms.DragEventHandler(transPanel_DragDrop);
			m_transPanel.DragEnter += new System.Windows.Forms.DragEventHandler(transPanel_DragEnter);
			m_transPanel.DragOver += new System.Windows.Forms.DragEventHandler(transPanel_DragOver);
			//m_transPanel.DragLeave += new System.EventHandler(transPanel_DragLeave);

			this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.label1.AutoSize = true;
			this.label1.BackColor = System.Drawing.Color.Red;
			this.label1.Location = new System.Drawing.Point(2, 2);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(34, 13);
			this.label1.TabIndex = 0;
			this.label1.Text = "Panel";

			m_ruleManager = Singleton<CodeRuleManager>.Instance;
			cmw = Singleton<CtagsManagerWrapper>.Instance;

			m_ruleManager.AddRules();
			

			m_fileNum = 0;
			NewFile();
		}
		

		
		

		#region Tab handlers
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
		#endregion

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
			string cppConfigXML = CU.Utilities.GetResource("Chameleon.ConfigCPP.xml");
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
				if(doSaveAs)
				{
					RemoteFileDialog rfd = RemoteFileDialog.Instance;
					rfd.Prepare(false, "");

					DialogResult dr = rfd.ShowDialog();

					if(dr == DialogResult.OK)
					{
						filename = rfd.SelectedFile.GetFullPath();
					}
					else
					{
						return false;
					}

					if(filename == "")
					{
						return false;
					}
				}
				else
				{
					filename = m_currentEditor.Filename;
				}

				FilePath fp = new FilePath(filename, PathFormat.Unix);

				Networking.Instance.SendFileContents(fp, fileContents);
				
			}
			// shouldn't be Unknown by this point

			editor.SetFileSaved(filename, location);

			// TODO Parse files saved remotely as well
			if(editor.FileLocation == FileLocation.Local)
			{
				//cmw.AddParserRequestSingleFile(editor.Filename);
				List<string> files = new List<string>();
				files.Add(editor.Filename);

				cmw.DeleteFilesTags(files);
				cmw.AddParserRequestSingleFile(files[0]);
				//cmw.RetagFiles(files, false);

				RunCodeRules(editor);
			}

			return true;
		}

		private void RunCodeRules(ChameleonEditor editor)
		{
			if(cmw.Parsing)
			{
				// TODO Get rid of this hack
				Thread.Sleep(50);
			}

			m_ruleManager.ClearErrors();
			editor.ClearErrors();

			List<Tag> functions = cmw.GetFunctions(editor.Filename, false);
			ANTLRParser parser = Singleton<ANTLRParser>.Instance;

			Range wholeFile = editor.GetRange();

			// run all global rules
			m_ruleManager.ExamineSource(editor, wholeFile, true);

			foreach(Tag fn in functions)
			//for(int i = 4; i < 5; i++ )
			{
				//Tag fn = functions[i];
				int fnStart = 0, fnOpen = 0, fnClose = 0;

				if(fn.kind == "prototype")// || fn.name != "main")
				{
					continue;
				}

				if(editor.Context.GetFunctionStartEnd(fn.lineNumber - 1, ref fnStart, ref fnOpen, ref fnClose))
				{
					Range r = new Range(fnStart, fnClose, editor);

					// run local rules for each function
					m_ruleManager.ExamineSource(editor, r, false);					
				}
			}


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
				else
				{
					return null;
				}
			}
			else if(location == FileLocation.Remote)
			{
				RemoteFileDialog rfd = RemoteFileDialog.Instance;

				rfd.Prepare(true, "");
				DialogResult dr = rfd.ShowDialog();

				if(dr == DialogResult.OK)
				{
					fileInfo.Filename.Assign(rfd.SelectedFile);
				}
				else
				{
					return null;
				}
			}

			fileInfo.Location = location;
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
					//MessageBox.Show("Remote file opening not implemented yet!");
					Networking.Instance.GetFileContents(fileInfo.Filename, ref fileContents);
					break;
					//return false;
				}
			}

			return true;
		}


		private void transPanel_DragEnter(object sender, DragEventArgs de)
		{
			if(m_draggingItem && m_dragInitialized)
			{
				return;
			}

			DataFormats.Format format = DataFormats.GetFormat("ChameleonSnippet");
			if(de.Data.GetDataPresent(format.Name))
			{
				de.Effect = DragDropEffects.Copy;

				Point clientPoint = CurrentEditor.PointToClient(new Point(de.X, de.Y));
				int pos = CurrentEditor.PositionFromPoint(clientPoint.X, clientPoint.Y);

				DropMarker dm = CurrentEditor.DropMarkers.Drop(pos);
			}
			else
			{
				de.Effect = DragDropEffects.None;
			}

			m_dragInitialized = true;
		}

		private void transPanel_DragOver(object sender, DragEventArgs de)
		{
			if(m_draggingItem)
			{
				Point clientPoint = CurrentEditor.PointToClient(new Point(de.X, de.Y));
				int pos = CurrentEditor.PositionFromPoint(clientPoint.X, clientPoint.Y);

				if(CurrentEditor.DropMarkers.MarkerStack.Count > 0)
				{
					DropMarker dm = CurrentEditor.DropMarkers.MarkerStack.Peek();

					dm.Change(pos, pos);
				}
			}
		}

		private void transPanel_DragDrop(object sender, DragEventArgs de)
		{
			if(m_draggingItem)
			{
				DataFormats.Format format = DataFormats.GetFormat("ChameleonSnippet");

				string snippetName = (string)de.Data.GetData(format.Name);

				Point clientPoint = CurrentEditor.PointToClient(new Point(de.X, de.Y));
				int pos = CurrentEditor.PositionFromPoint(clientPoint.X, clientPoint.Y);

				DropMarker dm = CurrentEditor.DropMarkers.MarkerStack.Peek();
				dm.Collect();

				m_transPanel.SendToBack();

				string snippetShortcut = m_snippets[snippetName];

				CurrentEditor.Focus();
				CurrentEditor.Snippets.InsertSnippet(snippetShortcut);
			}

		}

		public void StartDrag()
		{
			if(m_draggingItem)
			{
				return;
			}

			m_draggingItem = true;
			m_dragInitialized = false;

			m_transPanel.Bounds = CurrentEditor.Bounds;
			m_transPanel.BringToFront();
		}

		public void EndDrag()
		{
			m_draggingItem = false;
			m_transPanel.SendToBack();

			if(CurrentEditor.DropMarkers.MarkerStack.Count > 0)
			{
				DropMarker dm = CurrentEditor.DropMarkers.MarkerStack.Pop();

				try
				{
					dm.Collect();
				}
				catch(NullReferenceException nre)
				{
					Console.WriteLine(nre.ToString());
				}
			}
		}
	}
}
