using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Chameleon.Util;
using Chameleon.Network;
using Etier.IconHelper;
using DevInstinct.Patterns;


namespace Chameleon.GUI
{
	public partial class RemoteFileDialog : Form
	{
		private IconListManager m_iconManager;

		private bool m_openMode;
		private int m_filterAllFilesIndex;
		private int m_currentFilterIndex;

		private FilePath m_currentPath;
		private FilePath m_userHomeDir;
		private FilePath m_selectedFile;
		
		private DirectoryListing m_currentDirListing;

		private List<string> m_dirs;
		private List<string> m_files;

		private List<List<string>> m_fileExtensionList;

		private Networking m_networking;

		string[] m_fileTypeStrings = new string[] 
		{
			"C++ source files (*.c, *.cpp)",
			"C++ header files (*.h, *.hpp)",
			"All files (*.*)"
		};

		public static RemoteFileDialog Instance
		{
			get
			{
				return Singleton<RemoteFileDialog>.Instance;
			}
		}

		public Chameleon.Util.FilePath SelectedFile
		{
			get { return m_selectedFile; }
		}

		private RemoteFileDialog()
		{
			InitializeComponent();

			FormFontFixer.Fix(this);

			m_currentPath = new FilePath("~", PathFormat.Unix);
			m_userHomeDir = new FilePath("~", PathFormat.Unix);
			m_selectedFile = new FilePath();

			m_dirs = new List<string>();
			m_files = new List<string>();

			m_fileExtensionList = new List<List<string>>();
				
			cbFileType.Items.AddRange(m_fileTypeStrings);
			cbFileType.SelectedItem = m_fileTypeStrings[0];			

			m_iconManager = new IconListManager(m_images, IconReader.IconSize.Small);
			listView1.SmallImageList = m_images;

			m_networking = Networking.Instance;

			/*
			string[] extensions = new string[] { ".cpp", ".c", ".h", "default", ".exe", ".zip", ".docx", ".ini", ".txt" };

			ListViewItem lvi;
			foreach(string s in extensions)
			{
				int index = m_iconManager.GetFileIcon(s);
				lvi = new ListViewItem(s, index);

				listView1.Items.Add(lvi);
			}

			lvi = new ListViewItem("folder", 0);
			listView1.Items.Add(lvi);
			 */ 

		}

		private void OnItemSelected(object sender, ListViewItemSelectionChangedEventArgs e)
		{
			txtFilename.Text = e.Item.Text;
		}

		private void OnItemActivated(object sender, EventArgs e)
		{
			ItemActivated();
		}

		private void OnFileTypeSelected(object sender, EventArgs e)
		{
			int selectionIndex = cbFileType.SelectedIndex;

			if(selectionIndex != m_currentFilterIndex)
			{
				m_currentFilterIndex = selectionIndex;
				FillListView();
			}
		}
		
		private void OnButtonUpFolderClick(object sender, EventArgs e)
		{
			int numDirs = m_currentPath.GetDirCount();

			if(numDirs > 0)
			{
				string currPath = m_currentPath.GetPath(PathReturnType.NoSeparator, PathFormat.Unix);

				// if we're not at a user's home directory, get rid of the last
				// directory in the path
				if(currPath != "~")
				{
					m_currentPath.RemoveDir(numDirs - 1);
				}
			}

			string path = m_currentPath.GetPath(PathReturnType.NoSeparator, PathFormat.Unix);
			ShowDirectory(path, false, false);

		}

		private void OnButtonHomeFolderClick(object sender, EventArgs e)
		{
			string path = m_userHomeDir.GetPath();

			ShowDirectory(path, true, false);
		}

		private void OnButtonRefreshFolderClick(object sender, EventArgs e)
		{
			string path = m_currentPath.GetPath();
			ShowDirectory(path, true, false);
		}

		private void OnFilenameKeyDown(object sender, PreviewKeyDownEventArgs e)
		{
			// if Enter was pressed, accept the item
			if (e.KeyValue == 13)
			{
				ItemActivated();
			}
		}

		private void OnButtonAcceptClick(object sender, EventArgs e)
		{
			ItemActivated();
		}

		private void OnButtonCancelClick(object sender, EventArgs e)
		{
			this.DialogResult = DialogResult.Cancel;
		}


		private void ItemActivated()
		{
			string userFilename = txtFilename.Text;

			FilePath fp = new FilePath();

			int dirResult = m_currentDirListing.dirNames.IndexOf(userFilename);

			// the item name isn't a directory
			if(dirResult == -1)
			{
				// if we're trying to save a file and the filter isn't "All files (*.*)
				if(!m_openMode && (m_currentFilterIndex != m_filterAllFilesIndex))
				{
					fp.FullName = userFilename;
					string ext = fp.Extension;

					if(ext == "")
					{
						// retrieves the appropriate set of extensions for the 
						// selected filter and uses the first one as the default extension
						fp.Extension = m_fileExtensionList[m_currentFilterIndex][0];
						userFilename = fp.FullName;
					}
				}

				int fileResult = m_currentDirListing.fileNames.IndexOf(userFilename);

				// and it's not a file either
				if(fileResult == -1)
				{
					// trying to open a non-existent file
					if(m_openMode)
					{
						string errorMessage = "\"" + userFilename + "\" was not found.  Please enter or select a valid file name.";
						MessageBox.Show(errorMessage, "Invalid File", MessageBoxButtons.OK, MessageBoxIcon.Error);
						return;
					}
					// trying to save a file and it's a new file
					else
					{
						SaveSelectedFilename(userFilename);
					}
				}
				else
				{
					// filename found, open it
					if(m_openMode)
					{
						SaveSelectedFilename(userFilename);
					}
					// file already exists - confirm overwrite
					else
					{
						string confirmMessage = userFilename + " already exists.  Do you want to replace it?";
						DialogResult dr = MessageBox.Show(confirmMessage, "Overwrite File?", MessageBoxButtons.YesNo, MessageBoxIcon.Question);

						if(dr == DialogResult.Yes)
						{
							SaveSelectedFilename(userFilename);
						}
						else
						{
							return;
						}
					}
				}
			}
			else
			{
				// call ShowDirectory with the new directory

				// m_currentPath gets overwritten in ShowDirectory iff the call succeeds

				FilePath tempDirName = new FilePath(m_currentPath);
				tempDirName.AppendDir(userFilename);

				if(!ShowDirectory(tempDirName.GetPath(PathReturnType.NoSeparator, PathFormat.Unix), false, false))
				{
					// TODO update error message
					string errorMessage = "The directory listing failed.  Please run in circles, scream and shout.";
					MessageBox.Show(errorMessage);
				}
			}

			txtFilename.Text = "";
		}

