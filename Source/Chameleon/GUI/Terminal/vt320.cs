/*
* This file is part of "JTA - Telnet/SSH for the JAVA(tm) platform".
*
* (c) Matthias L. Jugel, Marcus MeiĂźner 1996-2005. All Rights Reserved.
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
	
	/// <summary> Implementation of a VT terminal emulation plus ANSI compatible.
	/// <P>
	/// <B>Maintainer:</B> Marcus Mei\u00dfner
	/// 
	/// </summary>
	/// <version>  $Id: vt320.java 507 2005-10-25 10:14:52Z marcus $
	/// </version>
	/// <author>   Matthias L. Jugel, Marcus Mei\u00dfner
	/// </author>
	public abstract class vt320:VDUBuffer, VDUInput
	{
		/// <summary> Enable or disable the local echo property of the terminal.</summary>
		/// <param name="echo">true if the terminal should echo locally
		/// </param>
		virtual public bool LocalEcho
		{
			set
			{
				localecho = value;
			}
			
		}
		/// <summary> Enable the VMS mode of the terminal to handle some things differently
		/// for VMS hosts.
		/// </summary>
		/// <param name="vms">true for vms mode, false for normal mode
		/// </param>
		virtual public bool VMS
		{
			set
			{
				this.vms = value;
			}
			
		}
		/// <summary> Enable the usage of the IBM character set used by some BBS's. Special
		/// graphical character are available in this mode.
		/// </summary>
		/// <param name="ibm">true to use the ibm character set
		/// </param>
		virtual public bool IBMCharset
		{
			set
			{
				useibmcharset = value;
			}
			
		}
		/// <summary> Override the standard key codes used by the terminal emulation.</summary>
		/// <param name="codes">a properties object containing key code definitions
		/// </param>
		//UPGRADE_ISSUE: Class hierarchy differences between 'java.util.Properties' and 'System.Collections.Specialized.NameValueCollection' may cause compilation errors. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1186'"
		virtual public System.Collections.Specialized.NameValueCollection KeyCodes
		{
			set
			{
				System.String res;
				System.String[] prefixes = new System.String[]{"", "S", "C", "A"};
				int i;
				
				for (i = 0; i < 10; i++)
				{
					res = value.Get("NUMPAD" + i);
					if (res != null)
						Numpad[i] = unEscape(res);
				}
				for (i = 1; i < 20; i++)
				{
					res = value.Get("F" + i);
					if (res != null)
						FunctionKey[i] = unEscape(res);
					res = value.Get("SF" + i);
					if (res != null)
						FunctionKeyShift[i] = unEscape(res);
					res = value.Get("CF" + i);
					if (res != null)
						FunctionKeyCtrl[i] = unEscape(res);
					res = value.Get("AF" + i);
					if (res != null)
						FunctionKeyAlt[i] = unEscape(res);
				}
				for (i = 0; i < 4; i++)
				{
					res = value.Get(prefixes[i] + "PGUP");
					if (res != null)
						PrevScn[i] = unEscape(res);
					res = value.Get(prefixes[i] + "PGDOWN");
					if (res != null)
						NextScn[i] = unEscape(res);
					res = value.Get(prefixes[i] + "END");
					if (res != null)
						KeyEnd[i] = unEscape(res);
					res = value.Get(prefixes[i] + "HOME");
					if (res != null)
						KeyHome[i] = unEscape(res);
					res = value.Get(prefixes[i] + "INSERT");
					if (res != null)
						Insert[i] = unEscape(res);
					res = value.Get(prefixes[i] + "REMOVE");
					if (res != null)
						Remove[i] = unEscape(res);
					res = value.Get(prefixes[i] + "UP");
					if (res != null)
						KeyUp[i] = unEscape(res);
					res = value.Get(prefixes[i] + "DOWN");
					if (res != null)
						KeyDown[i] = unEscape(res);
					res = value.Get(prefixes[i] + "LEFT");
					if (res != null)
						KeyLeft[i] = unEscape(res);
					res = value.Get(prefixes[i] + "RIGHT");
					if (res != null)
						KeyRight[i] = unEscape(res);
					res = value.Get(prefixes[i] + "ESCAPE");
					if (res != null)
						Escape[i] = unEscape(res);
					res = value.Get(prefixes[i] + "BACKSPACE");
					if (res != null)
						BackSpace[i] = unEscape(res);
					res = value.Get(prefixes[i] + "TAB");
					if (res != null)
						TabKey[i] = unEscape(res);
					res = value.Get(prefixes[i] + "NUMPLUS");
					if (res != null)
						NUMPlus[i] = unEscape(res);
					res = value.Get(prefixes[i] + "NUMDECIMAL");
					if (res != null)
						NUMDot[i] = unEscape(res);
				}
			}
			
		}
		//UPGRADE_NOTE: Respective javadoc comments were merged.  It should be changed in order to comply with .NET documentation conventions. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1199'"
		/// <summary> Get the terminal id used to identify this terminal.</summary>
		/// <summary> Set the terminal id used to identify this terminal.</summary>
		/// <param name="terminalID">the id string
		/// </param>
		virtual public System.String TerminalID
		{
			get
			{
				return terminalID;
			}
			
			set
			{
				this.terminalID = value;
				
				if (value.Equals("scoansi"))
				{
					FunctionKey[1] = "\u001b[M"; FunctionKey[2] = "\u001b[N";
					FunctionKey[3] = "\u001b[O"; FunctionKey[4] = "\u001b[P";
					FunctionKey[5] = "\u001b[Q"; FunctionKey[6] = "\u001b[R";
					FunctionKey[7] = "\u001b[S"; FunctionKey[8] = "\u001b[T";
					FunctionKey[9] = "\u001b[U"; FunctionKey[10] = "\u001b[V";
					FunctionKey[11] = "\u001b[W"; FunctionKey[12] = "\u001b[X";
					FunctionKey[13] = "\u001b[Y"; FunctionKey[14] = "?";
					FunctionKey[15] = "\u001b[a"; FunctionKey[16] = "\u001b[b";
					FunctionKey[17] = "\u001b[c"; FunctionKey[18] = "\u001b[d";
					FunctionKey[19] = "\u001b[e"; FunctionKey[20] = "\u001b[f";
					PrevScn[0] = PrevScn[1] = PrevScn[2] = PrevScn[3] = "\u001b[I";
					NextScn[0] = NextScn[1] = NextScn[2] = NextScn[3] = "\u001b[G";
					// more theoretically.
				}
			}
			
		}
		virtual public System.String AnswerBack
		{
			set
			{
				this.answerBack = unEscape(value);
			}
			
		}
		/// <summary>The current version id tag.<P>
		/// $Id: vt320.java 507 2005-10-25 10:14:52Z marcus $
		/// </summary>
		new public const System.String ID = "$Id: vt320.java 507 2005-10-25 10:14:52Z marcus $";
		
		/// <summary>the debug level </summary>
		new private static int debug = 0;
		
		/// <summary> Write an answer back to the remote host. This is needed to be able to
		/// send terminal answers requests like status and type information.
		/// </summary>
		/// <param name="b">the array of bytes to be sent
		/// </param>
		public abstract void  write(sbyte[] b);
		
		/// <summary> Play the beep sound ...</summary>
		public virtual void  beep()
		{
			/* do nothing by default */
		}
		
		/// <summary> Put string at current cursor position. Moves cursor
		/// according to the String. Does NOT wrap.
		/// </summary>
		/// <param name="s">the string
		/// </param>
		public virtual void  putString(System.String s)
		{
			putString(s.ToCharArray(), 0, s.Length);
		}
		
		/// <summary> Put string at current cursor position. Moves cursor
		/// according to the String. Does NOT wrap.
		/// </summary>
		public virtual void  putString(char[] chars, int start, int count)
		{	
			if (count > 0)
			{
				markLine(R, 1);
				for (int i = start; i < start + count; i++)
				{
					// System.err.print(s.charAt(i)+"("+(int)s.charAt(i)+")");
					putChar(chars[i], false);
				}
				setCursorPosition(C, R);
				redraw();
			}
		}

		//UPGRADE_NOTE: Access modifiers of method 'sendTelnetCommand' were changed to 'public'. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1204'"
		public virtual void  sendTelnetCommand(sbyte cmd)
		{
		}
		
		/// <summary> Sent the changed window size from the terminal to all listeners.</summary>
		//UPGRADE_NOTE: Access modifiers of method 'setWindowSize' were changed to 'public'. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1204'"
		public virtual void  setWindowSize(int c, int r)
		{
			/* To be overridden by Terminal.java */
		}
		
		public override void  setScreenSize(int c, int r, bool broadcast)
		{
			int oldrows = Rows, oldcols = Columns;
			
			if (debug > 2)
				System.Console.Error.WriteLine("setscreensize (" + c + "," + r + "," + broadcast + ")");
			
			base.setScreenSize(c, r, false);
			
			/* Tricky, since the VDUBuffer works strangely. */
			if (r > oldrows)
			{
				setCursorPosition(C, R + (r - oldrows));
				redraw();
			}
			if (broadcast)
			{
				setWindowSize(c, r); /* broadcast up */
			}
		}
		
		
		/// <summary> Create a new vt320 terminal and intialize it with useful settings.</summary>
		public vt320(int width, int height):base(width, height)
		{
			VMS = false;
			IBMCharset = false;
			TerminalID = "vt320";
			BufferSize = 100;
			//setBorder(2, false);
			
			int nw = Columns;
			if (nw < 132)
				nw = 132; //catch possible later 132/80 resizes
			Tabs = new sbyte[nw];
			for (int i = 0; i < nw; i += 8)
			{
				Tabs[i] = 1;
			}
			
			/* top row of numpad */
			PF1 = "\u001bOP";
			PF2 = "\u001bOQ";
			PF3 = "\u001bOR";
			PF4 = "\u001bOS";
			
			/* the 3x2 keyblock on PC keyboards */
			Insert = new System.String[4];
			Remove = new System.String[4];
			KeyHome = new System.String[4];
			KeyEnd = new System.String[4];
			NextScn = new System.String[4];
			PrevScn = new System.String[4];
			Escape = new System.String[4];
			BackSpace = new System.String[4];
			TabKey = new System.String[4];
			Insert[0] = Insert[1] = Insert[2] = Insert[3] = "\u001b[2~";
			Remove[0] = Remove[1] = Remove[2] = Remove[3] = "\u001b[3~";
			PrevScn[0] = PrevScn[1] = PrevScn[2] = PrevScn[3] = "\u001b[5~";
			NextScn[0] = NextScn[1] = NextScn[2] = NextScn[3] = "\u001b[6~";
			KeyHome[0] = KeyHome[1] = KeyHome[2] = KeyHome[3] = "\u001b[1~"; // "\u001b[H"; <-- original values, they did not work RADEK
			KeyEnd[0] = KeyEnd[1] = KeyEnd[2] = KeyEnd[3] = "\u001b[4~"; // "\u001b[F";
			Escape[0] = Escape[1] = Escape[2] = Escape[3] = "\u001b";
			if (vms)
			{
				BackSpace[1] = "" + (char) 10; //  VMS shift deletes word back
				BackSpace[2] = "\u0018"; //  VMS control deletes line back
				BackSpace[0] = BackSpace[3] = "\u007f"; //  VMS other is delete
			}
			else
			{
				BackSpace[0] = BackSpace[1] = BackSpace[2] = BackSpace[3] = "\b";
			}
			
			/* some more VT100 keys */
			Find = "\u001b[1~";
			Select = "\u001b[4~";
			Help = "\u001b[28~";
			Do = "\u001b[29~";
			
			FunctionKey = new System.String[21];
			FunctionKey[0] = "";
			FunctionKey[1] = PF1;
			FunctionKey[2] = PF2;
			FunctionKey[3] = PF3;
			FunctionKey[4] = PF4;
			/* following are defined differently for vt220 / vt132 ... */
			FunctionKey[5] = "\u001b[15~";
			FunctionKey[6] = "\u001b[17~";
			FunctionKey[7] = "\u001b[18~";
			FunctionKey[8] = "\u001b[19~";
			FunctionKey[9] = "\u001b[20~";
			FunctionKey[10] = "\u001b[21~";
			FunctionKey[11] = "\u001b[23~";
			FunctionKey[12] = "\u001b[24~";
			FunctionKey[13] = "\u001b[25~";
			FunctionKey[14] = "\u001b[26~";
			FunctionKey[15] = Help;
			FunctionKey[16] = Do;
			FunctionKey[17] = "\u001b[31~";
			FunctionKey[18] = "\u001b[32~";
			FunctionKey[19] = "\u001b[33~";
			FunctionKey[20] = "\u001b[34~";
			
			FunctionKeyShift = new System.String[21];
			FunctionKeyAlt = new System.String[21];
			FunctionKeyCtrl = new System.String[21];
			
			for (int i = 0; i < 20; i++)
			{
				FunctionKeyShift[i] = "";
				FunctionKeyAlt[i] = "";
				FunctionKeyCtrl[i] = "";
			}
			FunctionKeyShift[15] = Find;
			FunctionKeyShift[16] = Select;
			
			
			TabKey[0] = "\u0009";
			TabKey[1] = "\u001bOP\u0009";
			TabKey[2] = TabKey[3] = "";
			
			KeyUp = new System.String[4];
			KeyUp[0] = "\u001b[A";
			KeyDown = new System.String[4];
			KeyDown[0] = "\u001b[B";
			KeyRight = new System.String[4];
			KeyRight[0] = "\u001b[C";
			KeyLeft = new System.String[4];
			KeyLeft[0] = "\u001b[D";
			Numpad = new System.String[10];
			Numpad[0] = "\u001bOp";
			Numpad[1] = "\u001bOq";
			Numpad[2] = "\u001bOr";
			Numpad[3] = "\u001bOs";
			Numpad[4] = "\u001bOt";
			Numpad[5] = "\u001bOu";
			Numpad[6] = "\u001bOv";
			Numpad[7] = "\u001bOw";
			Numpad[8] = "\u001bOx";
			Numpad[9] = "\u001bOy";
			KPMinus = PF4;
			KPComma = "\u001bOl";
			KPPeriod = "\u001bOn";
			KPEnter = "\u001bOM";
			
			NUMPlus = new System.String[4];
			NUMPlus[0] = "+";
			NUMDot = new System.String[4];
			NUMDot[0] = ".";
		}
		
		/// <summary> Create a default vt320 terminal with 80 columns and 24 lines.</summary>
		public vt320():this(80, 24)
		{
		}
		
		/// <summary> Terminal is mouse-aware and requires (x,y) coordinates of
		/// on the terminal (character coordinates) and the button clicked.
		/// </summary>
		/// <param name="x">
		/// </param>
		/// <param name="y">
		/// </param>
		/// <param name="modifiers">
		/// </param>
		public virtual void  mousePressed(int x, int y, int modifiers)
		{
			if (mouserpt == 0)
				return ;
			
			int mods = modifiers;
			mousebut = 3;
			if ((mods & 16) == 16)
				mousebut = 0;
			if ((mods & 8) == 8)
				mousebut = 1;
			if ((mods & 4) == 4)
				mousebut = 2;
			
			int mousecode;
			if (mouserpt == 9)
			/* X10 Mouse */
				mousecode = 0x20 | mousebut;
			/* normal xterm mouse reporting */
			else
				mousecode = mousebut | 0x20 | ((mods & 7) << 2);
			
			sbyte[] b = new sbyte[6];
			
			b[0] = 27;
			b[1] = (sbyte) '[';
			b[2] = (sbyte) 'M';
			b[3] = (sbyte) mousecode;
			b[4] = (sbyte) (0x20 + x + 1);
			b[5] = (sbyte) (0x20 + y + 1);
			
			write(b); // FIXME: writeSpecial here
		}
		
		/// <summary> Terminal is mouse-aware and requires the coordinates and button
		/// of the release.
		/// </summary>
		/// <param name="x">
		/// </param>
		/// <param name="y">
		/// </param>
		/// <param name="modifiers">
		/// </param>
		public virtual void  mouseReleased(int x, int y, int modifiers)
		{
			if (mouserpt == 0)
				return ;
			
			/* problem is tht modifiers still have the released button set in them.
			int mods = modifiers;
			mousebut = 3;
			if ((mods & 16)==16) mousebut=0;
			if ((mods &  8)==8 ) mousebut=1;
			if ((mods &  4)==4 ) mousebut=2;
			*/
			
			int mousecode;
			if (mouserpt == 9)
				mousecode = 0x20 + mousebut;
			/* same as press? appears so. */
			else
				mousecode = '#';
			
			sbyte[] b = new sbyte[6];
			b[0] = 27;
			b[1] = (sbyte) '[';
			b[2] = (sbyte) 'M';
			b[3] = (sbyte) mousecode;
			b[4] = (sbyte) (0x20 + x + 1);
			b[5] = (sbyte) (0x20 + y + 1);
			write(b); // FIXME: writeSpecial here
			mousebut = 0;
		}
		
		
		/// <summary>we should do localecho (passed from other modules). false is default </summary>
		private bool localecho = false;
		
		/// <summary> A small conveniance method thar converts the string to a byte array
		/// for sending.
		/// </summary>
		/// <param name="s">the string to be sent
		/// </param>
		public bool write(System.String s, bool doecho)
		{
			if (debug > 2)
				System.Console.Out.WriteLine("write(|" + s + "|," + doecho);
			if (s == null)
			// aka the empty string.
				return true;
			/* NOTE: getBytes() honours some locale, it *CONVERTS* the string.
			* However, we output only 7bit stuff towards the target, and *some*
			* 8 bit control codes. We must not mess up the latter, so we do hand
			* by hand copy.
			*/
			
			sbyte[] arr = new sbyte[s.Length];
			for (int i = 0; i < s.Length; i++)
			{
				arr[i] = (sbyte) s[i];
			}
			write(arr);
			
			if (doecho)
				putString(s);
			return true;
		}
		
		public bool write(System.String s)
		{
			return write(s, localecho);
		}
		
		// ===================================================================
		// the actual terminal emulation code comes here:
		// ===================================================================
		
		private System.String terminalID = "vt320";
		private System.String answerBack = "Use Terminal.answerback to set ...\n";
		
		// X - COLUMNS, Y - ROWS
		internal int R, C;
		internal int attributes = 0;
		
		internal int Sc, Sr, Sa, Stm, Sbm;
		internal char Sgr, Sgl;
		internal char[] Sgx;
		
		internal int insertmode = 0;
		internal int statusmode = 0;
		internal bool vt52mode = false;
		internal bool keypadmode = false; /* false - numeric, true - application */
		internal bool output8bit = false;
		internal int normalcursor = 0;
		internal bool moveoutsidemargins = true;
		internal bool wraparound = true;
		internal bool sendcrlf = true;
		internal bool capslock = false;
		internal bool numlock = false;
		internal int mouserpt = 0;
		internal sbyte mousebut = 0;
		
		internal bool useibmcharset = false;
		
		internal int lastwaslf = 0;
		internal bool usedcharsets = false;
		
		private const char ESC = (char) (27);
		private const char IND = (char) (132);
		private const char NEL = (char) (133);
		private const char RI = (char) (141);
		private const char SS2 = (char) (142);
		private const char SS3 = (char) (143);
		private const char DCS = (char) (144);
		private const char HTS = (char) (136);
		private const char CSI = (char) (155);
		private const char OSC = (char) (157);
		private const int TSTATE_DATA = 0;
		private const int TSTATE_ESC = 1; /* ESC */
		private const int TSTATE_CSI = 2; /* ESC [ */
		private const int TSTATE_DCS = 3; /* ESC P */
		private const int TSTATE_DCEQ = 4; /* ESC [? */
		private const int TSTATE_ESCSQUARE = 5; /* ESC # */
		private const int TSTATE_OSC = 6; /* ESC ] */
		private const int TSTATE_SETG0 = 7; /* ESC (? */
		private const int TSTATE_SETG1 = 8; /* ESC )? */
		private const int TSTATE_SETG2 = 9; /* ESC *? */
		private const int TSTATE_SETG3 = 10; /* ESC +? */
		private const int TSTATE_CSI_DOLLAR = 11; /* ESC [ Pn $ */
		private const int TSTATE_CSI_EX = 12; /* ESC [ ! */
		private const int TSTATE_ESCSPACE = 13; /* ESC <space> */
		private const int TSTATE_VT52X = 14;
		private const int TSTATE_VT52Y = 15;
		private const int TSTATE_CSI_TICKS = 16;
		private const int TSTATE_CSI_EQUAL = 17; /* ESC [ = */
		
		/* The graphics charsets
		* B - default ASCII
		* A - ISO Latin 1
		* 0 - DEC SPECIAL
		* < - User defined
		* ....
		*/
		internal char[] gx = new char[]{'B', '0', 'B', 'B'};
		internal char gl = (char) (0); // default GL to G0
		internal char gr = (char) (2); // default GR to G2
		internal int onegl = - 1; // single shift override for GL.
		
		// Map from scoansi linedrawing to DEC _and_ unicode (for the stuff which
		// is not in linedrawing). Got from experimenting with scoadmin.
		private const System.String scoansi_acs = "Tm7k3x4u?kZl@mYjEnB\u2566DqCtAvM\u2550:\u2551N\u2557I\u2554;\u2557H\u255a0a<\u255d";
		// array to store DEC Special -> Unicode mapping
		//  Unicode   DEC  Unicode name    (DEC name)
		private static char[] DECSPECIAL = new char[]{'\u0040', '\u2666', '\u2592', '\u2409', '\u240c', '\u240d', '\u240a', '\u00ba', '\u00b1', '\u2424', '\u240b', '\u2518', '\u2510', '\u250c', '\u2514', '\u253c', '\u2594', '\u2580', '\u2500', '\u25ac', '\u005f', '\u251c', '\u2524', '\u2534', '\u252c', '\u2502', '\u2264', '\u2265', '\u00b6', '\u2260', '\u00a3', '\u00b7'};
		
		/// <summary>Strings to send on function key pressing </summary>
		private System.String[] Numpad;
		private System.String[] FunctionKey;
		private System.String[] FunctionKeyShift;
		private System.String[] FunctionKeyCtrl;
		private System.String[] FunctionKeyAlt;
		private System.String[] TabKey;
		private System.String[] KeyUp, KeyDown, KeyLeft, KeyRight;
		private System.String KPMinus, KPComma, KPPeriod, KPEnter;
		private System.String PF1, PF2, PF3, PF4;
		private System.String Help, Do, Find, Select;
		
		private System.String[] KeyHome, KeyEnd, Insert, Remove, PrevScn, NextScn;
		private System.String[] Escape, BackSpace, NUMDot, NUMPlus;
		
		private System.String osc, dcs; /* to memorize OSC & DCS control sequence */
		
		/// <summary>vt320 state variable (internal) </summary>
		private int term_state = TSTATE_DATA;
		/// <summary>in vms mode, set by Terminal.VMS property </summary>
		private bool vms = false;
		/// <summary>Tabulators </summary>
		private sbyte[] Tabs;
		/// <summary>The list of integers as used by CSI </summary>
		private int[] DCEvars = new int[30];
		private int DCEvar;
		
		/// <summary> Replace escape code characters (backslash + identifier) with their
		/// respective codes.
		/// </summary>
		/// <param name="tmp">the string to be parsed
		/// </param>
		/// <returns> a unescaped string
		/// </returns>
		internal static System.String unEscape(System.String tmp)
		{
			int idx = 0, oldidx = 0;
			System.String cmd;
			// System.err.println("unescape("+tmp+")");
			cmd = "";
			//UPGRADE_WARNING: Method 'java.lang.String.indexOf' was converted to 'System.String.IndexOf' which may throw an exception. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1101'"
			while ((idx = tmp.IndexOf('\\', oldidx)) >= 0 && ++idx <= tmp.Length)
			{
				cmd += tmp.Substring(oldidx, (idx - 1) - (oldidx));
				if (idx == tmp.Length)
					return cmd;
				switch (tmp[idx])
				{
					
					case 'b': 
						cmd += "\b";
						break;
					
					case 'e': 
						cmd += "\u001b";
						break;
					
					case 'n': 
						cmd += "\n";
						break;
					
					case 'r': 
						cmd += "\r";
						break;
					
					case 't': 
						cmd += "\t";
						break;
					
					case 'v': 
						cmd += "\u000b";
						break;
					
					case 'a': 
						cmd += "\u0012";
						break;
					
					default: 
						if ((tmp[idx] >= '0') && (tmp[idx] <= '9'))
						{
							int i;
							for (i = idx; i < tmp.Length; i++)
								if ((tmp[i] < '0') || (tmp[i] > '9'))
									break;
							cmd += (char) System.Int32.Parse(tmp.Substring(idx, (i) - (idx)));
							idx = i - 1;
						}
						else
							cmd += tmp.Substring(idx, (++idx) - (idx));
						break;
					
				}
				oldidx = ++idx;
			}
			if (oldidx <= tmp.Length)
				cmd += tmp.Substring(oldidx);
			return cmd;
		}
		
		/// <summary> A small conveniance method thar converts a 7bit string to the 8bit
		/// version depending on VT52/Output8Bit mode.
		/// 
		/// </summary>
		/// <param name="s">the string to be sent
		/// </param>
		private bool writeSpecial(System.String s)
		{
			if (s == null)
				return true;
			if (((s.Length >= 3) && (s[0] == 27) && (s[1] == 'O')))
			{
				if (vt52mode)
				{
					if ((s[2] >= 'P') && (s[2] <= 'S'))
					{
						s = "\u001b" + s.Substring(2); /* ESC x */
					}
					else
					{
						s = "\u001b?" + s.Substring(2); /* ESC ? x */
					}
				}
				else
				{
					if (output8bit)
					{
						s = "\u008f" + s.Substring(2); /* SS3 x */
					} /* else keep string as it is */
				}
			}
			if (((s.Length >= 3) && (s[0] == 27) && (s[1] == '[')))
			{
				if (output8bit)
				{
					s = "\u009b" + s.Substring(2); /* CSI ... */
				} /* else keep */
			}
			return write(s, false);
		}
		
		/// <summary> main keytyping event handler...</summary>
		public virtual void  keyPressed(int keyCode, char keyChar, int modifiers)
		{
			bool control = (modifiers & de.mud.terminal.VDUInput_Fields.KEY_CONTROL) != 0;
			bool shift = (modifiers & de.mud.terminal.VDUInput_Fields.KEY_SHIFT) != 0;
			bool alt = (modifiers & de.mud.terminal.VDUInput_Fields.KEY_ALT) != 0;
			
			if (debug > 1)
				System.Console.Out.WriteLine("keyPressed(" + keyCode + ", " + (int) keyChar + ", " + modifiers + ")");
			
			int xind;
			System.String[] fmap;
			xind = 0;
			fmap = FunctionKey;
			if (shift)
			{
				fmap = FunctionKeyShift;
				xind = 1;
			}
			if (control)
			{
				fmap = FunctionKeyCtrl;
				xind = 2;
			}
			if (alt)
			{
				fmap = FunctionKeyAlt;
				xind = 3;
			}
			
			switch (keyCode)
			{
				
				case (int) System.Windows.Forms.Keys.Pause: 
					if (shift || control)
						sendTelnetCommand(Convert.ToSByte(243)); // BREAK
					break;
				
				case (int) System.Windows.Forms.Keys.F1: 
					writeSpecial(fmap[1]);
					break;
				
				case (int) System.Windows.Forms.Keys.F2: 
					writeSpecial(fmap[2]);
					break;
				
				case (int) System.Windows.Forms.Keys.F3: 
					writeSpecial(fmap[3]);
					break;
				
				case (int) System.Windows.Forms.Keys.F4: 
					writeSpecial(fmap[4]);
					break;
				
				case (int) System.Windows.Forms.Keys.F5: 
					writeSpecial(fmap[5]);
					break;
				
				case (int) System.Windows.Forms.Keys.F6: 
					writeSpecial(fmap[6]);
					break;
				
				case (int) System.Windows.Forms.Keys.F7: 
					writeSpecial(fmap[7]);
					break;
				
				case (int) System.Windows.Forms.Keys.F8: 
					writeSpecial(fmap[8]);
					break;
				
				case (int) System.Windows.Forms.Keys.F9: 
					writeSpecial(fmap[9]);
					break;
				
				case (int) System.Windows.Forms.Keys.F10: 
					writeSpecial(fmap[10]);
					break;
				
				case (int) System.Windows.Forms.Keys.F11: 
					writeSpecial(fmap[11]);
					break;
				
				case (int) System.Windows.Forms.Keys.F12: 
					writeSpecial(fmap[12]);
					break;
				
				case (int) System.Windows.Forms.Keys.Up: 
					writeSpecial(KeyUp[xind]);
					break;
				
				case (int) System.Windows.Forms.Keys.Down: 
					writeSpecial(KeyDown[xind]);
					break;
				
				case (int) System.Windows.Forms.Keys.Left: 
					writeSpecial(KeyLeft[xind]);
					break;
				
				case (int) System.Windows.Forms.Keys.Right: 
					writeSpecial(KeyRight[xind]);
					break;
				
				case (int) System.Windows.Forms.Keys.PageDown: 
					writeSpecial(NextScn[xind]);
					break;
				
				case (int) System.Windows.Forms.Keys.PageUp: 
					writeSpecial(PrevScn[xind]);
					break;
				
				case (int) System.Windows.Forms.Keys.Insert: 
					writeSpecial(Insert[xind]);
					break;
				
				case (int) System.Windows.Forms.Keys.Delete: 
					writeSpecial(Remove[xind]);
					break;
				
				case (int) System.Windows.Forms.Keys.Back: 
					// ASCII DEL character
					char c = (char)127;
					write(c.ToString());
					/*writeSpecial(BackSpace[xind]);
					if (localecho)
					{
						if ((System.Object) BackSpace[xind] == (System.Object) "\b")
						{
							putString("\b \b"); // make the last char 'deleted'
						}
						else
						{
							putString(BackSpace[xind]); // echo it
						}
					}
					*/
					break;
				
				case (int) System.Windows.Forms.Keys.Home: 
					writeSpecial(KeyHome[xind]);
					break;
				
				case (int) System.Windows.Forms.Keys.End: 
					writeSpecial(KeyEnd[xind]);
					break;
				
				case (int) System.Windows.Forms.Keys.NumLock: 
					if (vms && control)
					{
						writeSpecial(PF1);
					}
					if (!control)
						numlock = !numlock;
					break;
				
				case (int) System.Windows.Forms.Keys.CapsLock: 
					capslock = !capslock;
					return ;
				
				case (int) System.Windows.Forms.Keys.ShiftKey: 
				case (int) System.Windows.Forms.Keys.ControlKey: 
				case (int) System.Windows.Forms.Keys.Menu: 
					return ;
				
				default: 
					break;
				
			}
		}
		
		public virtual void  keyReleased(System.Windows.Forms.KeyEventArgs evt)
		{
			if (debug > 1)
				System.Console.Out.WriteLine("keyReleased(" + evt + ")");
			// ignore
		}
		
		/// <summary> Handle key Typed events for the terminal, this will get
		/// all normal key types, but no shift/alt/control/numlock.
		/// </summary>
		public virtual void  keyTyped(int keyCode, char keyChar, int modifiers)
		{
			bool control = (modifiers & de.mud.terminal.VDUInput_Fields.KEY_CONTROL) != 0;
			bool shift = (modifiers & de.mud.terminal.VDUInput_Fields.KEY_SHIFT) != 0;
			bool alt = (modifiers & de.mud.terminal.VDUInput_Fields.KEY_ALT) != 0;
			
			if (debug > 1)
				System.Console.Out.WriteLine("keyTyped(" + keyCode + ", " + (int) keyChar + ", " + modifiers + ")");
			
			if (keyChar == '\t')
			{
				if (shift)
				{
					write(TabKey[1], false);
				}
				else
				{
					if (control)
					{
						write(TabKey[2], false);
					}
					else
					{
						if (alt)
						{
							write(TabKey[3], false);
						}
						else
						{
							write(TabKey[0], false);
						}
					}
				}
				return ;
			}
			if (alt)
			{
				write("" + ((char) (keyChar | 0x80)));
				return ;
			}
			
			if (((keyCode == (int) System.Windows.Forms.Keys.Enter) || (keyChar == 10)) && !control)
			{
				write("\r", false);
				if (localecho)
					putString("\r\n"); // bad hack
				return ;
			}
			
			if ((keyCode == 10) && !control)
			{
				System.Console.Out.WriteLine("Sending \\r");
				write("\r", false);
				return ;
			}
			
			// FIXME: on german PC keyboards you have to use Alt-Ctrl-q to get an @,
			// so we can't just use it here... will probably break some other VMS
			// codes.  -Marcus
			// if(((!vms && keyChar == '2') || keyChar == '@' || keyChar == ' ')
			//    && control)
			if (((!vms && keyChar == '2') || keyChar == ' ') && control)
				write("" + (char) 0);
			
			if (vms)
			{
				if (keyChar == 127 && !control)
				{
					if (shift)
						writeSpecial(Insert[0]);
					//  VMS shift delete = insert
					else
						writeSpecial(Remove[0]); //  VMS delete = remove
					return ;
				}
				else if (control)
					switch (keyChar)
					{
						
						case '0': 
							writeSpecial(Numpad[0]);
							return ;
						
						case '1': 
							writeSpecial(Numpad[1]);
							return ;
						
						case '2': 
							writeSpecial(Numpad[2]);
							return ;
						
						case '3': 
							writeSpecial(Numpad[3]);
							return ;
						
						case '4': 
							writeSpecial(Numpad[4]);
							return ;
						
						case '5': 
							writeSpecial(Numpad[5]);
							return ;
						
						case '6': 
							writeSpecial(Numpad[6]);
							return ;
						
						case '7': 
							writeSpecial(Numpad[7]);
							return ;
						
						case '8': 
							writeSpecial(Numpad[8]);
							return ;
						
						case '9': 
							writeSpecial(Numpad[9]);
							return ;
						
						case '.': 
							writeSpecial(KPPeriod);
							return ;
						
						case '-': 
						case (char) (31): 
							writeSpecial(KPMinus);
							return ;
						
						case '+': 
							writeSpecial(KPComma);
							return ;
						
						case (char) (10): 
							writeSpecial(KPEnter);
							return ;
						
						case '/': 
							writeSpecial(PF2);
							return ;
						
						case '*': 
							writeSpecial(PF3);
							return ;
							/* NUMLOCK handled in keyPressed */
						
						default: 
							break;
						
					}
				/* Now what does this do and how did it get here. -Marcus
				if (shift && keyChar < 32) {
				write(PF1+(char)(keyChar + 64));
				return;
				}
				*/
			}
			
			// FIXME: not used?
			System.String[] fmap;
			int xind;
			xind = 0;
			fmap = FunctionKey;
			if (shift)
			{
				fmap = FunctionKeyShift;
				xind = 1;
			}
			if (control)
			{
				fmap = FunctionKeyCtrl;
				xind = 2;
			}
			if (alt)
			{
				fmap = FunctionKeyAlt;
				xind = 3;
			}
			
			if (keyCode == (int) System.Windows.Forms.Keys.Escape)
			{
				writeSpecial(Escape[xind]);
				return ;
			}
			
			if ((modifiers & de.mud.terminal.VDUInput_Fields.KEY_ACTION) != 0)
				switch (keyCode)
				{
					
					case (int) System.Windows.Forms.Keys.NumPad0: 
						writeSpecial(Numpad[0]);
						return ;
					
					case (int) System.Windows.Forms.Keys.NumPad1: 
						writeSpecial(Numpad[1]);
						return ;
					
					case (int) System.Windows.Forms.Keys.NumPad2: 
						writeSpecial(Numpad[2]);
						return ;
					
					case (int) System.Windows.Forms.Keys.NumPad3: 
						writeSpecial(Numpad[3]);
						return ;
					
					case (int) System.Windows.Forms.Keys.NumPad4: 
						writeSpecial(Numpad[4]);
						return ;
					
					case (int) System.Windows.Forms.Keys.NumPad5: 
						writeSpecial(Numpad[5]);
						return ;
					
					case (int) System.Windows.Forms.Keys.NumPad6: 
						writeSpecial(Numpad[6]);
						return ;
					
					case (int) System.Windows.Forms.Keys.NumPad7: 
						writeSpecial(Numpad[7]);
						return ;
					
					case (int) System.Windows.Forms.Keys.NumPad8: 
						writeSpecial(Numpad[8]);
						return ;
					
					case (int) System.Windows.Forms.Keys.NumPad9: 
						writeSpecial(Numpad[9]);
						return ;
					
					case (int) System.Windows.Forms.Keys.Decimal: 
						writeSpecial(NUMDot[xind]);
						return ;
					
					case (int) System.Windows.Forms.Keys.Add: 
						writeSpecial(NUMPlus[xind]);
						return ;
					}
			
			if (!((keyChar == 8) || (keyChar == 127) || (keyChar == '\r') || (keyChar == '\n')))
			{
				write("" + keyChar);
				return ;
			}
		}
		
		private void  handle_dcs(System.String dcs)
		{
			System.Console.Out.WriteLine("DCS: " + dcs);
		}
		
		private void  handle_osc(System.String osc)
		{
			System.Console.Out.WriteLine("OSC: " + osc);
		}
		
		//UPGRADE_NOTE: Final was removed from the declaration of 'unimap'. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1003'"
		private static readonly char[] unimap = new char[]{(char) (0x0000), (char) (0x0001), (char) (0x0002), (char) (0x0003), (char) (0x0004), (char) (0x0005), (char) (0x0006), (char) (0x0007), (char) (0x0008), (char) (0x0009), (char) (0x000a), (char) (0x000b), (char) (0x000c), (char) (0x000d), (char) (0x000e), (char) (0x000f), (char) (0x0010), (char) (0x0011), (char) (0x0012), (char) (0x0013), (char) (0x0014), (char) (0x0015), (char) (0x0016), (char) (0x0017), (char) (0x0018), (char) (0x0019), (char) (0x001a), (char) (0x001b), (char) (0x001c), (char) (0x001d), (char) (0x001e), (char) (0x001f), (char) (0x0020), (char) (0x0021), (char) (0x0022), (char) (0x0023), (char) (0x0024), (char) (0x0025), (char) (0x0026), (char) (0x0027), (char) (0x0028), (char) (0x0029), (char) (0x002a), (char) (0x002b), (char) (0x002c), (char) (0x002d), (char) (0x002e), (char) (0x002f), (char) (0x0030), (char) (0x0031), (char) (0x0032), (char) (0x0033), (char) (0x0034), (char) (0x0035), (char) (0x0036), (char) (0x0037), (char) (0x0038), (char) (0x0039), (char) (0x003a), (char) (0x003b), (char) (0x003c), (char) (0x003d), (char) (0x003e), (char) (0x003f), (char) (0x0040), (char) (0x0041), (char) (0x0042), (char) (0x0043), (char) (0x0044), (char) (0x0045), (char) (0x0046), (char) (0x0047), (char) (0x0048), (char) (0x0049), (char) (0x004a), (char) (0x004b), (char) (0x004c), (char) (0x004d), (char) (0x004e), (char) (0x004f), (char) (0x0050), (char) (0x0051), (char) (0x0052), (char) (0x0053), (char) (0x0054), (char) (0x0055), (char) (0x0056), (char) (0x0057), (char) (0x0058), (char) (0x0059), (char) (0x005a), (char) (0x005b), (char) (0x005c), (char) (0x005d), (char) (0x005e), (char) (0x005f), (char) (0x0060), (char) (0x0061), (char) (0x0062), (char) (0x0063), (char) (0x0064), (char) (0x0065), (char) (0x0066), (char) (0x0067), (char) (0x0068), (char) (0x0069), (char) (0x006a), (char) (0x006b), (char) (0x006c), (char) (0x006d), (char) (0x006e), (char) (0x006f), (char) (0x0070), (char) (0x0071), (char) (0x0072), (char) (0x0073), (char) (
			0x0074), (char) (0x0075), (char) (0x0076), (char) (0x0077), (char) (0x0078), (char) (0x0079), (char) (0x007a), (char) (0x007b), (char) (0x007c), (char) (0x007d), (char) (0x007e), (char) (0x007f), (char) (0x00c7), (char) (0x00fc), (char) (0x00e9), (char) (0x00e2), (char) (0x00e4), (char) (0x00e0), (char) (0x00e5), (char) (0x00e7), (char) (0x00ea), (char) (0x00eb), (char) (0x00e8), (char) (0x00ef), (char) (0x00ee), (char) (0x00ec), (char) (0x00c4), (char) (0x00c5), (char) (0x00c9), (char) (0x00e6), (char) (0x00c6), (char) (0x00f4), (char) (0x00f6), (char) (0x00f2), (char) (0x00fb), (char) (0x00f9), (char) (0x00ff), (char) (0x00d6), (char) (0x00dc), (char) (0x00a2), (char) (0x00a3), (char) (0x00a5), (char) (0x20a7), (char) (0x0192), (char) (0x00e1), (char) (0x00ed), (char) (0x00f3), (char) (0x00fa), (char) (0x00f1), (char) (0x00d1), (char) (0x00aa), (char) (0x00ba), (char) (0x00bf), (char) (0x2310), (char) (0x00ac), (char) (0x00bd), (char) (0x00bc), (char) (0x00a1), (char) (0x00ab), (char) (0x00bb), (char) (0x2591), (char) (0x2592), (char) (0x2593), (char) (0x2502), (char) (0x2524), (char) (0x2561), (char) (0x2562), (char) (0x2556), (char) (0x2555), (char) (0x2563), (char) (0x2551), (char) (0x2557), (char) (0x255d), (char) (0x255c), (char) (0x255b), (char) (0x2510), (char) (0x2514), (char) (0x2534), (char) (0x252c), (char) (0x251c), (char) (0x2500), (char) (0x253c), (char) (0x255e), (char) (0x255f), (char) (0x255a), (char) (0x2554), (char) (0x2569), (char) (0x2566), (char) (0x2560), (char) (0x2550), (char) (0x256c), (char) (0x2567), (char) (0x2568), (char) (0x2564), (char) (0x2565), (char) (0x2559), (char) (0x2558), (char) (0x2552), (char) (0x2553), (char) (0x256b), (char) (0x256a), (char) (0x2518), (char) (0x250c), (char) (0x2588), (char) (0x2584), (char) (0x258c), (char) (0x2590), (char) (0x2580), (char) (0x03b1), (char) (0x00df), (char) (0x0393), (char) (0x03c0), (char) (0x03a3), (char) (0x03c3), (char) (0x00b5), (char) (0x03c4), (char) (0x03a6), (char) (0x0398), (char) (0x03a9), (char) (0x03b4)
			, (char) (0x221e), (char) (0x03c6), (char) (0x03b5), (char) (0x2229), (char) (0x2261), (char) (0x00b1), (char) (0x2265), (char) (0x2264), (char) (0x2320), (char) (0x2321), (char) (0x00f7), (char) (0x2248), (char) (0x00b0), (char) (0x2219), (char) (0x00b7), (char) (0x221a), (char) (0x207f), (char) (0x00b2), (char) (0x25a0), (char) (0x00a0)};
		
		public virtual char map_cp850_unicode(char x)
		{
			if (x >= 0x100)
				return x;
			return unimap[x];
		}
		
		private void  _SetCursor(int row, int col)
		{
			int maxr = Rows;
			int tm = TopMargin;
			
			R = (row < 0)?0:row;
			C = (col < 0)?0:col;
			
			if (!moveoutsidemargins)
			{
				R += tm;
				maxr = BottomMargin;
			}
			if (R > maxr)
				R = maxr;
		}
		
		private void  putChar(char c, bool doshowcursor)
		{
			int rows = Rows; //statusline
			int columns = Columns;
			int tm = TopMargin;
			int bm = BottomMargin;
			// byte msg[];
			bool mapped = false;
			
			if (debug > 4)
				System.Console.Out.WriteLine("putChar(" + c + " [" + ((int) c) + "]) at R=" + R + " , C=" + C + ", columns=" + columns + ", rows=" + rows);
			markLine(R, 1);
			if (c > 255)
			{
				if (debug > 0)
					System.Console.Out.WriteLine("char > 255:" + (int) c);
				//return;
			}
			
			
			switch (term_state)
			{
				
				case TSTATE_DATA: 
					/* FIXME: we shouldn't use chars with bit 8 set if ibmcharset.
					* probably... but some BBS do anyway...
					*/
					if (!useibmcharset)
					{
						bool doneflag = true;
						switch (c)
						{
							
							case OSC: 
								osc = "";
								term_state = TSTATE_OSC;
								break;
							
							case RI: 
								if (R > tm)
									R--;
								else
									insertLine(R, 1, SCROLL_DOWN);
								if (debug > 1)
									System.Console.Out.WriteLine("RI");
								break;
							
							case IND: 
								if (debug > 2)
									System.Console.Out.WriteLine("IND at " + R + ", tm is " + tm + ", bm is " + bm);
								if (R == bm || R == rows - 1)
									insertLine(R, 1, SCROLL_UP);
								else
									R++;
								if (debug > 1)
									System.Console.Out.WriteLine("IND (at " + R + " )");
								break;
							
							case NEL: 
								if (R == bm || R == rows - 1)
									insertLine(R, 1, SCROLL_UP);
								else
									R++;
								C = 0;
								if (debug > 1)
									System.Console.Out.WriteLine("NEL (at " + R + " )");
								break;
							
							case HTS: 
								Tabs[C] = 1;
								if (debug > 1)
									System.Console.Out.WriteLine("HTS");
								break;
							
							case DCS: 
								dcs = "";
								term_state = TSTATE_DCS;
								break;
							
							default: 
								doneflag = false;
								break;
							
						}
						if (doneflag)
							break;
					}
					switch (c)
					{
						
						case SS3: 
							onegl = 3;
							break;
						
						case SS2: 
							onegl = 2;
							break;
						
						case CSI:  // should be in the 8bit section, but some BBS use this
							DCEvar = 0;
							DCEvars[0] = 0;
							DCEvars[1] = 0;
							DCEvars[2] = 0;
							DCEvars[3] = 0;
							term_state = TSTATE_CSI;
							break;
						
						case ESC: 
							term_state = TSTATE_ESC;
							lastwaslf = 0;
							break;
						
						case (char) (5):  /* ENQ */
							write(answerBack, false);
							break;
						
						case (char) (12): 
							/* FormFeed, Home for the BBS world */
							deleteArea(0, 0, columns, rows, attributes);
							C = R = 0;
							break;
						
						case '\b':  /* 8 */
							C--;
							if (C < 0)
								C = 0;
							lastwaslf = 0;
							break;
						
						case '\t': 
							do 
							{
								// Don't overwrite or insert! TABS are not destructive, but movement!
								C++;
							}
							while (C < columns && (Tabs[C] == 0));
							lastwaslf = 0;
							break;
						
						case '\r': 
							C = 0;
							break;
						
						case '\n': 
							if (debug > 3)
								System.Console.Out.WriteLine("R= " + R + ", bm " + bm + ", tm=" + tm + ", rows=" + rows);
							if (!vms)
							{
								if (lastwaslf != 0 && lastwaslf != c)
								//  Ray: I do not understand this logic.
									break;
								lastwaslf = c;
								/*C = 0;*/
							}
							if (R == bm || R >= rows - 1)
								insertLine(R, 1, SCROLL_UP);
							else
								R++;
							break;
						
						case (char) (7): 
							beep();
							break;
						
						case '\x000E':  /* SMACS , as */
							/* ^N, Shift out - Put G1 into GL */
							gl = (char) (1);
							usedcharsets = true;
							break;
						
						case '\x000F':  /* RMACS , ae */
							/* ^O, Shift in - Put G0 into GL */
							gl = (char) (0);
							usedcharsets = true;
							break;
						
						default: 
						{
							int thisgl = gl;
							
							if (onegl >= 0)
							{
								thisgl = onegl;
								onegl = - 1;
							}
							lastwaslf = 0;
							if (c < 32)
							{
								if (c != 0)
									if (debug > 0)
										System.Console.Out.WriteLine("TSTATE_DATA char: " + ((int) c));
								/*break; some BBS really want those characters, like hearst etc. */
								if (c == 0)
								/* print 0 ... you bet */
									break;
							}
							if (C >= columns)
							{
								if (wraparound)
								{
									if (R < rows - 1)
										R++;
									else
										insertLine(R, 1, SCROLL_UP);
									C = 0;
								}
								else
								{
									// cursor stays on last character.
									C = columns - 1;
								}
							}
							
							// Mapping if DEC Special is chosen charset
							if (usedcharsets)
							{
								if (c >= '\u0020' && c <= '\u007f')
								{
									switch (gx[thisgl])
									{
										
										case '0': 
											// Remap SCOANSI line drawing to VT100 line drawing chars
											// for our SCO using customers.
											if (terminalID.Equals("scoansi") || terminalID.Equals("ansi"))
											{
												for (int i = 0; i < scoansi_acs.Length; i += 2)
												{
													if (c == scoansi_acs[i])
													{
														c = scoansi_acs[i + 1];
														break;
													}
												}
											}
											if (c >= '\u005f' && c <= '\u007e')
											{
												c = DECSPECIAL[(short) c - 0x5f];
												mapped = true;
											}
											break;
										
										case '<':  // 'user preferred' is currently 'ISO Latin-1 suppl
											c = (char) (((int) c & 0x7f) | 0x80);
											mapped = true;
											break;
										
										case 'A': 
										case 'B':  // Latin-1 , ASCII -> fall through
											mapped = true;
											break;
										
										default: 
											System.Console.Out.WriteLine("Unsupported GL mapping: " + gx[thisgl]);
											break;
										
									}
								}
								if (!mapped && (c >= '\u0080' && c <= '\u00ff'))
								{
									switch (gx[gr])
									{
										
										case '0': 
											if (c >= '\u00df' && c <= '\u00fe')
											{
												c = DECSPECIAL[c - '\u00df'];
												mapped = true;
											}
											break;
										
										case '<': 
										case 'A': 
										case 'B': 
											mapped = true;
											break;
										
										default: 
											System.Console.Out.WriteLine("Unsupported GR mapping: " + gx[gr]);
											break;
										
									}
								}
							}
							if (!mapped && useibmcharset)
								c = map_cp850_unicode(c);
							
							/*if(true || (statusmode == 0)) { */
							if (insertmode == 1)
							{
								insertChar(C, R, c, attributes);
							}
							else
							{
								putChar(C, R, c, attributes);
							}
							/*
							} else {
							if (insertmode==1) {
							insertChar(C, rows, c, attributes);
							} else {
							putChar(C, rows, c, attributes);
							}
							}
							*/
							C++;
							break;
						}
						
					} /* switch(c) */
					break;
				
				case TSTATE_OSC: 
					if ((c < 0x20) && (c != ESC))
					{
						// NP - No printing character
						handle_osc(osc);
						term_state = TSTATE_DATA;
						break;
					}
					//but check for vt102 ESC \
					if (c == '\\' && osc[osc.Length - 1] == ESC)
					{
						handle_osc(osc);
						term_state = TSTATE_DATA;
						break;
					}
					osc = osc + c;
					break;
				
				case TSTATE_ESCSPACE: 
					term_state = TSTATE_DATA;
					switch (c)
					{
						
						case 'F':  /* S7C1T, Disable output of 8-bit controls, use 7-bit */
							output8bit = false;
							break;
						
						case 'G':  /* S8C1T, Enable output of 8-bit control codes*/
							output8bit = true;
							break;
						
						default: 
							System.Console.Out.WriteLine("ESC <space> " + c + " unhandled.");
							break;
						
					}
					break;
				
				case TSTATE_ESC: 
					term_state = TSTATE_DATA;
					switch (c)
					{
						
						case ' ': 
							term_state = TSTATE_ESCSPACE;
							break;
						
						case '#': 
							term_state = TSTATE_ESCSQUARE;
							break;
						
						case 'c': 
							/* Hard terminal reset */
							/* reset character sets */
							gx[0] = 'B';
							gx[1] = '0';
							gx[2] = 'B';
							gx[3] = 'B';
							gl = (char) (0); // default GL to G0
							gr = (char) (1); // default GR to G1
							/* reset tabs */
							int nw = Columns;
							if (nw < 132)
								nw = 132;
							Tabs = new sbyte[nw];
							for (int i = 0; i < nw; i += 8)
							{
								Tabs[i] = 1;
							}
							/*FIXME:*/
							break;
						
						case '[': 
							DCEvar = 0;
							DCEvars[0] = 0;
							DCEvars[1] = 0;
							DCEvars[2] = 0;
							DCEvars[3] = 0;
							term_state = TSTATE_CSI;
							break;
						
						case ']': 
							osc = "";
							term_state = TSTATE_OSC;
							break;
						
						case 'P': 
							dcs = "";
							term_state = TSTATE_DCS;
							break;
						
						case 'A':  /* CUU */
							R--;
							if (R < 0)
								R = 0;
							break;
						
						case 'B':  /* CUD */
							R++;
							if (R > rows - 1)
								R = rows - 1;
							break;
						
						case 'C': 
							C++;
							if (C >= columns)
								C = columns - 1;
							break;
						
						case 'I':  // RI
							insertLine(R, 1, SCROLL_DOWN);
							break;
						
						case 'E':  /* NEL */
							if (R == bm || R == rows - 1)
								insertLine(R, 1, SCROLL_UP);
							else
								R++;
							C = 0;
							if (debug > 1)
								System.Console.Out.WriteLine("ESC E (at " + R + ")");
							break;
						
						case 'D':  /* IND */
							if (R == bm || R == rows - 1)
								insertLine(R, 1, SCROLL_UP);
							else
								R++;
							if (debug > 1)
								System.Console.Out.WriteLine("ESC D (at " + R + " )");
							break;
						
						case 'J':  /* erase to end of screen */
							if (R < rows - 1)
								deleteArea(0, R + 1, columns, rows - R - 1, attributes);
							if (C < columns - 1)
								deleteArea(C, R, columns - C, 1, attributes);
							break;
						
						case 'K': 
							if (C < columns - 1)
								deleteArea(C, R, columns - C, 1, attributes);
							break;
						
						case 'M':  // RI
							System.Console.Out.WriteLine("ESC M : R is " + R + ", tm is " + tm + ", bm is " + bm);
							if (R > bm)
							// outside scrolling region
								break;
							if (R > tm)
							{
								// just go up 1 line.
								R--;
							}
							else
							{
								// scroll down
								insertLine(R, 1, SCROLL_DOWN);
							}
							/* else do nothing ; */
							if (debug > 2)
								System.Console.Out.WriteLine("ESC M ");
							break;
						
						case 'H': 
							if (debug > 1)
								System.Console.Out.WriteLine("ESC H at " + C);
							/* right border probably ...*/
							if (C >= columns)
								C = columns - 1;
							Tabs[C] = 1;
							break;
						
						case 'N':  // SS2
							onegl = 2;
							break;
						
						case 'O':  // SS3
							onegl = 3;
							break;
						
						case '=': 
							/*application keypad*/
							if (debug > 0)
								System.Console.Out.WriteLine("ESC =");
							keypadmode = true;
							break;
						
						case '<':  /* vt52 mode off */
							vt52mode = false;
							break;
						
						case '>':  /*normal keypad*/
							if (debug > 0)
								System.Console.Out.WriteLine("ESC >");
							keypadmode = false;
							break;
						
						case '7':  /*save cursor, attributes, margins */
							Sc = C;
							Sr = R;
							Sgl = gl;
							Sgr = gr;
							Sa = attributes;
							Sgx = new char[4];
							for (int i = 0; i < 4; i++)
								Sgx[i] = gx[i];
							Stm = TopMargin;
							Sbm = BottomMargin;
							if (debug > 1)
								System.Console.Out.WriteLine("ESC 7");
							break;
						
						case '8':  /*restore cursor, attributes, margins */
							C = Sc;
							R = Sr;
							gl = Sgl;
							gr = Sgr;
							for (int i = 0; i < 4; i++)
								gx[i] = Sgx[i];
							TopMargin = Stm;
							BottomMargin = Sbm;
							attributes = Sa;
							if (debug > 1)
								System.Console.Out.WriteLine("ESC 8");
							break;
						
						case '(':  /* Designate G0 Character set (ISO 2022) */
							term_state = TSTATE_SETG0;
							usedcharsets = true;
							break;
						
						case ')':  /* Designate G1 character set (ISO 2022) */
							term_state = TSTATE_SETG1;
							usedcharsets = true;
							break;
						
						case '*':  /* Designate G2 Character set (ISO 2022) */
							term_state = TSTATE_SETG2;
							usedcharsets = true;
							break;
						
						case '+':  /* Designate G3 Character set (ISO 2022) */
							term_state = TSTATE_SETG3;
							usedcharsets = true;
							break;
						
						case '~':  /* Locking Shift 1, right */
							gr = (char) (1);
							usedcharsets = true;
							break;
						
						case 'n':  /* Locking Shift 2 */
							gl = (char) (2);
							usedcharsets = true;
							break;
						
						case '}':  /* Locking Shift 2, right */
							gr = (char) (2);
							usedcharsets = true;
							break;
						
						case 'o':  /* Locking Shift 3 */
							gl = (char) (3);
							usedcharsets = true;
							break;
						
						case '|':  /* Locking Shift 3, right */
							gr = (char) (3);
							usedcharsets = true;
							break;
						
						case 'Y':  /* vt52 cursor address mode , next chars are x,y */
							term_state = TSTATE_VT52Y;
							break;
						
						default: 
							System.Console.Out.WriteLine("ESC unknown letter: " + c + " (" + ((int) c) + ")");
							break;
						
					}
					break;
				
				case TSTATE_VT52X: 
					C = c - 37;
					term_state = TSTATE_VT52Y;
					break;
				
				case TSTATE_VT52Y: 
					R = c - 37;
					term_state = TSTATE_DATA;
					break;
				
				case TSTATE_SETG0: 
					if (c != '0' && c != 'A' && c != 'B' && c != '<')
						System.Console.Out.WriteLine("ESC ( " + c + ": G0 char set?  (" + ((int) c) + ")");
					else
					{
						if (debug > 2)
							System.Console.Out.WriteLine("ESC ( : G0 char set  (" + c + " " + ((int) c) + ")");
						gx[0] = c;
					}
					term_state = TSTATE_DATA;
					break;
				
				case TSTATE_SETG1: 
					if (c != '0' && c != 'A' && c != 'B' && c != '<')
					{
						System.Console.Out.WriteLine("ESC ) " + c + " (" + ((int) c) + ") :G1 char set?");
					}
					else
					{
						if (debug > 2)
							System.Console.Out.WriteLine("ESC ) :G1 char set  (" + c + " " + ((int) c) + ")");
						gx[1] = c;
					}
					term_state = TSTATE_DATA;
					break;
				
				case TSTATE_SETG2: 
					if (c != '0' && c != 'A' && c != 'B' && c != '<')
						System.Console.Out.WriteLine("ESC*:G2 char set?  (" + ((int) c) + ")");
					else
					{
						if (debug > 2)
							System.Console.Out.WriteLine("ESC*:G2 char set  (" + c + " " + ((int) c) + ")");
						gx[2] = c;
					}
					term_state = TSTATE_DATA;
					break;
				
				case TSTATE_SETG3: 
					if (c != '0' && c != 'A' && c != 'B' && c != '<')
						System.Console.Out.WriteLine("ESC+:G3 char set?  (" + ((int) c) + ")");
					else
					{
						if (debug > 2)
							System.Console.Out.WriteLine("ESC+:G3 char set  (" + c + " " + ((int) c) + ")");
						gx[3] = c;
					}
					term_state = TSTATE_DATA;
					break;
				
				case TSTATE_ESCSQUARE: 
					switch (c)
					{
						
						case '8': 
							for (int i = 0; i < columns; i++)
								for (int j = 0; j < rows; j++)
									putChar(i, j, 'E', 0);
							break;
						
						default: 
							System.Console.Out.WriteLine("ESC # " + c + " not supported.");
							break;
						
					}
					term_state = TSTATE_DATA;
					break;
				
				case TSTATE_DCS: 
					if (c == '\\' && dcs[dcs.Length - 1] == ESC)
					{
						handle_dcs(dcs);
						term_state = TSTATE_DATA;
						break;
					}
					dcs = dcs + c;
					break;
				
				
				case TSTATE_DCEQ: 
					term_state = TSTATE_DATA;
					switch (c)
					{
						
						case '0': 
						case '1': 
						case '2': 
						case '3': 
						case '4': 
						case '5': 
						case '6': 
						case '7': 
						case '8': 
						case '9': 
							DCEvars[DCEvar] = DCEvars[DCEvar] * 10 + ((int) c) - 48;
							term_state = TSTATE_DCEQ;
							break;
						
						case ';': 
							DCEvar++;
							DCEvars[DCEvar] = 0;
							term_state = TSTATE_DCEQ;
							break;
						
						case 's':  // XTERM_SAVE missing!
							if (true || debug > 1)
								System.Console.Out.WriteLine("ESC [ ? " + DCEvars[0] + " s unimplemented!");
							break;
						
						case 'r':  // XTERM_RESTORE
							if (true || debug > 1)
								System.Console.Out.WriteLine("ESC [ ? " + DCEvars[0] + " r");
							/* DEC Mode reset */
							for (int i = 0; i <= DCEvar; i++)
							{
								switch (DCEvars[i])
								{
									
									case 3:  /* 80 columns*/
										setScreenSize(80, Rows, true);
										break;
									
									case 4:  /* scrolling mode, smooth */
										break;
									
									case 5:  /* light background */
										break;
									
									case 6:  /* DECOM (Origin Mode) move inside margins. */
										moveoutsidemargins = true;
										break;
									
									case 7:  /* DECAWM: Autowrap Mode */
										wraparound = false;
										break;
									
									case 12:  /* local echo off */
										break;
									
									case 9: 
									/* X10 mouse */
									case 1000: 
									/* xterm style mouse report on */
									case 1001: 
									case 1002: 
									case 1003: 
										mouserpt = DCEvars[i];
										break;
									
									default: 
										System.Console.Out.WriteLine("ESC [ ? " + DCEvars[0] + " r, unimplemented!");
										break;
									
								}
							}
							break;
						
						case 'h':  // DECSET
							if (debug > 0)
								System.Console.Out.WriteLine("ESC [ ? " + DCEvars[0] + " h");
							/* DEC Mode set */
							for (int i = 0; i <= DCEvar; i++)
							{
								switch (DCEvars[i])
								{
									
									case 1:  /* Application cursor keys - this does not work for me - radek */
										//KeyUp[0] = "\u001bOA";
										//KeyDown[0] = "\u001bOB";
										//KeyRight[0] = "\u001bOC";
										//KeyLeft[0] = "\u001bOD";
										break;
									
									case 2:  /* DECANM */
										vt52mode = false;
										break;
									
									case 3:  /* 132 columns*/
										setScreenSize(132, Rows, true);
										break;
									
									case 6:  /* DECOM: move inside margins. */
										moveoutsidemargins = false;
										break;
									
									case 7:  /* DECAWM: Autowrap Mode */
										wraparound = true;
										break;
									
									case 25:  /* turn cursor on */
										showCursor(true);
										break;
									
									case 9: 
									/* X10 mouse */
									case 1000: 
									/* xterm style mouse report on */
									case 1001: 
									case 1002: 
									case 1003: 
										mouserpt = DCEvars[i];
										break;
										
										/* unimplemented stuff, fall through */
										/* 4  - scrolling mode, smooth */
										/* 5  - light background */
										/* 12 - local echo off */
										/* 18 - DECPFF - Printer Form Feed Mode -> On */
										/* 19 - DECPEX - Printer Extent Mode -> Screen */
									
									default: 
										System.Console.Out.WriteLine("ESC [ ? " + DCEvars[0] + " h, unsupported.");
										break;
									
								}
							}
							break;
						
						case 'i':  // DEC Printer Control, autoprint, echo screenchars to printer
							// This is different to CSI i!
							// Also: "Autoprint prints a final display line only when the
							// cursor is moved off the line by an autowrap or LF, FF, or
							// VT (otherwise do not print the line)."
							switch (DCEvars[0])
							{
								
								case 1: 
									if (debug > 1)
										System.Console.Out.WriteLine("CSI ? 1 i : Print line containing cursor");
									break;
								
								case 4: 
									if (debug > 1)
										System.Console.Out.WriteLine("CSI ? 4 i : Start passthrough printing");
									break;
								
								case 5: 
									if (debug > 1)
										System.Console.Out.WriteLine("CSI ? 4 i : Stop passthrough printing");
									break;
								}
							break;
						
						case 'l':  //DECRST
							/* DEC Mode reset */
							if (debug > 0)
								System.Console.Out.WriteLine("ESC [ ? " + DCEvars[0] + " l");
							for (int i = 0; i <= DCEvar; i++)
							{
								switch (DCEvars[i])
								{
									
									case 1:  /* Application cursor keys */
										KeyUp[0] = "\u001b[A";
										KeyDown[0] = "\u001b[B";
										KeyRight[0] = "\u001b[C";
										KeyLeft[0] = "\u001b[D";
										break;
									
									case 2:  /* DECANM */
										vt52mode = true;
										break;
									
									case 3:  /* 80 columns*/
										setScreenSize(80, Rows, true);
										break;
									
									case 6:  /* DECOM: move outside margins. */
										moveoutsidemargins = true;
										break;
									
									case 7:  /* DECAWM: Autowrap Mode OFF */
										wraparound = false;
										break;
									
									case 25:  /* turn cursor off */
										showCursor(false);
										break;
										/* Unimplemented stuff: */
										/* 4  - scrolling mode, jump */
										/* 5  - dark background */
										/* 7  - DECAWM - no wrap around mode */
										/* 12 - local echo on */
										/* 18 - DECPFF - Printer Form Feed Mode -> Off*/
										/* 19 - DECPEX - Printer Extent Mode -> Scrolling Region */
									
									case 9: 
									/* X10 mouse */
									case 1000: 
									/* xterm style mouse report OFF */
									case 1001: 
									case 1002: 
									case 1003: 
										mouserpt = 0;
										break;
									
									default: 
										System.Console.Out.WriteLine("ESC [ ? " + DCEvars[0] + " l, unsupported.");
										break;
									
								}
							}
							break;
						
						case 'n': 
							if (debug > 0)
								System.Console.Out.WriteLine("ESC [ ? " + DCEvars[0] + " n");
							switch (DCEvars[0])
							{
								
								case 15: 
									/* printer? no printer. */
									write(((char) ESC) + "[?13n", false);
									System.Console.Out.WriteLine("ESC[5n");
									break;
								
								default: 
									System.Console.Out.WriteLine("ESC [ ? " + DCEvars[0] + " n, unsupported.");
									break;
								
							}
							break;
						
						default: 
							System.Console.Out.WriteLine("ESC [ ? " + DCEvars[0] + " " + c + ", unsupported.");
							break;
						
					}
					break;
				
				case TSTATE_CSI_EX: 
					term_state = TSTATE_DATA;
					switch (c)
					{
						
						case ESC: 
							term_state = TSTATE_ESC;
							break;
						
						default: 
							System.Console.Out.WriteLine("Unknown character ESC[! character is " + (int) c);
							break;
						
					}
					break;
				
				case TSTATE_CSI_TICKS: 
					term_state = TSTATE_DATA;
					switch (c)
					{
						
						case 'p': 
							System.Console.Out.WriteLine("Conformance level: " + DCEvars[0] + " (unsupported)," + DCEvars[1]);
							if (DCEvars[0] == 61)
							{
								output8bit = false;
								break;
							}
							if (DCEvars[1] == 1)
							{
								output8bit = false;
							}
							else
							{
								output8bit = true; /* 0 or 2 */
							}
							break;
						
						default: 
							System.Console.Out.WriteLine("Unknown ESC [...  \"" + c);
							break;
						
					}
					break;
				
				case TSTATE_CSI_EQUAL: 
					term_state = TSTATE_DATA;
					switch (c)
					{
						
						case '0': 
						case '1': 
						case '2': 
						case '3': 
						case '4': 
						case '5': 
						case '6': 
						case '7': 
						case '8': 
						case '9': 
							DCEvars[DCEvar] = DCEvars[DCEvar] * 10 + ((int) c) - 48;
							term_state = TSTATE_CSI_EQUAL;
							break;
						
						case ';': 
							DCEvar++;
							DCEvars[DCEvar] = 0;
							term_state = TSTATE_CSI_EQUAL;
							break;
						
						
						case 'F':  /* SCO ANSI foreground */
							{
								int newcolor;
								
								System.Console.Out.WriteLine("ESC [ = " + DCEvars[0] + " F");
								
								attributes &= ~ COLOR_FG;
								newcolor = ((DCEvars[0] & 1) << 2) | (DCEvars[0] & 2) | ((DCEvars[0] & 4) >> 2);
								attributes |= (newcolor + 1) << COLOR_FG_SHIFT;
								
								break;
							}
						
						case 'G':  /* SCO ANSI background */
							{
								int newcolor;
								
								System.Console.Out.WriteLine("ESC [ = " + DCEvars[0] + " G");
								
								attributes &= ~ COLOR_BG;
								newcolor = ((DCEvars[0] & 1) << 2) | (DCEvars[0] & 2) | ((DCEvars[0] & 4) >> 2);
								attributes |= (newcolor + 1) << COLOR_BG_SHIFT;
								break;
							}
						
						
						default: 
							System.Console.Out.Write("Unknown ESC [ = ");
							for (int i = 0; i <= DCEvar; i++)
								System.Console.Out.Write(DCEvars[i] + ",");
							System.Console.Out.WriteLine("" + c);
							break;
						
					}
					break;
				
				case TSTATE_CSI_DOLLAR: 
					term_state = TSTATE_DATA;
					switch (c)
					{
						
						case '}': 
							System.Console.Out.WriteLine("Active Status Display now " + DCEvars[0]);
							statusmode = DCEvars[0];
							break;
							/* bad documentation?
							case '-':
							System.out.println("Set Status Display now "+DCEvars[0]);
							break;
							*/
						
						case '~': 
							System.Console.Out.WriteLine("Status Line mode now " + DCEvars[0]);
							break;
						
						default: 
							System.Console.Out.WriteLine("UNKNOWN Status Display code " + c + ", with Pn=" + DCEvars[0]);
							break;
						
					}
					break;
				
				case TSTATE_CSI: 
					term_state = TSTATE_DATA;
					switch (c)
					{
						
						case '"': 
							term_state = TSTATE_CSI_TICKS;
							break;
						
						case '$': 
							term_state = TSTATE_CSI_DOLLAR;
							break;
						
						case '=': 
							term_state = TSTATE_CSI_EQUAL;
							break;
						
						case '!': 
							term_state = TSTATE_CSI_EX;
							break;
						
						case '?': 
							DCEvar = 0;
							DCEvars[0] = 0;
							term_state = TSTATE_DCEQ;
							break;
						
						case '0': 
						case '1': 
						case '2': 
						case '3': 
						case '4': 
						case '5': 
						case '6': 
						case '7': 
						case '8': 
						case '9': 
							DCEvars[DCEvar] = DCEvars[DCEvar] * 10 + ((int) c) - 48;
							term_state = TSTATE_CSI;
							break;
						
						case ';': 
							DCEvar++;
							DCEvars[DCEvar] = 0;
							term_state = TSTATE_CSI;
							break;
						
						case 'c':  /* send primary device attributes */
							/* send (ESC[?61c) */
							
							System.String subcode = "";
							if (terminalID.Equals("vt320"))
								subcode = "63;";
							if (terminalID.Equals("vt220"))
								subcode = "62;";
							if (terminalID.Equals("vt100"))
								subcode = "61;";
							write(((char) ESC) + "[?" + subcode + "1;2c", false);
							if (debug > 1)
								System.Console.Out.WriteLine("ESC [ " + DCEvars[0] + " c");
							break;
						
						case 'q': 
							if (debug > 1)
								System.Console.Out.WriteLine("ESC [ " + DCEvars[0] + " q");
							break;
						
						case 'g': 
							/* used for tabsets */
							switch (DCEvars[0])
							{
								
								case 3:  /* clear them */
									Tabs = new sbyte[Columns];
									break;
								
								case 0: 
									Tabs[C] = 0;
									break;
								}
							if (debug > 1)
								System.Console.Out.WriteLine("ESC [ " + DCEvars[0] + " g");
							break;
						
						case 'h': 
							switch (DCEvars[0])
							{
								
								case 4: 
									insertmode = 1;
									break;
								
								case 20: 
									System.Console.Out.WriteLine("Setting CRLF to TRUE");
									sendcrlf = true;
									break;
								
								default: 
									System.Console.Out.WriteLine("unsupported: ESC [ " + DCEvars[0] + " h");
									break;
								
							}
							if (debug > 1)
								System.Console.Out.WriteLine("ESC [ " + DCEvars[0] + " h");
							break;
						
						case 'i':  // Printer Controller mode.
							// "Transparent printing sends all output, except the CSI 4 i
							//  termination string, to the printer and not the screen,
							//  uses an 8-bit channel if no parity so NUL and DEL will be
							//  seen by the printer and by the termination recognizer code,
							//  and all translation and character set selections are
							//  bypassed."
							switch (DCEvars[0])
							{
								
								case 0: 
									if (debug > 1)
										System.Console.Out.WriteLine("CSI 0 i:  Print Screen, not implemented.");
									break;
								
								case 4: 
									if (debug > 1)
										System.Console.Out.WriteLine("CSI 4 i:  Enable Transparent Printing, not implemented.");
									break;
								
								case 5: 
									if (debug > 1)
										System.Console.Out.WriteLine("CSI 4/5 i:  Disable Transparent Printing, not implemented.");
									break;
								
								default: 
									System.Console.Out.WriteLine("ESC [ " + DCEvars[0] + " i, unimplemented!");
									break;
								
							}
							break;
						
						case 'l': 
							switch (DCEvars[0])
							{
								
								case 4: 
									insertmode = 0;
									break;
								
								case 20: 
									System.Console.Out.WriteLine("Setting CRLF to FALSE");
									sendcrlf = false;
									break;
								
								default: 
									System.Console.Out.WriteLine("ESC [ " + DCEvars[0] + " l, unimplemented!");
									break;
								
							}
							break;
						
						case 'A':  // CUU
							{
								int limit;
								/* FIXME: xterm only cares about 0 and topmargin */
								if (R > bm)
									limit = bm + 1;
								else if (R >= tm)
								{
									limit = tm;
								}
								else
									limit = 0;
								if (DCEvars[0] == 0)
									R--;
								else
									R -= DCEvars[0];
								if (R < limit)
									R = limit;
								if (debug > 1)
									System.Console.Out.WriteLine("ESC [ " + DCEvars[0] + " A");
								break;
							}
						
						case 'B':  // CUD
							/* cursor down n (1) times */
							{
								int limit;
								if (R < tm)
									limit = tm - 1;
								else if (R <= bm)
								{
									limit = bm;
								}
								else
									limit = rows - 1;
								if (DCEvars[0] == 0)
									R++;
								else
									R += DCEvars[0];
								if (R > limit)
									R = limit;
								else
								{
									if (debug > 2)
										System.Console.Out.WriteLine("Not limited.");
								}
								if (debug > 2)
									System.Console.Out.WriteLine("to: " + R);
								if (debug > 1)
									System.Console.Out.WriteLine("ESC [ " + DCEvars[0] + " B (at C=" + C + ")");
								break;
							}
						
						case 'C': 
							if (DCEvars[0] == 0)
								C++;
							else
								C += DCEvars[0];
							if (C > columns - 1)
								C = columns - 1;
							if (debug > 1)
								System.Console.Out.WriteLine("ESC [ " + DCEvars[0] + " C");
							break;
						
						case 'd':  // CVA
							R = DCEvars[0];
							if (debug > 1)
								System.Console.Out.WriteLine("ESC [ " + DCEvars[0] + " d");
							break;
						
						case 'D': 
							if (DCEvars[0] == 0)
								C--;
							else
								C -= DCEvars[0];
							if (C < 0)
								C = 0;
							if (debug > 1)
								System.Console.Out.WriteLine("ESC [ " + DCEvars[0] + " D");
							break;
						
						case 'r':  // DECSTBM
							if (DCEvar > 0)
							//  Ray:  Any argument is optional
							{
								R = DCEvars[1] - 1;
								if (R < 0)
									R = rows - 1;
								else if (R >= rows)
								{
									R = rows - 1;
								}
							}
							else
								R = rows - 1;
							BottomMargin = R;
							if (R >= DCEvars[0])
							{
								R = DCEvars[0] - 1;
								if (R < 0)
									R = 0;
							}
							TopMargin = R;
							_SetCursor(0, 0);
							if (debug > 1)
								System.Console.Out.WriteLine("ESC [" + DCEvars[0] + " ; " + DCEvars[1] + " r");
							break;
						
						case 'G':  /* CUP  / cursor absolute column */
							C = DCEvars[0];
							if (debug > 1)
								System.Console.Out.WriteLine("ESC [ " + DCEvars[0] + " G");
							break;
						
						case 'H':  /* CUP  / cursor position */
							/* gets 2 arguments */
							_SetCursor(DCEvars[0] - 1, DCEvars[1] - 1);
							if (debug > 2)
							{
								System.Console.Out.WriteLine("ESC [ " + DCEvars[0] + ";" + DCEvars[1] + " H, moveoutsidemargins " + moveoutsidemargins);
								System.Console.Out.WriteLine("	-> R now " + R + ", C now " + C);
							}
							break;
						
						case 'f':  /* move cursor 2 */
							/* gets 2 arguments */
							R = DCEvars[0] - 1;
							C = DCEvars[1] - 1;
							if (C < 0)
								C = 0;
							if (R < 0)
								R = 0;
							if (debug > 2)
								System.Console.Out.WriteLine("ESC [ " + DCEvars[0] + ";" + DCEvars[1] + " f");
							break;
						
						case 'S':  /* ind aka 'scroll forward' */
							if (DCEvars[0] == 0)
								insertLine(rows - 1, SCROLL_UP);
							else
								insertLine(rows - 1, DCEvars[0], SCROLL_UP);
							break;
						
						case 'L': 
							/* insert n lines */
							if (DCEvars[0] == 0)
								insertLine(R, SCROLL_DOWN);
							else
								insertLine(R, DCEvars[0], SCROLL_DOWN);
							if (debug > 1)
								System.Console.Out.WriteLine("ESC [ " + DCEvars[0] + "" + (c) + " (at R " + R + ")");
							break;
						
						case 'T':  /* 'ri' aka scroll backward */
							if (DCEvars[0] == 0)
								insertLine(0, SCROLL_DOWN);
							else
								insertLine(0, DCEvars[0], SCROLL_DOWN);
							break;
						
						case 'M': 
							if (debug > 1)
								System.Console.Out.WriteLine("ESC [ " + DCEvars[0] + "" + (c) + " at R=" + R);
							if (DCEvars[0] == 0)
								deleteLine(R);
							else
								for (int i = 0; i < DCEvars[0]; i++)
									deleteLine(R);
							break;
						
						case 'K': 
							if (debug > 1)
								System.Console.Out.WriteLine("ESC [ " + DCEvars[0] + " K");
							/* clear in line */
							switch (DCEvars[0])
							{
								
								case 6: 
								/* 97801 uses ESC[6K for delete to end of line */
								case 0:  /*clear to right*/
									if (C < columns - 1)
										deleteArea(C, R, columns - C, 1, attributes);
									break;
								
								case 1:  /*clear to the left, including this */
									if (C > 0)
										deleteArea(0, R, C + 1, 1, attributes);
									break;
								
								case 2:  /*clear whole line */
									deleteArea(0, R, columns, 1, attributes);
									break;
								}
							break;
						
						case 'J': 
							/* clear below current line */
							switch (DCEvars[0])
							{
								
								case 0: 
									if (R < rows - 1)
										deleteArea(0, R + 1, columns, rows - R - 1, attributes);
									if (C < columns - 1)
										deleteArea(C, R, columns - C, 1, attributes);
									break;
								
								case 1: 
									if (R > 0)
										deleteArea(0, 0, columns, R, attributes);
									if (C > 0)
										deleteArea(0, R, C + 1, 1, attributes); // include up to and including current
									break;
								
								case 2: 
									deleteArea(0, 0, columns, rows, attributes);
									break;
								}
							if (debug > 1)
								System.Console.Out.WriteLine("ESC [ " + DCEvars[0] + " J");
							break;
						
						case '@': 
							if (debug > 1)
								System.Console.Out.WriteLine("ESC [ " + DCEvars[0] + " @");
							for (int i = 0; i < DCEvars[0]; i++)
								insertChar(C, R, ' ', attributes);
							break;
						
						case 'X': 
						{
							int toerase = DCEvars[0];
							if (debug > 1)
								System.Console.Out.WriteLine("ESC [ " + DCEvars[0] + " X, C=" + C + ",R=" + R);
							if (toerase == 0)
								toerase = 1;
							if (toerase + C > columns)
								toerase = columns - C;
							deleteArea(C, R, toerase, 1, attributes);
							// does not change cursor position
							break;
						}
						
						case 'P': 
							if (debug > 1)
								System.Console.Out.WriteLine("ESC [ " + DCEvars[0] + " P, C=" + C + ",R=" + R);
							if (DCEvars[0] == 0)
								DCEvars[0] = 1;
							for (int i = 0; i < DCEvars[0]; i++)
								deleteChar(C, R);
							break;
						
						case 'n': 
							switch (DCEvars[0])
							{
								
								case 5:  /* malfunction? No malfunction. */
									writeSpecial(((char) ESC) + "[0n");
									if (debug > 1)
										System.Console.Out.WriteLine("ESC[5n");
									break;
								
								case 6: 
									// DO NOT offset R and C by 1! (checked against /usr/X11R6/bin/resize
									// FIXME check again.
									// FIXME: but vttest thinks different???
									writeSpecial(((char) ESC) + "[" + R + ";" + C + "R");
									if (debug > 1)
										System.Console.Out.WriteLine("ESC[6n");
									break;
								
								default: 
									if (debug > 0)
										System.Console.Out.WriteLine("ESC [ " + DCEvars[0] + " n??");
									break;
								
							}
							break;
						
						case 's':  /* DECSC - save cursor */
							Sc = C;
							Sr = R;
							Sa = attributes;
							if (debug > 3)
								System.Console.Out.WriteLine("ESC[s");
							break;
						
						case 'u':  /* DECRC - restore cursor */
							C = Sc;
							R = Sr;
							attributes = Sa;
							if (debug > 3)
								System.Console.Out.WriteLine("ESC[u");
							break;
						
						case 'm':  /* attributes as color, bold , blink,*/
							if (debug > 3)
								System.Console.Out.Write("ESC [ ");
							if (DCEvar == 0 && DCEvars[0] == 0)
								attributes = 0;
							for (int i = 0; i <= DCEvar; i++)
							{
								switch (DCEvars[i])
								{
									
									case 0: 
										if (DCEvar > 0)
										{
											if (terminalID.Equals("scoansi"))
											{
												attributes &= COLOR; /* Keeps color. Strange but true. */
											}
											else
											{
												attributes = 0;
											}
										}
										break;
									
									case 1: 
										attributes |= BOLD;
										attributes &= ~ LOW;
										break;
									
									case 2: 
										/* SCO color hack mode */
										if (terminalID.Equals("scoansi") && ((DCEvar - i) >= 2))
										{
											int ncolor;
											attributes &= ~ (COLOR | BOLD);
											
											ncolor = DCEvars[i + 1];
											if ((ncolor & 8) == 8)
												attributes |= BOLD;
											ncolor = ((ncolor & 1) << 2) | (ncolor & 2) | ((ncolor & 4) >> 2);
											attributes |= ((ncolor) + 1) << COLOR_FG_SHIFT;
											ncolor = DCEvars[i + 2];
											ncolor = ((ncolor & 1) << 2) | (ncolor & 2) | ((ncolor & 4) >> 2);
											attributes |= ((ncolor) + 1) << COLOR_BG_SHIFT;
											i += 2;
										}
										else
										{
											attributes |= LOW;
										}
										break;
									
									case 4: 
										attributes |= UNDERLINE;
										break;
									
									case 7: 
										attributes |= INVERT;
										break;
									
									case 8: 
										attributes |= INVISIBLE;
										break;
									
									case 5:  /* blink on */
										break;
										/* 10 - ANSI X3.64-1979, select primary font, don't display control
										*      chars, don't set bit 8 on output */
									
									case 10: 
										gl = (char) (0);
										usedcharsets = true;
										break;
										/* 11 - ANSI X3.64-1979, select second alt. font, display control
										*      chars, set bit 8 on output */
									
									case 11: 
									/* SMACS , as */
									case 12: 
										gl = (char) (1);
										usedcharsets = true;
										break;
									
									case 21:  /* normal intensity */
										attributes &= ~ (LOW | BOLD);
										break;
									
									case 25:  /* blinking off */
										break;
									
									case 27: 
										attributes &= ~ INVERT;
										break;
									
									case 28: 
										attributes &= ~ INVISIBLE;
										break;
									
									case 24: 
										attributes &= ~ UNDERLINE;
										break;
									
									case 22: 
										attributes &= ~ BOLD;
										break;
									
									case 30: 
									case 31: 
									case 32: 
									case 33: 
									case 34: 
									case 35: 
									case 36: 
									case 37: 
										attributes &= ~ COLOR_FG;
										attributes |= ((DCEvars[i] - 30) + 1) << COLOR_FG_SHIFT;
										break;
									
									case 39: 
										attributes &= ~ COLOR_FG;
										break;
									
									case 40: 
									case 41: 
									case 42: 
									case 43: 
									case 44: 
									case 45: 
									case 46: 
									case 47: 
										attributes &= ~ COLOR_BG;
										attributes |= ((DCEvars[i] - 40) + 1) << COLOR_BG_SHIFT;
										break;
									
									case 49: 
										attributes &= ~ COLOR_BG;
										break;
									
									
									default: 
										System.Console.Out.WriteLine("ESC [ " + DCEvars[i] + " m unknown...");
										break;
									
								}
								if (debug > 3)
									System.Console.Out.Write("" + DCEvars[i] + ";");
							}
							if (debug > 3)
								System.Console.Out.Write(" (attributes = " + attributes + ")m \n");
							break;
						
						default: 
							System.Console.Out.WriteLine("ESC [ unknown letter:" + c + " (" + ((int) c) + ")");
							break;
						
					}
					break;
				
				default: 
					term_state = TSTATE_DATA;
					break;
				
			}
			if (C > columns)
				C = columns;
			if (R > rows)
				R = rows;
			if (C < 0)
				C = 0;
			if (R < 0)
				R = 0;
			if (doshowcursor)
				setCursorPosition(C, R);
			markLine(R, 1);
		}
		
		/* hard reset the terminal */
		public virtual void  reset()
		{
			gx[0] = 'B';
			gx[1] = '0';
			gx[2] = 'B';
			gx[3] = 'B';
			gl = (char) (0); // default GL to G0
			gr = (char) (1); // default GR to G1
			/* reset tabs */
			int nw = Columns;
			if (nw < 132)
				nw = 132;
			Tabs = new sbyte[nw];
			for (int i = 0; i < nw; i += 8)
			{
				Tabs[i] = 1;
			}
			/*FIXME:*/
			term_state = TSTATE_DATA;
		}
	}
}