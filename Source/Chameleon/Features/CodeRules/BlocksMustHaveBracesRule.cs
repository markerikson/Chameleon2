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
	class BlocksMustHaveBracesRule : CodeRuleBase
	{
		private BlocksMustHaveBracesRule()
		{

		}


		public override bool ExamineSource(ChameleonEditor ed, Range searchRange)
		{
			bool ruleResult = true;

			string[] blockKeywords = new string[] { "if", "else", "for", "while" };


			return ruleResult;
		}
	}
}
