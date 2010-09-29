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
namespace de.mud.terminal
{
	
	/// <summary> Implementation of a Video Display Unit (VDU) buffer. This class contains
	/// all methods to manipulate the buffer that stores characters and their
	/// attributes as well as the regions displayed.
	/// 
	/// </summary>
	/// <author>  Matthias L. Jugel, Marcus Mei�ner
	/// </author>
	/// <version>  $Id: VDUBuffer.java 503 2005-10-24 07:34:13Z marcus $
	/// </version>
	public class VDUBuffer
	{
		/// <summary> Get the current column of the cursor position.</summary>
		virtual public int CursorColumn
		{
			get
			{
				return cursorX;
			}
			
		}
		/// <summary> Get the current line of the cursor position.</summary>
		virtual public int CursorRow
		{
			get
			{
				return cursorY;
			}
			
		}
		
		/// <summary> Get the current window base.</summary>
		/// <seealso cref="setWindowBase">
		/// </seealso>
		/// <summary> Set the current window base. This allows to view the scrollback buffer.</summary>
		/// <param name="line">the line where the screen window starts
		/// </param>
		/// <seealso cref="setBufferSize">
		/// </seealso>
		/// <seealso cref="getBufferSize">
		/// </seealso>
		virtual public int WindowBase
		{
			get
			{
				return windowBase;
			}
			
			set
			{
				if (value > screenBase)
					value = screenBase;
				else 
				if (value < 0)
					value = 0;
				windowBase = value;
				update[0] = true;
				redraw();
			}
			
		}
		
		/// <summary> Get the top scroll margin.</summary>
		/// <summary> Set the top scroll margin for the screen. If the current bottom margin
		/// is smaller it will become the top margin and the line will become the
		/// bottom margin.
		/// </summary>
		/// <param name="l">line that is the margin
		/// </param>
		virtual public int TopMargin
		{
			get
			{
				return topMargin;
			}
			
			set
			{
				if (value > bottomMargin)
				{
					topMargin = bottomMargin;
					bottomMargin = value;
				}
				else
					topMargin = value;
				if (topMargin < 0)
					topMargin = 0;
				if (bottomMargin > height - 1)
					bottomMargin = height - 1;
			}
			
		}
		
		/// <summary> Get the bottom scroll margin.</summary>
		/// <summary> Set the bottom scroll margin for the screen. If the current top margin
		/// is bigger it will become the bottom margin and the line will become the
		/// top margin.
		/// </summary>
		/// <param name="l">line that is the margin
		/// </param>
		virtual public int BottomMargin
		{
			get
			{
				return bottomMargin;
			}
			
			set
			{
				if (value < topMargin)
				{
					bottomMargin = topMargin;
					topMargin = value;
				}
				else
					bottomMargin = value;
				if (topMargin < 0)
					topMargin = 0;
				if (bottomMargin > height - 1)
					bottomMargin = height - 1;
			}
			
		}
		
