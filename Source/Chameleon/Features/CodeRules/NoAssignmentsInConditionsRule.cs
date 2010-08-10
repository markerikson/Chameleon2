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
			m_checkSucceeded = false;

			ANTLRParser parser = Singleton<ANTLRParser>.Instance;

			if(!parser.ParseCompleted)
			{
				return false;
			}


			ASTNode root = parser.GetAST();

			List<ASTNode> conditions = (from n in root.Descendants()
										where n.text == "condition"
										select n).ToList();

			string astText = root.ASTToString();

			foreach(ASTNode condition in conditions)
			{
				List<ASTNode> assignments = (from n in condition.Descendants()
											 where n.text == "assign" || n.text == "="
											 select n).ToList();

				if(assignments.Count > 0)
				{
					foreach(ASTNode assignment in assignments)
					{
						int actualLine = searchRange.StartingLine.Number + assignment.lineNumber - 1;
						string errorText = "Assignment not allowed in boolean conditions";

						AddError(ed, actualLine, errorText);
					}
				}
			}

			List<ASTNode> whiles = (from n in root.Descendants()
										where n.text == "while"
										select n).ToList();

			foreach(ASTNode wh in whiles)
			{
				if(wh.firstChild.text != "condition")
				{
					List<ASTNode> pieces = new List<ASTNode>();

					Func<ASTNode, bool> getWhilePieces = (node =>
					{
						bool keepGoing = true;
						if(node.text == "body")
						{
							keepGoing = false;
						}
						else
						{
							pieces.Add(node);
						}							 

						return keepGoing;
					});

					wh.TraverseAST(false, getWhilePieces);

					List<ASTNode> assignmentPieces = pieces.Where( node => node.text == "assign" || 
													node.text == "=").ToList();

					if(assignmentPieces.Count > 0)
					{
						foreach(ASTNode assignmentPiece in assignmentPieces)
						{
							int actualLine = searchRange.StartingLine.Number + assignmentPiece.lineNumber - 1;
							string errorText = "Assignment not allowed in boolean conditions";

							AddError(ed, actualLine, errorText);
						}
					}
				}
			}

			m_checkSucceeded = true;
			return m_checkSucceeded;
		}
	}
}
