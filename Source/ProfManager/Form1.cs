using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;
using System.Windows.Forms;
using Aspiring.Linq;
using Chameleon.Features;
using Chameleon.GUI;
using Chameleon.Network;
using Chameleon.Util;
using DevInstinct.Patterns;
using LVE = ListViewEx;
using PSTaskDialog;
using System.Text;

namespace ProfManager
{
	public partial class Form1 : Form
	{
		private Networking m_networking;

		private Dictionary<string, ChameleonFeatures> m_groups;
		private Dictionary<string, string> m_students;

		private TextBox m_cellText;
		private ComboBox m_cellCombo;

		private static string m_groupPrefix = "group-";
		private static char[] m_invalidChars = Path.GetInvalidFileNameChars();
		//private Regex m_reInvalidChars;

		private FileInformation m_currentFolder;

		private string m_prevGroupName;

		private int m_nextGroupNum;
		private int m_nextStudentNum;

		public Form1()
		{
			InitializeComponent();

			m_currentFolder = new FileInformation();

			//string invalidCharPattern = string.Format("[{0}]", m_invalidChars);
			//m_reInvalidChars = new Regex(invalidCharPattern);


			m_networking = Networking.Instance;
			Singleton<RemoteFileDialog>.Instance.Networking = m_networking;

			m_students = new Dictionary<string, string>();
			m_groups = new Dictionary<string, ChameleonFeatures>();

			m_cellCombo = new ComboBox();
			m_cellCombo.Parent = this;
			m_cellCombo.Visible = false;
			m_cellCombo.DropDownStyle = ComboBoxStyle.DropDownList;

			m_cellCombo.SelectedValueChanged += new EventHandler(m_cellCombo_SelectedValueChanged);


			m_cellText = new TextBox();
			m_cellText.Parent = this;
			m_cellText.Visible = false;

			toolStripStatusLabel1.Text = "Not connected";

			m_nextGroupNum = 0;

			List<ChameleonFeatures> cf = GetCFValues();

			foreach(ChameleonFeatures feature in cf)
			{
				lbFeatures.Items.Add(feature);
			}

			lvGroups.DoubleClickActivation = true;
			lvStudents.DoubleClickActivation = true;

			ClearAllItems();
			/*
			AddNewGroup("group1");
			AddNewGroup("group2");
			AddNewGroup("group3");
			
			if(lvGroups.Items.Count > 0)
			{
				lvGroups.SelectedItems.Clear();
				lvGroups.Items[0].Selected = false;
				lvGroups.Items[0].Selected = true;

				lvGroups.Focus();
			}
			*/
		}

		void m_cellCombo_SelectedValueChanged(object sender, EventArgs e)
		{
			if(m_cellCombo.Visible)
			{
				m_cellCombo.DroppedDown = false;

				lvStudents.EndEditing(true);
			}
		}

		private List<ChameleonFeatures> GetCFValues()
		{
			return Enum.GetValues(typeof(ChameleonFeatures)).Cast<ChameleonFeatures>().ToList();
		}

		private void AddNewGroup()
		{
			string name = GetNextGroupName();

			while(m_groups.ContainsKey(name))
			{
				name = GetNextGroupName();
			}

			//SaveCurrentFeatures();

			m_groups[name] = new ChameleonFeatures();

			ListViewItem lvi = lvGroups.Items.Add(name);
			lvi.Selected = true;			
		}

		private void AddNewGroup(string groupName)
		{
			ChameleonFeatures cf = new ChameleonFeatures();

			AddNewGroup(groupName, cf);
		}

		private void AddNewGroup(string groupName, ChameleonFeatures cf)
		{
			m_groups[groupName] = cf;

			ListViewItem lvi = lvGroups.Items.Add(groupName);
			lvi.Selected = true;
		}

