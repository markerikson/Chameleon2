using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using ArtisticStyle;
using Chameleon.Features;
using Chameleon.GUI;
using Chameleon.Network;
using Chameleon.Util;
using CodeLite;
using DevInstinct.Patterns;
using Guifreaks.NavigationBar;
using ScintillaNet;
using SSHClient;

namespace Chameleon
{
	public partial class ChameleonForm : Form
	{
		#region Private fields
		private static bool m_appClosing = false;

		private ChameleonNetworking m_networking;
		private SSHProtocol m_sshProtocol;

		private CtagsManagerWrapper cmw;
		private AStyleInterface m_astyle;

		private bool parserInitialized;
		private bool m_clickedSnippet;
		
		#endregion

		#region properties
		public static bool AppClosing
		{
			get { return m_appClosing; }
		}
		#endregion

		#region Form construction
		

		public ChameleonForm()
		{
			InitializeComponent();

			this.m_editors = new Chameleon.GUI.EditorContainer();
			this.splitEditorTerminal.Panel1.Controls.Add(this.m_editors);
			this.m_editors.Dock = System.Windows.Forms.DockStyle.Fill;
			this.m_editors.Location = new System.Drawing.Point(0, 0);
			this.m_editors.Name = "m_editors";
			this.m_editors.Size = new System.Drawing.Size(660, 261);
			this.m_editors.TabIndex = 4;

			FormFontFixer.Fix(this);

			Options options = App.Configuration;

			menuEditUndo.ShortcutKeyDisplayString = "Ctrl+Z";
			menuEditRedo.ShortcutKeyDisplayString = "Ctrl+Y";
			menuEditCopy.ShortcutKeyDisplayString = "Ctrl+C";
			menuEditCut.ShortcutKeyDisplayString = "Ctrl+X";
			menuEditPaste.ShortcutKeyDisplayString = "Ctrl+V";

			toolTextPassword.TextBox.UseSystemPasswordChar = true;

			m_networking = ChameleonNetworking.Instance;

			toolStatusConnected.Text = "Disconnected";
			toolHostDisconnect.Enabled = false;

			m_sshProtocol = new SSHProtocol(terminalEmulator1);

			m_astyle = new AStyleInterface();
			m_astyle.SetDefaultChameleonStyleOptions();

			RemoteFileDialog rfd = Singleton<RemoteFileDialog>.Instance;
			rfd.Networking = ChameleonNetworking.Instance;

			/*
			string cppSourceFilter = "C++ source files (*.c, *.cpp)";
			string cppHeaderFilter = "C++ header files (*.h, *.hpp)";

			List<string> sourceExtensions = new List<string>();
			sourceExtensions.Add("cpp");
			sourceExtensions.Add("c");

			List<string> headerExtensions = new List<string>();
			headerExtensions.Add("h");
			headerExtensions.Add("hpp");

			rfd.AddFileType(cppSourceFilter, sourceExtensions);
			rfd.AddFileType(cppHeaderFilter, headerExtensions);
			*/
			/*
			string[] featureNames = Enum.GetNames(typeof(ChameleonFeatures));

			ChameleonFeatures perms = App.Configuration.PermittedFeatures;

			for(int i = 1; i < featureNames.Length; i++)
			{
				ToolStripMenuItem item = new ToolStripMenuItem();
				item.Text = featureNames[i];

				ChameleonFeatures feature;
				Enum.TryParse<ChameleonFeatures>(featureNames[i], out feature);

				if(perms.HasFlag(feature))
				{
					item.Checked = true;
				}

				item.CheckedChanged += new EventHandler(test1ToolStripMenuItem_CheckedChanged);
				item.CheckOnClick = true;

				menuFeatures.DropDownItems.Add(item);
			}
			*/
			cmw = Singleton<CtagsManagerWrapper>.Instance;

			cmw.FileParsed += new FileParsedDelegate(cmw_FileParsed);

			string indexerPath = Path.GetDirectoryName(Application.ExecutablePath);
			string tagsDBPath = Path.Combine(Options.DataFolder, "ChameleonTags.db");

			cmw.CodeLiteParserInit(indexerPath, tagsDBPath);
			parserInitialized = true;

			ShowPermittedUI();

			toolTextHost.Text = App.Configuration.LastHostname;
			toolTextUser.Text = App.Configuration.LastUsername;

			m_clickedSnippet = false;

			
			AddSnippetGroups();
		}
		

		private List<string> GetSnippetCategories()
		{
			SnippetList sl = m_editors.CurrentEditor.Snippets.List;

			List<string> categories = (from sn in sl
									   select sn.Category).Distinct().ToList();

			return categories;
		}

