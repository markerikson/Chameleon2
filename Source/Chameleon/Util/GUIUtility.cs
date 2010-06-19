using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Reflection;
using System.IO;

namespace Chameleon.Util
{
	public class GUIUtility
	{
		public static Icon GetIconFromBitmap(String name)
		{
			try
			{
				Assembly execAssembly = Assembly.GetCallingAssembly();
				string fullName = execAssembly.GetName().Name + "." + name;

				Stream stream = execAssembly.GetManifestResourceStream(fullName);

				Bitmap b = new Bitmap(stream);

				Icon icon = Icon.FromHandle(b.GetHicon());
				return icon;
			}
			catch(Exception)
			{
				return null;
			}			
		}
	}
}
