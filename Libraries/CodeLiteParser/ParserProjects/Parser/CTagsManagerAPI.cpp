#include "ctags_manager.h"
#include "parse_thread.h"
#include "fc_fileopener.h"

#include <wx/wx.h>
#include <wx/app.h>

#include "CTagsManagerAPI.h"

static clProcess* indexerProcess;
static wxEvtHandler* pmr;
static TagsManager * m_tags;
static bool m_initialized = false;

static TagEntryPtr tempTagPtr;
static wxString tempString;



bool CLP_NativeInit(wxEvtHandler* handler, wxString& idxPath, wxString& databasePath) 
{		
	if(m_initialized)
	{
		return false;
	}

	/*
	wxSetInstance(GetModuleHandle(NULL));
	int argc = 0;
	char **argv = NULL;
	wxEntryStart(argc, argv);
	if ( !wxTheApp || !wxTheApp->CallOnInit() )
		return FALSE;
	*/

	pmr = handler;


	//wxString indexerPath = ConvertString(idxPath);

	ParseThread* parser = ParseThreadST::Get();
	m_tags = TagsManagerST::Get();

	//m_tags = tagmgr;

	LanguageST::Get()->SetTagsManager(m_tags );
	TagsManagerST::Get()->SetLanguage( LanguageST::Get() );

	m_tags->SetCodeLiteIndexerPath(idxPath);
	indexerProcess = m_tags->StartCtagsProcess();


	//pmr = pr;
	parser->SetNotifyWindow(pmr);

	parser->Start();

	//CtagsManagerWrapper::databasePath = databasePath;
	//wxString dbPath = ConvertString(databasePath);
	m_tags->OpenDatabase(databasePath);
	//m_appLoopTimer->Enabled = true;
	m_initialized = true;

	return true;
}

void CLP_NativeEnd() 
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

	//ShutdownIndexerThread^ o1 = gcnew ShutdownIndexerThread(indexerProcess);
	//Thread^ t1 = gcnew Thread(gcnew ThreadStart(o1, &ShutdownIndexerThread::ThreadEntryPoint));

	fcFileOpener::Release();

	m_initialized = false;
	//m_appLoopTimer->Enabled = false;

	//wxEntryCleanup();		
}

void ProcessNativeEvents()
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


void CLP_CTM_AddParserRequestSingleFile(ParseRequest* request)
{
	ParseThread* parser = ParseThreadST::Get();
	parser->Add(request);	
}

void CLP_CTM_RenameTaggedFile(const wxString& oldFile, const wxString& newFile)
{
	m_tags->RenameTaggedFile(oldFile, newFile);
	
}




