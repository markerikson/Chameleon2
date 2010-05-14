using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

using Granados.Extensions;

namespace Granados
{
	class Util
	{
		internal static string Unquote(string _path)
		{
			byte[] path = _path.GetBytes();
			int pathlen = path.Length;
			int i = 0;

			while(i < pathlen)
			{
				if(path[i] == '\\')
				{
					if(i + 1 == pathlen)
						break;

					//java.System.arraycopy(path, i + 1, path, i, path.Length - (i + 1));
					Array.Copy(path, i + 1, path, i, path.Length - (i + 1));
					pathlen--;
					continue;
				}
				i++;
			}
			if(pathlen == path.Length) return _path;
			byte[] foo = new byte[pathlen];
			//java.System.arraycopy(path, 0, foo, 0, pathlen);
			Array.Copy(path, 0, foo, 0, pathlen);
			return Encoding.Default.GetString(foo);
		}

		internal static bool glob(byte[] pattern, byte[] name)
		{
			return glob(pattern, 0, name, 0);
		}
		private static bool glob(byte[] pattern, int pattern_index,
			byte[] name, int name_index)
		{
			//System.out.println("glob: "+new String(pattern)+", "+new String(name));
			int patternlen = pattern.Length;
			if(patternlen == 0)
				return false;
			int namelen = name.Length;
			int i = pattern_index;
			int j = name_index;
			while(i < patternlen && j < namelen)
			{
				if(pattern[i] == '\\')
				{
					if(i + 1 == patternlen)
						return false;
					i++;
					if(pattern[i] != name[j]) return false;
					i++; j++;
					continue;
				}
				if(pattern[i] == '*')
				{
					if(patternlen == i + 1) return true;
					i++;
					byte foo = pattern[i];
					while(j < namelen)
					{
						if(foo == name[j])
						{
							if(glob(pattern, i, name, j))
							{
								return true;
							}
						}
						j++;
					}
					return false;
					/*
					if(j==namelen) return false;
					i++; j++;
					continue;
					*/
				}
				if(pattern[i] == '?')
				{
					i++; j++;
					continue;
				}
				if(pattern[i] != name[j]) return false;
				i++; j++;
				continue;
			}
			if(i == patternlen && j == namelen) return true;
			return false;
		}

		public static DateTime Time_T2DateTime(uint time_t)
		{
			long win32FileTime = 10000000 * (long)time_t + 116444736000000000;
			return DateTime.FromFileTimeUtc(win32FileTime).ToLocalTime();
		}

		public static string[] ListFiles(string path)
		{
			string[] dirs = Directory.GetDirectories(path);
			string[] files = Directory.GetFiles(path);
			string[] _list = new string[dirs.Length + files.Length];
			Array.Copy(dirs, 0, _list, 0, dirs.Length);
			Array.Copy(files, 0, _list, dirs.Length, files.Length);
			return _list;
		}
	}
}
