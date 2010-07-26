using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Chameleon.Parsing;
using DevInstinct.Patterns;
using ScintillaNet;
using Chameleon.GUI;
using Chameleon.Util;
using CodeLite;

namespace Chameleon.Features.CodeRules
{
	public class NoGlobalVariablesRule : CodeRuleBase
	{
		private NoGlobalVariablesRule()
		{
			// run over the whole file
			m_isGlobal = true;
		}

		public override bool ExamineSource(ChameleonEditor ed, ScintillaNet.Range searchRange)
		{
			bool ruleResult = true;

			CtagsManagerWrapper cmw = Singleton<CtagsManagerWrapper>.Instance;

			List<Tag> globals = cmw.TagsByScope("<global>", "variable", false, false);

			if(globals.Count > 0)
			{
				foreach(Tag t in globals)
				{
					int actualLine = t.lineNumber - 1;
					//int actualLine = searchRange.StartingLine.Number + assignment.lineNumber - 1;
					string errorText = "Global variables are not allowed";

					AddError(ed, actualLine, errorText);
				}
			}
			
			return ruleResult;
		}
	}
}
