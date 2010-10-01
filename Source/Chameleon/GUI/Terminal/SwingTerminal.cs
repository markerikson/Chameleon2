/*
* This file is part of "JTA - Telnet/SSH for the JAVA(tm) platform".
*
* (c) Matthias L. Jugel, Marcus Meißner 1996-2005. All Rights Reserved.
*
* Please visit http://javatelnet.org/ for updates and contact.
*
* --LICENSE NOTICE--
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
* --LICENSE NOTICE--
*
*/
using System;
using System.Drawing;
using System.Windows.Forms;
using Chameleon.Util;
namespace de.mud.terminal
{
	
	/// <summary> Video Display Unit emulation for Swing/AWT. This class implements all necessary
	/// features of a character display unit, but not the actual terminal emulation.
	/// It can be used as the base for terminal emulations of any kind.
	/// <P>
	/// This is a lightweight component. It will render very badly if used
	/// in standard AWT components without overloaded update() method. The
	/// update() method must call paint() immediately without clearing the
	/// components graphics context or parts of the screen will simply
	/// disappear.
	/// <P>
	/// <B>Maintainer:</B> Matthias L. Jugel
	/// 
	/// </summary>
	/// <version>  $Id: SwingTerminal.java 511 2005-11-18 19:36:06Z marcus $
	/// </version>
	/// <author>   Matthias L. Jugel, Marcus Mei�ner
	/// </author>
	[Serializable]
	public class SwingTerminal:System.Windows.Forms.Control, VDUDisplay		// when inheriting from Control, keys are not working on .NET
	{
		static private System.Int32 state59;
		private static int debug = 0;
		
		/// <summary>the VDU buffer </summary>
		protected VDUBuffer buffer;

		private System.Drawing.Font normalFont; /* normal font */
		private System.Drawing.Font boldFont;

		protected float charWidth; /* current width of a char */
		protected float charHeight; /* current height of a char */

		private System.Drawing.Point selectBegin_Renamed, selectEnd_Renamed; /* selection coordinates */
		private System.String selection; /* contains the selected text */
		
		private System.Windows.Forms.ScrollBar scrollBar;
		private bool m_updatingScrollbar;
		
		/// <summary>A list of colors used for representation of the display </summary>
		private Color[] color = new Color[]{ Color.Black,
							Color.Red, 
							Color.Green, 
							Color.Yellow, 
							Color.Blue, 
							Color.Magenta,
							Color.Cyan,
							Color.White,
							Color.Empty,	// bold color
							Color.Empty };	// inverted color
		
		public const int COLOR_BOLD = 8;
		public const int COLOR_INVERT = 9;		
		
		/* definitions of standards for the display unit */
		private const int COLOR_FG_STD = 7;
		private const int COLOR_BG_STD = 0;
		
		protected SolidBrush cursorBrushFG = new SolidBrush(Color.White);
		protected SolidBrush cursorBrushBG = new SolidBrush(Color.DarkGray);

		public int FontSize
		{
			get
			{
				return (int)this.Font.Size;
			}
			set
			{
				Font f = new Font("Courier New", value);
				this.Font = f;

				ResizeBuffer();
			}
		}


		/// <summary> Create a new video display unit with the passed width and height in
		/// characters using a special font and font size. These features can
		/// be set independently using the appropriate properties.
		/// </summary>
		/// <param name="buffer">a VDU buffer to be associated with the display
		/// </param>
		/// <param name="font">the font to be used (usually Monospaced)
		/// </param>
		public SwingTerminal(VDUBuffer buffer, System.Drawing.Font font) : base()
		{
			// set the normal font to use
			this.Font = font;

			VDUBuffer = buffer;

			ScrollBar s = new VScrollBar();
			s.Parent = this;
			s.Dock = DockStyle.Right;
			this.Scrollbar = s;

			this.MouseWheel += (sender, e) =>
			{
				int wb = buffer.WindowBase;
				
				if(e.Delta > 0)
				{
					wb -= 3;

					if(wb < 0)
					{
						wb = 0;
					}
				}
				else if(e.Delta < 0)
				{
					wb += 3;
				}

				buffer.WindowBase = wb;
				updateScrollBar();

				int q = 42;
			};

			clearSelection();
			selection = null;

			SetStyle(
				//this has been said to increase performance, it does not in fact, well it depends
				ControlStyles.AllPaintingInWmPaint |
				ControlStyles.DoubleBuffer |
				ControlStyles.UserPaint |
				ControlStyles.Opaque |
				//ControlStyles.SupportsTransparentBackColor |
				ControlStyles.Selectable |
				ControlStyles.StandardClick |
				ControlStyles.ResizeRedraw, true);
		}
		