		private void SaveCurrentFeatures(string groupName)
		{
			ChameleonFeatures cf = new ChameleonFeatures();

			for(int i = 0; i < lbFeatures.Items.Count; i++)
			{
				bool flag = lbFeatures.GetItemChecked(i);
				
				if(flag)
				{
					
					ChameleonFeatures cfResult;
					Enum.TryParse<ChameleonFeatures>(lbFeatures.Items[i].ToString(), out cfResult);

					cf |= cfResult;
				}
			}

			m_groups[groupName] = cf;
			
		}

		private string GetNextGroupName()
		{
			string name = "newGroup" + m_nextGroupNum;
			m_nextGroupNum++;

			return name;
		}

		private string GetNextStudentName()
		{
			string name = "newStudent" + m_nextStudentNum;
			m_nextStudentNum++;

			return name;
		}

		private void btnAddGroup_Click(object sender, EventArgs e)
		{
			AddNewGroup();
		}

		private void OnSubItemClicked(object sender, ListViewEx.SubItemEventArgs e)
		{
			Control c = null;
			LVE.ListViewEx lv = sender as LVE.ListViewEx;

			if(lv == lvGroups)
			{
				c = m_cellText;

				m_prevGroupName = e.Item.Text;
			}
			else
			{
				if(e.SubItem == 0)
				{
					c = m_cellCombo;
					m_cellCombo.Items.Clear();

					string[] groups = (from lvi in lvGroups.Items.Cast<ListViewItem>()
									   select lvi.Text).ToArray();

					Array.Sort<string>(groups);

					m_cellCombo.Items.AddRange(groups);
				}
				else
				{
					c = m_cellText;
				}				
			}

			lv.StartEditing(c, e.Item, e.SubItem);
			
		}

		private void lvGroups_SubItemEndEditing(object sender, LVE.SubItemEndEditingEventArgs e)
		{
			string newGroupName = e.DisplayText;

			if(m_prevGroupName != newGroupName)
			{
				ReplaceGroupName(m_prevGroupName, newGroupName);

				ChameleonFeatures cf = m_groups[m_prevGroupName];
				m_groups.Remove(m_prevGroupName);
				m_groups[newGroupName] = cf;
			}
		}

		private void ReplaceGroupName(string oldGroupName, string newGroupName)
		{
			int count = (from lvi in lvStudents.Items.Cast<ListViewItem>()
						 where lvi.Text == oldGroupName
						 select lvi).Update(lvi => lvi.Text = newGroupName);
		}

		private void ClearCheckedFeatures()
		{
			for(int i = 0; i < lbFeatures.Items.Count; i++)
			{
				lbFeatures.SetItemChecked(i, false);
			}
		}

		private void CheckFeatures(string groupName)
		{
			ChameleonFeatures cf = m_groups[groupName];
			List<ChameleonFeatures> cfValues = GetCFValues();

			for(int i = 0; i < cfValues.Count; i++)
			{
				bool flag = cf.HasFlag(cfValues[i]);

				lbFeatures.SetItemChecked(i, flag);
			}
		}

		private void lvGroups_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
		{
			//Console.WriteLine("Item: {0}, index = {1}, selected = {2}", e.Item.Text, e.ItemIndex, e.IsSelected);
			string groupName = e.Item.Text;
			if(!m_groups.ContainsKey(groupName))
			{
				return;
			}

			if(e.IsSelected)
			{
				CheckFeatures(groupName);
			}
			else
			{
				SaveCurrentFeatures(groupName);
				ClearCheckedFeatures();
			}
		}

		private void btnRemoveGroup_Click(object sender, EventArgs e)
		{
			if(lvGroups.SelectedItems.Count == 1)
			{
				ListViewItem lvi = lvGroups.SelectedItems[0];
				int prevIndex = lvi.Index;

				lvGroups.Items.Remove(lvi);

				m_groups.Remove(lvi.Text);

				ReplaceGroupName(lvi.Text, "");

				if(lvGroups.Items.Count > 0)
				{
					int nextSelection = 0;

					if(prevIndex < lvGroups.Items.Count)
					{
						nextSelection = prevIndex;
					}
					else
					{
						if(prevIndex > 0)
						{
							nextSelection = prevIndex - 1;
						}
					}

					lvGroups.SelectedIndices.Add(nextSelection);
				}
			}
		}

