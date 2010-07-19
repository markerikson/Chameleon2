// AStyleInterface.cs

using System;
using System.Runtime.InteropServices;

/// AStyleInterface contains methods to call the Artistic Style formatter.
public class AStyleInterface
{   // Disable warnings for 'Missing XML comment for publicly visible type or member'
#pragma warning disable 1591

    // predefinedStyle valid predefined styles
    public const int STYLE_NONE       = 0;
    public const int STYLE_ALLMAN     = 1;
    public const int STYLE_JAVA       = 2;
    public const int STYLE_KandR      = 3;
    public const int STYLE_STROUSTRUP = 4;
    public const int STYLE_WHITESMITH = 5;
    public const int STYLE_BANNER     = 6;
    public const int STYLE_GNU        = 7;
    public const int STYLE_LINUX      = 8;
    public const int STYLE_HORSTMANN  = 9;
    public const int STYLE_1TBS       = 10;

    // bracketFormatMode valid bracket modes
    public const int BRACKETS_NONE       = 0;
    public const int BRACKETS_ATTACH     = 1;
    public const int BRACKETS_BREAK      = 2;
    public const int BRACKETS_LINUX      = 3;
    public const int BRACKETS_STROUSTRUP = 4;
    public const int BRACKETS_HORSTMANN  = 5;

    // indentType valid indent types
    public const int INDENT_SPACES = 0;
    public const int INDENT_TABS   = 1;
    public const int INDENT_FTABS  = 2;

    // alignPointers valid pointer alignments
    public const int ALIGN_NONE     = 0;
    public const int ALIGN_TYPE     = 1;
    public const int ALIGN_MIDDLE   = 2;
    public const int ALIGN_NAME     = 3;

    // fileMode valid file modes
    public const int FILEMODE_CPP   = 0;
    public const int FILEMODE_JAVA  = 1;
    public const int FILEMODE_SHARP = 2;

    // Restore warnings for 'Missing XML comment for publicly visible type or member'
#pragma warning restore 1591

    // option variable names are ALMOST the same as Artistic Style
    // the initial value is the default value in Artistic Style
    // comments are the command line option used to set the variable

    // predefined style option
    private int  predefinedStyle = STYLE_NONE;      // --style=?

    // tabs/spaces options
    private int  indentLength = 4;                  // --indent=?, --indent=force-tab=#
    private int  indentType   = INDENT_SPACES;      // --indent=?, --indent=force-tab=#

    // brackets options
    private int  bracketFormatMode = BRACKETS_NONE; // --brackets=?

    // indentation options
    private bool classIndent         = false;      // --indent-classes
    private bool switchIndent        = false;      // --indent-switches
    private bool caseIndent          = false;      // --indent-cases
    private bool bracketIndent       = false;      // --indent-brackets
    private bool blockIndent         = false;      // --indent-blocks
    private bool namespaceIndent     = false;      // --indent-namespaces
    private bool labelIndent         = false;      // --indent-labels
    private bool preprocessorIndent  = false;      // --indent-preprocessor
    private bool col1CommentIndent   = false;      // --indent-col1-comments
    private int maxInStatementIndent = 40;         // --max-instatement-indent=#
    private int minConditionalIndent = 8;          // --min-conditional-indent=#

    // padding options
    private bool breakHeaderBlocks   = false;      // --break-blocks, --break-blocks=all
    private bool breakClosingBlocks  = false;      // --break-blocks=all
    private bool padOperators        = false;      // --pad-oper
    private bool padParensOutside    = false;      // --pad-paren, --pad-paren-out
    private bool padParensInside     = false;      // --pad-paren, --pad-paren-in
    private bool padHeaders          = false;      // --pad-header
    private bool unpadParens         = false;      // --unpad-paren
    private bool deleteEmptyLines    = false;      // --delete-empty-lines
    private bool fillEmptyLines      = false;      // --fill-empty-lines

    // formatting options
    private bool breakCloseBrackets   = false;      // --break-closing-brackets
    private bool breakElseIfs         = false;      // --break-elseifs
    private bool addBrackets          = false;      // --add-brackets
    private bool addOneLineBrackets   = false;      // --add-one-line-brackets
    private bool breakOneLineStmts    = true;       // --keep-one-line-statements
    private bool breakOneLineBlocks   = true;       // --keep-one-line-blocks
    private bool convertTabs          = false;      // --convert-tabs
    private int alignPointers         = ALIGN_NONE; // align-pointer= none, type, middle, name

    // file mode option
    private int     fileMode = FILEMODE_CPP;        // --mode=?