		/// <summary> Create a display unit with size 80x24 and Font "Monospaced", size 12.</summary>
		public SwingTerminal(VDUBuffer buffer):this(buffer, new System.Drawing.Font(FontFamily.GenericMonospace, 10.0F))
		{
		}

		/// <summary> Create a display unit with size 80x24 and Font "Monospaced", size 12.</summary>
		public SwingTerminal():this(null, new System.Drawing.Font(FontFamily.GenericMonospace, 10.0F))
		{
		}		

		private static void  mouseDown(System.Object event_sender, System.Windows.Forms.MouseEventArgs e)
		{
			state59 = ((int) e.Button | (int) System.Windows.Forms.Control.ModifierKeys);
		}
		
		/// <summary> Return the currently associated VDUBuffer.</summary>
		/// <returns> the current buffer
		/// </returns>
		/// <summary> Set a new terminal (VDU) buffer.</summary>
		/// <param name="buffer">new buffer
		/// </param>
		virtual public VDUBuffer VDUBuffer
		{
			get
			{
				return buffer;
			}
			
			set
			{
				this.buffer = value;
				if(buffer != null)
				{
					buffer.Display = this;
				}
			}
			
		}
		
		/// <summary> Get current color set.</summary>
		/// <returns> the color set currently associated
		/// </returns>
		/// <summary> Set new color set for the display.</summary>
		/// <param name="colorset">new color set
		/// </param>
		virtual public System.Drawing.Color[] ColorSet
		{
			get
			{
				return color;
			}
			
			set
			{
				Array.Copy(value, 0, color, 0, 10);
				buffer.update[0] = true;
				redraw();
				
			}
			
		}
		
		/// <summary> Set the font to be used for rendering the characters on screen.</summary>
		/// <param name="font">the new font to be used.
		/// </param>
		public override System.Drawing.Font Font
		{
			get
			{
				return base.Font;
			}
			
			set
			{
				base.Font = normalFont = value;
				boldFont = new Font(normalFont, FontStyle.Bold);
				if(buffer != null && buffer.update != null)
					buffer.update[0] = true;
				ComputeFontSize();
				redraw();
			}
			
		}
		/// <summary> Connect a scrollbar to the VDU. This should be done differently
		/// using a property change listener.
		/// </summary>
		/// <param name="scrollBar">the scroll bar
		/// </param>
		virtual public System.Windows.Forms.ScrollBar Scrollbar
		{
			set
			{
				if (value == null)
					return ;
				this.scrollBar = value;

				if(buffer != null)
				{
					this.scrollBar.ValueChanged += (sender, e) =>
					{
						if(!m_updatingScrollbar) this.buffer.WindowBase = this.scrollBar.Value;
					};

					updateScrollBar();
				}
				
			}
			get
			{
				return this.scrollBar;
			}
		}
		virtual public System.String Selection
		{
			get
			{
				return selection;
			}
			
		}		
		private System.Drawing.Point selectBegin
		{
			get
			{
				return selectBegin_Renamed;
			}
			
			set
			{
				selectBegin_Renamed = value;
			}
			
		}
		private System.Drawing.Point selectEnd
		{
			get
			{
				return selectEnd_Renamed;
			}
			
			set
			{
				selectEnd_Renamed = value;
			}
			
		}		
		/// <summary> Create a color representation that is brighter than the standard
		/// color but not what we would like to use for bold characters.
		/// </summary>
		/// <param name="clr">the standard color
		/// </param>
		/// <returns> the new brighter color
		/// </returns>
		private System.Drawing.Color brighten(ref System.Drawing.Color clr)
		{
			int r, g, b;
			
			r = (int) min((int) clr.R * 1.2, 255.0);
			g = (int) min((int) clr.G * 1.2, 255.0);
			b = (int) min((int) clr.B * 1.2, 255.0);
			return System.Drawing.Color.FromArgb(r, g, b);
		}
		
