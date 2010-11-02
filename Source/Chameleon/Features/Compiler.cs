using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Chameleon.Util;
using Chameleon.Network;
using System.Windows.Forms;
using System.Text.RegularExpressions;
using Chameleon.Extensions.System;

namespace Chameleon.Features
{
	public enum CompileStatus
	{
		Started,
		Finished,
	}

	public enum CompilerMessageType
	{
		Warning,
		Error,
	}

	public class CompilerMessage
	{
		public CompilerMessageType MessageType {get; set;}
		public string Message {get; set;}
		public string Filename { get; set; }
		public int Line { get; set; }
		public int Column { get; set; }
	}

	public class CompilerEventArgs : EventArgs
	{
		public CompileStatus Status { get; set; }

		public List<CompilerMessage> Messages { get; set; }

		public CompilerEventArgs()
		{
			Messages = new List<CompilerMessage>();
		}
	}

	public class Compiler
	{
		// indicates the result of the compilation command (not the compilation itself)
		private static readonly string COMPILE_SUCCESS_TOKEN = "C_O_M_P_I_L_E_SUCCESS";
		private static readonly string COMPILE_FAILURE_TOKEN = "C_O_M_P_I_L_E_FAILED";

		private string m_outputBuffer = "";

		private FileInformation m_currentFile;

		private bool m_compiling;
		private Regex m_reOutput;

		public bool IsCompiling
		{
			get { return m_compiling; }
		}

		public event EventHandler<CompilerEventArgs> CompilerEvent;

		public Compiler()
		{
			m_compiling = false;
			m_reOutput = new Regex(@"(?<filename>.+?):(?<line>\d+)(?::(?<column>\d+))?: ((?<type>warning|error):)?(?<message>.+)$", 
									RegexOptions.Compiled | RegexOptions.Multiline);
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
			m_outputBuffer = "";

			FilePath outputFile = new FilePath(fileInfo.Filename);
			m_currentFile = fileInfo;

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

			if(CompilerEvent != null)
			{
				CompilerEventArgs cea = new CompilerEventArgs();
				cea.Status = CompileStatus.Started;

				CompilerEvent(this, cea);
			}
		}

		private void OnCompilerOutput(string output)
		{
			//MessageBox.Show(output);
			if(output.Contains(Compiler.COMPILE_SUCCESS_TOKEN))
			{
				m_outputBuffer += output.Replace(Compiler.COMPILE_SUCCESS_TOKEN, "");
				
			}
			else if(output.Contains(Compiler.COMPILE_FAILURE_TOKEN))
			{
				m_outputBuffer += output.Replace(Compiler.COMPILE_FAILURE_TOKEN, "");
			}
			else
			{
				m_outputBuffer += output;
				return;
			}

			MatchCollection mc = m_reOutput.Matches(m_outputBuffer);

			CompilerEventArgs cea = new CompilerEventArgs();
			cea.Status = CompileStatus.Finished;

			foreach(Match m in mc)
			{
				CompilerMessage message = new CompilerMessage();
				message.Filename = m.Groups["filename"].Value;
				message.Line = int.Parse(m.Groups["line"].Value);

				string type = m.Groups["type"].Value.ToTitleCase();

				CompilerMessageType messageType;
				if(Enum.TryParse<CompilerMessageType>(type, true, out messageType))
				{
					message.MessageType = messageType;
				}
				else
				{
					message.MessageType = CompilerMessageType.Error;
				}

				
				Group groupColumn = m.Groups["column"];

				if(groupColumn.Success)
				{
					message.Column = int.Parse(groupColumn.Value);
				}
				else
				{
					message.Column = 0;
				}

				message.Message = m.Groups["message"].Value.Trim();

				cea.Messages.Add(message);
			}

			if(this.CompilerEvent != null)
			{
				CompilerEvent(this, cea);
			}

			//MessageBox.Show(m_outputBuffer);
		}
	}
}
