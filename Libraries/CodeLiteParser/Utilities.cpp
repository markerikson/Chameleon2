#include "stdafx.h"

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
using namespace CodeLite;

String^ ConvertString(const wxString& nativeString)
{
	String^ managedString = marshal_as<String^>(nativeString.wc_str());
	return managedString;
}

String^ ConvertString(const string& nativeString)
{
	String^ managedString = marshal_as<String^>(nativeString.c_str());
	return managedString;
}



wxString ConvertString(String^ managedString)
{
	marshal_context context;
	wxString nativeString = context.marshal_as<const wchar_t*>(managedString);
	return nativeString;
}

List<Tag^>^ TagVectorToTagList( vector<TagEntryPtr> &tags ) 
{
	List<Tag^>^ returnTags = gcnew List<Tag^>();

	for(int i = 0; i < tags.size(); i++)
	{
		TagEntryPtr pTag = tags[i];
		Tag^ tag = TagPointerToTag(pTag);

		returnTags->Add(tag);
	}

	return returnTags;
}

Tag^ TagPointerToTag( TagEntryPtr& pTag ) 
{
	if(pTag == NULL)
	{
		return nullptr;
	}

	Tag^ tag = gcnew Tag();			

	tag->path = ConvertString(pTag->GetPath());
	tag->file = ConvertString(pTag->GetFile());
	tag->pattern = ConvertString(pTag->GetPattern());
	tag->kind = ConvertString(pTag->GetKind());
	tag->parent = ConvertString(pTag->GetParent());
	tag->scope = ConvertString(pTag->GetScopeName());
	tag->name = ConvertString(pTag->GetName());

	tag->lineNumber = pTag->GetLine();
	tag->id = pTag->GetId();
	tag->differOnlyByLineNumber = pTag->GetDifferOnByLineNumber();

	//array<String^>^ extFields = gcnew array<String^> {"access", "Colin"};

	wxString extFields[5] = {"access", "signature", "inherits", "typeref", "returns"};

	for(int i = 0; i < 5; i++)
	{
		wxString extKey = extFields[i];
		wxString extValue = pTag->GetExtField(extKey);

		String^ sKey = ConvertString(extKey);
		String^ sValue = ConvertString(extValue);

		tag->extFields->Add(sKey, sValue);
	}

	return tag;
}

namespace CodeLite
{
	
	

	
};


