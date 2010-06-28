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

#using <System.dll>
#using <mscorlib.dll>

using namespace System;
using namespace System::Collections::Generic;
using namespace msclr::interop;
using namespace System::Runtime::InteropServices;
using namespace System::Timers;
//using namespace stdcli::language;


using namespace std;

/*
void CodeLiteParserInit(const char* idxPath);
void CodeLiteParserEnd();
void AddParserRequestSingleFile(const char* filename, const char* databasePath) ;
*/


typedef void (__stdcall * ParsingCompleteCallback)(const char* text);

extern HINSTANCE g_hModule;



class ParserMessageReceiver : public wxEvtHandler
{
	friend class Singleton<ParserMessageReceiver>;
public:
	
	//bool waitingOnThread;
	//wxString filename;
	queue<wxString> filesToParse;
	ParsingCompleteCallback parsingCallback;

	void OnParsingThreadDone(wxCommandEvent& e)
	{
		//cout << "Parsing thread completed" << endl;
		wxString parsedFile = filesToParse.front();
		filesToParse.pop();

		if(parsingCallback != NULL)
		{
			parsingCallback(parsedFile);
		}
	}

protected:

	DECLARE_EVENT_TABLE()

};

BEGIN_EVENT_TABLE(ParserMessageReceiver, wxEvtHandler)
	EVT_COMMAND(wxID_ANY, wxEVT_PARSE_THREAD_UPDATED_FILE_SYMBOLS, ParserMessageReceiver::OnParsingThreadDone   )
END_EVENT_TABLE()

typedef Singleton<ParserMessageReceiver> ParserMessageReceiverST;