		/// <summary> Create a color representation that is darker than the standard
		/// color but not what we would like to use for bold characters.
		/// </summary>
		/// <param name="clr">the standard color
		/// </param>
		/// <returns> the new darker color
		/// </returns>
		private System.Drawing.Color darken(ref System.Drawing.Color clr)
		{
			int r, g, b;
			
			r = (int) max((int) clr.R * 0.8, 0.0);
			g = (int) max((int) clr.G * 0.8, 0.0);
			b = (int) max((int) clr.B * 0.8, 0.0);
			return System.Drawing.Color.FromArgb(r, g, b);
		}
		/// <summary>User defineable cursor colors </summary>
		private System.Drawing.Color cursorColorFG
		{
			get
			{
				return cursorBrushFG.Color;
			}
			
			set
			{
				cursorBrushFG.Color = value;
			}
			
		}
		private System.Drawing.Color cursorColorBG
		{
			get
			{
				return cursorBrushBG.Color;
			}
			
			set
			{
				cursorBrushBG.Color = value;
			}			
		}
		
		protected internal virtual double max(double f1, double f2)
		{
			return (f1 < f2)?f2:f1;
		}
		
		protected internal virtual double min(double f1, double f2)
		{
			return (f1 < f2)?f1:f2;
		}
			
		/// <summary> Redraw marked lines.</summary>
		public virtual void  redraw()
		{
			Invalidate();
		}
		
		public virtual void  updateScrollBar()
		{
			updateScrollBar(0, 0);
		}

		public void updateScrollBar(int oldBottomMargin, int oldTopMargin)
		{
			if (scrollBar == null)
				return ;
			Action updateValues = () =>
			{
				if(buffer.BufferSize > buffer.Rows)
				{
					m_updatingScrollbar = true;
					scrollBar.Enabled = true;
					scrollBar.Minimum = 0;
					int max = buffer.BufferSize - buffer.Rows - 3;
					if(max < 0)
					{
						max = buffer.BufferSize - buffer.Rows;
					}
					scrollBar.Maximum = max;
					int val = buffer.WindowBase;

					if(val > max)
					{
						val = max;
					}
					scrollBar.Value = val;
					scrollBar.LargeChange = 10;
					m_updatingScrollbar = false;
				}
				else
				{
					scrollBar.Enabled = false;
				}				
			};

			if(this.InvokeRequired)
			{
				this.Invoke(updateValues);
			}
			else
			{
				updateValues();
			}			
		}