		/// <summary> Retrieve current scrollback buffer size.</summary>
		/// <seealso cref="setBufferSize">
		/// </seealso>
		/// <summary> Set scrollback buffer size.</summary>
		/// <param name="amount">new size of the buffer
		/// </param>
		virtual public int BufferSize
		{
			get
			{
				return bufSize;
			}
			
			set
			{
				if (value < height)
					value = height;
				if (value < maxBufSize)
				{
					char[][] tmpArray = new char[value][];
					for (int i = 0; i < value; i++)
					{
						tmpArray[i] = new char[width];
					}
					char[][] cbuf = tmpArray;
					int[][] tmpArray2 = new int[value][];
					for (int i2 = 0; i2 < value; i2++)
					{
						tmpArray2[i2] = new int[width];
					}
					int[][] abuf = tmpArray2;
					int copyStart = bufSize - value < 0?0:bufSize - value;
					int copyCount = bufSize - value < 0?bufSize:value;
					if (charArray != null)
						Array.Copy(charArray, copyStart, cbuf, 0, copyCount);
					if (charAttributes != null)
						Array.Copy(charAttributes, copyStart, abuf, 0, copyCount);
					charArray = cbuf;
					charAttributes = abuf;
					bufSize = copyCount;
					screenBase = bufSize - height;
					windowBase = screenBase;
				}
				maxBufSize = value;
				
				update[0] = true;
				redraw();
			}
			
		}
		/// <summary> Retrieve maximum buffer Size.</summary>
		/// <seealso cref="getBufferSize">
		/// </seealso>
		virtual public int MaxBufferSize
		{
			get
			{
				return maxBufSize;
			}
			
		}
		/// <summary> Get amount of rows on the screen.</summary>
		virtual public int Rows
		{
			get
			{
				return height;
			}
			
		}
		/// <summary> Get amount of columns on the screen.</summary>
		virtual public int Columns
		{
			get
			{
				return width;
			}
			
		}
		virtual public VDUDisplay Display
		{
			set
			{
				this.display = value;
				if(display.VDUBuffer != this)
				{
					display.VDUBuffer = this;
				}
			}
			
		}
		
		/// <summary>The current version id tag </summary>
		public const System.String ID = "$Id: VDUBuffer.java 503 2005-10-24 07:34:13Z marcus $";
		
		/// <summary>Enable debug messages. </summary>
		public static int debug = 0;
		
		public int height, width; /* rows and columns */
		public bool[] update; /* contains the lines that need update */
		public char[][] charArray; /* contains the characters */
		public int[][] charAttributes; /* contains character attrs */
		public int bufSize;
		public int maxBufSize; /* buffer sizes */
		public int screenBase; /* the actual screen start */
		public int windowBase; /* where the start displaying */
		public int scrollMarker; /* marks the last line inserted */
		
		private int topMargin; /* top scroll margin */
		private int bottomMargin; /* bottom scroll margin */
		
		// cursor variables
		protected internal bool showcursor = true;
		protected internal int cursorX, cursorY;
		
		/// <summary>Scroll up when inserting a line. </summary>
		public const bool SCROLL_UP = false;
		/// <summary>Scroll down when inserting a line. </summary>
		public const bool SCROLL_DOWN = true;
		
		/// <summary>Make character normal. </summary>
		public const int NORMAL = 0x00;
		/// <summary>Make character bold. </summary>
		public const int BOLD = 0x01;
		/// <summary>Underline character. </summary>
		public const int UNDERLINE = 0x02;
		/// <summary>Invert character. </summary>
		public const int INVERT = 0x04;
		/// <summary>Lower intensity character. </summary>
		public const int LOW = 0x08;
		/// <summary>Invisible character. </summary>
		public const int INVISIBLE = 0x10;
		
		/// <summary>how much to left shift the foreground color </summary>
		public const int COLOR_FG_SHIFT = 5;
		/// <summary>how much to left shift the background color </summary>
		public const int COLOR_BG_SHIFT = 9;
		/// <summary>color mask </summary>
		public const int COLOR = 0x1fe0;
		/// <summary>foreground color mask </summary>
		public const int COLOR_FG = 0x1e0;
		/// <summary>background color mask </summary>
		public const int COLOR_BG = 0x1e00;
		
		/// <summary> Create a new video display buffer with the passed width and height in
		/// characters.
		/// </summary>
		/// <param name="width">the length of the character lines
		/// </param>
		/// <param name="height">the amount of lines on the screen
		/// </param>
		public VDUBuffer(int width, int height)
		{
			// set the display screen size
			setScreenSize(width, height, false);
		}
		
		/// <summary> Create a standard video display buffer with 80 columns and 24 lines.</summary>
		public VDUBuffer():this(80, 24)
		{
		}
		
