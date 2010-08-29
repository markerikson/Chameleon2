using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ExceptionReporting
{
	public static class MailUtilities
	{
		public static string CreateMailTo(string to,
									string cc,
									string bcc,
									string subject,
									string body,
									string attachmentPath)
		{
			StringBuilder stringBuilder = new StringBuilder();
			stringBuilder.Append(Uri.UriSchemeMailto + ':');
			stringBuilder.Append(FormatMailToArgument(to));

			if(!string.IsNullOrEmpty(cc) || !string.IsNullOrEmpty(bcc) ||
				!string.IsNullOrEmpty(subject) || !string.IsNullOrEmpty(body) ||
				!string.IsNullOrEmpty(attachmentPath))
			{
				stringBuilder.Append('?');

				List<string> arguments = new List<string>();

				if(!string.IsNullOrEmpty(subject))
				{
					arguments.Add("subject=" + FormatMailToArgument(subject));
				}

				if(!string.IsNullOrEmpty(body))
				{
					arguments.Add("body=" + FormatMailToArgument(body));
				}

				if(!string.IsNullOrEmpty(cc))
				{
					arguments.Add("CC=" + FormatMailToArgument(cc));
				}

				if(!string.IsNullOrEmpty(bcc))
				{
					arguments.Add("BCC=" + FormatMailToArgument(bcc));
				}

				if(!string.IsNullOrEmpty(attachmentPath))
				{
					arguments.Add("attachment=" + FormatMailToArgument(attachmentPath));
				}

				stringBuilder.Append(string.Join("&", arguments.ToArray()));
			}

			return stringBuilder.ToString();
		}

		private static string FormatMailToArgument(string argument)
		{
			//return Uri.EscapeDataString(argument);    
			string replaced = argument.
				Replace("%", "%25").
				Replace("&", "%26").
				//Replace(":", "%3A").
				Replace("\t", "%0D").
				Replace("\n", "%0A").
				Replace("?", "%3F");//.
				//Replace("\"", "%22").
				//Replace(" ", "%20");

			return replaced;
		}


	}
}
