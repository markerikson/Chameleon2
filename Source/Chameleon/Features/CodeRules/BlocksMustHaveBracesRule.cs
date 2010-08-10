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
			

		}




		public override bool ExamineSource(ChameleonEditor ed, Range searchRange)
		{
			m_checkSucceeded = false;

			ANTLRParser parser = Singleton<ANTLRParser>.Instance;
			
			if(!parser.ParseCompleted)
			{
				return false;
			}

			string[] blockKeywords = new string[] { "if" };//, "for", "while", "case" };
			/*
			Dictionary<string, string> firstNodeAfterBrace = new Dictionary<string, string>();
			firstNodeAfterBrace["if"] = "left";
			firstNodeAfterBrace["for"] = "body";
			firstNodeAfterBrace["while"] = "body";
			*/
			
			int startingLine = searchRange.StartingLine.Number;

			ASTNode root = parser.GetAST();

			string astText = root.ASTToString();

			CPPScannerWrapper scanner = new CPPScannerWrapper();

			foreach(string keyword in blockKeywords)
			{
				List<ASTNode> nodes = (from n in root.Descendants()
										where n.text == keyword
										select n).ToList();

				BlockInfo bi = m_blocks[keyword];

				foreach(ASTNode keywordNode in nodes)
				{
					Line l = ed.Lines[startingLine + keywordNode.lineNumber - 1];

					int col = 1;

					if(keywordNode.column > 0)
					{
						col = keywordNode.column;
					}
					else
					{
						int index = l.Text.IndexOf(keywordNode.text);
						col = index + keywordNode.text.Length + 1;
						/*
						if(index != -1)
						{
							
						}
						else
						{

						}
						col =  + keywordNode.text
						*/
					}
					
					int pos = l.StartPosition + col;//ed.FindColumn(l.Number, col);

					//pos += keywordNode.text.Length;

					if(bi.hasCondition)
					{
						//int openBracePos = ed.Context.FindNextString(pos, "{", false);
						int endPos = 0;
						ed.MatchBraceForward('(', pos, ref endPos);
						pos = endPos;
					}

					
					

					
					int nextCharPos = ed.Context.FindNextString(pos + 1, "[^\\s]", true);
					char c = ed.NativeInterface.GetCharAt(nextCharPos);//GetRange(nextCharPos, nextCharPos).Text[0];
					bool foundBraces = false;
					

					if(c == '{')
					{
						/*
						ASTNode blockNode = keywordNode.GetBlock();

						if(blockNode == null)
						{
							continue;
						}

						ASTNode lastChildNode = (from n in blockNode.Descendants()
													orderby n.lineNumber, n.column descending
													select n).First();
						*/
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
							//Line braceLine = ed.Lines.FromPosition(closeBracePos);
						}

					}
					
					
					if(!foundBraces)
					{
						AddError(ed, l.Number, "Must have '{ }' around a block");
					}
					
					int q = 42;
				}

			}


			m_checkSucceeded = true;
			return m_checkSucceeded;
		}
	}
}
