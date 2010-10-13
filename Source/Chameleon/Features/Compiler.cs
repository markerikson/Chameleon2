using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Chameleon.Util;
using Chameleon.Network;
using System.Windows.Forms;

namespace Chameleon.Features
{
	public class Compiler
	{
		private static readonly string COMPILE_SUCCESS_TOKEN = "C_O_M_P_I_L_E_SUCCESS";
		private static readonly string COMPILE_FAILURE_TOKEN = "C_O_M_P_I_L_E_FAILED";

		public Compiler()
		{

		}



		public void CompileFile(FileInformation fileInfo)
		{
			if(fileInfo.Location == FileLocation.Unknown)
			{
				return;
			}

			if(fileInfo.Filename.Extension != "cpp")
			{
				return;
			}

			StringBuilder sb = new StringBuilder();

			FilePath outputFile = new FilePath(fileInfo.Filename);

			if(fileInfo.Location == FileLocation.Remote)
			{
				// run g++, include GDB debug info, each error message on one line
				sb.Append("g++ -g -fmessage-length=0 ");

				outputFile.Extension = "out";

				// output filename, quoted
				sb.AppendFormat("-o \"{0}\" ", outputFile.GetFullPath(PathFormat.Unix));
				
				// input filename, quoted
				sb.AppendFormat("\"{0}\" ", fileInfo.Filename.GetFullPath(PathFormat.Unix));

				// finish by echoing a token to indicate whether the command (not compile) succeeded
				sb.AppendFormat(" && echo {0} || echo {1}", Compiler.COMPILE_SUCCESS_TOKEN, 
								Compiler.COMPILE_FAILURE_TOKEN);

				ChameleonNetworking.Instance.ExecuteRemoteCommand(sb.ToString(), OnCompilerOutput);
			}
			else if(fileInfo.Location == FileLocation.Local)
			{
				outputFile.Extension = "exe";
			}
		}

		private void OnCompilerOutput(string output)
		{
			MessageBox.Show(output);
		}
	}
}