		private Dictionary<string, List<Snippet>> GetSnippets()
		{
			Dictionary<string, List<Snippet>> snippets = new Dictionary<string, List<Snippet>>();

			SnippetList sl = m_editors.CurrentEditor.Snippets.List;

			List<string> categories = GetSnippetCategories();

			foreach(string s in categories)
			{
				List<Snippet> categorySnippets = (from sn in sl
												  where sn.Category == s
												  select sn).ToList();
				snippets[s] = categorySnippets;
			}

			return snippets;
		}

		private void AddSnippetGroups()
		{
			List<string> categories = GetSnippetCategories();
			Dictionary<string, List<Snippet>> categorizedSnippets = GetSnippets();

			categories.Sort();
			categories.Reverse();

			foreach(String category in categories)
			{
				if(category == "Templates")
				{
					continue;
				}

				List<Snippet> snippets = categorizedSnippets[category];

				NaviGroup group = new NaviGroup(this.components);
				group.Caption = category;				
				group.Dock = DockStyle.Top;
				//group.Location = new Point(0, 0);
				group.LayoutStyle = NaviLayoutStyle.Office2007Black;
				group.Size = new Size(100, 150);

				naviBand1.ClientArea.Controls.Add(group);

				List<Snippet> orderedSnippets = snippets.OrderByDescending(s => s.LongName).ToList();

				foreach(Snippet sn in orderedSnippets)
				{
					Label l1 = new Label();

					if(sn.LongName == "")
					{
						l1.Text = sn.Shortcut;
					}
					else
					{
						l1.Text = sn.LongName;
					}
					
					group.Controls.Add(l1);
					l1.Dock = DockStyle.Top;

					l1.ImageList = imageList1;

					if(sn.IconName == "")
					{
						l1.ImageIndex = imageList1.Images.IndexOfKey("default.png");
					}
					else
					{
						l1.ImageIndex = imageList1.Images.IndexOfKey(sn.IconName);
					}
					
					l1.ImageAlign = ContentAlignment.TopCenter;
					l1.TextAlign = ContentAlignment.BottomCenter;
					l1.Height = 50;
					l1.BackColor = Color.Transparent;
					l1.Margin = new System.Windows.Forms.Padding(4);

					l1.MouseDown += new MouseEventHandler(l1_MouseDown);
					l1.MouseMove += new MouseEventHandler(l1_MouseMove);
					l1.MouseUp += new MouseEventHandler(l1_MouseUp);

					l1.Tag = sn;
				}

				int height = snippets.Count * 60;
				group.ExpandedHeight = height;
				group.Expand();
			}
		}

		#endregion

		#region Child control event handlers
		void l1_MouseDown(object sender, MouseEventArgs e)
		{
			m_clickedSnippet = true;			
		}

		void l1_MouseUp(object sender, MouseEventArgs e)
		{
			m_clickedSnippet = false;
		}

		void l1_MouseMove(object sender, MouseEventArgs e)
		{
			if(m_clickedSnippet)
			{
				Label l = (Label)sender;
				Snippet sn = (Snippet)l.Tag;

				string itemName = sn.Shortcut;

				m_editors.StartDrag();

				DataFormats.Format format = DataFormats.GetFormat("ChameleonSnippet");
				DataObject dobj = new DataObject(format.Name, itemName);
				DragDropEffects dde = DoDragDrop(dobj, DragDropEffects.Copy);

				m_editors.EndDrag();
			}
		}
		
		void cmw_FileParsed(string filename)
		{
			//MessageBox.Show("File parsed: " + filename);
		}

		#endregion

		#region Closing handler

		private void ChameleonForm_FormClosing(object sender, FormClosingEventArgs e)
		{
			m_appClosing = true;
			if(!m_editors.CloseAllFiles())
			{
				e.Cancel = true;
			}

			if(m_networking.IsConnected)
			{
				m_networking.Disconnect();
			}

			m_appClosing = false;

			App.Configuration.LastHostname = toolTextHost.Text;
			App.Configuration.LastUsername = toolTextUser.Text;

			App.Configuration.SaveSettings();
		}
		#endregion

		#region File Menu handlers
		private void OnFileNewBlank(object sender, EventArgs e)
		{
			m_editors.NewFile();
		}

		private void OnFileNewSimpleTemplate(object sender, EventArgs e)
		{
			m_editors.NewFile("simpleTemplate");
		}

		private void OnFileOpenLocal(object sender, EventArgs e)
		{
			m_editors.OpenFile(FileLocation.Local);

			if(!parserInitialized)
			{
				return;
			}

			cmw.AddParserRequestSingleFile(m_editors.CurrentEditor.Filename);
		}

