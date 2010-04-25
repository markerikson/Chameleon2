using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace Chameleon.Util
{
	public enum PathFormat
	{
		Windows,
		Unix,
	}

	[Flags]
	public enum PathNormalize
	{
		 ENV_VARS = 0x0001,  // replace env vars with their values
		 DOTS = 0x0002,  // squeeze all .. and . and prepend cwd
		 TILDE = 0x0004,  // Unix only: replace ~ and ~user
		 CASE = 0x0008,  // if case insensitive => tolower
		 ABSOLUTE = 0x0010,  // make the path absolute
		 LONG = 0x0020,  // make the path the long form
		 SHORTCUT = 0x0040,  // resolve the shortcut, if it is a shortcut
		 ALL = 0x00ff & ~ CASE,
	}

	[Flags]
	public enum PathReturnType
	{
		NoSeparator,
		GetVolume,
		GetSeparator,
	}

	public class FilePath
	{
		private List<string> m_dirs;
		
		private string m_ext;
		private string m_name;
		private string m_volume;
		private PathFormat m_format;
		
		private bool m_relative;
		private bool m_hasExt;

		public FilePath()
		{
			Clear();
		}

		public FilePath(FilePath otherPath)
		{
			Assign(otherPath);
		}

		public FilePath(string fullPath, PathFormat format)
		{
			Assign(fullPath, format);
		}

		public void Clear()
		{
			m_ext = "";
			m_name = "";
			m_volume = "";
			m_format = PathFormat.Windows;

			m_relative = true;
			m_hasExt = false;

			m_dirs = new List<string>();
		}

		public FilePath(string path, string name, PathFormat format)
		{
			Assign(path, name, format);
		}

		public void Assign(FilePath otherPath)
		{
			Clear();

			m_volume = otherPath.Volume;
			m_name = otherPath.Name;
			m_ext = otherPath.Extension;
			m_relative = otherPath.m_relative;
			m_hasExt = otherPath.m_hasExt;

			m_dirs.AddRange(otherPath.m_dirs);
		}

		public void Assign(string fullPath, PathFormat format)
		{
			Clear();

			Dictionary<string, string> split = SplitPath(fullPath, format);

			string name = "";

			if(fullPath.IndexOf("<untitled>") > -1)
			{
				name = fullPath;
			}
			else
			{
				name = split["name"];
			}

			Assign(split["volume"], split["path"], name, split["ext"], format);
		}

		public void Assign(string path, string name, PathFormat format)
		{
			Clear();

			// always recognize path as a directory, even if it doesn't end
			// with a slash
			string originalPath = path;

			if(!String.IsNullOrWhiteSpace(path) && !EndsWithPathSeparator(path))
			{
				path += GetPathSeparator(format);
			}

			Dictionary<string, string> splitPath = SplitPath(path, format);
			Dictionary<string, string> splitName = SplitPath(name, format);

			Assign(splitPath["volume"], splitPath["path"], splitName["name"], splitName["ext"], format);
		}

		public void Assign(string volume, string path, string name, 
							string ext, PathFormat format)
		{
			m_volume = volume;
			m_name = name;
			m_ext = ext;
			m_format = format;
			m_hasExt = (ext != "");

			SetPath(path, format);
		}

		public void AssignDir(string dir, PathFormat format)
		{
			Assign(dir, "", format);
		}


		public bool Normalize(PathNormalize flags, string cwd, PathFormat format)
		{
			if(flags.HasFlag(PathNormalize.ENV_VARS))
			{
				// not dealing with environment variables for now
			}

			FilePath curDir = new FilePath();
			List<string> dirs = new List<string>(m_dirs);

			if( flags.HasFlag(PathNormalize.ABSOLUTE) && !IsAbsolute(format) )
			{
				if(cwd == "")
				{
					curDir.Volume = Volume;
				}
				else
				{
					curDir.Volume = cwd;
				}
			}

			if(format == PathFormat.Unix && flags.HasFlag(PathNormalize.TILDE))
			{
				if(dirs.Count > 0)
				{
					string dir = dirs[0];

					if(!(dir == "") && dir[0] == '~')
					{
						// TODO
						// curDir.AssignDir(// get user home here);

						m_relative = true;
						dirs.RemoveAt(0);
					}
				}
			}

			if(curDir.IsOK)
			{
				if(Volume != "" && curDir.Volume != "")
				{
					Volume = curDir.Volume;

					if(!m_relative)
					{
						curDir.Clear();
					}
				}

				List<string> dirsNew = curDir.Dirs;

				dirs.InsertRange(0, dirsNew);

				if(curDir.IsAbsolute(format))
				{
					m_relative = false;
				}

				
			}

			m_dirs.Clear();

			for(int i = 0; i < dirs.Count; i++)
			{
				string dir = dirs[i];

				if(flags.HasFlag(PathNormalize.DOTS))
				{
					if(dir == ".")
					{
						continue;
					}

					if(dir == "..")
					{
						if(m_dirs.Count == 0)
						{
							// Too many ".." in the path
							return false;
						}

						m_dirs.RemoveAt(m_dirs.Count - 1);
						continue;
					}

					m_dirs.Add(dir);
				}
			}

			return true;
		}

		public bool MakeRelativeTo(string pathBase, PathFormat format)
		{
			FilePath fnBase = new FilePath();
			fnBase.AssignDir(pathBase, format);

			string cwd = Directory.GetCurrentDirectory();

			PathNormalize norm = PathNormalize.ALL & ~PathNormalize.CASE;

			Normalize(norm, cwd, format);
			fnBase.Normalize(norm, cwd, format);

			bool withCase = IsCaseSensitive(format);

			if(string.Compare(Volume, fnBase.Volume, !withCase) == 0)
			{
				return false;
			}

			m_volume = "";

			while( m_dirs.Count > 0 && fnBase.m_dirs.Count > 0 &&
				string.Compare(m_dirs[0], fnBase.m_dirs[0], !withCase) == 0)
			{
				m_dirs.RemoveAt(0);
				fnBase.m_dirs.RemoveAt(0);
			}

			int count = fnBase.m_dirs.Count;

			for(int i = 0; i < count; i++)
			{
				m_dirs.Insert(0, "..");
			}

			if(format == PathFormat.Unix || format == PathFormat.Windows)
			{
				if(m_dirs.Count == 0 && IsDir)
				{
					m_dirs.Add(".");
				}
			}

			m_relative = true;

			return true;
		}

		public bool MakeAbsolute(string cwd, PathFormat format)
		{
			PathNormalize norm = PathNormalize.DOTS | PathNormalize.ABSOLUTE
								| PathNormalize.TILDE;
			return Normalize(norm, cwd, format);
		}

		public bool SameAs(FilePath otherPath, PathFormat format)
		{
			FilePath path1 = new FilePath(this);
			FilePath path2 = new FilePath(otherPath);
			
			string cwd = Directory.GetCurrentDirectory();

			PathNormalize norm = PathNormalize.ALL | PathNormalize.CASE;

			path1.Normalize(norm, cwd, format);
			path2.Normalize(norm, cwd, format);

			if(path1.GetFullPath() == path2.GetFullPath())
			{
				return true;
			}

			return false;
		}

		public bool IsAbsolute(PathFormat format)
		{
			if(m_relative)
			{
				return false;
			}

			if(GetVolumeSeparator(format) == "")
			{
				if(Volume == "")
				{
					return false;
				}
			}

			return true;
		}

		public bool IsRelative(PathFormat format)
		{
			return !IsAbsolute(format);
		}

		public static string GetVolumeSeparator(PathFormat format)
		{
			string sepVol = "";

			if(format == PathFormat.Windows)
			{
				sepVol = ":";
			}

			return sepVol;
		}

		public static string GetPathSeparators(PathFormat format)
		{
			string seps = "";

			switch(format)
			{
				case PathFormat.Windows:
				{
					seps = "\\" + "/";
					break;
				}
				case PathFormat.Unix:
				{
					seps = "/";
					break;
				}
			}

			return seps;
		}

		public static string GetPathTerminators(PathFormat format)
		{
			return GetPathSeparators(format);
		}

		public static char GetPathSeparator(PathFormat format)
		{
			return GetPathSeparators(format)[0];
		}

		public static bool IsPathSeparator(char ch, PathFormat format)
		{
			return ch != '\0' && GetPathSeparators(format).IndexOf(ch) != -1;
		}

		public static bool IsCaseSensitive(PathFormat format)
		{
			return (format == PathFormat.Unix);
		}

		public static Dictionary<string, string> SplitPath(string fullPathWithVolume, PathFormat format)
		{
			Dictionary<string, string> splitVol = SplitVolume(fullPathWithVolume, format);
			Dictionary<string, string> results = new Dictionary<string, string>();

			string fullPath = splitVol["path"];

			int posLastDot = fullPath.LastIndexOf(".");
			int posLastSlash = fullPath.LastIndexOf(GetPathSeparator(format));

			if(posLastDot != -1 && 
				(posLastDot == 0 || 
				IsPathSeparator(fullPath[posLastDot - 1], format)))
			{
				posLastDot = -1;
			}
			
			if( posLastDot != -1 && posLastSlash != -1 && 
				posLastDot < posLastSlash)
			{
				posLastDot = -1;
			}

			if(posLastSlash == -1)
			{
				results["path"] = "";
			}
			else
			{
				int len = posLastSlash;
				results["path"] = fullPath.Substring(0, len);
			}

			int nStart = (posLastSlash == -1) ? 0 : posLastSlash + 1;
			int count;

			if(posLastDot == -1)
			{
				count = fullPath.Length;
			}
			else if(posLastSlash == -1)
			{
				count = posLastDot;
			}
			else
			{
				count = posLastDot - posLastSlash - 1;
			}

			results["name"] = fullPath.Substring(nStart, count);

			if(posLastDot == -1)
			{
				results["ext"] = "";
			}
			else
			{
				results["ext"] = fullPath.Substring(posLastDot + 1);
			}

			results["volume"] = splitVol["volume"];

			return results;
		}

		public static Dictionary<string, string> SplitVolume(string pathWithVolume, PathFormat format)
		{
			string fullPath = pathWithVolume;

			Dictionary<string, string> results = new Dictionary<string, string>();

			if(format == PathFormat.Windows)
			{
				string sepVol = GetVolumeSeparator(format);

				int posFirstColon = fullPath.IndexOf(sepVol);

				if(posFirstColon > -1)
				{
					results["volume"] = fullPath.Substring(0, posFirstColon);

					fullPath = fullPath.Substring(posFirstColon + sepVol.Length);
				}
				else
				{
					results["volume"] = "";
				}

				results["path"] = fullPath;
			}
			else
			{
				results["volume"] = "";
				results["path"] = "";
			}

			return results;
		}

		public int GetDirCount()
		{
			return m_dirs.Count;
		}

		public void AppendDir(string dir)
		{
			if(IsValidDirComponent(dir))
			{
				m_dirs.Add(dir);
			}
		}

		public void PrependDir(string dir)
		{
			InsertDir(0, dir);
		}

		public void InsertDir(int before, string dir)
		{
			if(IsValidDirComponent(dir))
			{
				m_dirs.Insert(before, dir);
			}
		}

		public void RemoveDir(int pos)
		{
			m_dirs.RemoveAt(pos);
		}

		public void RemoveLastDir()
		{
			RemoveDir(GetDirCount() - 1);
		}

		public string Extension
		{
			get { return m_ext; }
			set { m_ext = value; }
		}

		public string Name
		{
			get { return m_name; }
			set  {m_name = value; }
		}

		public string Volume
		{
			get { return m_volume;  }
			set { m_volume = value; }
		}

		public string FullName
		{
			get 
			{
				string fullName = "";

				if(m_hasExt)
				{
					fullName = m_name + "." + m_ext;
				}
				else
				{
					fullName = m_name;
				}

				return fullName;
			}
			set 
			{
				Dictionary<string, string> split = SplitPath(value, m_format);
				m_name = split["name"];
				m_ext = split["ext"];
				m_hasExt = (m_ext != "");
			}
		}

		public List<string> Dirs
		{
			get { return m_dirs; }
			set { m_dirs = value; }
		}

		public string Path
		{
			get 
			{
				PathReturnType pathType = PathReturnType.GetSeparator | PathReturnType.GetVolume;
				return GetPath(pathType, m_format);
			}
			set 
			{
				SetPath(value, m_format);
			}
		}

		public PathFormat Format
		{
			get { return m_format; }
			set { m_format = value; }
		}

		public string GetPath(PathReturnType pathType, PathFormat format)
		{
			string fullPath = "";

			if(pathType.HasFlag(PathReturnType.GetVolume))
			{
				fullPath = Volume + GetVolumeSeparator(format);
			}

			switch(format)
			{
				case PathFormat.Windows:
				{
					if(!m_relative)
					{
						fullPath += "\\";						
					}
					break;
				}
				case PathFormat.Unix:
				{
					if(!m_relative)
					{
						if(m_dirs.Count == 0 || m_dirs[0] != "~")
						{
							fullPath += "/";
						}
					}
					break;
				}
			}

			if(m_dirs.Count == 0)
			{
				return fullPath;
			}

			for(int i = 0; i < m_dirs.Count; i++)
			{
				switch(format)
				{
					case PathFormat.Windows:
					case PathFormat.Unix:
					{
						fullPath += m_dirs[i];
						break;
					}
				}

				if(pathType.HasFlag(PathReturnType.GetSeparator) || 
					i != m_dirs.Count - 1)
				{
					fullPath += GetPathSeparator(format);
				}
			}

			return fullPath;
		}

		public void SetPath(string pathOrig, PathFormat format)
		{
			m_dirs.Clear();

			if(pathOrig == "")
			{
				m_relative = true;

				return;
			}


			Dictionary<string, string> results = SplitVolume(pathOrig, format);
			string volume = results["volume"];
			string path = results["path"];

			if(volume != "")
			{
				m_relative = false;
				Volume = volume;
			}

			char leadingChar = path[0];

			switch(format)
			{
				case PathFormat.Unix:
				{
					m_relative = (leadingChar != '/' && leadingChar != '~');
					break;
				}
				case PathFormat.Windows:
				{
					m_relative = !IsPathSeparator(leadingChar, format);
					break;
				}
			}

			char[] delimiters = GetPathSeparators(format).ToCharArray();
			string[] tokens = path.Split(delimiters, StringSplitOptions.RemoveEmptyEntries);

			foreach(string token in tokens)
			{
				if(token != "")
				{
					m_dirs.Add(token);
				}
			}
		}

		public string GetFullPath()
		{
			return GetFullPath(m_format);
		}

		public string GetFullPath(PathFormat format)
		{
			PathReturnType pathType = PathReturnType.GetVolume | PathReturnType.GetSeparator;
			string fullPath = GetPath(pathType, format);

			fullPath += FullName;

			return fullPath;
		}

		public bool IsOK
		{
			get
			{
				return m_dirs.Count > 0 || m_name != "" || !m_relative ||
				   m_ext != "" || m_hasExt;
			}
		}

		public bool IsDir
		{
			get { return m_name == "" && m_ext == "";  }
		}

		private bool EndsWithPathSeparator(string path)
		{
			string pathSeps = GetPathSeparators(m_format);

			char lastChar = path[path.Length - 1];

			foreach(char c in pathSeps)
			{
				if(c == lastChar)
				{
					return true;
				}
			}

			return false;
		}

		private bool IsValidDirComponent(string dir)
		{
			if(dir == "")
			{
				return false;
			}

			string volSep = GetVolumeSeparator(m_format);

			for(int i = 0; i < dir.Length; i++)
			{
				char ch = dir[i];
				string str = ch.ToString();
				if(str == volSep || IsPathSeparator(ch, m_format))
				{
					return false;
				}
			}

			return true;
		}


	}
}
