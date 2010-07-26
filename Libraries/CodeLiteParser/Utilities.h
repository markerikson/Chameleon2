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