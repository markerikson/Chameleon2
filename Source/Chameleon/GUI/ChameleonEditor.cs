using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using Chameleon.Features;
using Chameleon.Features.CodeRules;
using Chameleon.Util;
using FarsiLibrary.Win;
using ScintillaNet;
using ArtisticStyle;

namespace Chameleon.GUI
{
	public class ChameleonEditor : Scintilla
	{
		#region Private fields
		private FileInformation m_fileInfo;
		
		private FATabStripItem m_parentTab;

		private static Dictionary<FileLocation, string> m_titlePrefixes;

		private char m_lastCharAdded;
		private bool m_autoAddMatchedBrace;

		private CppContext m_context;
		private static AStyleInterface m_astyle;

		private List<CodeRuleError> m_ruleErrors;

		private List<string> m_cppExtensions;

		private bool m_isCompiled;
		
		#endregion


		#region Properties
		public FATabStripItem ParentTab
		{
			get { return m_parentTab; }
			set { m_parentTab = value; }
		}

		public Chameleon.Util.FileInformation FileInfo
		{
			get { return m_fileInfo; }
			set { m_fileInfo = value; }
		}

		public Chameleon.Util.FileLocation FileLocation
		{
			get { return m_fileInfo.Location; }
			set 
			{ 
				m_fileInfo.Location = value;

				UpdateTitleText();
			}
		}

		public string Filename
		{
			get { return m_fileInfo.Filename.GetFullPath(); }
			set 
			{ 
				PathFormat pathFormat;

				if(m_fileInfo.Location == FileLocation.Local)
				{
					pathFormat = PathFormat.Windows;
				}
				else
				{
					pathFormat = PathFormat.Unix;
				}
				m_fileInfo.Filename.Assign(value, pathFormat);
				UpdateTitleText();
			}
		}

		public Chameleon.Features.CppContext Context
		{
			get { return m_context; }
			set { m_context = value; }
		}

		public bool IsCompiled
		{
			get { return m_isCompiled; }
			set { m_isCompiled = value; }
		}
		#endregion

		#region Constructors
		static ChameleonEditor()
		{
			m_titlePrefixes = new Dictionary<FileLocation, string>();
			m_titlePrefixes[FileLocation.Local] = "(L) ";
			m_titlePrefixes[FileLocation.Remote] = "(R) ";
			m_titlePrefixes[FileLocation.Unknown] = "(?) ";

			m_astyle = new AStyleInterface();
			m_astyle.SetDefaultChameleonStyleOptions();
		}

		public ChameleonEditor()
		{
			m_fileInfo = new FileInformation();
			Filename = "";
			FileLocation = FileLocation.Unknown;

			m_context = new CppContext(this);
			m_autoAddMatchedBrace = true;
			m_lastCharAdded = char.MinValue;

			m_cppExtensions = new List<string>(){"c", "cpp", "h", "hpp"};


			m_ruleErrors = new List<CodeRuleError>();

			NativeInterface.SetMouseDwellTime(500);

			

			this.ModifiedChanged += new EventHandler(OnEditorModifiedChanged);
		}
		#endregion

		private void UpdateTitleText()
		{
			if(m_parentTab != null)
			{
				FileInformation fi = m_fileInfo;
				string prefix = m_titlePrefixes[fi.Location];

				m_parentTab.Title = prefix + fi.Filename.FullName;
			}
		}

		private void OnEditorModifiedChanged(object sender, EventArgs e)
		{
			ChameleonEditor editor = sender as ChameleonEditor;

			if(m_parentTab != null)
			{
				FATabStripItem tab = m_parentTab;

				string tabText = tab.Title;

				if(editor.Modified)
				{
					if(!tabText.EndsWith(" *"))
						tabText += " *";
				}
				else
				{
					if(tabText.EndsWith(" *"))
						tabText = tabText.Substring(0, tabText.Length - 2);
				}

				tab.Title = tabText;
			}
		}

