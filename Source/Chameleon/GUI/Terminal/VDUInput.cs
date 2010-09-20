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
	
	/// <summary> An interface for a terminal that accepts input from keyboard and mouse.
	/// 
	/// </summary>
	/// <author>  Matthias L. Jugel, Marcus Mei�ner
	/// </author>
	/// <version>  $Id: VDUInput.java 499 2005-09-29 08:24:54Z leo $
	/// </version>
	public struct VDUInput_Fields{
		public readonly static int KEY_CONTROL = 0x01;
		public readonly static int KEY_SHIFT = 0x02;
		public readonly static int KEY_ALT = 0x04;
		public readonly static int KEY_ACTION = 0x08;
	}
	public interface VDUInput
	{
		//UPGRADE_NOTE: Members of interface 'VDUInput' were extracted into structure 'VDUInput_Fields'. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1045'"
		/// <summary> Override the standard key codes used by the terminal emulation.</summary>
		/// <param name="codes">a properties object containing key code definitions
		/// </param>
		System.Collections.Specialized.NameValueCollection KeyCodes
		{
			set;
			
		}
		
		
		/// <summary> Direct access to writing data ...</summary>
		/// <param name="b">
		/// </param>
		void  write(sbyte[] b);
		
		/// <summary> Terminal is mouse-aware and requires (x,y) coordinates of
		/// on the terminal (character coordinates) and the button clicked.
		/// </summary>
		/// <param name="x">
		/// </param>
		/// <param name="y">
		/// </param>
		/// <param name="modifiers">
		/// </param>
		void  mousePressed(int x, int y, int modifiers);
		
		/// <summary> Terminal is mouse-aware and requires the coordinates and button
		/// of the release.
		/// </summary>
		/// <param name="x">
		/// </param>
		/// <param name="y">
		/// </param>
		/// <param name="modifiers">
		/// </param>
		void  mouseReleased(int x, int y, int modifiers);
		
		/// <summary> main keytyping event handler...</summary>
		/// <param name="keyCode">the key code
		/// </param>
		/// <param name="keyChar">the character represented by the key
		/// </param>
		/// <param name="modifiers">shift/alt/control modifiers
		/// </param>
		void  keyPressed(int keyCode, char keyChar, int modifiers);
		
		/// <summary> Handle key Typed events for the terminal, this will get
		/// all normal key types, but no shift/alt/control/numlock.
		/// </summary>
		/// <param name="keyCode">the key code
		/// </param>
		/// <param name="keyChar">the character represented by the key
		/// </param>
		/// <param name="modifiers">shift/alt/control modifiers
		/// </param>
		void  keyTyped(int keyCode, char keyChar, int modifiers);
	}
}