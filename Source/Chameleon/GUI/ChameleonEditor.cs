using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ScintillaNet;
using System.Drawing;
using System.IO;
using FarsiLibrary.Win;
using Chameleon.Features;
using Chameleon.Util;

namespace Chameleon.GUI
{
	public class ChameleonEditor : Scintilla
	{
		private FileInformation m_fileInfo;
		private FATabStripItem m_parentTab;

		private static Dictionary<FileLocation, string> m_titlePrefixes;

		private char m_lastCharAdded;

		private CppContext m_context;
		

		#region Properties
		public FATabStripItem ParentTab
		{
			get { return m_parentTab; }
			set { m_parentTab = value; }
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
		#endregion

		#region Constructors
		static ChameleonEditor()
		{
			m_titlePrefixes = new Dictionary<FileLocation, string>();
			m_titlePrefixes[FileLocation.Local] = "(L) ";
			m_titlePrefixes[FileLocation.Remote] = "(R) ";
			m_titlePrefixes[FileLocation.Unknown] = "(?) ";
		}

		public ChameleonEditor()
		{
			m_fileInfo = new FileInformation();
			Filename = "";
			FileLocation = FileLocation.Unknown;

			m_context = new CppContext(this);
			

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

		public void SetDefaultEditorStyles()
		{
			this.ConfigurationManager.Language = "cpp";

			Styles.ClearAll();

			IsBraceMatching = true;

			Margins.Margin0.Type = MarginType.Number;
			Margins.Margin0.Width = 40;


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

			// secondary keywords are bright green for now
			Styles[16].ForeColor = Color.FromArgb(0, 255, 0);
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

			string filename = Path.GetFileName(this.Filename);

			string indicator = (this.FileLocation == FileLocation.Local) ? "L" : "R";

			string tabText = string.Format("({0}) {1}", indicator, filename);
			m_parentTab.Title = tabText;

			// do stuff with EOL here?

			UndoRedo.EmptyUndoBuffer();
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
			UndoRedo.EmptyUndoBuffer();
			this.FileLocation = location;
			this.Filename = filename;
			this.Modified = false;
		}

		protected override void OnCharAdded(CharAddedEventArgs e)
		{
			int pos = this.CurrentPos;

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
					if(m_lastCharAdded == '{' )// && m_autoAddMatchedBrace)
					{
						matchChar = '}';
						InsertText(pos, matchChar.ToString());
						UndoRedo.BeginUndoAction();
						NativeInterface.CharRight();

						m_context.AutoIndent('}');

						InsertText(pos, Environment.NewLine);
						NativeInterface.CharRight();
						Selection.Start = pos;

						m_context.AutoIndent('\n');

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

			if(matchChar != char.MinValue ) // && m_autoAddMatchedBrace && !m_ccntext.IsCommentOrString(pos))
			{
				if(matchChar == ')')
				{
					// avoid adding close brace if the next char is not a whitespace
					// character
				}
			}
		}

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

		public void SetCaretAt(int pos)
		{
			NativeInterface.SetCurrentPos(pos);
			NativeInterface.SetSelectionStart(pos);
			NativeInterface.SetSelectionEnd(pos);
		}
	}
}