		private void btnSave_Click(object sender, EventArgs e)
		{
			bool emptyGroups = (from lvi in lvStudents.Items.Cast<ListViewItem>()
								where lvi.Text == ""
								select lvi).Any();

			if(emptyGroups)
			{
				MessageBox.Show("Can't save until all students are part of a group");
				return;
			}

			SaveCurrentItems();
		}

		private void btnAddFromFile_Click(object sender, EventArgs e)
		{
			OpenFileDialog ofd = new OpenFileDialog();
			ofd.CheckFileExists = true;
			ofd.Multiselect = false;
			ofd.Filter = "Text Files (*.txt)|*.txt";

			if(ofd.ShowDialog() == DialogResult.OK)
			{
				string filename = ofd.FileName;

				string[] fileLines = File.ReadAllLines(filename);

				foreach(string line in fileLines)
				{
					ListViewItem lvi = lvStudents.Items.Add("");
					lvi.SubItems.Add(line);
				}
			}
		}

		private void btnAddStudent_Click(object sender, EventArgs e)
		{
			string name = GetNextStudentName();

			ListViewItem lvi = lvStudents.Items.Add("");
			lvi.SubItems.Add(name);
		}

		private void btnRemoveStudent_Click(object sender, EventArgs e)
		{
			if(lvStudents.SelectedItems.Count == 1)
			{
				lvStudents.Items.Remove(lvStudents.SelectedItems[0]);
			}
		}

		private void btnConnect_Click(object sender, EventArgs e)
		{
			DoConnect();
		}

		private void btnDisconnect_Click(object sender, EventArgs e)
		{
			if(m_currentFolder.Location == FileLocation.Remote)
			{
				DialogResult dr = MessageBox.Show("Your current loaded settings are remote.  Disconnect and clear all items?",
									"Disconnect", MessageBoxButtons.YesNo, MessageBoxIcon.Question);

				if(dr == DialogResult.No)
				{
					return;
				}

				ClearAllItems();

			}
			m_networking.Disconnect();

			btnConnect.Enabled = true;
			btnDisconnect.Enabled = false;
			btnFolderRemote.Enabled = false;

			toolStripStatusLabel1.Text = "Not connected";
		}

		private void ClearAllItems()
		{
			lvStudents.Items.Clear();
			lvGroups.Items.Clear();
			ClearCheckedFeatures();
			m_groups.Clear();

			txtSelectedFolder.Text = "";
			m_currentFolder.Filename.Clear();
			m_currentFolder.Location = FileLocation.Unknown;
		}

		private void btnFolderRemote_Click(object sender, EventArgs e)
		{
			FileInformation folder = SelectRemoteFolder();			

			if(folder != null)
			{
				LoadSelectedFolder(folder);
			}
		}

		
		private void btnFolderLocal_Click(object sender, EventArgs e)
		{
			FileInformation folder = SelectLocalFolder();

			if(folder != null)
			{
				LoadSelectedFolder(folder);
			}			
		}

		private FileInformation SelectLocalFolder()
		{
			FolderBrowserDialog fbd = new FolderBrowserDialog();

			if(m_currentFolder.Location == FileLocation.Local)
			{
				string path = m_currentFolder.Filename.GetFullPath();

				if(path != "")
				{
					fbd.SelectedPath = path;
				}
			}

			DialogResult dr = fbd.ShowDialog();

			if(dr == DialogResult.OK)
			{
				string folderName = fbd.SelectedPath;

				FilePath fp = new FilePath(folderName, PathFormat.Windows);
				FileInformation folder = new FileInformation();
				folder.Filename = fp;
				folder.Location = FileLocation.Local;

				return folder;
			}

			return null;
		}

