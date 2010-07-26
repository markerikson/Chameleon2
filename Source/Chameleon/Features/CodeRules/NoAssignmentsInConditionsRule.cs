using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Chameleon.Parsing;
using DevInstinct.Patterns;
using ScintillaNet;
using Chameleon.GUI;
using Chameleon.Util;

namespace Chameleon.Features.CodeRules
{
	public class NoAssignmentsInConditionsRule : CodeRuleBase
	{
		private NoAssignmentsInConditionsRule()
		{
		}

		public override bool ExamineSource(ChameleonEditor ed, ScintillaNet.Range searchRange)
		{
			

			ANTLRParser parser = Singleton<ANTLRParser>.Instance;

			if(!parser.ParseCompleted)
			{
				return false;
			}


			ASTNode root = parser.GetAST();
			bool ruleResult = true;

			List<ASTNode> conditions = (from n in root.Descendants()
										where n.text == "condition"
										select n).ToList<ASTNode>();

			string astText = root.ASTToString();

			foreach(ASTNode condition in conditions)
			{
				List<ASTNode> assignments = (from n in condition.Descendants()
											 where n.text == "assign" || n.text == "="
											 select n).ToList<ASTNode>();

				if(assignments.Count > 0)
				{
					foreach(ASTNode assignment in assignments)
					{
						int actualLine = searchRange.StartingLine.Number + assignment.lineNumber - 1;
						string errorText = "Assignment not allowed in boolean conditions";

						AddError(ed, actualLine, errorText);
					}

					ruleResult = false;
				}
			}

			return ruleResult;
		}
	}
}