		private void OnFileOpenRemote(object sender, EventArgs e)
		{
			if(!m_networking.IsConnected)
			{
				MessageBox.Show("Can't do anything remote if not connected!");
				return;
			}

			m_editors.OpenFile(FileLocation.Remote);
			
		}

		private void OnFileSave(object sender, EventArgs e)
		{
			ChameleonEditor editor = m_editors.CurrentEditor;
			m_editors.SaveFile(editor, editor.FileLocation, false, true);

			
		}

		private void OnFileSaveAsLocal(object sender, EventArgs e)
		{
			ChameleonEditor editor = m_editors.CurrentEditor;
			m_editors.SaveFile(editor, FileLocation.Local, true, false);
		}

		private void OnFileSaveAsRemote(object sender, EventArgs e)
		{
			ChameleonEditor editor = m_editors.CurrentEditor;
			m_editors.SaveFile(editor, FileLocation.Remote, true, false);
		}

		private void OnFileClose(object sender, EventArgs e)
		{
			m_editors.CloseFile(m_editors.CurrentEditor);
		}

		private void OnFileCloseAll(object sender, EventArgs e)
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

		private void menuEditFind_Click(object sender, EventArgs e)
		{
			m_editors.CurrentEditor.FindReplace.ShowFind();
		}

		private void menuEditReplace_Click(object sender, EventArgs e)
		{
			m_editors.CurrentEditor.FindReplace.ShowReplace();
		}

		private void menuEditReformatFile_Click(object sender, EventArgs e)
		{
			ChameleonEditor ed = m_editors.CurrentEditor;

			ed.Text = m_astyle.FormatSource(ed.Text);
		}

		private void menuEditReformatSelectedCode_Click(object sender, EventArgs e)
		{
			ChameleonEditor ed = m_editors.CurrentEditor;

			ed.Selection.Text = m_astyle.FormatSource(ed.Selection.Text);
		}

		#endregion

		#region Help Menu handlers

		private void menuHelpAbout_Click(object sender, EventArgs e)
		{
			MessageBox.Show("Chameleon 2.0 alpha 0.0002");
		}

		#endregion

		#region Toolbar handlers
		private void OnHostConnect(object sender, EventArgs e)
		{
			DoConnect();		
		}

