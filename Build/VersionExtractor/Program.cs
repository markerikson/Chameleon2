using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;
using System.IO;
using System.Xml.Linq;
using Microsoft.Win32;
using System.Diagnostics;

namespace VersionExtractor
{
	class Program
	{
		static void Main(string[] args)
		{
			Console.Write("Reading Chameleon version...");
			Assembly asm = Assembly.LoadFrom(".\\input\\Chameleon.exe");
			AssemblyName an = asm.GetName();
			Version ver = an.Version;

			StringBuilder sb = new StringBuilder();

			sb.AppendFormat("!define VERSIONMAJOR {0}\n", ver.Major);
			sb.AppendFormat("!define VERSIONMINOR {0}\n", ver.Minor);
			sb.AppendFormat("!define VERSIONBUG {0}\n", ver.Build);
			sb.AppendFormat("!define VERSIONSVN {0}\n", ver.Revision);

			string versionString = string.Format("{0}.{1}.{2}", ver.Major, ver.Minor, ver.Build);
			string versionComplete = versionString + "." + ver.Revision;

			File.WriteAllText(".\\chameleonVersionDefinition.txt", sb.ToString());

			Console.WriteLine(" " + versionComplete);


			DirectoryInfo dir = new DirectoryInfo(".\\input");

			List<FileInfo> files = dir.GetFiles("*.exe").ToList();
			files.AddRange(dir.GetFiles("*.dll"));

			string releaseFolder = "Releases\\" + versionString;
			string relativeReleaseFolder = "..\\Releases\\" + versionString;

			XDocument doc = XDocument.Load("..\\Releases\\\\Updates\\ChameleonUpdateTemplate.xml");

			var xeAddVersion = doc.Descendants().Where(element => element.Name == "AddVersion").First();
			var xeVersion = xeAddVersion.Descendants().Where(element => element.Name == "Version").First();
			var xeChanges = xeAddVersion.Descendants().Where(element => element.Name == "Changes").First();
			var xeFiles = xeAddVersion.Descendants().Where(element => element.Name == "Files").First();
	
			bool wybuildInstalled = false;
			string wybuildPath = "";

			RegistryKey wybuildKey = Registry.LocalMachine.OpenSubKey("SOFTWARE\\wyUpdate\\Setup");

			if(wybuildKey != null)
			{
				string path = (string)wybuildKey.GetValue("");

				if(path != null)
				{
					wybuildInstalled = true;
					wybuildPath = Path.Combine(path, "wyBuild.exe");
				}
			}
			

			DirectoryInfo chameleonFolderInfo = Directory.GetParent(Directory.GetCurrentDirectory());
			string chameleonFolder = chameleonFolderInfo.FullName;
			string absoluteReleaseFolder = Path.Combine(chameleonFolder, releaseFolder);


			if(!Directory.Exists(relativeReleaseFolder))
			{
				Directory.CreateDirectory(relativeReleaseFolder);
			}

			Console.WriteLine("Copying files to " + absoluteReleaseFolder);

			foreach(FileInfo file in files)
			{
				file.CopyTo(relativeReleaseFolder + "\\" + file.Name, true);

				if(wybuildInstalled)
				{
					string filePath = Path.Combine(absoluteReleaseFolder, file.Name);
					XElement xeFile = new XElement("File");
					xeFile.SetAttributeValue("source", filePath);

					if(file.Name == "Chameleon.exe")
					{
						xeFile.SetAttributeValue("execute", "after");
						xeFile.SetAttributeValue("waitforexit", "false");
					}

					xeFiles.Add(xeFile);
				}
			}

			if(File.Exists("LatestChanges.txt"))
			{
				xeChanges.Value = File.ReadAllText("LatestChanges.txt");
			}

			xeVersion.Value = versionString;
			
			string updateFolder = Path.Combine(chameleonFolder, "Releases\\updates");
			string updateXMLPath = Path.Combine(updateFolder, "ChameleonUpdate.xml");
			doc.Save(updateXMLPath);

			ProcessStartInfo psi = new ProcessStartInfo();
			psi.FileName = wybuildPath;
			psi.Arguments = string.Format("\"{0}\" /bwu /bu -add=\"{1}\"", Path.Combine(updateFolder, "Chameleon.wyp"), 
											Path.Combine(updateFolder, "ChameleonUpdate.xml"));
			psi.CreateNoWindow = true;
			psi.UseShellExecute = true;

			Console.WriteLine("Building Chameleon updates...");
			Process proc = Process.Start(psi);

			proc.WaitForExit();

			File.Copy(Path.Combine(updateFolder, "wyUpdate", "wyUpdate.exe"), Path.Combine(absoluteReleaseFolder, "wyUpdate.exe"), true);
			File.Copy(Path.Combine(updateFolder, "wyUpdate", "client.wyc"), Path.Combine(absoluteReleaseFolder, "client.wyc"), true);

			Console.WriteLine("\nInitial build complete.  The installer script can now be run.");
			Console.ReadKey();
		}
	}
}
