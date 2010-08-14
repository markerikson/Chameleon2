#include "ParserProjects/Parser/ctags_manager.h"
#include "ParserProjects/Parser/parse_thread.h"
#include "ParserProjects/Parser/fc_fileopener.h"
#include "ParserProjects/Parser/CTagsManagerAPI.h"

#include <vector>

#include <wx/wx.h>
#include <wx/init.h>
#include <wx/string.h>
#include <wx/textfile.h>
#include <wx/msw/private.h>

#include "Utilities.h"
#include "ParserMessageReceiver.h"


#include <vcclr.h>
#include <msclr\marshal.h>

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
		
		wxString indexerPath = ConvertString(idxPath);

		
		wxString dbPath = ConvertString(databasePath);
		CtagsManagerWrapper::databasePath = databasePath;

		CLP_NativeInit(pmr, indexerPath, dbPath);
		
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

		CLP_NativeEnd();

		ShutdownIndexerThread^ o1 = gcnew ShutdownIndexerThread(indexerProcess);
		Thread^ t1 = gcnew Thread(gcnew ThreadStart(o1, &ShutdownIndexerThread::ThreadEntryPoint));
	}

	void AddParserRequestSingleFile(String^ filename) 
	{
		ParseRequest *parsingRequest = new ParseRequest();

		m_filesToParse->Add(filename);

		
		wxString fname = ConvertString(filename);
		wxString dbpath = ConvertString(databasePath);

		parsingRequest->setFile(fname);
		parsingRequest->setDbFile(dbpath);

		parsingRequest->setType(ParseRequest::PR_FILESAVED);
		parsingRequest->_quickRetag = false;

		pmr->filesToParse.push(fname);

		CLP_CTM_AddParserRequestSingleFile(parsingRequest);
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
		
		wxString name = ConvertString(fileName);

		vector<wxString> fileScopes;

		CLP_CTM_GetScopesFromFile(name, fileScopes);

		List<String^>^ returnScopes = gcnew List<String^>();

		for(int i = 0; i < fileScopes.size(); i++)
		{
			
			String^ scope = ConvertString(fileScopes[i]);
			returnScopes->Add(scope);
		}

		return returnScopes;
	}

	List<Tag^>^ FindSymbol(String^ symbolName)
	{
		
		wxString name = ConvertString(symbolName);

		vector<TagEntryPtr> tags;

		CLP_CTM_FindSymbol(name, tags);

		return TagVectorToTagList(tags);
	}

	List<Tag^>^ FindByPath(String^ path)
	{
		
		wxString filePath = ConvertString(path);

		vector<TagEntryPtr> tags;

		CLP_CTM_FindByPath(filePath, tags);

		return TagVectorToTagList(tags);
	}

	List<Tag^>^ TagsByScope(String^ scope)
	{
		
		wxString sScope = ConvertString(scope);

		vector<TagEntryPtr> tags;

		CLP_CTM_TagsByScope1(sScope, tags);

		return TagVectorToTagList(tags);
	}

	List<Tag^>^ TagsByScope(String^ scope, String^ kind, bool includeInherits, bool onlyWorkspace)
	{
		
		wxString sScope = ConvertString(scope);
		wxString sKind = ConvertString(kind);

		vector<TagEntryPtr> tags;

		CLP_CTM_TagsByScope2(sScope, sKind, tags, includeInherits, onlyWorkspace);

		return TagVectorToTagList(tags);
	}



	List<Tag^>^ TagsByScopeAndName(String^ scope, String^ name)
	{
		
		wxString sScope = ConvertString(scope);
		wxString sName = ConvertString(name);

		vector<TagEntryPtr> tags;

		CLP_CTM_TagsByScopeAndName(sScope, sName, tags);

		return TagVectorToTagList(tags);
	}

	List<Tag^>^ AutoCompletionCandidates(String^ filename, int linenum, String^ expr, String^ text)
	{
		
		wxFileName fname = ConvertString(filename);
		wxString sExpr = ConvertString(expr);
		wxString sText = ConvertString(text);

		vector<TagEntryPtr> tags;

		CLP_CTM_AutoCompleteCandidates(fname, linenum, sExpr, sText, tags);

		return TagVectorToTagList(tags);
	}

	List<Tag^>^ WordCompletionCandidates(String^ filename, int linenum, String^ expr, 
										String^ text, String^ word)
	{
		
		wxFileName fname = ConvertString(filename);
		wxString sExpr = ConvertString(expr);
		wxString sText = ConvertString(text);
		wxString sWord = ConvertString(word);

		vector<TagEntryPtr> tags;

		CLP_CTM_WordCompletionCandidates(fname, linenum, sExpr, sText, sWord, tags);

		return TagVectorToTagList(tags);
	}

	void DeleteFilesTags(List<String^>^ files)
	{
		
		vector<wxFileName> fnames;

		for(int i = 0; i < files->Count; i++)
		{
			wxFileName fname = ConvertString(files[i]);
			fnames.push_back(fname);
		}

	}

	void DeleteTagsByFilePrefix(String^ dbPath, String^ filePrefix)
	{
		

		wxString sDbPath = ConvertString(dbPath);
		wxString sPrefix = ConvertString(filePrefix);

		CLP_CTM_DeleteTagsByFilePrefix(sDbPath, sPrefix);
	}

	void RetagFiles(List<String^>^ files, bool quickRetag)
	{
		
		vector<wxFileName> fnames;

		for(int i = 0; i < files->Count; i++)
		{
			wxFileName fname = ConvertString(files[i]);
			fnames.push_back(fname);
		}

		CLP_CTM_RetagFiles(fnames, quickRetag);

	}

	List<String^>^ GetHoverTip(String^ filename, int linenum, String^ expr, 
		String^ word, String^ text)
	{
		
		wxFileName fname = ConvertString(filename);
		wxString sExpr = ConvertString(expr);
		wxString sText = ConvertString(text);
		wxString sWord = ConvertString(word);

		vector<wxString> tips;

		CLP_CTM_GetHoverTip(fname, linenum, sExpr, sWord, sText, tips);

		List<String^>^ list = gcnew List<String^>();

		for(int i = 0; i < tips.size(); i++)
		{
			String^ tip = ConvertString(tips[i]);
			list->Add(tip);
		}

		return list;
	}


	List<Tag^>^ OpenType()
	{
		vector<TagEntryPtr> tags;
		CLP_CTM_OpenType(tags);

		return TagVectorToTagList(tags);
	}


	List<Tag^>^ FindImplDecl(String^ filename, int linenum, String^ expr, String^ word, String^ text, 
							bool impl, bool workspaceOnly)
	{
		
		wxString sFilename = ConvertString(filename);
		wxString sExpr = ConvertString(expr);
		wxString sWord = ConvertString(word);
		wxString sText = ConvertString(text);

		vector<TagEntryPtr> tags;

		CLP_CTM_FindImplDecl(sFilename, linenum, sExpr, sWord, sText, tags, impl, workspaceOnly);

		return TagVectorToTagList(tags);
	}

	String^ GetScopeName(String^ scope)
	{
		
		wxString sScope = ConvertString(scope);

		wxString result = CLP_CTM_GetScopeName(sScope);

		String^ returnScope = ConvertString(result);
		return returnScope;
	}

	List<String^>^ GetFiles(String^ partialName)
	{
		
		wxString sName = ConvertString(partialName);

		vector<wxFileName> files;

		CLP_CTM_GetFiles(sName, files);

		List<String^>^ list = gcnew List<String^>();

		for(int i = 0; i < files.size(); i++)
		{
			wxFileName fname = files[i];
			String^ filename = ConvertString(fname.GetFullPath());

			list->Add(filename);
		}

		return list;
	}


	Tag^ FunctionFromFileLine(String^ filename, int linenum, bool nextFunction)
	{
		
		wxString sFilename = ConvertString(filename);

		TagEntryPtr pTag;
		CLP_CTM_FunctionFromFileLine(pTag, sFilename, linenum, nextFunction);

		Tag^ tag = TagPointerToTag(pTag);
		return tag;
	}

	Tag^ FirstFunctionOfFile(String^ filename)
	{
		
		wxString sFilename = ConvertString(filename);

		TagEntryPtr pTag;
		CLP_CTM_FirstFunctionOfFile(pTag, sFilename);

		Tag^ tag = TagPointerToTag(pTag);
		return tag;
	}

	Tag^ FirstScopeOfFile(String^ filename)
	{
		
		wxString sFilename = ConvertString(filename);

		TagEntryPtr pTag;
		CLP_CTM_FirstScopeOfFile(pTag, sFilename);

		Tag^ tag = TagPointerToTag(pTag);
		return tag;
	}

	
	bool GetMemberType(String^ scope, String^ name, [Out]String^% type, [Out]String^% typeScope)
	{
		
		wxString sScope = ConvertString(scope);
		wxString sName = ConvertString(name);

		wxString sType, sTypeScope;

		if(CLP_CTM_GetMemberType(sScope, sName, sType, sTypeScope))
		{
			type = ConvertString(sType);
			typeScope = ConvertString(sTypeScope);

			return true;
		}

		return false;
	}

	List<Tag^>^ TagsFromFileAndScope(String^ filename, String^ scopeName)
	{
		
		wxString sFilename = ConvertString(filename);
		wxString sScopeName = ConvertString(scopeName);

		vector<TagEntryPtr> tags;
		CLP_CTM_TagsFromFileAndScope(sFilename, sScopeName, tags);

		return TagVectorToTagList(tags);
	}


	bool GetFunctionDetails(String^ filename, int linenum, [Out]Tag^% tag, [Out]FunctionInfo^% func)
	{
		
		wxString sFilename = ConvertString(filename);

		TagEntryPtr pTag;
		clFunction clfunc;

		if(CLP_CTM_GetFunctionDetails(sFilename, linenum, pTag, clfunc))
		{
			tag = TagPointerToTag(pTag);

			func = gcnew FunctionInfo();
			func->name = ConvertString(clfunc.m_name);
			func->scope = ConvertString(clfunc.m_scope);
			func->returnValueConst = ConvertString(clfunc.m_retrunValusConst);
			func->signature = ConvertString(clfunc.m_signature);
			func->lineno = clfunc.m_lineno;
			func->isVirtual = clfunc.m_isVirtual;
			func->isPureVirtual = clfunc.m_isPureVirtual;
			func->isConst = clfunc.m_isConst;

			VariableInfo^ vi = gcnew VariableInfo();
			Variable v = clfunc.m_returnValue;
			
			vi->name = ConvertString(v.m_name);
			vi->templateDecl = ConvertString(v.m_templateDecl);
			vi->type = ConvertString(v.m_type);
			vi->typeScope = ConvertString(v.m_typeScope);
			vi->pattern = ConvertString(v.m_pattern);
			vi->starAmp = ConvertString(v.m_starAmp);
			vi->rightSideConst = ConvertString(v.m_rightSideConst);
			vi->defaultValue = ConvertString(v.m_defaultValue);
			vi->arrayBrackets = ConvertString(v.m_arrayBrackets);

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

		CLP_CTM_GetClasses(tags, onlyWorkspace);

		return TagVectorToTagList(tags);
	}

	List<Tag^>^ GetFunctions(String^ filename, bool onlyWorkspace)
	{
		
		wxString sFilename = ConvertString(filename);

		vector<TagEntryPtr> tags;

		CLP_CTM_GetFunctions(tags, sFilename, onlyWorkspace);

		return TagVectorToTagList(tags);
	}

	List<Tag^>^ GetTagsByKind(List<String^>^ kinds)
	{
		
		wxArrayString sKind;
		vector<TagEntryPtr> tags;

		for(int i = 0; i < kinds->Count; i++)
		{
			wxString temp = ConvertString(kinds[i]);
			sKind.Add(temp);
		}

		CLP_CTM_GetTagsByKind(tags, sKind);

		return TagVectorToTagList(tags);
	}


	bool ProcessExpression(String^ expr, String^% type, String^% typeScope)
	{
		
		wxString sExpr = ConvertString(expr);

		wxString sType, sTypeScope;

		if(CLP_CTM_ProcessExpression(sExpr, sType, sTypeScope))
		{
			type = ConvertString(sType);
			typeScope = ConvertString(sTypeScope);

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