		public void SetCPPEditorStyles()
		{
			this.ConfigurationManager.Language = "cpp";

			Styles.ClearAll();

			IsBraceMatching = true;

			Margins.Margin0.Type = MarginType.Number;
			Margins.Margin0.Width = 40;

			Indentation.IndentWidth = 4;

			Styles.Default.FontName = "Courier New";
			Styles.Default.Size = 10.0f;
			Styles.BraceLight.ForeColor = Color.FromArgb(0, 0, 0);
			Styles.BraceLight.Bold = true;
			Styles.BraceLight.BackColor = Color.FromArgb(0, 255, 255);

			
			// comments are mid-green
			Styles[1].ForeColor = Color.FromArgb(0, 128, 0);
			Styles[2].ForeColor = Color.FromArgb(0, 128, 0);
			Styles[3].ForeColor = Color.FromArgb(0, 128, 0);

			// numbers are red
			Styles[4].ForeColor = Color.FromArgb(255, 0, 0);
			Styles[8].ForeColor = Color.FromArgb(255, 0, 0);

			// keywords are blue
			Styles[5].ForeColor = Color.FromArgb(0, 0, 255);
			Styles[5].Bold = true;

			// strings are teal
			Styles[6].ForeColor = Color.FromArgb(0, 128, 128);
			Styles[7].ForeColor = Color.FromArgb(0, 128, 128);
			Styles[12].ForeColor = Color.FromArgb(0, 128, 128);
			
			// preprocessor is purple
			Styles[9].ForeColor = Color.FromArgb(128, 0, 128);

			// operators are black
			Styles[10].ForeColor = Color.FromArgb(0, 0, 0);
			Styles[10].Bold = true;

			// identifiers are yellow for now
			//Styles[11].ForeColor = Color.FromArgb(255, 255, 0);

			// secondary keywords are also purple, per Dr. Shomper's request
			Styles[16].ForeColor = Color.FromArgb(128, 0, 128);

			Indicators[0].Style = IndicatorStyle.Squiggle;
			Indicators[0].Color = Color.Red;
		}

		public void SetPlainTextEditorStyles()
		{
			this.ConfigurationManager.Language = "";

			Styles.ClearAll();

			IsBraceMatching = false;

			Margins.Margin0.Type = MarginType.Number;
			Margins.Margin0.Width = 40;

			Indentation.IndentWidth = 4;

			Styles.Default.FontName = "Courier New";
			Styles.Default.Size = 10.0f;
		}

		public void ResetEditor()
		{
			Text = string.Empty;
			Filename = string.Empty;
			FileLocation = FileLocation.Unknown;
			this.IsReadOnly = false;
			this.UndoRedo.EmptyUndoBuffer();
			Modified = false;
		}

		public void LoadFileText(FileInformation fileInfo, string text)
		{
			ResetEditor();
			this.Text = text;
			m_fileInfo.Filename.Assign(fileInfo.Filename);
			this.FileLocation = fileInfo.Location;

			UpdateSyntaxHighlighting();


			string filename = Path.GetFileName(this.Filename);

			string indicator = (this.FileLocation == FileLocation.Local) ? "L" : "R";

			string tabText = string.Format("({0}) {1}", indicator, filename);
			m_parentTab.Title = tabText;

			// do stuff with EOL here?

			UndoRedo.EmptyUndoBuffer();
		}

		private void UpdateSyntaxHighlighting()
		{
			string ext = m_fileInfo.Filename.Extension;

			if(m_cppExtensions.Contains(ext))
			{
				SetCPPEditorStyles();
			}
			else
			{
				SetPlainTextEditorStyles();
			}
		}
		public bool HasBeenSaved()
		{
			string filename = m_fileInfo.Filename.FullName;
			bool realFilename = (filename.IndexOf("<untitled>") == -1);
			bool realLocation = (m_fileInfo.Location != FileLocation.Unknown);

			return realFilename && realLocation;
		}

		public void SetFileSaved(string filename, FileLocation location)
		{
			this.FileLocation = location;
			this.Filename = filename;
			this.Modified = false;

			UpdateSyntaxHighlighting();
		}

