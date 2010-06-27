using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Chameleon.GUI;
using ScintillaNet;

namespace Chameleon.Features
{
	// Lexical states for SCLEX_CPP
	public enum CPPLexerStyles
	{
		DEFAULT = 0,
		COMMENT = 1,
		COMMENTLINE = 2,
		COMMENTDOC = 3,
		NUMBER = 4,
		WORD = 5,
		STRING = 6,
		CHARACTER = 7,
		UUID = 8,
		PREPROCESSOR = 9,
		OPERATOR = 10,
		IDENTIFIER = 11,
		STRINGEOL = 12,
		VERBATIM = 13,
		REGEX = 14,
		COMMENTLINEDOC = 15,
		WORD2 = 16,
		COMMENTDOCKEYWORD = 17,
		COMMENTDOCKEYWORDERROR = 18,
		GLOBALCLASS = 19,
	}

	public class CppContext
	{
		private ChameleonEditor m_editor;

		public CppContext(ChameleonEditor editor)
		{
			m_editor = editor;
		}

		public bool IsCommentOrString(int pos)
		{
			CPPLexerStyles style = (CPPLexerStyles)m_editor.Styles.GetStyleAt(pos);

			return (style == CPPLexerStyles.COMMENT ||
					style == CPPLexerStyles.COMMENTLINE ||
					style == CPPLexerStyles.COMMENTDOC ||
					style == CPPLexerStyles.COMMENTLINEDOC ||
					style == CPPLexerStyles.COMMENTDOCKEYWORD ||
					style == CPPLexerStyles.COMMENTDOCKEYWORDERROR ||
					style == CPPLexerStyles.STRING ||
					style == CPPLexerStyles.STRINGEOL ||
					style == CPPLexerStyles.CHARACTER);
		}

		public bool IsComment(int pos)
		{
			CPPLexerStyles style = (CPPLexerStyles)m_editor.Styles.GetStyleAt(pos);

			return (style == CPPLexerStyles.COMMENT ||
					style == CPPLexerStyles.COMMENTLINE ||
					style == CPPLexerStyles.COMMENTDOC ||
					style == CPPLexerStyles.COMMENTLINEDOC ||
					style == CPPLexerStyles.COMMENTDOCKEYWORD ||
					style == CPPLexerStyles.COMMENTDOCKEYWORDERROR);
		}