		protected override void  OnPaint(System.Windows.Forms.PaintEventArgs g_EventArg)
		{
			Graphics g = g_EventArg.Graphics;

			if(Utilities.IsDesignmode)
			{
				g.FillRectangle(Brushes.Black, this.ClientRectangle);
				return;
			}

			int selectStartLine = selectBegin.Y - buffer.windowBase;
			int selectEndLine = selectEnd.Y - buffer.windowBase;

			System.Drawing.Color fg = ForeColor;
			System.Drawing.Color bg = BackColor;

			SolidBrush fgBrush = new SolidBrush(fg);
			SolidBrush bgBrush = new SolidBrush(BackColor);
			
			Font fnt = normalFont;
			
			for (int l = 0; l < buffer.height; l++)
			{
				for (int c = 0; c < buffer.width; c++)
				{
					int addr = 0;
					int currAttr = buffer.charAttributes[buffer.windowBase + l][c];
					
					System.Drawing.Color tempAux = ForeColor;
					fg = darken(ref tempAux);
					System.Drawing.Color tempAux2 = BackColor;
					bg = darken(ref tempAux2);
					
					if ((currAttr & de.mud.terminal.VDUBuffer.COLOR_FG) != 0)
					{
						fg = darken(ref color[((currAttr & de.mud.terminal.VDUBuffer.COLOR_FG) >> de.mud.terminal.VDUBuffer.COLOR_FG_SHIFT) - 1]);
					}
					if ((currAttr & de.mud.terminal.VDUBuffer.COLOR_BG) != 0)
					{
						System.Drawing.Color tempAux3 = darken(ref color[((currAttr & de.mud.terminal.VDUBuffer.COLOR_BG) >> de.mud.terminal.VDUBuffer.COLOR_BG_SHIFT) - 1]);
						bg = darken(ref tempAux3);
					}
					
					if ((currAttr & VDUBuffer.BOLD) != 0)
					{
						fnt = boldFont;
						if (System.Drawing.Color.Empty != color[COLOR_BOLD])
						{
							fg = color[COLOR_BOLD];
						}
					}
					else
					{
						fnt = normalFont;
					}
					
					if ((currAttr & VDUBuffer.LOW) != 0)
					{
						fg = darken(ref fg);
					}
					if ((currAttr & VDUBuffer.INVERT) != 0)
					{
						if (color[COLOR_INVERT].IsEmpty)
						{
							System.Drawing.Color swapc = bg;
							bg = fg;
							fg = swapc;
						}
						else
						{
							if (color[COLOR_BOLD].IsEmpty)
							{
								fg = bg;
							}
							else
							{
								fg = color[COLOR_BOLD];
							}
							bg = color[COLOR_INVERT];
						}
					}
										
					// determine the maximum of characters we can print in one go
					while ((c + addr < buffer.width) && ((buffer.charArray[buffer.windowBase + l][c + addr] < ' ') || (buffer.charAttributes[buffer.windowBase + l][c + addr] == currAttr)))
					{
						if (buffer.charArray[buffer.windowBase + l][c + addr] < ' ')
						{
							buffer.charArray[buffer.windowBase + l][c + addr] = ' ';
							buffer.charAttributes[buffer.windowBase + l][c + addr] = 0;
							continue;
						}
						addr++;
					}
					
					
					// clear the part of the screen we want to change (fill rectangle)
					bgBrush.Color = bg;
					
					// Hack to make selection work
					// The fill rectangle was here without if - it messes up drawing of chars
					if(bg.R != BackColor.R || bg.G != BackColor.G || bg.B != BackColor.B)
					{
						g.FillRectangle(bgBrush, c * charWidth, l * charHeight, addr * charWidth, charHeight);
					}
					
					fgBrush.Color = fg;
					
					// draw the characters, if not invisible.
					if ((currAttr & VDUBuffer.INVISIBLE) == 0)
					{
						string str = new System.String(buffer.charArray[buffer.windowBase + l], c, addr);
						//Console.WriteLine(str + " x=" + c + " y=" + l);
						g.DrawString(str, fnt, fgBrush, c * charWidth, l * charHeight);
						//for(int i = 0; i < str.Length; i++)
						//{
						//	g.DrawString(str[i] + "", fnt, fgBrush, (c + i) * charWidth, l * charHeight);
						//}
					}
					
					if ((currAttr & VDUBuffer.UNDERLINE) != 0)
						g.DrawLine(new Pen(fgBrush), c * charWidth, (l + 1) * charHeight , c * charWidth + addr * charWidth, (l + 1) * charHeight );
					
					c += addr - 1;
				}
				
				// selection code, highlites line or part of it when it was
				// selected previously
				if (l >= selectStartLine && l <= selectEndLine)
				{
					int selectStartColumn = (l == selectStartLine?selectBegin.X:0);
					int selectEndColumn = (l == selectEndLine?(l == selectStartLine?selectEnd.X - selectStartColumn:selectEnd.X):buffer.width);
					if (selectStartColumn != selectEndColumn)
					{
						if (debug > 0)
							System.Console.Error.WriteLine("select(" + selectStartColumn + "-" + selectEndColumn + ")");
						//g.setXORMode(bg);
						g.FillRectangle(fgBrush, selectStartColumn * charWidth, l * charHeight, selectEndColumn * charWidth, charHeight);
						//g.setPaintMode();
					}
				}
			}
			
			// draw cursor
			if (buffer.showcursor && (buffer.screenBase + buffer.cursorY >= buffer.windowBase && buffer.screenBase + buffer.cursorY < buffer.windowBase + buffer.height))
			{
				DrawCursor(
					g,
					buffer.cursorX * charWidth,
					(buffer.cursorY + buffer.screenBase - buffer.windowBase) * charHeight,
					charWidth,
					charHeight,
					buffer.charArray[buffer.screenBase + buffer.cursorY][buffer.cursorX]
					);
			}
			
			buffer.update[0] = false;
		}
		// Draw cursor at given position, c is char under cursor
		public virtual void DrawCursor(Graphics g, float x, float y, float width, float height, char c)
		{
			if(Focused)
			{
				g.FillRectangle(cursorBrushBG, x + 3, y, width, height);
			}
			else
			{
				g.DrawRectangle(new Pen(cursorBrushBG), x + 3, y, width, height);
			}
			g.DrawString(Convert.ToString(c), Font, cursorBrushFG, x, y);
		}
		/// <summary> Convert Mouse Event coordinates into character cell coordinates</summary>
		/// <param name="evtpt">the mouse point to be converted
		/// </param>
		/// <returns> Character cell coordinate of passed point
		/// </returns>
		public virtual System.Drawing.Point mouseGetPos(ref System.Drawing.Point evtpt)
		{
			System.Drawing.Point mousepos;
			
			mousepos = new System.Drawing.Point(0, 0);
						
			mousepos.X = (int) (evtpt.X / charWidth);
			if (mousepos.X < 0)
				mousepos.X = 0;
			if (mousepos.X >= buffer.width)
				mousepos.X = buffer.width - 1;
			
			mousepos.Y = (int) (evtpt.Y / charHeight);
			if (mousepos.Y < 0)
				mousepos.Y = 0;
			if (mousepos.Y >= buffer.height)
				mousepos.Y = buffer.height - 1;
			
			return mousepos;
		}
		