		private FileInformation SelectRemoteFolder()
		{
			RemoteFileDialog rfd = Singleton<RemoteFileDialog>.Instance;

			rfd.FolderMode = true;
			rfd.Prepare(true, "");


			DialogResult dr = rfd.ShowDialog();

			if(dr == DialogResult.OK)
			{
				FilePath fp = rfd.SelectedFile;

				FileInformation folder = new FileInformation();
				folder.Filename = fp;
				folder.Location = FileLocation.Remote;

				return folder;
			}

			return null;
		}

		
		private void LoadSelectedFolder(FileInformation folder)
		{
			if(lvStudents.Items.Count > 0 || lvGroups.Items.Count > 0)
			{
				DialogResult dr = MessageBox.Show("Save current items before loading?", "Save Items?",
					MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question);

				if(dr == DialogResult.Yes)
				{
					SaveCurrentItems();
				}
				else if(dr == DialogResult.Cancel)
				{
					return;
				}
			}

			if(folder.Location == FileLocation.Unknown)
			{
				return;
			}

			bool isRemote = folder.Location == FileLocation.Remote;

			if(isRemote && !m_networking.IsConnected)
			{
				MessageBox.Show("Can't load remote files if not connected!", "Connection Error",
					MessageBoxButtons.OK, MessageBoxIcon.Warning);
				return;
			}

			ClearAllItems();

			txtSelectedFolder.Text = folder.Filename.GetFullPath();
			m_currentFolder = folder;

			List<FileInformation> folderTextFiles = GetFolderTextFiles(folder);
			List<FileInformation> groupFiles = GetGroupTextFiles(folderTextFiles);

			string fileContents = "";

			foreach(FileInformation groupFile in groupFiles)
			{
				string fileName = groupFile.Filename.Name;
				string groupName = fileName.Substring(m_groupPrefix.Length);

				fileContents = "";
				if(!GetFileContents(groupFile, ref fileContents))
				{
					continue;
				}

				int permValue = Convert.ToInt32(fileContents, 16);
				ChameleonFeatures cf = (ChameleonFeatures)permValue;
				
				AddNewGroup(groupName, cf);
			}

			List<FileInformation> studentFiles = (from fi in folderTextFiles
												  where fi.Filename.Name == "students"
												  select fi).ToList();

			if(studentFiles.Count == 0)
			{
				return;
			}

			FileInformation studentFile = studentFiles[0];

			if(!GetFileContents(studentFile, ref fileContents))
			{
				return;
			}

			Regex reNewlines = new Regex("\r\n?|\n");

			string[] studentLines = reNewlines.Split(fileContents);

			foreach(string s in studentLines)
			{
				string[] sg = s.Split(':');

				if(sg.Length != 2)
				{
					continue;
				}

				string student = sg[0];
				string group = sg[1];

				if(!m_groups.ContainsKey(group))
				{
					group = "";
				}

				ListViewItem lvi = lvStudents.Items.Add(group);
				lvi.SubItems.Add(student);
			}

			if(lvGroups.Items.Count > 0)
			{
				lvGroups.SelectedIndices.Clear();
				lvGroups.Items[0].Selected = true;

				lvGroups.Focus();
			}
			
		}

		private List<FileInformation> GetGroupTextFiles(List<FileInformation> folderTextFiles)
		{
			return (from fi in folderTextFiles
			 where fi.Filename.Name.StartsWith(m_groupPrefix)
			 select fi).ToList();
		}

		private bool GetFileContents(FileInformation fileInfo, ref string fileContents)
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
					if(!m_networking.IsConnected)
					{
						return false;
					}

