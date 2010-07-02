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

namespace CodeLite
{

	

	List<Tag^>^ ParserUtilities::TagVectorToTagList( vector<TagEntryPtr> &tags ) 
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

	Tag^ ParserUtilities::TagPointerToTag( TagEntryPtr& pTag ) 
	{
		Tag^ tag = gcnew Tag();			

		tag->path = marshal_as<String^>(pTag->GetPath().wc_str());
		tag->file = marshal_as<String^>(pTag->GetFile().wc_str());
		tag->pattern = marshal_as<String^>(pTag->GetPattern().wc_str());
		tag->kind = marshal_as<String^>(pTag->GetKind().wc_str());
		tag->parent = marshal_as<String^>(pTag->GetParent().wc_str());
		tag->scope = marshal_as<String^>(pTag->GetScopeName().wc_str());
		tag->name = marshal_as<String^>(pTag->GetName().wc_str());

		tag->lineNumber = pTag->GetLine();
		tag->id = pTag->GetId();
		tag->differOnlyByLineNumber = pTag->GetDifferOnByLineNumber();

		//array<String^>^ extFields = gcnew array<String^> {"access", "Colin"};

		wxString extFields[5] = {"access", "signature", "inherits", "typeref", "returns"};

		for(int i = 0; i < 5; i++)
		{
			wxString extKey = extFields[i];
			wxString extValue = pTag->GetExtField(extKey);

			String^ sKey = marshal_as<String^>(extKey.wc_str());
			String^ sValue = marshal_as<String^>(extValue.wc_str());

			tag->extFields->Add(sKey, sValue);
		}

		return tag;
	}
};


