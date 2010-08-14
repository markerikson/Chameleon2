#include "stdafx.h"

#include "ParserProjects/Parser/ctags_manager.h"
#include "ParserProjects/Parser/parse_thread.h"
#include "ParserProjects/Parser/fc_fileopener.h"

#include <vector>
#include <queue>

#include <wx/wx.h>
#include <wx/init.h>
#include <wx/string.h>
#include <wx/textfile.h>
#include <wx/msw/private.h>
#include "Utilities.h"

#pragma managed

#include <vcclr.h>
#include <msclr\marshal.h>

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

/*
//Debugger Utilities-
//.NET Equivalent of BreakPointInfo in the debugger.h file ("Interfaces" folder)

//what is a 'BPEntryPtr'??
List<BreakPointInfo^>^ BPVectorToBPList(vector<BPEntryPtr> &bps)
{
	List<BreakPointInfo^>^ returnBPs = gcnew List<BreakPointInfo^>();

	for(int i = 0; i < bps.size(); i++)
	{
		BPEntryPtr pBP = bps[i];
		BreakPointInfo^ bp = BPInfoPointerToBP(pBP);

		returnBPs->Add(bp);
	}

	return returnBPs;
}

BreakPointInfo^ BPInfoPointerToBP(BPEntryPtr& pBreakPoint)
{
	BreakPointInfo^ bp = gcnew BreakPointInfo();

	bp->file = ConvertString(pBreakPoint->GetFile());
	bp->lineno = pBreakPoint->GetLineNo();	//filetype int
	bp->watchpt_data = ConvertString(pBreakPoint->GetWatchPt());
	bp->function_name = ConvertString(pBreakPoint->GetFunctionName());
	bp->regex = pBreakPoint->GetRegX();		//filetype bool
	bp->memory_address = ConvertString(pBreakPoint->GetMemAddr());
	bp->internal_id = pBreakPoint->GetIntID();
	bp->debugger_id = pBreakPoint->GetDBID();
	bp->bp_type = pBreakPoint->GetBPType();	//needs a converter function? breakpointtype is a custom type
	bp->ignore_number = pBreakPoint->GetIgnoreNumber();
	bp->is_enabled = pBreakPoint->GetEnabled();
	bp->is_temp = pBreakPoint->GetTmp();
	bp->watchpoint_type = pBreakPoint->GetWPType(); //needs a converter function? watchpointtype is a custom type
	bp->commandlist = ConverString(pBreakPoint->GetCMDList());
	bp->conditions = ConvertString(pBreakPoint->GetConditions());
	bp->at = ConvertString(pBreakPoint->GetAt());
	bp->what = ConvertString(pBreakPoint->GetWhat());
	bp->origin = pBreakPoint->GetOrigin();	//needs a converter function? custom type

	return bp;
}

//-----------------
//end DebuggerWrapper items
*/
namespace CodeLite
{
	
	

	
};


