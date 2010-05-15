using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Chameleon.Util;
using Etier.IconHelper;


namespace Chameleon.GUI
{
	public partial class RemoteFileDialog : Form
	{
		private IconListManager m_iconManager;

		private bool m_openMode;
		private int m_filterAllFilesIndex;
		private int m_currentFilterIndex;

		private FilePath m_currentPath;
		private DirectoryListing m_currentDirListing;

		private List<string> m_dirs;
		private List<string> m_files;

		string[] m_fileTypeStrings = new string[] 
		{
			"C++ source files (*.c, *.cpp)",
			"C++ header files (*.h, *.hpp)",
			"All files (*.*)"
		};

		public RemoteFileDialog()
		{
			InitializeComponent();

			FormFontFixer.Fix(this);

			m_currentPath = new FilePath("~", PathFormat.Unix);

			m_dirs = new List<string>();
			m_files = new List<string>();
				
			cbFileType.Items.AddRange(m_fileTypeStrings);
			cbFileType.SelectedItem = m_fileTypeStrings[0];			

			m_iconManager = new IconListManager(m_images, IconReader.IconSize.Small);
			listView1.SmallImageList = m_images;

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

		}

		private void OnButtonHomeFolderClick(object sender, EventArgs e)
		{

		}

		private void OnButtonRefreshFolderClick(object sender, EventArgs e)
		{

		}

		private void OnFilenameKeyDown(object sender, PreviewKeyDownEventArgs e)
		{
			// if Enter was pressed, accept the item
			if (e.KeyValue == 13)
			{
				
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

		}

		public bool Prepare(bool open, string filterString)
		{
			m_openMode = open;
			cbFileType.Items.Clear();

			// TODO take in a real filter string and parse it?

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

			// TODO get user home path

			txtFilename.Text = "";
			string path = m_currentPath.GetPath(PathReturnType.GetSeparator, PathFormat.Unix);
			return ShowDirectory(path, false, false);

		}

		public bool ShowDirectory(string path, bool refresh, bool showHidden)
		{
			DirectoryListing dl = new DirectoryListing();

			string[] tempDirs = new string[] { "dir4", "dir1", "dir3", "dir2"};
			string[] tempFiles = new string[] { "code3.cpp", "code1.cpp", "code2.c",
												"other2.cpp", "header1.h", "header3.h", "header2.hpp",
												"text1.txt", "other1.zip", "test1.doc"};

			dl.dirNames.AddRange(tempDirs);
			dl.fileNames.AddRange(tempFiles);
			// TODO Get actual directory contents from network
			/*
			if(!Network.GetDirListing(path, ref dl, refresh, showHidden))
			{
				return false;
			}
			*/

			listView1.Items.Clear();
			txtCurrentPath.Text = path;
			m_currentPath.AssignDir(path, PathFormat.Unix);

			m_currentDirListing = dl;
			FillListView();

			return false;
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

			List<string> currentFilterExtensions = new List<string>();

			// TODO make this code generic again, instead of hardcoded
			switch(m_currentFilterIndex)
			{
				case 0:
				{
					currentFilterExtensions.Add("cpp");
					currentFilterExtensions.Add("c");
					break;
				}
				case 1:
				{
					currentFilterExtensions.Add("h");
					currentFilterExtensions.Add("hpp");
					break;
				}
				case 2:
				{
					currentFilterExtensions.Add("*.*");
					break;
				}
			}

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
	}
}
