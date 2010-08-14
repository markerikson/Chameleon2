#ifndef PARSER_UTILITIES_H
#define PARSER_UTILITIES_H

#include "ParserProjects/Parser/ctags_manager.h"
#include "ParserProjects/Parser/parse_thread.h"
#include "ParserProjects/Parser/fc_fileopener.h"
#include <vcclr.h>
#include <msclr\marshal.h>
#include <vector>
#include <queue>

#include <wx/wx.h>
#include <wx/init.h>
#include <wx/string.h>
#include <wx/textfile.h>
#include <wx/msw/private.h>
#include "Utilities.h"

#using <System.dll>
#using <mscorlib.dll>

using namespace System;
using namespace System::Collections::Generic;
using namespace msclr::interop;
using namespace System::Runtime::InteropServices;

using namespace std;



String^ ConvertString(const wxString& nativeString);
String^ ConvertString(const string& nativeString);
wxString ConvertString(String^ managedString);



namespace CodeLite
{
	public ref class Tag
	{
	public:
		Tag()
		{
			extFields = gcnew Dictionary<String^, String^>();
		}
		String^                     path;		///< Tag full path
		String^                     file;		///< File this tag is found
		int                         lineNumber;	///< Line number
		String^                     pattern;		///< A pattern that can be used to locate the tag in the file
		String^                     kind;		///< Member, function, class, typedef etc.
		String^                     parent;		///< Direct parent
		String^                     name;		///< Tag name (short name, excluding any scope names)
		Dictionary<String^, String^>^  extFields; ///< Additional extension fields
		int                         id;
		String^                     scope;
		bool                        differOnlyByLineNumber;

	};
	/*
	//for the DebuggerWrapper
	//entered by Benjamin Carhart  8/4/2010
	enum WatchpointType {
		WP_watch,
		WP_rwatch,
		WP_awatch
	};

	// Can be from the Editor (user clicked 'F9')
	// or from any other source (direct command to gdb, from the start up command etc)
	enum BreakpointOrigin {
		BO_Editor,
		BO_Other
	};
	
	public ref class BreakPointInfo
	{
	public:
		BreakPointInfo() {}
		// Where the bp is: file/lineno, function name (e.g. main()) or the memory location
		String^                file;
		int                    lineno;
		String^                watchpt_data;
		String^                function_name;
		bool                   regex;            // Is the function_name a regex?
		String^                memory_address;
		// How to identify the bp. Because the debugger won't always be running, we need an internal id as well as the debugger's one
		int                    internal_id;
		int                    debugger_id;	// -1 signifies not set
		BreakpointType         bp_type;  // Is it a plain vanilla breakpoint, or a temporary one, or a watchpoint, or...
		unsigned int           ignore_number; // 0 means 'not ignored'. >0 is the number of times the bp must be hit before it becomes enabled
		bool                   is_enabled;
		bool                   is_temp;
		WatchpointType         watchpoint_type;	// If this is a watchpoint, holds which sort it is
		String^                commandlist;
		String^                conditions;
		String^                at;
		String^                what;
		BreakPointOrigin	   origin;

		bool IsConditional() {
			return ! conditions.IsEmpty();
		}

		void Create(String^ filename, int line, int int_id, int ext_id = -1)
		{
			//CodeLite has 'normalization' code for filename... not sure what .NET equiv is
			file = filename;
			bp_type = BP_type_break;
			lineno = line;
			internal_id = int_id;
			debugger_id = ext_id;
		}

		BreakpointInfo& operator=(const BreakpointInfo& BI) {
			file             = BI.file;
			lineno           = BI.lineno;
			watchpt_data     = BI.watchpt_data;
			function_name    = BI.function_name;
			regex            = BI.regex;
			memory_address   = BI.memory_address;
			internal_id      = BI.internal_id;
			debugger_id      = BI.debugger_id;
			bp_type          = BI.bp_type;
			ignore_number    = BI.ignore_number;
			is_enabled       = BI.is_enabled;
			is_temp          = BI.is_temp;
			watchpoint_type  = BI.watchpoint_type;
			commandlist      = BI.commandlist;
			conditions       = BI.conditions;
			at               = BI.at;                 // Provided by the debugger, no need to serialize
			what             = BI.what;               // Provided by the debugger, no need to serialize
			origin           = BI.origin;
			return *this;
		}
	};
	//---->END DebuggerWrapper items
	*/
	/*
	public ref class ParserUtilities
	{
	public:
		static List<Tag^>^ TagVectorToTagList( vector<TagEntryPtr> &tags );
		static Tag^ TagPointerToTag( TagEntryPtr& pTag ); 
		
	};
	*/
}

using namespace CodeLite;

List<Tag^>^ TagVectorToTagList( vector<TagEntryPtr> &tags );
Tag^ TagPointerToTag( TagEntryPtr& pTag ); 

#endif