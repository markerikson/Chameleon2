using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;
using System.IO;

namespace VersionExtractor
{
	class Program
	{
		static void Main(string[] args)
		{
			Assembly asm = Assembly.LoadFrom(".\\input\\Chameleon.exe");
			AssemblyName an = asm.GetName();
			Version ver = an.Version;

			StringBuilder sb = new StringBuilder();

			sb.AppendFormat("!define VERSIONMAJOR {0}\n", ver.Major);
			sb.AppendFormat("!define VERSIONMINOR {0}\n", ver.Minor);
			sb.AppendFormat("!define VERSIONBUG {0}\n", ver.Build);
			sb.AppendFormat("!define VERSIONSVN {0}\n", ver.Revision);

			File.WriteAllText(".\\chameleonVersionDefinition.txt", sb.ToString());
		}
	}
}
