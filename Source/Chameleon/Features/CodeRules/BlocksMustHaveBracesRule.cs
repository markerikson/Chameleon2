using System.Collections.Generic;
using System.Linq;
using Chameleon.Extensions.ANTLRParser;
using Chameleon.GUI;
using Chameleon.Parsing;
using DevInstinct.Patterns;
using ScintillaNet;

namespace Chameleon.Features.CodeRules
{
	public class BlockInfo
	{
		public bool hasCondition;
		
		public string keyword;
		public string nextNodeAfterBrace;
		public bool nextNodeIsChild;

		public BlockInfo(string kw, bool cond, string node, bool nnode)
		{
			hasCondition = cond;
			keyword = kw;
			nextNodeAfterBrace = node;
			nextNodeIsChild = nnode;
		}
	}

	public class BlocksMustHaveBracesRule : CodeRuleBase
	{
		private Dictionary<string, BlockInfo> m_blocks;

		private BlocksMustHaveBracesRule()
		{
			m_blocks = new Dictionary<string, BlockInfo>();
			m_blocks["if"] = new BlockInfo("if", true, "right", true);
			m_blocks["for"] = new BlockInfo("for", true, "body", true);
			m_blocks["while"] = new BlockInfo("while", true, "body", true);
			m_blocks["switch"] = new BlockInfo("switch", true, "body", false);
			//m_blocks["right"] = new BlockInfo("right", false, "expression", true);
		}

		public override bool ExamineSource(ChameleonEditor ed, Range searchRange)
		{
			m_checkSucceeded = false;

			ANTLRParser parser = Singleton<ANTLRParser>.Instance;
			
			if(!parser.ParseCompleted)
			{
				return false;
			}

			int startingLine = searchRange.StartingLine.Number;

			ASTNode root = parser.GetAST();
			string astText = root.ASTToString();


			foreach(string keyword in m_blocks.Keys)
			{
				List<ASTNode> nodes = (from n in root.Descendants()
										where n.text == keyword
										select n).ToList();

				BlockInfo bi = m_blocks[keyword];

				foreach(ASTNode keywordNode in nodes)
				{
					Line l = ed.Lines[startingLine + keywordNode.lineNumber - 1];
					int pos = 0;

					// we want to find the char position right after this keyword
					if(keywordNode.column > 0)
					{
						pos = ed.FindColumn(l.Number, keywordNode.column + keywordNode.text.Length - 1);
					}
					else
					{
						if(keywordNode.text == "right")
						{
							pos = l.StartPosition;
						}
						else
						{
							int index = l.Text.IndexOf(keywordNode.text);
							pos = l.StartPosition + index + keywordNode.text.Length;
						}
						
					}

					if(bi.hasCondition)
					{
						int endPos = 0;
						ed.MatchBraceForward('(', pos, ref endPos);
						pos = endPos;
					}
					
					// probably something wrong, but we'll just bail out
					if(pos == 0)
					{
						continue;
					}

YesItsAGotoLabel:
					// find the next non-whitespace character
					int nextCharPos = ed.Context.FindNextString(pos + 1, "[^\\s]", true);
					char c = ed.NativeInterface.GetCharAt(nextCharPos);
					bool foundBraces = false;					

					if(c == '{')
					{
						ASTNode nextNode = null;

						if(bi.nextNodeIsChild)
						{
							nextNode = (from n in keywordNode.Descendants()
										where n.text == bi.nextNodeAfterBrace
										select n).First();
						}
						else
						{
							nextNode = (from n in keywordNode.GetSiblings()
										where n.text == bi.nextNodeAfterBrace
										select n).First();
						}

						int closeBracePos = 0;
						if(ed.MatchBraceForward('{', pos + 1, ref closeBracePos))
						{
							foundBraces = true;
						}

					}
					else if(c == ':' && keywordNode.text == "switch")
					{
						// switches have a ':' after the condition, try the next char
						pos++;
						goto YesItsAGotoLabel;
					}
					
					
					if(!foundBraces)
					{
						AddError(ed, l.Number, "Must have '{ }' around a block");
					}
				}
			}

			m_checkSucceeded = true;
			return m_checkSucceeded;
		}
	}
}
