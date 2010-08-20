using System;
using System.Collections.Generic;
using System.Text;
using ExceptionReporting.Extensions;
using ExceptionReporting.Config;
using ExceptionReporting.SystemInfo;

namespace ExceptionReporting.Core
{
	internal class ExceptionReportBuilder
	{
		private readonly ExceptionReportInfo _reportInfo;
		private StringBuilder _stringBuilder;
		private readonly IEnumerable<SysInfoResult> _sysInfoResults;
		private IFileReader _fileReader = new FileReader();

		public ExceptionReportBuilder(ExceptionReportInfo reportInfo)
		{
			_reportInfo = reportInfo;
		}

		public ExceptionReportBuilder(ExceptionReportInfo reportInfo, IEnumerable<SysInfoResult> sysInfoResults)
			: this(reportInfo)
		{
			_sysInfoResults = sysInfoResults;
		}

		public IFileReader FileReader
		{
			set { _fileReader = value; }
		}

		/// <summary>
		/// Build the exception report
		/// </summary>
		public ExceptionReport Build()
		{
			_stringBuilder = new StringBuilder().AppendDottedLine();

			if (_reportInfo.ShowGeneralTab) BuildGeneralInfo();
			if (_reportInfo.ShowExceptionsTab) BuildExceptionInfo();
			if (_reportInfo.ShowAssembliesTab) BuildAssemblyInfo();
			if (_reportInfo.ShowConfigTab) BuildConfigInfo();
			if (_reportInfo.ShowSysInfoTab) BuildSysInfo();
			if (_reportInfo.ShowContactTab) BuildContactInfo();

			return new ExceptionReport(_stringBuilder);
		}

		private void BuildGeneralInfo()
		{
			_stringBuilder.AppendLine("[General Info]")
				.AppendLine()
				.AppendLine("Application: " + _reportInfo.AppName)
				.AppendLine("Version:     " + _reportInfo.AppVersion)
				.AppendLine("Region:      " + _reportInfo.RegionInfo)
				.AppendLine("Machine:     " + _reportInfo.MachineName)
				.AppendLine("User:        " + _reportInfo.UserName)
				.AppendLine("Date: " + _reportInfo.ExceptionDate.ToShortDateString())
				.AppendLine("Time: " + _reportInfo.ExceptionDate.ToShortTimeString())
				.AppendLine();

			_stringBuilder.AppendLine("User Explanation:")
				.AppendLine()
				.AppendFormat("{0} said \"{1}\"", _reportInfo.UserName, _reportInfo.UserExplanation)
				.AppendLine().AppendDottedLine().AppendLine();
		}

		private void BuildExceptionInfo()
		{
		    for (var index = 0; index < _reportInfo.Exceptions.Count; index++)
		    {
		        var exception = _reportInfo.Exceptions[index];

				//TODO maybe omit a number when there's only 1 exception
		        _stringBuilder.AppendLine(string.Format("[Exception Info {0}]", index+1))
		            .AppendLine()
		            .AppendLine(ExceptionHierarchyToString(exception))
		            .AppendLine().AppendDottedLine().AppendLine();
		    }
		}

	    private void BuildAssemblyInfo()
	    {
	    	var digger = new AssemblyReferenceDigger(_reportInfo.AppAssembly);
	    
			_stringBuilder.AppendLine("[Assembly Info]")
				.AppendLine()
				.AppendLine(digger.CreateReferencesString())
				.AppendDottedLine().AppendLine();
		}

		private void BuildConfigInfo()
		{
			_stringBuilder.AppendLine("[Config Settings]").AppendLine();
			_stringBuilder.AppendLine(_fileReader.ReadAll(ConfigReader.GetConfigFilePath()));
			_stringBuilder.AppendDottedLine().AppendLine();
		}

		private void BuildSysInfo()
		{
			_stringBuilder.AppendLine("[System Info]").AppendLine();
			_stringBuilder.Append(SysInfoResultMapper.CreateStringList(_sysInfoResults));
			_stringBuilder.AppendDottedLine().AppendLine();
		}

		private void BuildContactInfo()
		{
			_stringBuilder.AppendLine("[Contact Info]")
				.AppendLine()
				.AppendLine("Email:  " + _reportInfo.ContactEmail)
				.AppendLine("Web:    " + _reportInfo.WebUrl)
				.AppendLine("Phone:  " + _reportInfo.Phone)
				.AppendLine("Fax:    " + _reportInfo.Fax)
				.AppendDottedLine().AppendLine();
		}

		/// <summary>
		/// Create a line-delimited string of the exception hierarchy 
		/// //TODO see Label='EH' in View, this is doing too much and is duplicated
		/// </summary>
		private static string ExceptionHierarchyToString(Exception exception)
		{
			var currentException = exception;
			var stringBuilder = new StringBuilder();
			var count = 0;

			while (currentException != null)
			{
				if (count++ == 0)
					stringBuilder.AppendLine("Top-level Exception");
				else
					stringBuilder.AppendLine("Inner Exception " + (count-1));

				stringBuilder.AppendLine("Type:        " + currentException.GetType())
							 .AppendLine("Message:     " + currentException.Message)
							 .AppendLine("Source:      " + currentException.Source);

				if (currentException.StackTrace != null)
					stringBuilder.AppendLine("Stack Trace: " + currentException.StackTrace.Trim());

				stringBuilder.AppendLine();
				currentException = currentException.InnerException;
			}

			var exceptionString = stringBuilder.ToString();
			return exceptionString.TrimEnd();
		}
	}
}
