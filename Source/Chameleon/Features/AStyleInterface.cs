// AStyleInterface.cs

using System;
using System.Runtime.InteropServices;

namespace ArtisticStyle
{
	// predefinedStyle valid predefined styles
	public enum PredefinedStyle
	{
		NONE       = 0,
		ALLMAN     = 1,
		JAVA       = 2,
		KandR      = 3,
		STROUSTRUP = 4,
		WHITESMITH = 5,
		BANNER     = 6,
		GNU        = 7,
		LINUX      = 8,
		HORSTMANN  = 9,
		ONETBS       = 10,
	}

	// bracketFormatMode valid bracket modes
	public enum Brackets
	{
		NONE       = 0,
		ATTACH     = 1,
		BREAK      = 2,
		LINUX      = 3,
		STROUSTRUP = 4,
		HORSTMANN  = 5,
	}

	// indentType valid indent types
	public enum Indent
	{
		SPACES = 0,
		TABS   = 1,
		FTABS  = 2,
	}

	// alignPointers valid pointer alignments
	public enum Align
	{
		NONE     = 0,
		TYPE     = 1,
		MIDDLE   = 2,
		NAME     = 3,
	}

	// fileMode valid file modes
	public enum FormatFileMode
	{
		CPP   = 0,
		JAVA  = 1,
		SHARP = 2,
	}


	/// AStyleInterface contains methods to call the Artistic Style formatter.
	public class AStyleInterface
	{   // Disable warnings for 'Missing XML comment for publicly visible type or member'
	#pragma warning disable 1591

		// Restore warnings for 'Missing XML comment for publicly visible type or member'
	#pragma warning restore 1591

		// option variable names are ALMOST the same as Artistic Style
		// the initial value is the default value in Artistic Style
		// comments are the command line option used to set the variable

		// predefined style option
		public PredefinedStyle  predefinedStyle = PredefinedStyle.NONE;      // --style=?

		// tabs/spaces options
		public int  indentLength = 4;                  // --indent=?, --indent=force-tab=#
		public Indent  indentType   = Indent.SPACES;      // --indent=?, --indent=force-tab=#

		// brackets options
		public Brackets  bracketFormatMode = Brackets.NONE; // --brackets=?

		// indentation options
		public bool classIndent         = false;      // --indent-classes
		public bool switchIndent        = false;      // --indent-switches
		public bool caseIndent          = false;      // --indent-cases
		public bool bracketIndent       = false;      // --indent-brackets
		public bool blockIndent         = false;      // --indent-blocks
		public bool namespaceIndent     = false;      // --indent-namespaces
		public bool labelIndent         = false;      // --indent-labels
		public bool preprocessorIndent  = false;      // --indent-preprocessor
		public bool col1CommentIndent   = false;      // --indent-col1-comments
		public int maxInStatementIndent = 40;         // --max-instatement-indent=#
		public int minConditionalIndent = 8;          // --min-conditional-indent=#

		// padding options
		public bool breakHeaderBlocks   = false;      // --break-blocks, --break-blocks=all
		public bool breakClosingBlocks  = false;      // --break-blocks=all
		public bool padOperators        = false;      // --pad-oper
		public bool padParensOutside    = false;      // --pad-paren, --pad-paren-out
		public bool padParensInside     = false;      // --pad-paren, --pad-paren-in
		public bool padHeaders          = false;      // --pad-header
		public bool unpadParens         = false;      // --unpad-paren
		public bool deleteEmptyLines    = false;      // --delete-empty-lines
		public bool fillEmptyLines      = false;      // --fill-empty-lines

		// formatting options
		public bool breakCloseBrackets   = false;      // --break-closing-brackets
		public bool breakElseIfs         = false;      // --break-elseifs
		public bool addBrackets          = false;      // --add-brackets
		public bool addOneLineBrackets   = false;      // --add-one-line-brackets
		public bool breakOneLineStmts    = true;       // --keep-one-line-statements
		public bool breakOneLineBlocks   = true;       // --keep-one-line-blocks
		public bool convertTabs          = false;      // --convert-tabs
		public Align alignPointers         = Align.NONE; // align-pointer= none, type, middle, name

		// file mode option
		public FormatFileMode     fileMode = FormatFileMode.CPP;        // --mode=?

		// default values for integer variables, saved by constructor
		public int    defaultIndentLength;             // default indentLength
		public int    defaultMaxInStatementIndent;     // default maxInStatementIndent
		public int    defaultMinConditionalIndent;     // default minConditionalIndent

