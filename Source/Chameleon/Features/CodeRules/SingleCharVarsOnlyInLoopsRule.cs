﻿using System.Collections.Generic;
using System.Linq;
using Chameleon.Extensions.ANTLRParser;
using Chameleon.GUI;
using Chameleon.Parsing;
using CodeLite;
using DevInstinct.Patterns;

namespace Chameleon.Features.CodeRules
{
	class SingleCharVarsOnlyInLoopsRule : CodeRuleBase
	{
		private SingleCharVarsOnlyInLoopsRule()
		{
		}

		public override bool ExamineSource(ChameleonEditor ed, ScintillaNet.Range searchRange)
		{
			bool m_checkSucceeded = true;

			CtagsManagerWrapper cmw = Singleton<CtagsManagerWrapper>.Instance;
			ANTLRParser parser = Singleton<ANTLRParser>.Instance;

			int lineNum = searchRange.StartingLine.Number;

			Tag fn = cmw.FunctionFromFileLine(ed.Filename, lineNum + 1, false);

			List<Tag> locals = ed.Context.GetLocalVariables(searchRange.Start);

			if(locals == null)
			{
				m_checkSucceeded = false;
				return false;
			}

			List<Tag> singleCharVars = locals.Where(t => t.name.Length == 1).ToList();

			if(singleCharVars.Count > 0)
			{
				//string functionText = ed.Context.GetFunctionText(lineNum);
				//parser.SetSource(functionText, ed.Filename);

				if(!parser.ParseCompleted)
				{
					// act like nothing's wrong
					return true;
				}

				foreach(Tag t in singleCharVars)
				{
					ASTNode root = parser.GetAST();

					string astText = root.ASTToString();

					List<ASTNode> usages = (from n in root.Descendants()
											where n.text == t.name && n.type == 10
											select n).ToList();

					if(usages.Count > 0)
					{
						ASTNode firstUsage = usages[0];

						ASTNode ancestor = firstUsage.GetAncestor(4);
						if(ancestor != null && ancestor.text == "for")
						{
							continue;
						}
					}

					int errorLine = -1;

					if(t.lineNumber < 0)
					{
						//string varDeclaration = t.scope + " " + t.name;

						//int declarationPos = searchRange.Text.IndexOf(varDeclaration);
						errorLine = searchRange.StartingLine.Number;


						/*
						if(declarationPos != -1)
						{
							int actualPos = searchRange.Start + declarationPos;
							errorLine = ed.Lines.FromPosition(actualPos).Number;
						}	
						*/
					}
					else
					{
						errorLine = searchRange.StartingLine.Number + t.lineNumber;
					}

					if(errorLine < 0)
					{
						continue;
					}

					string errorText = "Single-character variable names only allowed as loop indices";

					AddError(ed, errorLine, errorText);
				}
			}

			return m_checkSucceeded;
		}
	}
}