		/// <summary> Put a character on the screen with normal font and outline.
		/// The character previously on that position will be overwritten.
		/// You need to call redraw() to update the screen.
		/// </summary>
		/// <param name="c">x-coordinate (column)
		/// </param>
		/// <param name="l">y-coordinate (line)
		/// </param>
		/// <param name="ch">the character to show on the screen
		/// </param>
		/// <seealso cref="insertChar">
		/// </seealso>
		/// <seealso cref="deleteChar">
		/// </seealso>
		/// <seealso cref="redraw">
		/// </seealso>
		public virtual void  putChar(int c, int l, char ch)
		{
			putChar(c, l, ch, NORMAL);
		}
		
		/// <summary> Put a character on the screen with specific font and outline.
		/// The character previously on that position will be overwritten.
		/// You need to call redraw() to update the screen.
		/// </summary>
		/// <param name="c">x-coordinate (column)
		/// </param>
		/// <param name="l">y-coordinate (line)
		/// </param>
		/// <param name="ch">the character to show on the screen
		/// </param>
		/// <param name="attributes">the character attributes
		/// </param>
		/// <seealso cref="BOLD">
		/// </seealso>
		/// <seealso cref="UNDERLINE">
		/// </seealso>
		/// <seealso cref="INVERT">
		/// </seealso>
		/// <seealso cref="INVISIBLE">
		/// </seealso>
		/// <seealso cref="NORMAL">
		/// </seealso>
		/// <seealso cref="LOW">
		/// </seealso>
		/// <seealso cref="insertChar">
		/// </seealso>
		/// <seealso cref="deleteChar">
		/// </seealso>
		/// <seealso cref="redraw">
		/// </seealso>
		
		public virtual void  putChar(int c, int l, char ch, int attributes)
		{
			c = checkBounds(c, 0, width - 1);
			l = checkBounds(l, 0, height - 1);
			charArray[screenBase + l][c] = ch;
			charAttributes[screenBase + l][c] = attributes;
			markLine(l, 1);
		}
		
		/// <summary> Get the character at the specified position.</summary>
		/// <param name="c">x-coordinate (column)
		/// </param>
		/// <param name="l">y-coordinate (line)
		/// </param>
		/// <seealso cref="putChar">
		/// </seealso>
		public virtual char getChar(int c, int l)
		{
			c = checkBounds(c, 0, width - 1);
			l = checkBounds(l, 0, height - 1);
			return charArray[screenBase + l][c];
		}
		
		/// <summary> Get the attributes for the specified position.</summary>
		/// <param name="c">x-coordinate (column)
		/// </param>
		/// <param name="l">y-coordinate (line)
		/// </param>
		/// <seealso cref="putChar">
		/// </seealso>
		public virtual int getAttributes(int c, int l)
		{
			c = checkBounds(c, 0, width - 1);
			l = checkBounds(l, 0, height - 1);
			return charAttributes[screenBase + l][c];
		}
		
		/// <summary> Insert a character at a specific position on the screen.
		/// All character right to from this position will be moved one to the right.
		/// You need to call redraw() to update the screen.
		/// </summary>
		/// <param name="c">x-coordinate (column)
		/// </param>
		/// <param name="l">y-coordinate (line)
		/// </param>
		/// <param name="ch">the character to insert
		/// </param>
		/// <param name="attributes">the character attributes
		/// </param>
		/// <seealso cref="BOLD">
		/// </seealso>
		/// <seealso cref="UNDERLINE">
		/// </seealso>
		/// <seealso cref="INVERT">
		/// </seealso>
		/// <seealso cref="INVISIBLE">
		/// </seealso>
		/// <seealso cref="NORMAL">
		/// </seealso>
		/// <seealso cref="LOW">
		/// </seealso>
		/// <seealso cref="putChar">
		/// </seealso>
		/// <seealso cref="deleteChar">
		/// </seealso>
		/// <seealso cref="redraw">
		/// </seealso>
		public virtual void  insertChar(int c, int l, char ch, int attributes)
		{
			c = checkBounds(c, 0, width - 1);
			l = checkBounds(l, 0, height - 1);
			Array.Copy(charArray[screenBase + l], c, charArray[screenBase + l], c + 1, width - c - 1);
			Array.Copy(charAttributes[screenBase + l], c, charAttributes[screenBase + l], c + 1, width - c - 1);
			putChar(c, l, ch, attributes);
		}
		