		protected override void OnCharAdded(CharAddedEventArgs e)
		{
			int pos = this.CurrentPos;

			if(Context.IsCloseBracket(e.Ch) && e.Ch == this.NativeInterface.GetCharAt(pos)) 
			{
				this.NativeInterface.CharRight();
				this.NativeInterface.DeleteBack();
			}

			char matchChar = char.MinValue;

			switch(e.Ch)
			{
				case '(':
				{
					if(m_context.IsCommentOrString(CurrentPos) == false)
					{
						// TODO implement this
						//CodeComplete();
					}
					matchChar = ')';
					break;
				}
				case '[':
				{
					matchChar = ']';
					break;
				}

				case '{':
				{
					m_context.AutoIndent(e.Ch);
					matchChar = '}';
					break;
				}

				case ':':
				{
					m_context.AutoIndent(e.Ch);
					goto case '.';
				}

				// fall through...
				case '.':
				case '>':
				{
					if(m_context.IsCommentOrString(CurrentPos) == false)
					{
						//CodeComplete();
					}
					break;
				}
				case '}':
				{
					m_context.AutoIndent(e.Ch);
					break;
				}
				case '\n':
				{
					// in case ENTER was hit immediately after we inserted '{' into the code
					if(m_lastCharAdded == '{'  && m_autoAddMatchedBrace)
					{
						matchChar = '}';
						InsertText(pos, matchChar.ToString());
						UndoRedo.BeginUndoAction();
						NativeInterface.CharRight();

						m_context.AutoIndent('}');

						InsertText(pos, Environment.NewLine);

						NativeInterface.CharRight();
						SetCaretAt(pos);
						
						base.OnCharAdded(e);

						UndoRedo.EndUndoAction();
					}
					else
					{
						m_context.AutoIndent(e.Ch);						
					}
					break;
				}
				default:
				{
					break;
				}
			}

			if(matchChar != char.MinValue  && m_autoAddMatchedBrace && !m_context.IsCommentOrString(pos))
			{
				if(matchChar == ')')
				{
					// avoid adding close brace if the next char is not a whitespace
					// character
					int nextChar = SafeGetChar(pos);
					switch(nextChar)
					{
						case ' ':
						case '\t':
						case '\n':
						case '\r':
						InsertText(pos, matchChar.ToString());
						//SetIndicatorCurrent(MATCH_INDICATOR);
						// use grey colour rather than black, otherwise this indicator is invisible when using the
						// black theme
						//NativeInterface.IndicatorFillRange(pos, 1);
						break;
					}
				}
				else if(matchChar != '}')
				{
					InsertText(pos, matchChar.ToString());
					//SetIndicatorCurrent(MATCH_INDICATOR);
					// use grey colour rather than black, otherwise this indicator is invisible when using the
					// black theme
					//NativeInterface.IndicatorFillRange(pos, 1);

				}
				
			}

			if(e.Ch != '\r')
			{
				m_lastCharAdded = e.Ch;
			}
		}

		#region Text utility functions
		private char SafeGetChar(int pos)
		{
			if(pos < 0 || pos >= TextLength)
			{
				return char.MinValue;
			}
			return NativeInterface.GetCharAt(pos);
		}

		public char PreviousChar(int pos, ref int foundPos, bool wantWhitespace)
		{
			char ch = char.MinValue;
			int curpos = NativeInterface.PositionBefore( pos );

			if (curpos == 0) 
			{
				foundPos = curpos;
				return ch;
			}

			while ( true ) 
			{
				ch = NativeInterface.GetCharAt( curpos );
				if (ch == '\t' || ch == ' ' || ch == '\r' || ch == '\v' || ch == '\n') 
				{
					//if the caller is interested in whitespace,
					//simply return it
					if (wantWhitespace) 
					{
						foundPos = curpos;
						return ch;
					}

					long tmpPos = curpos;
					curpos = NativeInterface.PositionBefore( curpos );
					if (curpos == 0 && tmpPos == curpos)
						break;
				}
				else 
				{
					foundPos = curpos;
					return ch;
				}
			}
			foundPos = -1;
			return ch;
		}


		public string PreviousWord(int pos, ref int foundPos)
		{
			// Get the partial word that we have
			char ch = char.MinValue;
			int curpos = NativeInterface.PositionBefore( pos );

			if (curpos == 0) 
			{
				foundPos = -1;
				return "";
			}

			while ( true ) 
			{
				ch = NativeInterface.GetCharAt(curpos);

				if (ch == '\t' || ch == ' ' || ch == '\r' || ch == '\v' || ch == '\n') 
				{
					int tmpPos = curpos;
					curpos = NativeInterface.PositionBefore(curpos);
					if (curpos == 0 && tmpPos == curpos)
						break;
				}
				else 
				{
					int start = NativeInterface.WordStartPosition(curpos, true);
					int end = NativeInterface.WordEndPosition(curpos, true);
					//return NativeInterface.GetTextRange(start, end);
					return new Range(start, end, this).Text;
				}
			}
			foundPos = -1;
			return "";
		}

		public char NextChar(int pos, ref int foundPos )
		{
			char ch = char.MinValue;
			int nextpos = pos;

			while ( true ) 
			{
				if ( nextpos >= TextLength )
					break;

				ch = NativeInterface.GetCharAt(nextpos);
				if (ch == '\t' || ch == ' ' || ch == '\r' || ch == '\v' || ch == '\n') 
				{
					nextpos = NativeInterface.PositionAfter( nextpos );
					continue;
				} 
				else 
				{
					foundPos = nextpos;
					return ch;
				}
			}

			foundPos = -1;
			return ch;
		}