		public void AutoIndent(char ch)
		{
			if(m_editor.Indentation.SmartIndentType == ScintillaNet.SmartIndent.CPP2)
			{
				return;
			}			

			int curpos = m_editor.CurrentPos;
			if (IsComment(curpos) && ch == '\n') 
			{
				// TODO wanted?  needed?
				//AutoAddComment();
				return;
			}

			INativeScintilla ns = m_editor.NativeInterface;

			if (IsCommentOrString(curpos)) 
			{
				SimpleIndent(ch);

				return;
			}

			int line = ns.LineFromPosition(curpos);

			if (ch == '\n') 
			{
				int      prevpos = -1;
				int      foundPos = -1;
				string word;

				char prevChar = m_editor.PreviousChar(curpos, ref prevpos, false);
				word      = m_editor.PreviousWord(curpos, ref foundPos);

				// user hit ENTER after 'else'
				if ( word == "else" ) 
				{
					int prevLine = ns.LineFromPosition(prevpos);
					int prevIndent = ns.GetLineIndentation(prevLine);
					ns.SetLineIndentation(line, prevIndent);
					m_editor.SetCaretAt(ns.GetLineIndentPosition(line));
					ns.ChooseCaretX(); // set new column as "current" column
					return;
				}

				// User typed 'ENTER' immediately after closing brace ')'
				if ( prevpos != -1 && ch == ')' ) 
				{
					int openBracePos = -1;
					int  posWordBeforeOpenBrace = -1;

					if (m_editor.MatchBraceBack(')', prevpos, ref openBracePos)) 
					{
						m_editor.PreviousChar(openBracePos, ref posWordBeforeOpenBrace, false);
						if (posWordBeforeOpenBrace != -1) 
						{
							word = m_editor.PreviousWord(posWordBeforeOpenBrace, ref foundPos);

							// c++ expression with single line and should be treated separatly
							if ( word == "if" || word == "while" || word =="for") 
							{
								int prevLine = ns.LineFromPosition(prevpos);
								ns.SetLineIndentation(line, ns.GetIndent() + ns.GetLineIndentation(prevLine));
								m_editor.SetCaretAt(ns.GetLineIndentPosition(line));
								ns.ChooseCaretX(); // set new column as "current" column
								return;
							}
						}
					}
				}

				// User typed 'ENTER' immediately after colons ':'
				if ( prevpos != -1 && ch == ':' ) 
				{
					int posWordBeforeColons = -1;

					m_editor.PreviousChar(prevpos, ref posWordBeforeColons, false);
					if (posWordBeforeColons != -1) 
					{
						word = m_editor.PreviousWord(posWordBeforeColons, ref foundPos);
						int prevLine = ns.LineFromPosition(posWordBeforeColons);

						// If we found one of the following keywords, un-indent their line by (foldLevel - 1)*indentSize
						if ( word == "public" || word == "private" || word == "protected") 
						{
							SimpleIndent(ch);

							// Indent this line according to the block indentation level

							int foldLevel = (int)(ns.GetFoldLevel(prevLine) & (int)FoldLevel.NumberMask) - (int)FoldLevel.Base;
							if (foldLevel > 0) 
							{
								ns.SetLineIndentation(prevLine, ((foldLevel-1) * ns.GetIndent()) );
								ns.ChooseCaretX();
							}
							return;
						}
					}
				}

				// use the previous line indentation level
				if (prevpos == -1 || ch != '{' || IsCommentOrString(prevpos)) 
				{
					SimpleIndent(ch);
					return;
				}

				// Open brace? increase indent size
				int prevLineNum = ns.LineFromPosition(prevpos);
				int indent2 = ns.GetIndent();
				int lineIndent = ns.GetLineIndentation(prevLineNum);
				ns.SetLineIndentation(line, indent2 + lineIndent);//ns.GetIndent() + ns.GetLineIndentation(prevLineNum));
				m_editor.SetCaretAt(ns.GetLineIndentPosition(line));

			} 
			else if (ch == '}') 
			{
				int matchPos = -1;
				if (!m_editor.MatchBraceBack('}', ns.PositionBefore(curpos), ref matchPos))
					return;
				int secondLine = ns.LineFromPosition(matchPos);
				if (secondLine == line)
					return;
				ns.SetLineIndentation(line, ns.GetLineIndentation(secondLine));

			} 
			else if (ch == '{') 
			{
				string lineString = m_editor.Lines[line].Text;
				lineString.Trim();

				int matchPos = -1;
				char previousChar = m_editor.PreviousChar(ns.PositionBefore(curpos), ref matchPos, false);

				if(previousChar != '{' && lineString == "{") 
				{
					// indent this line according to the previous line
					int currLine = ns.LineFromPosition(m_editor.CurrentPos);
					ns.SetLineIndentation(line, ns.GetLineIndentation(currLine - 1));
					ns.ChooseCaretX();
				}
			}

			// set new column as "current" column
			ns.ChooseCaretX();
		}

		private void SimpleIndent(char ch )
		{
			if(ch == '\n')
			{
				INativeScintilla ns = m_editor as INativeScintilla;
				//just copy the previous line indentation
				Line currentLine = m_editor.Lines.FromPosition(m_editor.CurrentPos);
				ns.SetLineIndentation(currentLine.Number, ns.GetLineIndentation(currentLine.Number - 1));

				//place the caret at the end of the line
				m_editor.SetCaretAt(ns.GetLineIndentPosition(currentLine.Number));
				ns.ChooseCaretX();
			}
		}
	}
}