		/// <summary> Delete a character at a given position on the screen.
		/// All characters right to the position will be moved one to the left.
		/// You need to call redraw() to update the screen.
		/// </summary>
		/// <param name="c">x-coordinate (column)
		/// </param>
		/// <param name="l">y-coordinate (line)
		/// </param>
		/// <seealso cref="putChar">
		/// </seealso>
		/// <seealso cref="insertChar">
		/// </seealso>
		/// <seealso cref="redraw">
		/// </seealso>
		public virtual void  deleteChar(int c, int l)
		{
			c = checkBounds(c, 0, width - 1);
			l = checkBounds(l, 0, height - 1);
			if (c < width - 1)
			{
				Array.Copy(charArray[screenBase + l], c + 1, charArray[screenBase + l], c, width - c - 1);
				Array.Copy(charAttributes[screenBase + l], c + 1, charAttributes[screenBase + l], c, width - c - 1);
			}
			putChar(width - 1, l, (char) 0);
		}
		
		/// <summary> Put a String at a specific position. Any characters previously on that
		/// position will be overwritten. You need to call redraw() for screen update.
		/// </summary>
		/// <param name="c">x-coordinate (column)
		/// </param>
		/// <param name="l">y-coordinate (line)
		/// </param>
		/// <param name="s">the string to be shown on the screen
		/// </param>
		/// <seealso cref="BOLD">
		/// </seealso>
		/// <seealso cref="UNDERLINE">
		/// </seealso>
		/// <seealso cref="INVERT">
		/// </seealso>
		/// <seealso cref="INVISIBLE">
		/// </seealso>
		/// <seealso cref="NORMAL">
		/// </seealso>
		/// <seealso cref="LOW">
		/// </seealso>
		/// <seealso cref="putChar">
		/// </seealso>
		/// <seealso cref="insertLine">
		/// </seealso>
		/// <seealso cref="deleteLine">
		/// </seealso>
		/// <seealso cref="redraw">
		/// </seealso>
		public virtual void  putString(int c, int l, System.String s)
		{
			putString(c, l, s, NORMAL);
		}
		
		/// <summary> Put a String at a specific position giving all characters the same
		/// attributes. Any characters previously on that position will be
		/// overwritten. You need to call redraw() to update the screen.
		/// </summary>
		/// <param name="c">x-coordinate (column)
		/// </param>
		/// <param name="l">y-coordinate (line)
		/// </param>
		/// <param name="s">the string to be shown on the screen
		/// </param>
		/// <param name="attributes">character attributes
		/// </param>
		/// <seealso cref="BOLD">
		/// </seealso>
		/// <seealso cref="UNDERLINE">
		/// </seealso>
		/// <seealso cref="INVERT">
		/// </seealso>
		/// <seealso cref="INVISIBLE">
		/// </seealso>
		/// <seealso cref="NORMAL">
		/// </seealso>
		/// <seealso cref="LOW">
		/// </seealso>
		/// <seealso cref="putChar">
		/// </seealso>
		/// <seealso cref="insertLine">
		/// </seealso>
		/// <seealso cref="deleteLine">
		/// </seealso>
		/// <seealso cref="redraw">
		/// </seealso>
		public virtual void  putString(int c, int l, System.String s, int attributes)
		{
			for (int i = 0; i < s.Length && c + i < width; i++)
				putChar(c + i, l, s[i], attributes);
		}
		