		private void DoConnect()
		{
			string host = toolTextHost.Text;
			string user = toolTextUser.Text;
			string password = toolTextPassword.Text;

			string errorMessage = "";


			if(host == "")
			{
				errorMessage = "Please enter a server address";
				goto OnConnectError;
			}

			if(user == "")
			{
				errorMessage = "Please enter a username";
				goto OnConnectError;
			}

			if(password == "")
			{
				errorMessage = "Please enter a password";
			}


		OnConnectError:
			if(errorMessage != "")
			{
				MessageBox.Show(errorMessage, "Connection Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
				return;
			}

			if(m_networking.Connect(host, user, password))
			{
				m_sshProtocol.Connect();

				toolTextHost.Enabled = false;
				toolTextUser.Enabled = false;
				toolTextPassword.Enabled = false;
				toolHostConnect.Enabled = false;
				toolHostDisconnect.Enabled = true;

				toolStatusConnected.Text = "Connected";
			}
		}

		private void OnHostDisconnect(object sender, EventArgs e)
		{
			if(m_networking.IsConnected)
			{
				m_networking.Disconnect();
				m_sshProtocol.Disconnect();

				toolTextHost.Enabled = true;
				toolTextUser.Enabled = true;
				toolTextPassword.Enabled = true;
				toolHostConnect.Enabled = true;
				toolHostDisconnect.Enabled = false;

				toolStatusConnected.Text = "Disconnected";
			}
			
		}
		
		private void toolTextPassword_KeyPress(object sender, KeyPressEventArgs e)
		{
			if(e.KeyChar == '\r')
			{
				e.Handled = true;
				DoConnect();
			}

				
		}

		#endregion

		#region Permissions/UI functions

		private void ShowPermittedUI()
		{
			ChameleonFeatures perms = App.Configuration.PermittedFeatures;

			// clear out toolbar items after the basics
			int indexAfterSave = toolStrip1.Items.IndexOf(btnSave) + 1;
			int remainingItems = toolStrip1.Items.Count - (indexAfterSave);

			for(int i = 0; i < remainingItems; i++)
			{
				toolStrip1.Items.RemoveAt(indexAfterSave);
			}

			if(perms.HasFlag(ChameleonFeatures.Feature1))
			{
				toolStrip1.Items.Add(new ToolStripSeparator());
				toolStrip1.Items.Add(btnDummyFeature1);
			}

			if(perms.HasFlag(ChameleonFeatures.Feature2))
			{
				toolStrip1.Items.Add(new ToolStripSeparator());
				toolStrip1.Items.Add(btnDummyFeature2);
			}

			splitSnippetsEditor.Panel1Collapsed = !perms.HasFlag(ChameleonFeatures.DragDropSnippets);
		}

		#endregion

		#region Debug/test handlers
		private void test1ToolStripMenuItem_CheckedChanged(object sender, EventArgs e)
		{
			ToolStripMenuItem item = sender as ToolStripMenuItem;

			ChameleonFeatures feature;
			Enum.TryParse<ChameleonFeatures>(item.Text, out feature);

			ChameleonFeatures perms = App.Configuration.PermittedFeatures;

			if(item.Checked)
			{
				perms |= feature;
			}
			else
			{
				perms &= ~feature;
			}

			App.Configuration.PermittedFeatures = perms;
			App.Configuration.SaveSettings();

			ShowPermittedUI();
		}

		private void listView1_ItemDrag(object sender, ItemDragEventArgs e)
		{
			ListViewItem lvi = (ListViewItem)e.Item;

			string itemName = (string)lvi.Tag;

			if(string.IsNullOrEmpty(itemName))
			{
				return;
			}

			m_editors.StartDrag();

			DataFormats.Format format = DataFormats.GetFormat("ChameleonSnippet");
			DataObject dobj = new DataObject(format.Name, itemName);
			DragDropEffects dde = DoDragDrop(dobj, DragDropEffects.Copy);

			m_editors.EndDrag();
		}

		private void tagsByScopeToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if(!parserInitialized)
			{
				return;
			}

			string text = m_editors.CurrentEditor.GetRange(0, m_editors.CurrentEditor.CurrentPos).Text;

			List<string> fileScopes = cmw.GetScopesFromFile(m_editors.CurrentEditor.Filename);
			LanguageWrapper lw = new LanguageWrapper();

			foreach(String scope in fileScopes)
			{
				Console.WriteLine(scope);
			}

			string optiScope = lw.OptimizeScope(text);
			string scopeName = cmw.GetScopeName(optiScope);
			
			List<CodeLite.Tag> tags = cmw.TagsFromFileAndScope(m_editors.CurrentEditor.Filename, scopeName);

			if(tags.Count == 0)
			{
				MessageBox.Show("No tags!");
				return;
			}

			StringBuilder sb = new StringBuilder();

			foreach(Tag t in tags)
			{
				string line = string.Format("name: {0}, kind: {1}\n", t.name, t.kind);
				sb.Append(line);
			}

			string message = sb.ToString();

			MessageBox.Show(message);
		}

		private void localVariablesToolStripMenuItem_Click(object sender, EventArgs e)
		{
			LanguageWrapper lw = new LanguageWrapper();

			string text = m_editors.CurrentEditor.Selection.Text;
			int lineNum = m_editors.CurrentEditor.Lines.Current.Number;

			Tag fn = cmw.FunctionFromFileLine(m_editors.CurrentEditor.Filename, lineNum, false);


			List<Tag> tags = lw.GetLocalVariables(text, "", 0);

			if(tags.Count == 0)
			{
				MessageBox.Show("No tags!");
				return;
			}
			
			StringBuilder sb = new StringBuilder();

			
			foreach(Tag t in tags)
			{
				string line = string.Format("name: {0}, kind: {1}\n", t.name, t.kind);
				sb.Append(line);
			}

			string message = sb.ToString();

			MessageBox.Show(message);
			
		}



		private void parseExpressionToolStripMenuItem_Click(object sender, EventArgs e)
		{
			ChameleonEditor ed = m_editors.CurrentEditor;

			string lineText = ed.Lines.Current.Text;

			Line currLine = ed.Lines.Current;

			char chOpenBrace = '(';

			int openPos = lineText.IndexOf(chOpenBrace);

			int startPos = currLine.StartPosition + openPos;

			int matchedPos = 0;
			bool found = ed.MatchBraceForward(chOpenBrace, startPos, ref matchedPos);


		}

		private void TestStringFunc(string text)
		{
			MessageBox.Show(text);
			//Console.WriteLine(text);
		}

		private void executeRemoteCommandToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if(m_networking.IsConnected)
			{
				string command = "g++ fizzBuzzTest.cpp -o fizzBuzzTest.exe && echo C_O_M_P_I_L_E_SUCCESS || echo C_O_M_P_I_L_E_FAILED";
				string lsCommand = "ls";
				m_networking.ExecuteRemoteCommand(lsCommand, TestStringFunc);
			}
		}
		#endregion

		

		

		

		

	}
}
