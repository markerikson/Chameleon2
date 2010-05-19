using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace Granados.Extensions
{
	public static class ExtensionMethods
	{
		public static byte[] GetBytes(this string str)
		{
			return GetBytesUTF8(str);
		}

		public static byte[] GetBytesUTF8(this string str)
		{
			return Encoding.UTF8.GetBytes(str);
		}

		public static string GetString(this byte[] bytes)
		{
			return Encoding.Default.GetString(bytes);
		}

		public static long Skip(this Stream stream, long len)
		{
			//Seek doesn't work
			//return Seek(offset, IO.SeekOrigin.Current);
			int i = 0;
			int count = 0;
			byte[] buf = new byte[len];
			while(len > 0)
			{
				i = stream.Read(buf, count, (int)len);//tamir: possible lost of pressision
				if(i <= 0)
				{
					throw new Exception("inputstream is closed");
					//return (s-foo)==0 ? i : s-foo;
				}
				count += i;
				len -= i;
			}
			return count;
		}

	}
}