		/// <summary> Insert a blank line at a specific position.
		/// The current line and all previous lines are scrolled one line up. The
		/// top line is lost. You need to call redraw() to update the screen.
		/// </summary>
		/// <param name="l">the y-coordinate to insert the line
		/// </param>
		/// <seealso cref="deleteLine">
		/// </seealso>
		/// <seealso cref="redraw">
		/// </seealso>
		public virtual void  insertLine(int l)
		{
			insertLine(l, 1, SCROLL_UP);
		}
		
		/// <summary> Insert blank lines at a specific position.
		/// You need to call redraw() to update the screen
		/// </summary>
		/// <param name="l">the y-coordinate to insert the line
		/// </param>
		/// <param name="n">amount of lines to be inserted
		/// </param>
		/// <seealso cref="deleteLine">
		/// </seealso>
		/// <seealso cref="redraw">
		/// </seealso>
		public virtual void  insertLine(int l, int n)
		{
			insertLine(l, n, SCROLL_UP);
		}
		
		/// <summary> Insert a blank line at a specific position. Scroll text according to
		/// the argument.
		/// You need to call redraw() to update the screen
		/// </summary>
		/// <param name="l">the y-coordinate to insert the line
		/// </param>
		/// <param name="scrollDown">scroll down
		/// </param>
		/// <seealso cref="deleteLine">
		/// </seealso>
		/// <seealso cref="SCROLL_UP">
		/// </seealso>
		/// <seealso cref="SCROLL_DOWN">
		/// </seealso>
		/// <seealso cref="redraw">
		/// </seealso>
		public virtual void  insertLine(int l, bool scrollDown)
		{
			insertLine(l, 1, scrollDown);
		}
		