		/// Constructor
		public AStyleInterface()
		{   // Declare callback functions
			AStyleMemAlloc = new AStyleMemAllocDelgate(OnAStyleMemAlloc);
			AStyleError = new AStyleErrorDelgate(OnAStyleError);
			// save integer default values
			defaultIndentLength         = indentLength;
			defaultMaxInStatementIndent = maxInStatementIndent;
			defaultMinConditionalIndent = minConditionalIndent;
		}

		/// Display error messages for the class.
		/// This method should be changed to display messages in a manner consistent
		/// with the rest of the user program (e.g. a message box).
		private void DisplayErrorMessage(String errorMessage)
		{   Console.WriteLine(errorMessage);
		}

		/// Build a string of options from the current option variables.
		/// An option is generated only if the current value is different
		/// than the default value.
		/// If an option variable is invalid, invalid text is sent to
		/// Artistic Style so the private ErrorHandler method will be called.
		public String GetOptions()
		{   String options = null;             // options to Artistic Style
			String separator = "\n";           // can be new-line, tab, space, or comma

			// predefined style will override other options
			if (predefinedStyle != PredefinedStyle.NONE)
			{   if (predefinedStyle == PredefinedStyle.ALLMAN)
					options += "style=allman";
				else if (predefinedStyle == PredefinedStyle.JAVA)
					options += "style=java";
				else if (predefinedStyle == PredefinedStyle.KandR)
					options += "style=k&r";
				else if (predefinedStyle == PredefinedStyle.STROUSTRUP)
					options += "style=stroustrup";
				else if (predefinedStyle == PredefinedStyle.WHITESMITH)
					options += "style=whitesmith";
				else if (predefinedStyle == PredefinedStyle.BANNER)
					options += "style=banner";
				else if (predefinedStyle == PredefinedStyle.GNU)
					options += "style=gnu";
				else if (predefinedStyle == PredefinedStyle.LINUX)
					options += "style=linux";
				else if (predefinedStyle == PredefinedStyle.HORSTMANN)
					options += "style=horstmann";
				else if (predefinedStyle == PredefinedStyle.ONETBS)
					options += "style=1tbs";
				else
					options += "predefinedStyle="      // force an error message
							   + predefinedStyle;
				options += separator;
			}
			// begin indent check
			if (indentType == Indent.SPACES)           // space is the default
			{   if (!(indentLength == defaultIndentLength
						|| predefinedStyle == PredefinedStyle.GNU
						|| predefinedStyle == PredefinedStyle.LINUX))
				{   options += "indent=spaces=" + indentLength;
					options += separator;
				}
			}
			else if (indentType == Indent.TABS)        // tab is not the default
			{   // check conditions to use default tab setting
				if (indentLength == defaultIndentLength
						&& predefinedStyle != PredefinedStyle.GNU
						&& predefinedStyle != PredefinedStyle.LINUX)
					options += "indent=tab";
				else
					options += "indent=tab=" + indentLength;
				options += separator;
			}
			else if (indentType == Indent.FTABS)
			{   options += "indent=force-tab=" + indentLength;
				options += separator;
			}
			else
			{   options += "indentType="               // force an error message
						   + indentType;
				options += separator;
			}
			// end indent check
			if(bracketFormatMode != Brackets.NONE)
			{
				if(bracketFormatMode == Brackets.ATTACH)
					options += "brackets=attach";
				else if(bracketFormatMode == Brackets.BREAK)
					options += "brackets=break";
				else if(bracketFormatMode == Brackets.LINUX)
					options += "brackets=linux";
				else if(bracketFormatMode == Brackets.STROUSTRUP)
					options += "brackets=stroustrup";
				else if(bracketFormatMode == Brackets.HORSTMANN)
					options += "brackets=horstmann";
				else
					options += "bracketFormatMode="    // force an error message
							   + bracketFormatMode;
				options += separator;
			}
			if (classIndent)
			{   options += "indent-classes";
				options += separator;
			}
			if (switchIndent)
			{   options += "indent-switches";
				options += separator;
			}
			if (caseIndent)
			{   options += "indent-cases";
				options += separator;
			}
			if (bracketIndent)
			{   options += "indent-brackets";
				options += separator;
			}
			if (blockIndent)
			{   options += "indent-blocks";
				options += separator;
			}
			if (namespaceIndent)
			{   options += "indent-namespaces";
				options += separator;
			}
			if (labelIndent)
			{   options += "indent-labels";
				options += separator;
			}
			if (preprocessorIndent)
			{   options += "indent-preprocessor";
				options += separator;
			}
			if (col1CommentIndent)
			{   options += "indent-col1-comments";
				options += separator;
			}
			if (maxInStatementIndent != defaultMaxInStatementIndent)
			{   options += "max-instatement-indent="
						   + maxInStatementIndent;
				options += separator;
			}
			if (minConditionalIndent != defaultMinConditionalIndent)
			{   options += "min-conditional-indent="
						   + minConditionalIndent;
				options += separator;
			}
			// begin break-blocks check
			if (breakClosingBlocks)
			{   options += "break-blocks=all";
				options += separator;
			}
			else if (breakHeaderBlocks)
			{   options += "break-blocks";
				options += separator;
			}
			// end break-blocks check
			if (padOperators)
			{   options += "pad-oper";
				options += separator;
			}
			// begin pad parens check
			if (padParensOutside && padParensInside)
			{   options += "pad-paren";
				options += separator;
			}
			else if (padParensOutside)
			{   options += "pad-paren-out";
				options += separator;
			}
			else if (padParensInside)
			{   options += "pad-paren-in";
				options += separator;
			}
			// end pad parens check
			if (padHeaders)
			{   options += "pad-header";
				options += separator;
			}
			if (unpadParens)
			{   options += "unpad-paren";
				options += separator;
			}
			if (deleteEmptyLines)
			{   options += "delete-empty-lines";
				options += separator;
			}
			if (fillEmptyLines)
			{   options += "fill-empty-lines";
				options += separator;
			}
			if (breakCloseBrackets)
			{   options += "break-closing-brackets";
				options += separator;
			}
			if (breakElseIfs)
			{   options += "break-elseifs";
				options += separator;
			}
			if (addBrackets)
			{   options += "add-brackets";
				options += separator;
			}
			if (addOneLineBrackets)
			{   options += "add-one-line-brackets";
				options += separator;
			}
			if (! breakOneLineStmts)               // default = true
			{   options += "keep-one-line-statements ";
				options += separator;
			}
			if (! breakOneLineBlocks)              // default = true
			{   options += "keep-one-line-blocks";
				options += separator;
			}
			if (convertTabs)
			{   options += "convert-tabs";
				options += separator;
			}
			// begin align pointers check
			if (alignPointers == Align.TYPE)
			{   options += "align-pointer=type";
				options += separator;
			}
			else if (alignPointers == Align.MIDDLE)
			{   options += "align-pointer=middle";
				options += separator;
			}
			else if (alignPointers == Align.NAME)
			{   options += "align-pointer=name";
				options += separator;
			}
			// end align pointers check
			// add the file mode, default is C++
			if (fileMode == FormatFileMode.CPP)
			{   if (options.Length > 0)            // delete the last separator
					options.TrimEnd('\n');
			}
			else if (fileMode == FormatFileMode.JAVA)
				options += "mode=java";
			else if (fileMode == FormatFileMode.SHARP)
				options += "mode=cs";
			else
				options += "fileMode=" + fileMode; // force an error message

			return options;
		}

