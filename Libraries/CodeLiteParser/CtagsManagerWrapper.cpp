#include "ParserProjects/Parser/ctags_manager.h"
#include "ParserProjects/Parser/parse_thread.h"
#include "ParserProjects/Parser/fc_fileopener.h"

#include <vector>


#include <wx/wx.h>
#include <wx/init.h>
#include <wx/string.h>
#include <wx/textfile.h>
#include <wx/msw/private.h>

#include <vcclr.h>
#include <msclr\marshal.h>
#include "Utilities.h"
#include "ParserMessageReceiver.h"

#using <System.dll>
#using <mscorlib.dll>

using namespace System;
using namespace System::Collections::Generic;
using namespace msclr::interop;
using namespace System::Runtime::InteropServices;
using namespace System::Timers;
using namespace System::Threading;


using namespace std;


extern HINSTANCE g_hModule;

namespace CodeLite
{
[UnmanagedFunctionPointer(CallingConvention::Cdecl)]
public delegate void FileParsedDelegate(String^ filename);



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

private ref class ShutdownIndexerThread
{
public:
	ShutdownIndexerThread(clProcess* indexerProcess)
	{
		 m_process = indexerProcess;
	}

	clProcess* m_process;

	void ThreadEntryPoint()
	{
		Console::WriteLine("Closing indexer process");
		m_process->Terminate();
	}
};



public ref class CtagsManagerWrapper
{
private:
	CtagsManagerWrapper()  
	{
		
		fileParsedCallback = gcnew FileParsedDelegate(this, &CtagsManagerWrapper::OnFileParsed);
		fileParsedEvent = nullptr;

		m_initialized = false;

		m_filesToParse = gcnew List<String^>();

		m_appLoopTimer = gcnew System::Timers::Timer();
		m_appLoopTimer->Elapsed += gcnew ElapsedEventHandler(this, &CtagsManagerWrapper::OnTimedEvent);

		m_appLoopTimer->Interval = 50;

		ParserMessageReceiver* pr = ParserMessageReceiverST::Get();
		pmr = pr;
		if(pmr != NULL)
		{			
			IntPtr pCallback = Marshal::GetFunctionPointerForDelegate(fileParsedCallback);
			pmr->parsingCallback =  static_cast<ParsingCompleteCallback>(pCallback.ToPointer());
		}		
	}

	~CtagsManagerWrapper() 	{ }

public:
	event FileParsedDelegate^ FileParsed
	{
		void add(FileParsedDelegate^ fp)
		{
			fileParsedEvent += fp;
		}

		void remove(FileParsedDelegate^ fp)
		{
			fileParsedEvent -= fp;
		}

		void raise(String^ file)
		{
			FileParsedDelegate^ tmp = fileParsedEvent;

			if(tmp)
			{
				tmp->Invoke(file);
			}
		}
	}

protected:
	!CtagsManagerWrapper() {}
	

private:
	clProcess* indexerProcess;
	ParserMessageReceiver* pmr;
	TagsManager * m_tags;
	FileParsedDelegate^ fileParsedCallback;
	FileParsedDelegate^ fileParsedEvent;
	List<String^>^ m_filesToParse;
	String^ databasePath;
	System::Timers::Timer^ m_appLoopTimer;
	bool m_initialized;
	CtagsManagerWrapper^ m_wrapper;

	void OnTimedEvent(Object^ source, ElapsedEventArgs^ e)
	{
		ProcessParserEvents();
	}

public:
	bool CodeLiteParserInit(String^ idxPath, String^ databasePath) 
	{		
		if(m_initialized)
		{
			return false;
		}

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

		CtagsManagerWrapper::databasePath = databasePath;
		wxString dbPath = context.marshal_as<const wchar_t*>(databasePath);
		tagmgr->OpenDatabase(dbPath);
		
		m_appLoopTimer->Enabled = true;
		m_initialized = true;

		return true;
	}

	void CodeLiteParserEnd() 
	{
		if(!m_initialized)
		{
			return;
		}
		
		ParseThread* parser = ParseThreadST::Get();
		parser->Stop();
		
		TagsManagerST::Get()->CloseDatabase();
		
		ParseThreadST::Free();
		TagsManagerST::Free();
		LanguageST::Free();

		ShutdownIndexerThread^ o1 = gcnew ShutdownIndexerThread(indexerProcess);
		Thread^ t1 = gcnew Thread(gcnew ThreadStart(o1, &ShutdownIndexerThread::ThreadEntryPoint));
		
		fcFileOpener::Release();
				
		m_initialized = false;
		m_appLoopTimer->Enabled = false;

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

		return ParserUtilities::TagVectorToTagList(tags);
	}

	List<Tag^>^ FindByPath(String^ path)
	{
		marshal_context context;
		wxString filePath = context.marshal_as<const wchar_t*>(path);

		vector<TagEntryPtr> tags;

		m_tags->FindByPath(filePath, tags);

		return ParserUtilities::TagVectorToTagList(tags);
	}

	List<Tag^>^ TagsByScope(String^ scope)
	{
		marshal_context context;
		wxString sScope = context.marshal_as<const wchar_t*>(scope);

		vector<TagEntryPtr> tags;

		m_tags->TagsByScope(sScope, tags);

		return ParserUtilities::TagVectorToTagList(tags);
	}

	List<Tag^>^ TagsByScope(String^ scope, String^ kind, bool includeInherits, bool onlyWorkspace)
	{
		marshal_context context;
		wxString sScope = context.marshal_as<const wchar_t*>(scope);
		wxString sKind = context.marshal_as<const wchar_t*>(kind);

		vector<TagEntryPtr> tags;

		m_tags->TagsByScope(sScope, sKind, tags, includeInherits, onlyWorkspace);

		return ParserUtilities::TagVectorToTagList(tags);
	}



	List<Tag^>^ TagsByScopeAndName(String^ scope, String^ name)
	{
		marshal_context context;
		wxString sScope = context.marshal_as<const wchar_t*>(scope);
		wxString sName = context.marshal_as<const wchar_t*>(name);

		vector<TagEntryPtr> tags;

		m_tags->TagsByScopeAndName(sScope, sName, tags);

		return ParserUtilities::TagVectorToTagList(tags);
	}

	List<Tag^>^ AutoCompletionCandidates(String^ filename, int linenum, String^ expr, String^ text)
	{
		marshal_context context;
		wxFileName fname = context.marshal_as<const wchar_t*>(filename);
		wxString sExpr = context.marshal_as<const wchar_t*>(expr);
		wxString sText = context.marshal_as<const wchar_t*>(text);

		vector<TagEntryPtr> tags;

		m_tags->AutoCompleteCandidates(fname, linenum, sExpr, sText, tags);

		return ParserUtilities::TagVectorToTagList(tags);
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

		return ParserUtilities::TagVectorToTagList(tags);
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

		return ParserUtilities::TagVectorToTagList(tags);
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

		return ParserUtilities::TagVectorToTagList(tags);
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

		Tag^ tag = ParserUtilities::TagPointerToTag(pTag);
		return tag;
	}

	Tag^ FirstFunctionOfFile(String^ filename)
	{
		marshal_context context;
		wxString sFilename = context.marshal_as<const wchar_t*>(filename);

		TagEntryPtr pTag = m_tags->FirstFunctionOfFile(sFilename);

		Tag^ tag = ParserUtilities::TagPointerToTag(pTag);
		return tag;
	}

	Tag^ FirstScopeOfFile(String^ filename)
	{
		marshal_context context;
		wxString sFilename = context.marshal_as<const wchar_t*>(filename);

		TagEntryPtr pTag = m_tags->FirstScopeOfFile(sFilename);

		Tag^ tag = ParserUtilities::TagPointerToTag(pTag);
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

		return ParserUtilities::TagVectorToTagList(tags);
	}


	bool GetFunctionDetails(String^ filename, int linenum, [Out]Tag^% tag, [Out]FunctionInfo^% func)
	{
		marshal_context context;
		wxString sFilename = context.marshal_as<const wchar_t*>(filename);

		TagEntryPtr pTag;
		clFunction clfunc;

		if(m_tags->GetFunctionDetails(sFilename, linenum, pTag, clfunc))
		{
			tag = ParserUtilities::TagPointerToTag(pTag);

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

		return ParserUtilities::TagVectorToTagList(tags);
	}

	List<Tag^>^ GetFunctions(String^ filename, bool onlyWorkspace)
	{
		marshal_context context;
		wxString sFilename = context.marshal_as<const wchar_t*>(filename);

		vector<TagEntryPtr> tags;

		m_tags->GetFunctions(tags, sFilename, onlyWorkspace);

		return ParserUtilities::TagVectorToTagList(tags);
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

		return ParserUtilities::TagVectorToTagList(tags);
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

	
	void ProcessParserEvents() 
	{
		if(!m_initialized)
		{
			return;
		}

		if(wxTheApp->HasPendingEvents())
		{
			wxTheApp->ProcessPendingEvents();
		}
	}

	void OnFileParsed(String^ filename)
	{
		Console::WriteLine("Parsing completed for file: " + filename);

		FileParsed(filename);

		m_filesToParse->Remove(filename);
	}
		
};

}