		/// <summary> Insert blank lines at a specific position.
		/// The current line and all previous lines are scrolled one line up. The
		/// top line is lost. You need to call redraw() to update the screen.
		/// </summary>
		/// <param name="l">the y-coordinate to insert the line
		/// </param>
		/// <param name="n">number of lines to be inserted
		/// </param>
		/// <param name="scrollDown">scroll down
		/// </param>
		/// <seealso cref="deleteLine">
		/// </seealso>
		/// <seealso cref="SCROLL_UP">
		/// </seealso>
		/// <seealso cref="SCROLL_DOWN">
		/// </seealso>
		/// <seealso cref="redraw">
		/// </seealso>
		public virtual void  insertLine(int l, int n, bool scrollDown)
		{
			lock (this)
			{
				l = checkBounds(l, 0, height - 1);
				
				char[][] cbuf = null;
				int[][] abuf = null;
				int offset = 0;
				int oldBase = screenBase;
				
				if (l > bottomMargin)
				/* We do not scroll below bottom margin (below the scrolling region). */
					return ;
				int top = (l < topMargin?0:(l > bottomMargin?(bottomMargin + 1 < height?bottomMargin + 1:height - 1):topMargin));
				int bottom = (l > bottomMargin?height - 1:(l < topMargin?(topMargin > 0?topMargin - 1:0):bottomMargin));
				
				// System.out.println("l is "+l+", top is "+top+", bottom is "+bottom+", bottomargin is "+bottomMargin+", topMargin is "+topMargin);
				
				if (scrollDown)
				{
					if (n > (bottom - top))
						n = (bottom - top);
					char[][] tmpArray = new char[bottom - l - (n - 1)][];
					for (int i = 0; i < bottom - l - (n - 1); i++)
					{
						tmpArray[i] = new char[width];
					}
					cbuf = tmpArray;
					int[][] tmpArray2 = new int[bottom - l - (n - 1)][];
					for (int i2 = 0; i2 < bottom - l - (n - 1); i2++)
					{
						tmpArray2[i2] = new int[width];
					}
					abuf = tmpArray2;
					
					Array.Copy(charArray, oldBase + l, cbuf, 0, bottom - l - (n - 1));
					Array.Copy(charAttributes, oldBase + l, abuf, 0, bottom - l - (n - 1));
					Array.Copy(cbuf, 0, charArray, oldBase + l + n, bottom - l - (n - 1));
					Array.Copy(abuf, 0, charAttributes, oldBase + l + n, bottom - l - (n - 1));
					cbuf = charArray;
					abuf = charAttributes;
				}
				else
				{
					try
					{
						if (n > (bottom - top) + 1)
							n = (bottom - top) + 1;
						if (bufSize < maxBufSize)
						{
							if (bufSize + n > maxBufSize)
							{
								offset = n - (maxBufSize - bufSize);
								scrollMarker += offset;
								bufSize = maxBufSize;
								screenBase = maxBufSize - height - 1;
								windowBase = screenBase;
							}
							else
							{
								scrollMarker += n;
								screenBase += n;
								windowBase += n;
								bufSize += n;
							}
							
							char[][] tmpArray3 = new char[bufSize][];
							for (int i3 = 0; i3 < bufSize; i3++)
							{
								tmpArray3[i3] = new char[width];
							}
							cbuf = tmpArray3;
							int[][] tmpArray4 = new int[bufSize][];
							for (int i4 = 0; i4 < bufSize; i4++)
							{
								tmpArray4[i4] = new int[width];
							}
							abuf = tmpArray4;
						}
						else
						{
							offset = n;
							cbuf = charArray;
							abuf = charAttributes;
						}
						// copy anything from the top of the buffer (+offset) to the new top
						// up to the screenBase.
						if (oldBase > 0)
						{
							Array.Copy(charArray, offset, cbuf, 0, oldBase - offset);
							Array.Copy(charAttributes, offset, abuf, 0, oldBase - offset);
						}
						// copy anything from the top of the screen (screenBase) up to the
						// topMargin to the new screen
						if (top > 0)
						{
							Array.Copy(charArray, oldBase, cbuf, screenBase, top);
							Array.Copy(charAttributes, oldBase, abuf, screenBase, top);
						}
						// copy anything from the topMargin up to the amount of lines inserted
						// to the gap left over between scrollback buffer and screenBase
						if (oldBase > 0)
						{
							Array.Copy(charArray, oldBase + top, cbuf, oldBase - offset, n);
							Array.Copy(charAttributes, oldBase + top, abuf, oldBase - offset, n);
						}
						// copy anything from topMargin + n up to the line linserted to the
						// topMargin
						Array.Copy(charArray, oldBase + top + n, cbuf, screenBase + top, l - top - (n - 1));
						Array.Copy(charAttributes, oldBase + top + n, abuf, screenBase + top, l - top - (n - 1));
						//
						// copy the all lines next to the inserted to the new buffer
						if (l < height - 1)
						{
							Array.Copy(charArray, oldBase + l + 1, cbuf, screenBase + l + 1, (height - 1) - l);
							Array.Copy(charAttributes, oldBase + l + 1, abuf, screenBase + l + 1, (height - 1) - l);
						}
					}
					catch (System.IndexOutOfRangeException e)
					{
						// this should not happen anymore, but I will leave the code
						// here in case something happens anyway. That code above is
						// so complex I always have a hard time understanding what
						// I did, even though there are comments
						System.Console.Error.WriteLine("*** Error while scrolling up:");
						System.Console.Error.WriteLine("--- BEGIN STACK TRACE ---");
						System.Console.Error.WriteLine(e.StackTrace);
						System.Console.Error.WriteLine("--- END STACK TRACE ---");
						System.Console.Error.WriteLine("bufSize=" + bufSize + ", maxBufSize=" + maxBufSize);
						System.Console.Error.WriteLine("top=" + top + ", bottom=" + bottom);
						System.Console.Error.WriteLine("n=" + n + ", l=" + l);
						System.Console.Error.WriteLine("screenBase=" + screenBase + ", windowBase=" + windowBase);
						System.Console.Error.WriteLine("oldBase=" + oldBase);
						System.Console.Error.WriteLine("size.width=" + width + ", size.height=" + height);
						System.Console.Error.WriteLine("abuf.length=" + abuf.Length + ", cbuf.length=" + cbuf.Length);
						System.Console.Error.WriteLine("*** done dumping debug information");
					}
				}
				
				// this is a little helper to mark the scrolling
				scrollMarker -= n;
				
				
				for (int i = 0; i < n; i++)
				{
					cbuf[(screenBase + l) + (scrollDown?i:- i)] = new char[width];
					abuf[(screenBase + l) + (scrollDown?i:- i)] = new int[width];
				}
				
				charArray = cbuf;
				charAttributes = abuf;
				
				if (scrollDown)
					markLine(l, bottom - l + 1);
				else
					markLine(top, l - top + 1);
				
				display.updateScrollBar();
			}
		}
		