		/// Set the fileMode variable from the input file extension.
		public void SetFileMode(String fileName)
		{   fileMode = FormatFileMode.CPP;
			if (fileName.EndsWith(".java"))
				fileMode = FormatFileMode.JAVA;
			else if (fileName.EndsWith(".cs"))
				fileMode = FormatFileMode.SHARP;
		}

		public void SetDefaultChameleonStyleOptions()
		{
			predefinedStyle = PredefinedStyle.ALLMAN;

			// tabs / spaces options
			indentLength = 4;
			indentType = Indent.TABS;

			// brackets option
			bracketFormatMode = Brackets.NONE;

			// fileMode option - FILEMODE_JAVA is required for Java files
			fileMode = FormatFileMode.CPP;

			// indentation options
			classIndent = true;
			switchIndent = true;
			caseIndent = false;
			bracketIndent = false;
			blockIndent = false;
			namespaceIndent = true;
			labelIndent = false;
			preprocessorIndent = false;
			col1CommentIndent = false;
			maxInStatementIndent = 50;
			minConditionalIndent = 10;

			// padding options
			breakHeaderBlocks = false;
			breakClosingBlocks = false;
			padOperators = true;
			padParensOutside = false;
			padParensInside = false;
			padHeaders = false;
			unpadParens = true;
			deleteEmptyLines = false;
			fillEmptyLines = false;

			// formatting options
			breakCloseBrackets = true;
			breakElseIfs = false;
			addBrackets = true;
			addOneLineBrackets = false;
			breakOneLineStmts = true;
			breakOneLineBlocks = true;
			convertTabs = true;
			alignPointers = Align.TYPE;
		}