bool CLP_CTM_AutoCompleteCandidates(const wxString &fileName, int lineno, const wxString& expr, const wxString& text, std::vector<TagEntryPtr> &candidates)
{
	return m_tags->AutoCompleteCandidates(fileName, lineno, expr, text, candidates);
}
void CLP_CTM_DeleteFilesTags(const std::vector<wxFileName> &projectFiles)
{
	m_tags->DeleteFilesTags(projectFiles);
}
void CLP_CTM_DeleteTagsByFilePrefix(const wxString &dbfileName, const wxString &filePrefix)
{
	m_tags->DeleteTagsByFilePrefix(dbfileName, filePrefix);
}
void CLP_CTM_FindByPath(const wxString &path, std::vector<TagEntryPtr> &tags)
{
	m_tags->FindByPath(path, tags);
}
void CLP_CTM_FindImplDecl(const wxString &fileName, int lineno, const wxString & expr, const wxString &word,  const wxString &text, std::vector<TagEntryPtr> &tags, bool impl, bool workspaceOnly)
{
	m_tags->FindImplDecl(fileName, lineno, expr, word, text, tags, impl, workspaceOnly);
}
void CLP_CTM_FindSymbol(const wxString& name, std::vector<TagEntryPtr> &tags)
{
	m_tags->FindSymbol(name, tags);
}
void CLP_CTM_FirstFunctionOfFile(TagEntryPtr& returnTag, const wxString &fileName)
{
	returnTag = m_tags->FirstFunctionOfFile(fileName);
	//return tempTagPtr.Get();
}
void CLP_CTM_FirstScopeOfFile(TagEntryPtr& returnTag, const wxString &fileName)
{
	returnTag = m_tags->FirstScopeOfFile(fileName);
}
void CLP_CTM_FunctionFromFileLine(TagEntryPtr& returnTag, const wxString &fileName, int lineno, bool nextFunction)
{
	returnTag = m_tags->FunctionFromFileLine(fileName, lineno, nextFunction);
	//return tempTagPtr.Get();
}
void CLP_CTM_GetClasses(std::vector< TagEntryPtr > &tags, bool onlyWorkspace)
{
	m_tags->GetClasses(tags, onlyWorkspace);
}
void CLP_CTM_GetFunctions(std::vector< TagEntryPtr > &tags, const wxString &fileName, bool onlyWorkspace)
{
	m_tags->GetFunctions(tags, fileName, onlyWorkspace);
}
bool CLP_CTM_GetFunctionDetails(const wxString &fileName, int lineno, TagEntryPtr &tag, clFunction &func)
{
	return m_tags->GetFunctionDetails(fileName, lineno, tag, func);
}
void CLP_CTM_GetFiles(const wxString &partialName, std::vector<wxFileName> &files)
{
	m_tags->GetFiles(partialName, files);
}
void CLP_CTM_GetHoverTip(const wxString &fileName, int lineno, const wxString & expr, const wxString &word, const wxString & text, std::vector<wxString> & tips)
{
	m_tags->GetHoverTip(fileName, lineno, expr, word, text, tips);
}
bool CLP_CTM_GetMemberType(const wxString &scope, const wxString &name, wxString &type, wxString &typeScope)
{
	return m_tags->GetMemberType(scope, name, type, typeScope);
}
void CLP_CTM_GetScopesFromFile(const wxFileName &fileName, std::vector<wxString> &scopes)
{
	m_tags->GetScopesFromFile(fileName, scopes);
}
const wchar_t* CLP_CTM_GetScopeName(const wxString &scope)
{
	tempString = m_tags->GetScopeName(scope);
	return tempString.wc_str();
}
void CLP_CTM_GetTagsByKind(std::vector<TagEntryPtr> &tags, const wxArrayString &kind, const wxString &partName)
{
	m_tags->GetTagsByKind(tags, kind, partName);
}
void CLP_CTM_OpenType(std::vector<TagEntryPtr> &tags)
{
	m_tags->OpenType(tags);
}
bool CLP_CTM_ProcessExpression(const wxString &expression, wxString &type, wxString &typeScope)
{
	return m_tags->ProcessExpression(expression, type, typeScope);
}
void CLP_CTM_RetagFiles(const std::vector<wxFileName> &files, bool quickRetag)
{
	m_tags->RetagFiles(files, quickRetag);
}
void CLP_CTM_TagsByScope1(const wxString& scope, std::vector<TagEntryPtr> &tags)
{
	m_tags->TagsByScope(scope, tags);
}
void CLP_CTM_TagsByScope2(const wxString &scopeName, const wxString &kind, std::vector<TagEntryPtr> &tags, bool includeInherits, bool onlyWorkspace)
{
	m_tags->TagsByScope(scopeName, kind, tags, includeInherits, onlyWorkspace);
}
void CLP_CTM_TagsByScopeAndName(const wxString& scope, const wxString &name, std::vector<TagEntryPtr> &tags, size_t flags)
{
	m_tags->TagsByScopeAndName(scope, name, tags, flags);
}
void CLP_CTM_TagsFromFileAndScope(const wxString &fileName, const wxString &scopeName, std::vector<TagEntryPtr> &tags)
{
	m_tags->TagsFromFileAndScope(fileName, scopeName, tags);
}
bool CLP_CTM_WordCompletionCandidates(const wxString &fileName, int lineno, const wxString& expr, const wxString& text, const wxString &word, std::vector<TagEntryPtr> &candidates)
{
	return m_tags->WordCompletionCandidates(fileName, lineno, expr, text, word, candidates);
}