		/// <summary> Delete a line at a specific position. Subsequent lines will be scrolled
		/// up to fill the space and a blank line is inserted at the end of the
		/// screen.
		/// </summary>
		/// <param name="l">the y-coordinate to insert the line
		/// </param>
		/// <seealso cref="deleteLine">
		/// </seealso>
		public virtual void  deleteLine(int l)
		{
			l = checkBounds(l, 0, height - 1);
			
			int bottom = (l > bottomMargin?height - 1:(l < topMargin?topMargin:bottomMargin + 1));
			Array.Copy(charArray, screenBase + l + 1, charArray, screenBase + l, bottom - l - 1);
			Array.Copy(charAttributes, screenBase + l + 1, charAttributes, screenBase + l, bottom - l - 1);
			charArray[screenBase + bottom - 1] = new char[width];
			charAttributes[screenBase + bottom - 1] = new int[width];
			markLine(l, bottom - l);
		}
		
		/// <summary> Delete a rectangular portion of the screen.
		/// You need to call redraw() to update the screen.
		/// </summary>
		/// <param name="c">x-coordinate (column)
		/// </param>
		/// <param name="l">y-coordinate (row)
		/// </param>
		/// <param name="w">with of the area in characters
		/// </param>
		/// <param name="h">height of the area in characters
		/// </param>
		/// <param name="curAttr">attribute to fill
		/// </param>
		/// <seealso cref="deleteChar">
		/// </seealso>
		/// <seealso cref="deleteLine">
		/// </seealso>
		/// <seealso cref="redraw">
		/// </seealso>
		public virtual void  deleteArea(int c, int l, int w, int h, int curAttr)
		{
			c = checkBounds(c, 0, width - 1);
			l = checkBounds(l, 0, height - 1);
			
			char[] cbuf = new char[w];
			int[] abuf = new int[w];
			
			for (int i = 0; i < w; i++)
				abuf[i] = curAttr;
			for (int i = 0; i < h && l + i < height; i++)
			{
				Array.Copy(cbuf, 0, charArray[screenBase + l + i], c, w);
				Array.Copy(abuf, 0, charAttributes[screenBase + l + i], c, w);
			}
			markLine(l, h);
		}
		
		/// <summary> Delete a rectangular portion of the screen.
		/// You need to call redraw() to update the screen.
		/// </summary>
		/// <param name="c">x-coordinate (column)
		/// </param>
		/// <param name="l">y-coordinate (row)
		/// </param>
		/// <param name="w">with of the area in characters
		/// </param>
		/// <param name="h">height of the area in characters
		/// </param>
		/// <seealso cref="deleteChar">
		/// </seealso>
		/// <seealso cref="deleteLine">
		/// </seealso>
		/// <seealso cref="redraw">
		/// </seealso>
		public virtual void  deleteArea(int c, int l, int w, int h)
		{
			c = checkBounds(c, 0, width - 1);
			l = checkBounds(l, 0, height - 1);
			
			char[] cbuf = new char[w];
			int[] abuf = new int[w];
			
			for (int i = 0; i < h && l + i < height; i++)
			{
				Array.Copy(cbuf, 0, charArray[screenBase + l + i], c, w);
				Array.Copy(abuf, 0, charAttributes[screenBase + l + i], c, w);
			}
			markLine(l, h);
		}
		
