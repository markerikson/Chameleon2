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
			m_checkSucceeded = false;

			CtagsManagerWrapper cmw = Singleton<CtagsManagerWrapper>.Instance;

			List<Tag> allGlobals = cmw.TagsByScope("<global>");
			List<Tag> globalVars = allGlobals.Where(t => t.kind == "variable").ToList();

			if(globalVars.Count > 0)
			{
				foreach(Tag t in globalVars)
				{
					int actualLine = t.lineNumber - 1;
					string errorText = "Global variables are not allowed";

					AddError(ed, actualLine, errorText);
				}
			}

			m_checkSucceeded = true;
			return m_checkSucceeded;
		}
	}
}
