using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Reflection;

namespace Chameleon.Util
{
	public class Utilities
	{
		public static string GetResource(string resourceName)
		{
			Assembly assembly = Assembly.GetExecutingAssembly();
			TextReader textReader = new StreamReader(assembly.GetManifestResourceStream(resourceName));
			string result = textReader.ReadToEnd();
			textReader.Close();

			return result;
		}
	}
}
