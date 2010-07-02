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
using Chameleon.Util;
using Chameleon.Network;
using Chameleon.Features;
using SSHClient;
using CodeLite;
using System.Reflection;

namespace Chameleon
{
	public partial class ChameleonForm : Form
	{
		private static bool m_appClosing = false;

		private Networking m_networking;
		private SSHProtocol m_sshProtocol;

		private CtagsManagerWrapper cmw;
		private bool parserInitialized;

		public static bool AppClosing
		{
			get { return m_appClosing; }
		}

		public ChameleonForm()
		{
			InitializeComponent();

			FormFontFixer.Fix(this);

			Options options = App.Configuration;

			/*
			string testSourcePath = "d:\\projects\\temp\\fizzbuzz.cpp";
			if(File.Exists(testSourcePath))
			{
				string source = File.ReadAllText(testSourcePath);
				m_editors.CurrentEditor.Text = source;
			}
			*/
			menuEditUndo.ShortcutKeyDisplayString = "Ctrl+Z";
			menuEditRedo.ShortcutKeyDisplayString = "Ctrl+Y";
			menuEditCopy.ShortcutKeyDisplayString = "Ctrl+C";
			menuEditCut.ShortcutKeyDisplayString = "Ctrl+X";
			menuEditPaste.ShortcutKeyDisplayString = "Ctrl+V";

			toolTextPassword.TextBox.UseSystemPasswordChar = true;

			m_networking = Networking.Instance;

			toolStatusConnected.Text = "Disconnected";

			toolTextHost.Text = "192.168.1.103";
			toolTextUser.Text = "root";

			toolHostDisconnect.Enabled = false;

			m_sshProtocol = new SSHProtocol(terminalEmulator1);

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

			cmw = new CtagsManagerWrapper();

			cmw.FileParsed += new FileParsedDelegate(cmw_FileParsed);

			string appPath = Application.ExecutablePath;
			string indexerPath = Path.GetDirectoryName(appPath);
			//string indexerPath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().CodeBase);
			//string indexerPath = Path.Combine(chameleonPath, "codelite_indexer.exe");

			cmw.CodeLiteParserInit(indexerPath, "d:\\temp\\ChameleonTagsDB.db");
			parserInitialized = true;


			ShowPermittedUI();
		}

		void cmw_FileParsed(string filename)
		{
			MessageBox.Show("File parsed: " + filename);
		}

		

		private void ChameleonForm_FormClosing(object sender, FormClosingEventArgs e)
		{
			m_appClosing = true;
			if(!m_editors.CloseAllFiles())
			{
				e.Cancel = true;
			}

			cmw.CodeLiteParserEnd();

			m_appClosing = false;

			App.Configuration.SaveSettings();
		}


		#region File Menu handlers
		private void OnNewFile(object sender, EventArgs e)
		{
			m_editors.NewFile();
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
			//m_editors.OpenFile(FileLocation.Remote);
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
			string host = toolTextHost.Text;
			string user = toolTextUser.Text;
			string password = toolTextPassword.Text;

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
			m_networking.Disconnect();
			m_sshProtocol.Disconnect();
			

			toolTextHost.Enabled = true;
			toolTextUser.Enabled = true;
			toolTextPassword.Enabled = true;
			toolHostConnect.Enabled = true;
			toolHostDisconnect.Enabled = false;

			toolStatusConnected.Text = "Disconnected";
		}

		#endregion

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

			//Console.WriteLine("Item {0} checked: {1}", item.Text, item.Checked);
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

			foreach(String scope in fileScopes)
			{
				Console.WriteLine(scope);
			}

			string scopeName = cmw.GetScopeName(text);
			List<CodeLite.Tag> tags = cmw.TagsByScope(scopeName);

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

			//string text = m_editors.CurrentEditor.GetRange(0, m_editors.CurrentEditor.CurrentPos).Text;
			string text = m_editors.CurrentEditor.Selection.Text;
			int lineNum = m_editors.CurrentEditor.Lines.Current.Number;

			Tag fn = cmw.FunctionFromFileLine(m_editors.CurrentEditor.Filename, lineNum, false);

			//string scope = lw.OptimizeScope(text);
			//string scopeName = lw.GetScopeName(text, null);

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

	}
}
