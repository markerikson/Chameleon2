using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Chameleon.GUI;
using ScintillaNet;
using CodeLite;
using DevInstinct.Patterns;
using System.Text.RegularExpressions;

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
		private CtagsManagerWrapper m_cmw;

		public CppContext(ChameleonEditor editor)
		{
			m_editor = editor;
			m_cmw = Singleton<CtagsManagerWrapper>.Instance;
		}

		public bool IsBracket(char ch)
		{
			switch(ch)
			{
				case '{':
				case '[':
				case '(':
				case '}':
				case ']':
				case ')':
					return true;
				default:
					return false;
			}
		}

		public bool IsCloseBracket(char ch)
		{
			switch(ch)
			{
				case '}':
				case ']':
				case ')':
					return true;
				default:
				return false;
			}
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
				SimpleIndent(ch);
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

		// this function expects the line number to be 0-based, like Scintilla
		public bool GetFunctionStartEnd(int lineNum, ref int functionStartPos, ref int functionOpenBracePos, ref int functionEndPos)
		{
			Tag fn = m_cmw.FunctionFromFileLine(m_editor.Filename, lineNum + 1, false);

			if(fn == null)
			{
				return false;
			}

			int pos = m_editor.NativeInterface.PositionFromLine(fn.lineNumber - 1);
			Range searchRange = new Range(pos, m_editor.TextLength, m_editor);
			//m_editor.FindReplace.FindNext("(", false, SearchFlags.Empty, searchRange).Start;
			int openBracePos = FindNextString(pos, "{", false);//m_editor.FindReplace.FindNext("{", false, SearchFlags.Empty, searchRange).Start;
			//int openBracePos = m_editor.NativeInterface.SearchNext(0, "{");
			int matchedPos = 0;

			
			
			if(m_editor.MatchBraceForward('{', openBracePos + 1, ref matchedPos))
			{				
				functionStartPos = pos;
				functionEndPos = matchedPos + 1;
				functionOpenBracePos = openBracePos;
				return true;
			}

			return false;
		}

		// this function expects the line number to be 0-based, like Scintilla
		public string GetFunctionText(int lineNum)
		{
			int fnStart = 0, fnOpen = 0, fnClose = 0;
			string text = "";

			if(GetFunctionStartEnd(lineNum, ref fnStart, ref fnOpen, ref fnClose))
			{
				text = new Range(fnStart, fnClose, m_editor).Text;
			}

			return text;
		}

		public List<Tag> GetLocalVariables(int currPos)
		{
			List<Tag> tags = null;

			int funcStart = 0, funcOpenBrace = 0, funcEnd = 0;
			int lineNum = m_editor.Lines.FromPosition(currPos).Number;

			if(GetFunctionStartEnd(lineNum, ref funcStart, ref funcOpenBrace, ref funcEnd))
			{
				int openParenPos = FindNextString(funcStart, "(", false);
			}
			else
			{
				return null;
			}

			CtagsManagerWrapper cmw = Singleton<CtagsManagerWrapper>.Instance;
			//string textToHere = m_editor.GetTextToPos(funcOpenBrace + 1);
			LanguageWrapper lw = new LanguageWrapper();
			//string scope = lw.OptimizeScope(textToHere);
			string localText = m_editor.GetTextChunk(funcOpenBrace, funcEnd);

			Tag func = cmw.FunctionFromFileLine(m_editor.Filename, lineNum, false);

			if(func == null)
			{
				return null;
			}

			string signature = func.extFields["signature"];

			List<Tag> localVars = lw.GetLocalVariables(localText, "", 0);
			List<Tag> funcVars = lw.GetLocalVariables(signature, "", 0);

			tags = funcVars; 
			tags.AddRange(localVars);

			return tags;
		}

		public int FindNextString(int startPos, string searchString, bool isRegex)
		{
			Range searchRange = new Range(startPos, m_editor.TextLength, m_editor);

			Range resultRange = null;

			if(isRegex)
			{
				Regex reSearch = new Regex(searchString);
				resultRange = m_editor.FindReplace.Find(searchRange, reSearch, false);
			}
			else
			{
				resultRange = m_editor.FindReplace.Find(searchRange, searchString, SearchFlags.Empty);
			}

			if(resultRange == null)
			{
				return -1;
			}

			return resultRange.Start;

			//int foundPos = m_editor.FindReplace.FindNext(text, false, SearchFlags.Empty, searchRange).Start;

			//return foundPos;
		}

		public string GetExpression(int pos, bool onlyWord, ChameleonEditor editor, bool forCC)
		{
			bool cont = true;
			int depth = 0;

			ChameleonEditor ctrl = (editor == null) ? m_editor : editor;


			int position = pos;
			int at = position;
			bool prevGt = false;
			bool prevColon = false;
			while (cont && depth >= 0) 
			{
				char ch = ctrl.PreviousChar(pos, ref at, true);
				position = at;
				//Eof?
				if (ch == 0) {
					at = 0;
					break;
				}

				//Comment?
				int style = ctrl.Styles.GetStyleAt(pos);
				if(IsCommentOrString(pos))
				{
					continue;
				}

				switch (ch) 
				{
					case ';':
					{
						// don't include this token
						at = ctrl.NativeInterface.PositionAfter(at);
						cont = false;
						prevColon = false;
						break;
					}
					case '-':
					{
						if(prevGt)
						{
							prevGt = false;
							//if previous char was '>', we found an arrow so reduce the depth
							//which was increased
							depth--;
						}
						else
						{
							if(depth <= 0)
							{
								//don't include this token
								at = ctrl.NativeInterface.PositionAfter(at);
								cont = false;
							}
						}
						prevColon = false;
						break;
					}
					case ' ':
					case '\n':
					case '\v':
					case '\t':
					case '\r':
					{
						prevGt = false;
						prevColon = false;
						if(depth <= 0)
						{
							cont = false;
							break;
						}
						break;
					}
					case '{':
					case '=':
					{
						prevGt = false;
						prevColon = false;
						cont = false;
						break;
					}
					case '(':
					case '[':
					{
						depth--;
						prevGt = false;
						prevColon = false;
						if(depth < 0)
						{
							//don't include this token
							at = ctrl.NativeInterface.PositionAfter(at);
							cont = false;
						}
						break;
					}
					case ',':
					case '*':
					case '&':
					case '!':
					case '~':
					case '+':
					case '^':
					case '|':
					case '%':
					case '?':
					{
						prevGt = false;
						prevColon = false;
						if(depth <= 0)
						{

							//don't include this token
							at = ctrl.NativeInterface.PositionAfter(at);
							cont = false;
						}
						break;
					}
					case '>':
					{
						prevGt = true;
						prevColon = false;
						depth++;
						break;
					}
					case '<':
					{
						prevGt = false;
						prevColon = false;
						depth--;
						if(depth < 0)
						{

							//don't include this token
							at = ctrl.NativeInterface.PositionAfter(at);
							cont = false;
						}
						break;
					}
					case ')':
					case ']':
					{
						prevGt = false;
						prevColon = false;
						depth++;
						break;
					}
					default:
					{
						prevGt = false;
						prevColon = false;
						break;
					}
				}
			}

			if (at < 0) at = 0;
			string expr = new Range(at, pos, ctrl).Text;
			if ( !forCC ) {
				// If we do not require the expression for CodeCompletion
				// return the un-touched buffer
				return expr;
			}

			//remove comments from it
			CPPScannerWrapper sc = new CPPScannerWrapper();
			sc.SetText(expr);
			string expression = "";
			int type=0;

			for(type = sc.Lex(); type != 0; )
			{
				string token = sc.TokenText();
				expression += token + " ";
			}

			return expression;
		}

		
	}
}