    // default values for integer variables, saved by constructor
    private int    defaultIndentLength;             // default indentLength
    private int    defaultMaxInStatementIndent;     // default maxInStatementIndent
    private int    defaultMinConditionalIndent;     // default minConditionalIndent

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
        if (predefinedStyle != STYLE_NONE)
        {   if (predefinedStyle == STYLE_ALLMAN)
                options += "style=allman";
            else if (predefinedStyle == STYLE_JAVA)
                options += "style=java";
            else if (predefinedStyle == STYLE_KandR)
                options += "style=k&r";
            else if (predefinedStyle == STYLE_STROUSTRUP)
                options += "style=stroustrup";
            else if (predefinedStyle == STYLE_WHITESMITH)
                options += "style=whitesmith";
            else if (predefinedStyle == STYLE_BANNER)
                options += "style=banner";
            else if (predefinedStyle == STYLE_GNU)
                options += "style=gnu";
            else if (predefinedStyle == STYLE_LINUX)
                options += "style=linux";
            else if (predefinedStyle == STYLE_HORSTMANN)
                options += "style=horstmann";
            else if (predefinedStyle == STYLE_1TBS)
                options += "style=1tbs";
            else
                options += "predefinedStyle="      // force an error message
                           + predefinedStyle;
            options += separator;
        }
        // begin indent check
        if (indentType == INDENT_SPACES)           // space is the default
        {   if (!(indentLength == defaultIndentLength
                    || predefinedStyle == STYLE_GNU
                    || predefinedStyle == STYLE_LINUX))
            {   options += "indent=spaces=" + indentLength;
                options += separator;
            }
        }
        else if (indentType == INDENT_TABS)        // tab is not the default
        {   // check conditions to use default tab setting
            if (indentLength == defaultIndentLength
                    && predefinedStyle != STYLE_GNU
                    && predefinedStyle != STYLE_LINUX)
                options += "indent=tab";
            else
                options += "indent=tab=" + indentLength;
            options += separator;
        }
        else if (indentType == INDENT_FTABS)
        {   options += "indent=force-tab=" + indentLength;
            options += separator;
        }
        else
        {   options += "indentType="               // force an error message
                       + indentType;
            options += separator;
        }
        // end indent check
        if (bracketFormatMode != BRACKETS_NONE)
        {   if (bracketFormatMode == BRACKETS_ATTACH)
                options += "brackets=attach";
            else if (bracketFormatMode == BRACKETS_BREAK)
                options += "brackets=break";
            else if (bracketFormatMode == BRACKETS_LINUX)
                options += "brackets=linux";
            else if (bracketFormatMode == BRACKETS_STROUSTRUP)
                options += "brackets=stroustrup";
            else if (bracketFormatMode == BRACKETS_HORSTMANN)
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
        if (alignPointers == ALIGN_TYPE)
        {   options += "align-pointer=type";
            options += separator;
        }
        else if (alignPointers == ALIGN_MIDDLE)
        {   options += "align-pointer=middle";
            options += separator;
        }
        else if (alignPointers == ALIGN_NAME)
        {   options += "align-pointer=name";
            options += separator;
        }
        // end align pointers check
        // add the file mode, default is C++
        if (fileMode == FILEMODE_CPP)
        {   if (options.Length > 0)            // delete the last separator
                options.TrimEnd('\n');
        }
        else if (fileMode == FILEMODE_JAVA)
            options += "mode=java";
        else if (fileMode == FILEMODE_SHARP)
            options += "mode=cs";
        else
            options += "fileMode=" + fileMode; // force an error message

        return options;
    }

    /// Set the fileMode variable from the input file extension.
    public void SetFileMode(String fileName)
    {   fileMode = FILEMODE_CPP;
        if (fileName.EndsWith(".java"))
            fileMode = FILEMODE_JAVA;
        else if (fileName.EndsWith(".cs"))
            fileMode = FILEMODE_SHARP;
    }

    /// Set options for testing.
    /// For test program only.
    /// Calls the private setTestOptionsX().
    public void setTestOptions()
    {   setTestOptionsX();
    }

    ///  Set option variables for testing.
    /// For test program only.
    /// This will not be used by an actual program.
    private void setTestOptionsX()
    {   // predefined Style options
        // will have precedence over conflicting options
        predefinedStyle = STYLE_NONE;

        // tabs / spaces options
        indentLength = 3;
        indentType   = INDENT_TABS;

        // brackets option
        bracketFormatMode = BRACKETS_NONE;

        // fileMode option - FILEMODE_JAVA is required for Java files
        fileMode = FILEMODE_JAVA;

        // indentation options
        classIndent        = true;
        switchIndent       = true;
        caseIndent         = true;
        bracketIndent      = true;
        blockIndent        = true;
        namespaceIndent    = true;
        labelIndent        = true;
        preprocessorIndent = true;
        col1CommentIndent  = true;
        maxInStatementIndent = 50;
        minConditionalIndent = 10;

        // padding options
        breakHeaderBlocks  = true;
        breakClosingBlocks = true;
        padOperators       = true;
        padParensOutside   = true;
        padParensInside    = true;
        padHeaders         = true;
        unpadParens        = true;
        deleteEmptyLines   = true;
        fillEmptyLines     = true;

        // formatting options
        breakCloseBrackets = true;
        breakElseIfs       = true;
        addBrackets        = true;
        addOneLineBrackets = true;
        breakOneLineStmts  = false;
        breakOneLineBlocks = false;
        convertTabs        = true;
        alignPointers      = ALIGN_TYPE;

        // generate some errors
        /*  predefinedStyle   = 10;
        bracketFormatMode = 7;
        maxInStatementIndent = 90;
        minConditionalIndent = 50;
        // cannot have both invalid indentLength and invalid indentType
        //indentLength      = 21;
        indentType        = 6;  */
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
    public String FormatSource(String textIn, int fileModeArg)
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
