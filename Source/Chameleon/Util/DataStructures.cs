using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Chameleon.Util
{
	public enum FileLocation
	{
		Local,
		Remote,
		Unknown
	}

	public class DirectoryListing
	{
		public List<string> dirNames;
		public List<string> fileNames;

		public DirectoryListing()
		{
			dirNames = new List<string>();
			fileNames = new List<string>();
		}
	}

	public class FileInformation
	{
		private FileLocation m_location;
		private FilePath m_filename;

		public FilePath Filename
		{
			get { return m_filename; }
			set
			{
				m_filename = value;
			}
		}
		public Chameleon.Util.FileLocation Location
		{
			get { return m_location; }
			set
			{ m_location = value; }
		}

		public FileInformation()
		{
			m_location = FileLocation.Unknown;
			m_filename = new FilePath();
		}
	}
}