					m_networking.GetFileContents(fileInfo.Filename, ref fileContents);
					break;
				}
			}

			return true;
		}

		private void WriteFileContents(FileInformation fileInfo, string fileContents)
		{
			switch(fileInfo.Location)
			{
				case FileLocation.Unknown:
				{
					return;
				}
				case FileLocation.Local:
				{
					string fullFilename = fileInfo.Filename.GetFullPath();
					if(File.Exists(fullFilename))
					{
						File.WriteAllText(fullFilename, fileContents);
					}
					break;
				}
				case FileLocation.Remote:
				{
					if(!m_networking.IsConnected)
					{
						return;
					}

					m_networking.SendFileContents(fileInfo.Filename, fileContents);
					break;
				}
			}
		}

		private void SaveCurrentItems()
		{
			List<string> groups = m_groups.Keys.ToList();
			Dictionary<string, string> groupFiles = new Dictionary<string, string>();

			bool anyInvalid = false;
			char badChar = ' ';

			foreach(string group in groups)
			{
				string filename = String.Format("{0}{1}.txt", m_groupPrefix, group);
				groupFiles[group] = filename;
			
				if(!IsValidFilename(filename, ref badChar))
				{
					anyInvalid = true;

					string charString = badChar.ToString();
					int charValue = (int)badChar;

					if(charValue <= 31)
					{
						charString = "[ASCII]" + charValue;
					}

					string message = string.Format("Group '{0}' contains characters not allowed in a filename. " + 
													"Please remove this character: '{1}'", filename, charString);
					MessageBox.Show(message, "Invalid Group Names", MessageBoxButtons.OK, MessageBoxIcon.Warning);
				}
			}

			if(anyInvalid)
			{
				return;
			}

			string path = m_currentFolder.Filename.GetFullPath();

			if(path == "" || m_currentFolder.Location == FileLocation.Unknown)
			{
				string message = "Where should these settings be saved?";

				List<string> buttons = new List<string>();
				buttons.Add("Locally (on the computer you're using)");
				buttons.Add("Remotely (on the server)");
				buttons.Add("Cancel");

				
				int button = cTaskDialog.ShowCommandBox("Save File Location", message, "",
					string.Join("|", buttons), false);
				FileLocation location = (FileLocation)button;

				if(location == FileLocation.Unknown)
				{
					// canceled
					return;
				}

				FileInformation folder = null;

				if(location == FileLocation.Remote)
				{
					folder = SelectRemoteFolder();
				}
				else
				{
					folder = SelectLocalFolder();
				}

				if(folder == null)
				{
					return;
				}

				m_currentFolder = folder;				
			}
			
			bool isRemote = (m_currentFolder.Location == FileLocation.Remote);
			List<FileInformation> textFiles = GetFolderTextFiles(m_currentFolder);
			List<FileInformation> groupTextFiles = GetGroupTextFiles(textFiles);

			if(isRemote)
			{
				DeleteOldRemoteSettings(groupTextFiles);
			}
			else
			{
				DeleteOldLocalSettings(groupTextFiles);
			}

			List<FileInformation> finalGroupFiles = groupFiles.Keys.ToList().ConvertAll<FileInformation>
				(groupName =>
			{
				FileInformation fi = new FileInformation();
				fi.Filename.Assign(m_currentFolder.Filename.GetFullPath(), groupName, m_currentFolder.Filename.Format);
				fi.Location = m_currentFolder.Location;

				return fi;
			});

			foreach(string group in groups)
			{
				string groupFilename = groupFiles[group];
				FileInformation fi = new FileInformation();
				fi.Filename.Assign(m_currentFolder.Filename.GetFullPath(), groupFilename, m_currentFolder.Filename.Format);
				fi.Location = m_currentFolder.Location;

				ChameleonFeatures cf = m_groups[group];
				int cfValue = (int)cf;
				string groupFileContents = String.Format("0x{0:X}", cfValue);

				WriteFileContents(fi, groupFileContents);
			}

			List<string> studentStrings = new List<string>();

			foreach(ListViewItem lvi in lvStudents.Items)
			{
				// listview shows group:student, but the file format needs student:group
				string line = string.Format("{0}:{1}", lvi.SubItems[1].Text, lvi.Text);
				studentStrings.Add(line);
			}

			string studentFileContents = string.Join("\r\n", studentStrings);

			FileInformation studentFile = new FileInformation();
			studentFile.Filename.Assign(m_currentFolder.Filename.GetFullPath(), "students.txt", m_currentFolder.Filename.Format);
			studentFile.Location = m_currentFolder.Location;

			WriteFileContents(studentFile, studentFileContents);

			txtSelectedFolder.Text = m_currentFolder.Filename.GetFullPath();
		}



		bool IsValidFilename(string text, ref char badChar)
        {
			foreach(char c in m_invalidChars)
			{
				if(text.Contains(c))
				{
					badChar = c;
					return false;
				}
			}

			return true;
        }


		private void DeleteOldRemoteSettings(List<FileInformation> groupFiles)
		{
			foreach(FileInformation fi in groupFiles)
			{
				m_networking.DeleteFile(fi.Filename);
			}
		}

		private void DeleteOldLocalSettings(List<FileInformation> groupFiles)
		{
			foreach(FileInformation fi in groupFiles)
			{
				File.Delete(fi.Filename.GetFullPath());
			}
		}

		private List<FileInformation> GetFolderTextFiles(FileInformation folder)
		{
			List<FileInformation> files = null;// = new List<FileInformation>();

			if(folder.Location == FileLocation.Local)
			{
				files = GetLocalFolderListing(folder);
			}
			else
			{
				files = GetRemoteFolderListing(folder);
			}

			List<FileInformation> txtFiles = files.Where
				(fi =>
			{
				return fi.Filename.Extension.ToLower() == "txt";
			}).ToList();

			return txtFiles;
		}

		private List<FileInformation> GetRemoteFolderListing(FileInformation folder)
		{
			DirectoryListing dl = m_networking.GetDirectoryListing(folder.Filename, true);
			string fullPath = folder.Filename.GetPath(PathReturnType.GetSeparator, PathFormat.Unix);


			List<string> fullFileNames = dl.fileNames.ConvertAll<string>(
				name =>
			{
				FilePath fp = new FilePath(fullPath, name, PathFormat.Unix);
				string s = fp.GetFullPath();
				return s;
			}).ToList();

			List<FileInformation> files = NamesToFileInformation(fullFileNames, PathFormat.Unix);

			return files;
		}

		private List<FileInformation> GetLocalFolderListing(FileInformation folder)
		{
			List<string> fileNames = Directory.GetFiles(folder.Filename.GetFullPath(), "*.txt").ToList();
			List<FileInformation> files = NamesToFileInformation(fileNames, PathFormat.Windows);

			return files;
		}

		private List<FileInformation> NamesToFileInformation(List<string> fileNames, PathFormat pf)
		{
			List<FileInformation> files = fileNames.ConvertAll<FileInformation>
				(name =>
			{
				FileInformation fi = new FileInformation();
				fi.Filename = new FilePath(name, pf);
				fi.Location = (pf == PathFormat.Windows) ? FileLocation.Local : FileLocation.Remote;

				return fi;
			});

			return files;
		}

		private void DoConnect()
		{
			string host = txtHost.Text;
			string user = txtUser.Text;
			string password = txtPassword.Text;


			

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
				btnConnect.Enabled = false;
				btnDisconnect.Enabled = true;
				btnFolderRemote.Enabled = true;

				toolStripStatusLabel1.Text = "Connected";
			}
		}

		private void txtPassword_KeyPress(object sender, KeyPressEventArgs e)
		{
			if(e.KeyChar == '\r')
			{
				e.Handled = true;
				DoConnect();
			}
		}

		private void lbFeatures_ItemCheck(object sender, ItemCheckEventArgs e)
		{
			if(lvGroups.SelectedItems.Count == 1)
			{
				string groupName = lvGroups.SelectedItems[0].Text;
				ChameleonFeatures cf = m_groups[groupName];

				string featureName = lbFeatures.Items[e.Index].ToString();

				ChameleonFeatures feature;
				Enum.TryParse<ChameleonFeatures>(featureName, out feature);

				if(e.NewValue == CheckState.Checked)
				{
					cf |= feature;
				}
				else
				{
					cf ^= feature;
				}

				m_groups[groupName] = cf;
			}
		}

		
	}
}