		/// <summary> Set cursor FG and BG colors</summary>
		/// <param name="fg">foreground color or null
		/// </param>
		/// <param name="bg">background color or null
		/// </param>
		public virtual void  setCursorColors(ref System.Drawing.Color fg, ref System.Drawing.Color bg)
		{
			if (fg.IsEmpty)
				cursorColorFG = color[COLOR_FG_STD];
			else
				cursorColorFG = fg;
			if (bg.IsEmpty)
				cursorColorBG = color[COLOR_BG_STD];
			else
				cursorColorBG = bg;
			//UPGRADE_TODO: Method 'java.awt.Component.repaint' was converted to 'System.Windows.Forms.Control.Refresh' which has a different behavior. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1073_javaawtComponentrepaint'"
			Refresh();
		}

		protected void ComputeFontSize()
		{
			int width = (buffer != null ? buffer.width : 512);
			using(Graphics g = CreateGraphics())
			{
				SizeF charSize = g.MeasureString(new String('x', width), Font);
				charWidth = charSize.Width / width;
				charHeight = charSize.Height;
			}
		}

		// Resize VDU buffer to so that text fits control size
		public virtual void ResizeBuffer()
		{
			int oldColumns = buffer.Columns;
			int oldRows = buffer.Rows;

			int newColumns = (int)(Width / charWidth);
			int newRows = (int) (Height / charHeight);

			int rowsDiff = newRows - oldRows;

			int oldTopMargin = buffer.TopMargin;
			int oldBottomMargin = buffer.BottomMargin;

			buffer.setScreenSize(newColumns, buffer.height = newRows, true);
			buffer.markLine(0, buffer.height);


			buffer.WindowBase -= rowsDiff;
			updateScrollBar(oldBottomMargin, oldTopMargin);
		}
		
