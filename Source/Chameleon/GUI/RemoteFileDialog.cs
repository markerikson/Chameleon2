#define CHAMELEON

using System;
using System.Collections.Generic;
using System.Collections.Specialized;
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
using Granados;


namespace Chameleon.GUI
{
	public partial class RemoteFileDialog : Form
	{
		#region Private fields
		private IconListManager m_iconManager;

		private bool m_openMode;
		private bool m_folderMode;
		
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

		private List<string> m_fileFilterDescriptions;
		private Dictionary<string, List<string>> m_fileFilterExtensions;

		private static string m_allFilesFilter = "All files (*.*)";
		/*
		string[] m_fileTypeStrings = new string[] 
		{
			"C++ source files (*.c, *.cpp)",
			"C++ header files (*.h, *.hpp)",
			"All files (*.*)"
		};
		*/

		#endregion

		#region Properties
		public static RemoteFileDialog Instance
		{
			get
			{
				return Singleton<RemoteFileDialog>.Instance;
			}
		}


		public Chameleon.Network.Networking Networking
		{
			get { return m_networking; }
			set { m_networking = value; }
		}

		public Chameleon.Util.FilePath SelectedFile
		{
			get { return m_selectedFile; }
		}

		public bool FolderMode
		{
			get { return m_folderMode; }
			set { m_folderMode = value; }
		}

		#endregion

		#region Constructor

		private RemoteFileDialog()
		{
			InitializeComponent();

			toolStrip1.ImageList = m_toolbarImages;
			toolUpFolder.ImageIndex = 0;
			toolHomeFolder.ImageIndex = 1;
			toolRefresh.ImageIndex = 2;

			FormFontFixer.Fix(this);

			m_currentPath = new FilePath("~", PathFormat.Unix);
			m_userHomeDir = new FilePath("~", PathFormat.Unix);
			m_selectedFile = new FilePath();

			m_dirs = new List<string>();
			m_files = new List<string>();

			m_fileExtensionList = new List<List<string>>();

			m_fileFilterDescriptions = new List<string>();
			m_fileFilterExtensions	= new Dictionary<string, List<string>>();						

			m_iconManager = new IconListManager(m_images, IconSize.Small);
			listView1.SmallImageList = m_images;

			m_folderMode = false;
		}

		#endregion

		public void AddFileType(string filterDescription, List<string> extensions)
		{
			m_fileFilterDescriptions.Add(filterDescription);

			for(int i = 0; i < extensions.Count; i++)
			{
				extensions[i] = extensions[i].ToLower();
			}

			m_fileFilterExtensions[filterDescription] = extensions;
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

			string path = m_currentPath.GetPath(PathReturnType.GetSeparator, PathFormat.Unix);
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
			if(m_openMode && m_folderMode)
			{
				if(!AcceptSelectedFolder())
				{
					ItemActivated();
				}

			}
			else
			{
				ItemActivated();
			}
		}

		private void OnButtonCancelClick(object sender, EventArgs e)
		{
			this.DialogResult = DialogResult.Cancel;
		}


		private void ItemActivated()
		{
			string userFilename = txtFilename.Text;

			FilePath fp = new FilePath();

			bool isFile = (m_currentDirListing.dirNames.IndexOf(userFilename) == -1);

			// the item name isn't a directory
			if(isFile)
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
					// at the moment, this will only fail if we get an exception, which is handled internally
					//string errorMessage = "The directory listing failed.  Please run in circles, scream and shout.";
					//MessageBox.Show(errorMessage);
				}
			}

			txtFilename.Text = "";
		}

		// takes a standard Windows file filter string ("Description1|type1;type2|Description2|type3");
		public bool Prepare(bool open, string filterString)
		{
			m_openMode = open;

			cbFileType.Items.Clear();			
			m_fileExtensionList.Clear();

			string[] filterItems = filterString.Split('|');

			if(filterItems.Length % 2 == 0)
			{
				for(int i = 0; i < filterItems.Length; i += 2)
				{
					string filterDescription = filterItems[i];
					string extensionList = filterItems[i + 1];

					string[] extensions = extensionList.Split(';');

					List<string> justExtensions = new List<string>();

					foreach(string starExt in extensions)
					{
						string ext = "";
						if(starExt == "*.*")
						{
							ext = starExt;
						}
						else
						{
							ext = starExt.Substring(2);
						}

						justExtensions.Add(ext);
					}

					cbFileType.Items.Add(filterDescription);
					m_fileExtensionList.Add(justExtensions);
				}
			}

			

			if(!cbFileType.Items.Contains(m_allFilesFilter))
			{
				cbFileType.Items.Add(m_allFilesFilter);

				List<string> allFilesExtension = new List<string>();
				allFilesExtension.Add("*.*");

				m_fileExtensionList.Add(allFilesExtension);

			}

			m_filterAllFilesIndex = cbFileType.Items.IndexOf(m_allFilesFilter);

			cbFileType.SelectedIndex = 0;

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
			FilePath fp = new FilePath(path, PathFormat.Unix);

			DirectoryListing dl = new DirectoryListing();
			
			try
			{
				dl = m_networking.GetDirectoryListing(fp, showHidden);
			}
			catch(Exception e)
			{
				// for simplicity, assume it's an SftpException and we don't have
				// permission to this directory
				MessageBox.Show("You don't have permission to read this directory", "Permissions Error",
								MessageBoxButtons.OK, MessageBoxIcon.Warning);
				return false;
			}
			

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

			if(m_openMode && m_folderMode)
			{
				return;
			}

			List<string> currentFilterExtensions = m_fileExtensionList[m_currentFilterIndex];		
	
			if(currentFilterExtensions.Count == 0)
			{
				return;
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

		private bool AcceptSelectedFolder()
		{
			string filename = txtFilename.Text;

			bool succeeded = true;

			if(filename == "")
			{
				SaveSelectedFilename(txtCurrentPath.Text);
			}
			else
			{
				bool isFolder = (m_currentDirListing.dirNames.IndexOf(filename) > -1);

				if(isFolder)
				{
					SaveSelectedFilename(filename);
				}

				else
				{
					succeeded = false;
				}
			}

			return succeeded;

			// If it's a folder, we're in open mode, and we're in select folders mode,
			// then the user just selected this and we want to return it
			//return isFolder && m_openMode && m_folderMode;
		}

		private void SaveSelectedFilename(string filename)
		{
			if(m_openMode && m_folderMode)
			{
				char pathSep = FilePath.GetPathSeparator(PathFormat.Unix);
				if(filename[0] == pathSep)
				{
					if(!filename.EndsWith(pathSep.ToString()))
					{
						filename += pathSep;
					}
					m_selectedFile.Assign(filename, PathFormat.Unix);
				}
				else
				{
					m_selectedFile.Assign(m_currentPath);
					m_selectedFile.AppendDir(filename);
				}

				m_currentPath.Assign(m_selectedFile);
			}
			else
			{
				m_selectedFile.Assign(m_currentPath.GetPath(), filename, PathFormat.Unix);
			}
			

			this.DialogResult = DialogResult.OK;
		}
	}


}