		public bool MatchBraceBack(char chCloseBrace, int pos, ref int matchedPos)
		{
			if (pos <= 0)
				return false;

			char chOpenBrace;

			switch (chCloseBrace) 
			{
				case '}':
				{
					chOpenBrace = '{';
					break;
				}

				case ')':
				{
					chOpenBrace = '(';
					break;
				}
				case ']':
				{
					chOpenBrace = '[';
					break;
				}
				case '>':
				{
					chOpenBrace = '<';
					break;
				}
				default:
				{
					return false;
				}
			}

			int nPrevPos = pos;
			char ch;
			int depth = 1;

			// We go backward
			while (true) 
			{
				if (nPrevPos == 0)
					break;
				nPrevPos = NativeInterface.PositionBefore(nPrevPos);

				// Make sure we are not in a comment
				if (m_context.IsCommentOrString(nPrevPos))
					continue;

				ch = NativeInterface.GetCharAt(nPrevPos);
				if (ch == chOpenBrace) 
				{
					// Dec the depth level
					depth--;
					if (depth == 0) 
					{
						matchedPos = nPrevPos;
						return true;
					}
				} 
				else if (ch == chCloseBrace) 
				{
					// Inc depth level
					depth++;
				}
			}
			return false;
		}

		public bool MatchBraceForward(char chOpenBrace, int pos, ref int matchedPos)
		{
			if(pos <= 0)
				return false;

			char chCloseBrace;

			switch(chOpenBrace)
			{
				case '{':
				{
					chCloseBrace = '}';
					break;
				}

				case '(':
				{
					chCloseBrace = ')';
					break;
				}
				case '[':
				{
					chCloseBrace = ']';
					break;
				}
				case '<':
				{
					chCloseBrace = '>';
					break;
				}
				default:
				{
					return false;
				}
			}

			int nNextPos = pos;
			char ch;
			int depth = 1;

			while(true)
			{
				if(nNextPos >= this.TextLength)
					break;
				nNextPos = NativeInterface.PositionAfter(nNextPos);

				// Make sure we are not in a comment
				if(m_context.IsCommentOrString(nNextPos))
					continue;

				ch = NativeInterface.GetCharAt(nNextPos);
				if(ch == chCloseBrace)
				{
					// Dec the depth level
					depth--;
					if(depth == 0)
					{
						matchedPos = nNextPos;
						return true;
					}
				}
				else if(ch == chOpenBrace)
				{
					// Inc depth level
					depth++;
				}
			}
			return false;
		}

		public void SetCaretAt(int pos)
		{
			NativeInterface.SetCurrentPos(pos);
			NativeInterface.SetSelectionStart(pos);
			NativeInterface.SetSelectionEnd(pos);
		}


		public void ReformatSelectedText()
		{
			this.Selection.Text = m_astyle.FormatSource(this.Selection.Text);
		}

		public void ReformatBuffer()
		{
			this.Text = m_astyle.FormatSource(this.Text);
		}

		#endregion

		public string GetTextToPos(int pos)
		{
			return GetTextChunk(0, pos);
		}

		public string GetTextChunk(int startPos, int endPos)
		{
			return new Range(startPos, endPos, this).Text;
		}

		#region Code rule error functions
		public void AddError(CodeRuleError error)
		{
			m_ruleErrors.Add(error);

			error.range.SetIndicator(0);
			
		}

		public void ClearErrors()
		{
			foreach(CodeRuleError error in m_ruleErrors)
			{
				error.range.ClearIndicator(0);
			}

			m_ruleErrors.Clear();
		}

		protected override void OnDwellStart(ScintillaMouseEventArgs mea)
		{
			base.OnDwellStart(mea);

			int pos = mea.Position;

			List<CodeRuleError> errors = (from e in m_ruleErrors
										  where pos >= e.range.Start &&
											pos < e.range.End
										  select e).ToList();

			if(errors.Count > 0)
			{
				string message = "";
				bool first = true;

				foreach(CodeRuleError error in errors)
				{
					if(!first)
					{
						message += "\r\n";
					}

					first = false;
					message += error.text;
				}

				CallTip.Show(message, pos);
			}
			
		}

		protected override void OnDwellEnd(ScintillaMouseEventArgs e)
		{
			base.OnDwellEnd(e);

			if(CallTip.IsActive)
			{
				CallTip.Cancel();
			}
		}

		#endregion
	}
}