		// functions to call Artistic Style ---------------------------------------------------

		// Cannot use String as a return value because Mono runtime will attempt to
		// free the returned pointer resulting in a runtime crash.
		[DllImport("astyle", CallingConvention = CallingConvention.StdCall)]
		private static extern IntPtr AStyleGetVersion();

		// Cannot use String as a return value because Mono runtime will attempt to
		// free the returned pointer resulting in a runtime crash.
		[DllImport("astyle", CallingConvention = CallingConvention.StdCall)]
		private static extern IntPtr AStyleMain
		(
			[MarshalAs(UnmanagedType.LPStr)] String sIn,
			[MarshalAs(UnmanagedType.LPStr)] String sOptions,
			AStyleErrorDelgate errorFunc,
			AStyleMemAllocDelgate memAllocFunc
		);

		/// AStyleMain callbacks
		private delegate IntPtr AStyleMemAllocDelgate(int size);
		private delegate void AStyleErrorDelgate
		(
			int errorNum,
			[MarshalAs(UnmanagedType.LPStr)] String error
		);

		/// AStyleMain Delegates
		private AStyleMemAllocDelgate AStyleMemAlloc;
		private AStyleErrorDelgate AStyleError;

		/// Call the AStyleMain function in Artistic Style.
		/// An empty string is returned on error.
		public String FormatSource(String textIn)
		{   String options = GetOptions();
	//        DisplayErrorMessage("--------------------\n"
	//                           + options + "\n"
	//                           + "--------------------" );
			// Return the allocated string
			// Memory space is allocated by OnAStyleMemAlloc, a callback function from AStyle
			String sTextOut = String.Empty;
			try
			{   IntPtr pText = AStyleMain(textIn, options, AStyleError, AStyleMemAlloc);
				if (pText != IntPtr.Zero)
				{   sTextOut = Marshal.PtrToStringAnsi(pText);
					Marshal.FreeHGlobal(pText);
				}
			}
			catch (DllNotFoundException e)
			{   DisplayErrorMessage(e.ToString());
			}
			catch (EntryPointNotFoundException e)
			{   DisplayErrorMessage(e.ToString());
			}
			return sTextOut;
		}

		/// Set fileMode and call the AStyleMain function in Artistic Style.
		public String FormatSource(String textIn, String fileName)
		{   SetFileMode(fileName);
			String textOut = FormatSource(textIn);
			return textOut;
		}

		/// Set fileMode and call the AStyleMain function in Artistic Style.
		public String FormatSource(String textIn, FormatFileMode fileModeArg)
		{   // set file mode before formatting source
			fileMode = fileModeArg;
			String textOut = FormatSource(textIn);
			return textOut;
		}

		/// Get the Artistic Style version number.
		/// Does not need to terminate on error.
		/// But the exception must be handled when a function is called.
		public String GetVersion()
		{   String sVersion = String.Empty;
			try
			{   IntPtr  pVersion = AStyleGetVersion();
				if (pVersion != IntPtr.Zero)
				{   sVersion = Marshal.PtrToStringAnsi(pVersion);
				}
			}
			catch (DllNotFoundException e)
			{   DisplayErrorMessage(e.ToString());
				Console.WriteLine("The program has terminated!");
				Environment.Exit(1);
			}
			catch (EntryPointNotFoundException e)
			{   DisplayErrorMessage(e.ToString());
				Console.WriteLine("The program has terminated!");
				Environment.Exit(1);
			}
			return sVersion;
		}

		/// Allocate the memory for the Artistic Style return string.
		private IntPtr OnAStyleMemAlloc(int size)
		{   return Marshal.AllocHGlobal(size);
		}

		/// Display errors from Artistic Style .
		private void OnAStyleError(int errorNumber, String errorMessage)
		{   DisplayErrorMessage("astyle error " + errorNumber + " - " + errorMessage);
		}

	}   // class AStyleInterface

}