		public bool Prepare(bool open, string filterString)
		{
			m_openMode = open;
			cbFileType.Items.Clear();


			// TODO take in a real filter string and parse it?
			// TODO make this code generic again, instead of hardcoded
			m_fileExtensionList.Clear();

			List<string> sourceExtensions = new List<string>();
			sourceExtensions.Add("cpp");
			sourceExtensions.Add("c");

			List<string> headerExtensions = new List<string>();
			headerExtensions.Add("h");
			headerExtensions.Add("hpp");

			List<string> allFilesExtensions = new List<string>();
			allFilesExtensions.Add("*.*");

			m_fileExtensionList.Add(sourceExtensions);
			m_fileExtensionList.Add(headerExtensions);
			m_fileExtensionList.Add(allFilesExtensions);


			cbFileType.Items.AddRange(m_fileTypeStrings);
			cbFileType.SelectedIndex = 0;

			m_filterAllFilesIndex = cbFileType.Items.IndexOf(m_fileTypeStrings[2]);

			if(m_openMode)
			{
				btnAccept.Text = "Open";
				this.Text = "Open File";
			}
			else
			{
				btnAccept.Text = "Save";
				this.Text = "Save File As";
			}

			txtFilename.ReadOnly = m_openMode;


			// TODO get user home path
			m_userHomeDir = m_networking.GetUserHomeDirectory();
			m_selectedFile = new FilePath("", PathFormat.Unix);

			txtFilename.Text = "";
			string pathToShow = m_currentPath.GetPath();

			if(pathToShow == "")
			{
				pathToShow = m_userHomeDir.GetPath();
			}

			return ShowDirectory(pathToShow, false, false);

		}

		public bool ShowDirectory(string path, bool refresh, bool showHidden)
		{
			/*
			DirectoryListing dl = new DirectoryListing();

			string[] tempDirs = new string[] { "dir4", "dir1", "dir3", "dir2"};
			string[] tempFiles = new string[] { "code3.cpp", "code1.cpp", "code2.c",
												"other2.cpp", "header1.h", "header3.h", "header2.hpp",
												"text1.txt", "other1.zip", "test1.doc"};

			dl.dirNames.AddRange(tempDirs);
			dl.fileNames.AddRange(tempFiles);
			*/
			// TODO Get actual directory contents from network

			FilePath fp = new FilePath(path, PathFormat.Unix);

			DirectoryListing dl = m_networking.GetDirectoryListing(fp, showHidden);

			listView1.Items.Clear();
			txtCurrentPath.Text = path;
			m_currentPath.AssignDir(path, PathFormat.Unix);

			m_currentDirListing = dl;
			FillListView();

			return true;
		}
		
		
		public void FillListView()
		{
			listView1.Items.Clear();

			m_currentDirListing.dirNames.Sort();
			m_currentDirListing.fileNames.Sort();

			m_dirs.Clear();
			m_files.Clear();

			for(int i = 0; i < m_currentDirListing.dirNames.Count; i++)
			{
				ListViewItem lvi = new ListViewItem();
				string dirName =  m_currentDirListing.dirNames[i];
				lvi.Text = dirName;
				// closed folder icon
				lvi.ImageIndex = 0;
				listView1.Items.Add(lvi);
				m_dirs.Add(dirName);
			}

			List<string> currentFilterExtensions = m_fileExtensionList[m_currentFilterIndex];//new List<string>();

			
			

			int currentFileIconIndex = 0;

			for(int i = 0; i < m_currentDirListing.fileNames.Count; i++)
			{
				FilePath currentFile = new FilePath(m_currentDirListing.fileNames[i], PathFormat.Unix);
				string currentExtension = currentFile.Extension;

				if(currentFilterExtensions[0] == "*.*" ||
					currentFilterExtensions.IndexOf(currentExtension.ToLower()) != -1)
				{
					string filename = currentFile.FullName;
					m_files.Add(filename);
					currentFileIconIndex = m_iconManager.GetFileIcon("." + currentExtension);

					ListViewItem lvi = new ListViewItem();
					lvi.Text = filename;
					lvi.ImageIndex = currentFileIconIndex;
					listView1.Items.Add(lvi);
				}
			}

		}

		private void SaveSelectedFilename(string filename)
		{
			m_selectedFile.Assign(m_currentPath.GetPath(), filename, PathFormat.Unix);

			this.DialogResult = DialogResult.OK;
		}
	}


}