		public virtual void  clearSelection()
		{
			selectBegin = new System.Drawing.Point(0, 0);
			selectEnd = new System.Drawing.Point(0, 0);
			selection = null;
		}
		
		private bool buttonCheck(int modifiers, int mask)
		{
			return (modifiers & mask) == mask;
		}
		
		public virtual void  mouseMoved(System.Object event_sender, System.Windows.Forms.MouseEventArgs evt)
		{
			/* nothing yet we do here */
		}
		
		public virtual void  mouseDragged(System.Object event_sender, System.Windows.Forms.MouseEventArgs evt)
		{
			if (buttonCheck(state59, (int) System.Windows.Forms.MouseButtons.Left) || state59 == 0)
			{
				int x = (int) (evt.X / charWidth);
				int y = (int) (evt.Y / charHeight + buffer.windowBase);
				int oldx = selectEnd.X, oldy = selectEnd.Y;
				
				if (oldx != x || oldy != y)
				{
					buffer.update[0] = true;
					if (debug > 0)
						System.Console.Error.WriteLine("select([" + selectBegin.X + "," + selectBegin.Y + "]," + "[" + selectEnd.X + "," + selectEnd.Y + "])");
					redraw();
				}
			}
		}
		
		public virtual void  mouseClicked(System.Object event_sender, System.EventArgs evt)
		{
			/* nothing yet we do here */
		}
		
		public virtual void  mouseEntered(System.Object event_sender, System.EventArgs evt)
		{
			/* nothing yet we do here */
		}
		
		public virtual void  mouseExited(System.Object event_sender, System.EventArgs evt)
		{
			/* nothing yet we do here */
		}
		
		/// <summary> Handle mouse pressed events for copy & paste.</summary>
		/// <param name="evt">the event that occured
		/// </param>
		/// <seealso cref="java.awt.event.MouseEvent">
		/// </seealso>
		public virtual void  mousePressed(System.Object event_sender, System.Windows.Forms.MouseEventArgs evt)
		{
			Focus();
			
			int xoffset = (int) ((Width - buffer.width * charWidth) / 2);
			int yoffset = (int) ((Height - buffer.height * charHeight) / 2);
			
			if (buffer is VDUInput)
			{
				((VDUInput) buffer).mousePressed(xoffset, yoffset, state59);
			}
			
		}
		
		/// <summary> Handle mouse released events for copy & paste.</summary>
		/// <param name="evt">the mouse event
		/// </param>
		public virtual void  mouseReleased(System.Object event_sender, System.Windows.Forms.MouseEventArgs evt)
		{
			int xoffset = (int) ((Width - buffer.width * charWidth) / 2);
			int yoffset = (int) ((Height - buffer.height * charHeight) / 2);
			
			if (buffer is VDUInput)
			{
				((VDUInput) buffer).mousePressed(xoffset, yoffset, state59);
			}
			
			if (buttonCheck(state59, (int) System.Windows.Forms.MouseButtons.Left))
			{
				mouseDragged(event_sender, evt);
				
				if (selectBegin.X == selectEnd.X && selectBegin.Y == selectEnd.Y)
				{
					buffer.update[0] = true;
					redraw();
					return ;
				}
				selection = "";

				// Initial buffer space for selectEnd - selectBegin + 1 lines
				// NOTE: Selection includes invisible text as spaces!
				// (also leaves invisible non-whitespace selection ending as spaces)
				System.Text.StringBuilder selectionBuf = new System.Text.StringBuilder(buffer.charArray[0].Length * (selectEnd.Y - selectBegin.Y + 1));
				
				for (int l = selectBegin.Y; l <= selectEnd.Y; l++)
				{
					int start, end;
					start = (l == selectBegin.Y?(start = selectBegin.X):0);
					end = (l == selectEnd.Y?(end = selectEnd.X):buffer.charArray[l].Length);
					
					bool newlineFound = false;
					char ch = ' ';
					for (int i = start; i < end; i++)
					{
						if ((buffer.charAttributes[l][i] & VDUBuffer.INVISIBLE) != 0)
							ch = ' ';
						else
							ch = buffer.charArray[l][i];
						if (ch == '\n')
							newlineFound = true;
						selectionBuf.Append(ch);
					}
					if (!newlineFound)
						selectionBuf.Append('\n');
					// Trim all spaces from end of line, like xterm does.
					selection += ("-" + (selectionBuf.ToString())).Trim().Substring(1);
					if (end == buffer.charArray[l].Length)
						selection += "\n";
				}
			}
		}
		