		/// <summary> Sets whether the cursor is visible or not.</summary>
		/// <param name="doshow">
		/// </param>
		public virtual void  showCursor(bool doshow)
		{
			if (doshow != showcursor)
				markLine(cursorY, 1);
			showcursor = doshow;
		}
		
		/// <summary> Puts the cursor at the specified position.</summary>
		/// <param name="c">column
		/// </param>
		/// <param name="l">line
		/// </param>
		public virtual void  setCursorPosition(int c, int l)
		{
			cursorX = checkBounds(c, 0, width - 1);
			cursorY = checkBounds(l, 0, height - 1);
			markLine(cursorY, 1);
		}
		
		/// <summary> Change the size of the screen. This will include adjustment of the
		/// scrollback buffer.
		/// </summary>
		/// <param name="w">of the screen
		/// </param>
		/// <param name="h">of the screen
		/// </param>
		public virtual void  setScreenSize(int w, int h, bool broadcast)
		{
			char[][] cbuf;
			int[][] abuf;
			int bsize = bufSize;
			
			if (w < 1 || h < 1)
				return ;
			
			if (debug > 0)
				System.Console.Error.WriteLine("VDU: screen size [" + w + "," + h + "]");
			
			if (h > maxBufSize)
				maxBufSize = h;
			
			if (h > bufSize)
			{
				bufSize = h;
				screenBase = 0;
				windowBase = 0;
			}
			
			if (windowBase + h >= bufSize)
				windowBase = bufSize - h;
			
			if (screenBase + h >= bufSize)
				screenBase = bufSize - h;
			
			
			cbuf = new char[bufSize][];
			for (int i = 0; i < bufSize; i++)
			{
				cbuf[i] = new char[w];
			}
			abuf = new int[bufSize][];
			for (int i2 = 0; i2 < bufSize; i2++)
			{
				abuf[i2] = new int[w];
			}
			
			if (charArray != null && charAttributes != null)
			{
				for (int i = 0; i < bsize && i < bufSize; i++)
				{
					Array.Copy(charArray[i], 0, cbuf[i], 0, w < width?w:width);
					Array.Copy(charAttributes[i], 0, abuf[i], 0, w < width?w:width);
				}
			}
			
			charArray = cbuf;
			charAttributes = abuf;
			width = w;
			height = h;
			topMargin = 0;
			bottomMargin = h - 1;
			update = new bool[h + 1];
			update[0] = true;
			/*  FIXME: ???
			if(resizeStrategy == RESIZE_FONT)
			setBounds(getBounds());
			*/
		}
		
		/// <summary> Mark lines to be updated with redraw().</summary>
		/// <param name="l">starting line
		/// </param>
		/// <param name="n">amount of lines to be updated
		/// </param>
		/// <seealso cref="redraw">
		/// </seealso>
		public virtual void  markLine(int l, int n)
		{
			l = checkBounds(l, 0, height - 1);
			for (int i = 0; (i < n) && (l + i < height); i++)
				update[l + i + 1] = true;
		}
		
		private int checkBounds(int value_Renamed, int lower, int upper)
		{
			if (value_Renamed < lower)
				return lower;
			if (value_Renamed > upper)
				return upper;
			return value_Renamed;
		}
		
		/// <summary>a generic display that should redraw on demand </summary>
		protected internal VDUDisplay display;
		
		/// <summary> Trigger a redraw on the display.</summary>
		protected internal virtual void  redraw()
		{
			if (display != null)
				display.redraw();
		}
	}
}