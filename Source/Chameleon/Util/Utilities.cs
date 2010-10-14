using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Reflection;
using System.Windows.Forms;
using System.Threading.Tasks;

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

		public static bool IsDesignmode
		{
			get
			{
				// HACK Crude, ugly, and nasty hack, but it seems there's not many
				// good ways to do this
				return Application.ExecutablePath.ToLower().IndexOf("devenv.exe") > -1;
			}
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

	public static class FireAndForgetMethods
	{
		public static void FireAndForget<T>(this Action<T> act, T arg1)
		{
			var tsk = Task.Factory.StartNew(() => act(arg1),
											 TaskCreationOptions.LongRunning);
			tsk.ContinueWith(cnt => cnt.Dispose());
		}
	}

}