		public virtual void  keyTyped(System.Object event_sender, System.Windows.Forms.KeyPressEventArgs e)
		{
			if (buffer != null && buffer is VDUInput)
			{
				((VDUInput) buffer).keyTyped((int) e.KeyChar, e.KeyChar, 0);
			}
		}
		
		public virtual void  keyPressed(System.Object event_sender, System.Windows.Forms.KeyEventArgs e)
		{
			if (buffer != null && buffer is VDUInput)
			{
				((VDUInput) buffer).keyPressed(e.KeyValue, (char) e.KeyValue, getModifiers(event_sender, e));
			}
		}
		
		public virtual void  keyReleased(System.Object event_sender, System.Windows.Forms.KeyEventArgs e)
		{
			// ignore
		}
		
		private int getModifiers(System.Object event_sender, System.Windows.Forms.KeyEventArgs e)
		{
			return ((System.Windows.Forms.Control.ModifierKeys == System.Windows.Forms.Keys.Control)?de.mud.terminal.VDUInput_Fields.KEY_CONTROL:0) | ((System.Windows.Forms.Control.ModifierKeys == System.Windows.Forms.Keys.Shift)?de.mud.terminal.VDUInput_Fields.KEY_SHIFT:0) | ((System.Windows.Forms.Control.ModifierKeys == System.Windows.Forms.Keys.Alt)?de.mud.terminal.VDUInput_Fields.KEY_ALT:0) | (e.Handled?de.mud.terminal.VDUInput_Fields.KEY_ACTION:0);
		}
		
		// We want to consume all keys
		protected override bool IsInputKey(Keys keyData)
		{
			return true;
		}

		// We want to consume all characters
		protected override bool IsInputChar(char charCode)
		{
			return true;
		}

		private void ScrollToBottom()
		{
			scrollBar.Value = scrollBar.Maximum;
		}
	
		protected override void OnKeyDown(KeyEventArgs e)
		{
			keyPressed(this, e);
		}
			
		protected override void OnKeyUp(KeyEventArgs e)
		{

			keyReleased(this, e);
		}
			
		protected override void OnKeyPress(KeyPressEventArgs e)
		{
			ScrollToBottom();
			keyTyped(this, e);
		}

		protected override void OnGotFocus(EventArgs e)
		{
			base.OnGotFocus (e);
			Invalidate();			// draw filled cursor rectangle
		}

		protected override void OnLostFocus(EventArgs e)
		{
			base.OnLostFocus (e);
			Invalidate();			// draw empty cursor rectangle
		}

		protected override void OnMouseDown(MouseEventArgs e)
		{
			base.OnMouseDown (e);
			Focus();				// <-- hack on .NET, otherwise we dont get focus - anyone knows why???
		}

		protected override void OnMouseUp(MouseEventArgs e)
		{
			base.OnMouseUp (e);
		}


	}
}
