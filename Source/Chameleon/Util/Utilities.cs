using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Reflection;

namespace Chameleon.Util
{
	public static class Utilities
	{
		public static string GetResource(string resourceName)
		{
			Assembly assembly = Assembly.GetExecutingAssembly();
			TextReader textReader = new StreamReader(assembly.GetManifestResourceStream(resourceName));
			string result = textReader.ReadToEnd();
			textReader.Close();

			return result;
		}

		/*
		public static int ColumnToIndex(int column, string text)
		{
			int index = 1;

			for(int i = 0; i < text.Length; i++)
			{
				char c = text[i];
				if(c == '\t')
				{
					index += 4;
				}
				else
				{
					index++;
				}
			}

			if()
		}
		*/
	}
}