namespace CodeLite
{
[UnmanagedFunctionPointer(CallingConvention::Cdecl)]
public delegate void FileParsedDelegate(String^ filename);

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


public ref class VariableInfo
{
public:
	String^     name;
	bool        isTemplate;
	String^     templateDecl;
	bool        isPtr;
	String^     type;		//as in 'int a;' -> type=int
	String^     typeScope;//as in 'std::string a;' -> typeScope = std, type=string
	String^     pattern;
	String^     starAmp;
	int         lineno;
	bool        isConst;
	String^     rightSideConst;
	String^     defaultValue;	// used mainly for function arguments with default values foo(int = 0);
	String^     arrayBrackets;
	bool        isEllipsis;
	bool        isBasicType;

};

public ref class FunctionInfo
{
public:
	String^      name;
	String^       scope;					//functions' scope
	String^       returnValueConst;			// is the return value a const?
	String^       signature;
	VariableInfo^         returnValue;
	int              lineno;
	bool             isVirtual;
	bool             isPureVirtual;
	bool             isConst;

};


public ref class CtagsManagerWrapper
{
public:
	// Allocate the native object on the C++ Heap via a constructor
	CtagsManagerWrapper()  
	  //: m_tags( TagsManagerST::Get() ) 
	{
		fileParsedCallback = gcnew FileParsedDelegate(this, &CtagsManagerWrapper::OnFileParsed);

		m_filesToParse = gcnew List<String^>();

		m_appLoopTimer = gcnew System::Timers::Timer();
		m_appLoopTimer->Elapsed += gcnew ElapsedEventHandler(this, &CtagsManagerWrapper::OnTimedEvent);

		m_appLoopTimer->Interval = 50;
		m_appLoopTimer->Enabled = true;


		ParserMessageReceiver* pr = ParserMessageReceiverST::Get();
		pmr = pr;
		if(pmr != NULL)
		{
			
			IntPtr pCallback = Marshal::GetFunctionPointerForDelegate(fileParsedCallback);
			pmr->parsingCallback =  static_cast<ParsingCompleteCallback>(pCallback.ToPointer());
		}
	}

	// Deallocate the native object on a destructor
	~CtagsManagerWrapper() {
		//delete m_Impl;
	}

protected:
	// Deallocate the native object on the finalizer just in case no destructor is called
	!CtagsManagerWrapper() {
		//delete m_Impl;
	}

private:
	clProcess* indexerProcess;
	ParserMessageReceiver* pmr;
	TagsManager * m_tags;
	FileParsedDelegate^ fileParsedCallback;
	List<String^>^ m_filesToParse;
	String^ databasePath;
	System::Timers::Timer^ m_appLoopTimer;

	void OnTimedEvent(Object^ source, ElapsedEventArgs^ e)
	{
		//Console.WriteLine("The Elapsed event was raised at {0}", e.SignalTime);
		//ProcessParserEvents();

		CtagsManagerWrapper::ProcessParserEvents();
	}

public:
	bool CodeLiteParserInit(String^ idxPath, String^ databasePath) 
	{		
		wxSetInstance(GetModuleHandle(NULL));
		int argc = 0;
		char **argv = NULL;
		wxEntryStart(argc, argv);
		if ( !wxTheApp || !wxTheApp->CallOnInit() )
			return FALSE;

		marshal_context context;
		wxString indexerPath = context.marshal_as<const wchar_t*>(idxPath);

		ParseThread* parser = ParseThreadST::Get();
		TagsManager* tagmgr = TagsManagerST::Get();

		m_tags = tagmgr;

		LanguageST::Get()->SetTagsManager(tagmgr );
		TagsManagerST::Get()->SetLanguage( LanguageST::Get() );

		tagmgr->SetCodeLiteIndexerPath(indexerPath);
		indexerProcess = tagmgr->StartCtagsProcess();

		
		//pmr = pr;
		parser->SetNotifyWindow(pmr);

		parser->Start();

		this->databasePath = databasePath;
		wxString dbPath = context.marshal_as<const wchar_t*>(databasePath);
		tagmgr->OpenDatabase(dbPath);

		return true;
	}

	void CodeLiteParserEnd() 
	{
		TagsManagerST::Get()->CloseDatabase();
		ParseThread* parser = ParseThreadST::Get();
		parser->Stop();

		ParseThreadST::Free();

		TagsManagerST::Free();
		LanguageST::Free();

		delete indexerProcess;

		fcFileOpener::Release();
		ProcessParserEvents();

		wxEntryCleanup();
	}

	void AddParserRequestSingleFile(String^ filename) 
	{
		ParseRequest *parsingRequest = new ParseRequest();

		m_filesToParse->Add(filename);

		marshal_context context;
		wxString fname = context.marshal_as<const wchar_t*>(filename);
		wxString dbpath = context.marshal_as<const wchar_t*>(databasePath);

		parsingRequest->setFile(fname);
		parsingRequest->setDbFile(dbpath);

		parsingRequest->setType(ParseRequest::PR_FILESAVED);
		parsingRequest->_quickRetag = false;

		pmr->filesToParse.push(fname);

		ParseThread* parser = ParseThreadST::Get();
		parser->Add(parsingRequest);	
	}

	property bool Parsing
	{
		bool get()
		{			
			return m_filesToParse->Count > 0;
		}
	}


	List<String^>^ GetScopesFromFile(String^ fileName)
	{
		marshal_context context;
		wxString name = context.marshal_as<const wchar_t*>(fileName);

		vector<wxString> fileScopes;

		m_tags->GetScopesFromFile(name, fileScopes);

		List<String^>^ returnScopes = gcnew List<String^>();

		for(int i = 0; i < fileScopes.size(); i++)
		{
			
			String^ scope = marshal_as<String^>(fileScopes[i].wc_str());
			returnScopes->Add(scope);
		}

		return returnScopes;
	}

	List<Tag^>^ FindSymbol(String^ symbolName)
	{
		marshal_context context;
		wxString name = context.marshal_as<const wchar_t*>(symbolName);

		vector<TagEntryPtr> tags;

		m_tags->FindSymbol(name, tags);

		return TagVectorToTagList(tags);
	}

	List<Tag^>^ FindByPath(String^ path)
	{
		marshal_context context;
		wxString filePath = context.marshal_as<const wchar_t*>(path);

		vector<TagEntryPtr> tags;

		m_tags->FindByPath(filePath, tags);

		return TagVectorToTagList(tags);
	}

	List<Tag^>^ TagsByScope(String^ scope)
	{
		marshal_context context;
		wxString sScope = context.marshal_as<const wchar_t*>(scope);

		vector<TagEntryPtr> tags;

		m_tags->TagsByScope(sScope, tags);

		return TagVectorToTagList(tags);
	}

	List<Tag^>^ TagsByScope(String^ scope, String^ kind, bool includeInherits, bool onlyWorkspace)
	{
		marshal_context context;
		wxString sScope = context.marshal_as<const wchar_t*>(scope);
		wxString sKind = context.marshal_as<const wchar_t*>(kind);

		vector<TagEntryPtr> tags;

		m_tags->TagsByScope(sScope, sKind, tags, includeInherits, onlyWorkspace);

		return TagVectorToTagList(tags);
	}



	List<Tag^>^ TagsByScopeAndName(String^ scope, String^ name)
	{
		marshal_context context;
		wxString sScope = context.marshal_as<const wchar_t*>(scope);
		wxString sName = context.marshal_as<const wchar_t*>(name);

		vector<TagEntryPtr> tags;

		m_tags->TagsByScopeAndName(sScope, sName, tags);

		return TagVectorToTagList(tags);
	}

	List<Tag^>^ AutoCompletionCandidates(String^ filename, int linenum, String^ expr, String^ text)
	{
		marshal_context context;
		wxFileName fname = context.marshal_as<const wchar_t*>(filename);
		wxString sExpr = context.marshal_as<const wchar_t*>(expr);
		wxString sText = context.marshal_as<const wchar_t*>(text);

		vector<TagEntryPtr> tags;

		m_tags->AutoCompleteCandidates(fname, linenum, sExpr, sText, tags);

		return TagVectorToTagList(tags);
	}

	List<Tag^>^ WordCompletionCandidates(String^ filename, int linenum, String^ expr, 
										String^ text, String^ word)
	{
		marshal_context context;
		wxFileName fname = context.marshal_as<const wchar_t*>(filename);
		wxString sExpr = context.marshal_as<const wchar_t*>(expr);
		wxString sText = context.marshal_as<const wchar_t*>(text);
		wxString sWord = context.marshal_as<const wchar_t*>(word);

		vector<TagEntryPtr> tags;

		m_tags->WordCompletionCandidates(fname, linenum, sExpr, sText, sWord, tags);

		return TagVectorToTagList(tags);
	}

	void DeleteFilesTags(List<String^>^ files)
	{
		marshal_context context;
		vector<wxFileName> fnames;

		for(int i = 0; i < files->Count; i++)
		{
			wxFileName fname = context.marshal_as<const wchar_t*>(files[i]);
			fnames.push_back(fname);
		}

	}

	void DeleteTagsByFilePrefix(String^ dbPath, String^ filePrefix)
	{
		marshal_context context;

		wxString sDbPath = context.marshal_as<const wchar_t*>(dbPath);
		wxString sPrefix = context.marshal_as<const wchar_t*>(filePrefix);

		m_tags->DeleteTagsByFilePrefix(sDbPath, sPrefix);
	}

	void RetagFiles(List<String^>^ files, bool quickRetag)
	{
		marshal_context context;
		vector<wxFileName> fnames;

		for(int i = 0; i < files->Count; i++)
		{
			wxFileName fname = context.marshal_as<const wchar_t*>(files[i]);
			fnames.push_back(fname);
		}

		m_tags->RetagFiles(fnames, quickRetag);

	}

	List<String^>^ GetHoverTip(String^ filename, int linenum, String^ expr, 
		String^ word, String^ text)
	{
		marshal_context context;
		wxFileName fname = context.marshal_as<const wchar_t*>(filename);
		wxString sExpr = context.marshal_as<const wchar_t*>(expr);
		wxString sText = context.marshal_as<const wchar_t*>(text);
		wxString sWord = context.marshal_as<const wchar_t*>(word);

		vector<wxString> tips;

		m_tags->GetHoverTip(fname, linenum, sExpr, sWord, sText, tips);

		List<String^>^ list = gcnew List<String^>();

		for(int i = 0; i < tips.size(); i++)
		{
			String^ tip = marshal_as<String^>(tips[i].wc_str());
			list->Add(tip);
		}

		return list;
	}


	List<Tag^>^ OpenType()
	{
		vector<TagEntryPtr> tags;
		m_tags->OpenType(tags);

		return TagVectorToTagList(tags);
	}


	List<Tag^>^ FindImplDecl(String^ filename, int linenum, String^ expr, String^ word, String^ text, 
							bool impl, bool workspaceOnly)
	{
		marshal_context context;
		wxString sFilename = context.marshal_as<const wchar_t*>(filename);
		wxString sExpr = context.marshal_as<const wchar_t*>(expr);
		wxString sWord = context.marshal_as<const wchar_t*>(word);
		wxString sText = context.marshal_as<const wchar_t*>(text);

		vector<TagEntryPtr> tags;

		m_tags->FindImplDecl(sFilename, linenum, sExpr, sWord, sText, tags, impl, workspaceOnly);

		return TagVectorToTagList(tags);
	}

	String^ GetScopeName(String^ scope)
	{
		marshal_context context;
		wxString sScope = context.marshal_as<const wchar_t*>(scope);

		wxString result = m_tags->GetScopeName(sScope);

		String^ returnScope = marshal_as<String^>(result.wc_str());
		return returnScope;
	}

	List<String^>^ GetFiles(String^ partialName)
	{
		marshal_context context;
		wxString sName = context.marshal_as<const wchar_t*>(partialName);

		vector<wxFileName> files;

		m_tags->GetFiles(sName, files);

		List<String^>^ list = gcnew List<String^>();

		for(int i = 0; i < files.size(); i++)
		{
			wxFileName fname = files[i];
			String^ filename = marshal_as<String^>(fname.GetFullPath().wc_str());

			list->Add(filename);
		}

		return list;
	}


	Tag^ FunctionFromFileLine(String^ filename, int linenum, bool nextFunction)
	{
		marshal_context context;
		wxString sFilename = context.marshal_as<const wchar_t*>(filename);

		TagEntryPtr pTag = m_tags->FunctionFromFileLine(sFilename, linenum, nextFunction);

		Tag^ tag = TagPointerToTag(pTag);
		return tag;
	}

	Tag^ FirstFunctionOfFile(String^ filename)
	{
		marshal_context context;
		wxString sFilename = context.marshal_as<const wchar_t*>(filename);

		TagEntryPtr pTag = m_tags->FirstFunctionOfFile(sFilename);

		Tag^ tag = TagPointerToTag(pTag);
		return tag;
	}

	Tag^ FirstScopeOfFile(String^ filename)
	{
		marshal_context context;
		wxString sFilename = context.marshal_as<const wchar_t*>(filename);

		TagEntryPtr pTag = m_tags->FirstScopeOfFile(sFilename);

		Tag^ tag = TagPointerToTag(pTag);
		return tag;
	}

	
	bool GetMemberType(String^ scope, String^ name, [Out]String^% type, [Out]String^% typeScope)
	{
		marshal_context context;
		wxString sScope = context.marshal_as<const wchar_t*>(scope);
		wxString sName = context.marshal_as<const wchar_t*>(name);

		wxString sType, sTypeScope;

		if(m_tags->GetMemberType(sScope, sName, sType, sTypeScope))
		{
			type = marshal_as<String^>(sType.wc_str());
			typeScope = marshal_as<String^>(sTypeScope.wc_str());

			return true;
		}

		return false;
	}

	List<Tag^>^ TagsFromFileAndScope(String^ filename, String^ scopeName)
	{
		marshal_context context;
		wxString sFilename = context.marshal_as<const wchar_t*>(filename);
		wxString sScopeName = context.marshal_as<const wchar_t*>(scopeName);

		vector<TagEntryPtr> tags;
		m_tags->TagsFromFileAndScope(sFilename, sScopeName, tags);

		return TagVectorToTagList(tags);
	}


	bool GetFunctionDetails(String^ filename, int linenum, [Out]Tag^% tag, [Out]FunctionInfo^% func)
	{
		marshal_context context;
		wxString sFilename = context.marshal_as<const wchar_t*>(filename);

		TagEntryPtr pTag;
		clFunction clfunc;

		if(m_tags->GetFunctionDetails(sFilename, linenum, pTag, clfunc))
		{
			tag = TagPointerToTag(pTag);

			func = gcnew FunctionInfo();
			func->name = marshal_as<String^>(clfunc.m_name.c_str());
			func->scope = marshal_as<String^>(clfunc.m_scope.c_str());
			func->returnValueConst = marshal_as<String^>(clfunc.m_retrunValusConst.c_str());
			func->signature = marshal_as<String^>(clfunc.m_signature.c_str());
			func->lineno = clfunc.m_lineno;
			func->isVirtual = clfunc.m_isVirtual;
			func->isPureVirtual = clfunc.m_isPureVirtual;
			func->isConst = clfunc.m_isConst;

			VariableInfo^ vi = gcnew VariableInfo();
			Variable v = clfunc.m_returnValue;
			
			vi->name = marshal_as<String^>(v.m_name.c_str());
			vi->templateDecl = marshal_as<String^>(v.m_templateDecl.c_str());
			vi->type = marshal_as<String^>(v.m_type.c_str());
			vi->typeScope = marshal_as<String^>(v.m_typeScope.c_str());
			vi->pattern = marshal_as<String^>(v.m_pattern.c_str());
			vi->starAmp = marshal_as<String^>(v.m_starAmp.c_str());
			vi->rightSideConst = marshal_as<String^>(v.m_rightSideConst.c_str());
			vi->defaultValue = marshal_as<String^>(v.m_defaultValue.c_str());
			vi->arrayBrackets = marshal_as<String^>(v.m_arrayBrackets.c_str());

			vi->isTemplate = v.m_isTemplate;
			vi->isPtr = v.m_isPtr;
			vi->lineno = v.m_lineno;
			vi->isConst = v.m_isConst;
			vi->isEllipsis = v.m_isEllipsis;
			vi->isBasicType = v.m_isBasicType;

			func->returnValue = vi;

			return true;
		}

		 return false;
	}

	List<Tag^>^ GetClasses(bool onlyWorkspace)
	{
		vector<TagEntryPtr> tags;

		m_tags->GetClasses(tags, onlyWorkspace);

		return TagVectorToTagList(tags);
	}

	List<Tag^>^ GetFunctions(String^ filename, bool onlyWorkspace)
	{
		marshal_context context;
		wxString sFilename = context.marshal_as<const wchar_t*>(filename);

		vector<TagEntryPtr> tags;

		m_tags->GetFunctions(tags, sFilename, onlyWorkspace);

		return TagVectorToTagList(tags);
	}

	List<Tag^>^ GetTagsByKind(List<String^>^ kinds)
	{
		marshal_context context;
		wxArrayString sKind;
		vector<TagEntryPtr> tags;

		for(int i = 0; i < kinds->Count; i++)
		{
			wxString temp = context.marshal_as<const wchar_t*>(kinds[i]);
			sKind.Add(temp);
		}

		m_tags->GetTagsByKind(tags, sKind);

		return TagVectorToTagList(tags);
	}


	bool ProcessExpression(String^ expr, String^% type, String^% typeScope)
	{
		marshal_context context;
		wxString sExpr = context.marshal_as<const wchar_t*>(expr);

		wxString sType, sTypeScope;

		if(m_tags->ProcessExpression(sExpr, sType, sTypeScope))
		{
			type = marshal_as<String^>(sType.wc_str());
			typeScope = marshal_as<String^>(sTypeScope.wc_str());

			return true;
		}

		return false;
	}

	
	static void  ProcessParserEvents() 
	{
		if(wxTheApp->HasPendingEvents())
		{
			wxTheApp->ProcessPendingEvents();
		}

	}

	void OnFileParsed(String^ filename)
	{
		Console::WriteLine("Parsing completed for file: " + filename);


		m_filesToParse->Remove(filename);
	}

private:
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
		Tag^ tag = gcnew Tag();			

		tag->path = marshal_as<String^>(pTag->GetPath().wc_str());
		tag->file = marshal_as<String^>(pTag->GetFile().wc_str());
		tag->pattern = marshal_as<String^>(pTag->GetPattern().wc_str());
		tag->kind = marshal_as<String^>(pTag->GetKind().wc_str());
		tag->parent = marshal_as<String^>(pTag->GetParent().wc_str());
		tag->scope = marshal_as<String^>(pTag->GetScopeName().wc_str());

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

}