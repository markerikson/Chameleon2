#ifndef CTAGSMANAGERAPI_H
#define CTAGSMANAGERAPI_H

#include <wx/wx.h>
#include <wx/filename.h>
class ParseRequest;

#ifdef WXMAKINGDLL_CODELITE
#    define WXDLLIMPEXP_CL WXEXPORT
#elif defined(WXUSINGDLL_CODELITE)
#    define WXDLLIMPEXP_CL WXIMPORT
#else /* not making nor using FNB as DLL */
#    define WXDLLIMPEXP_CL
#endif // WXMAKINGDLL_CODELITE

extern "C"
{

	WXDLLIMPEXP_CL void CLP_CTM_AddParserRequestSingleFile(ParseRequest* request);
	WXDLLIMPEXP_CL bool CLP_CTM_AutoCompleteCandidates(const wxFileName &fileName, int lineno, const wxString& expr, const wxString& text, std::vector<TagEntryPtr> &candidates);
	WXDLLIMPEXP_CL void CLP_CTM_DeleteTagsByFilePrefix(const wxString &dbfileName, const wxString &filePrefix);
	WXDLLIMPEXP_CL void CLP_CTM_FindByPath(const wxString &path, std::vector<TagEntryPtr> &tags);
	WXDLLIMPEXP_CL void CLP_CTM_FindImplDecl(const wxFileName &fileName, int lineno, const wxString & expr, const wxString &word,  const wxString &text, std::vector<TagEntryPtr> &tags, bool impl = true, bool workspaceOnly = false);
	WXDLLIMPEXP_CL void CLP_CTM_FindSymbol(const wxString& name, std::vector<TagEntryPtr> &tags);
	WXDLLIMPEXP_CL void CLP_CTM_FirstFunctionOfFile(TagEntryPtr& returnTag, const wxFileName &fileName);
	WXDLLIMPEXP_CL void CLP_CTM_FirstScopeOfFile(TagEntryPtr& returnTag, const wxFileName &fileName);
	WXDLLIMPEXP_CL void CLP_CTM_FunctionFromFileLine(TagEntryPtr& returnTag, const wxFileName &fileName, int lineno, bool nextFunction = false);
	WXDLLIMPEXP_CL void CLP_CTM_GetClasses(std::vector< TagEntryPtr > &tags, bool onlyWorkspace = true);
	WXDLLIMPEXP_CL void CLP_CTM_GetFunctions(std::vector< TagEntryPtr > &tags, const wxString &fileName = wxEmptyString, bool onlyWorkspace = true);
	WXDLLIMPEXP_CL bool CLP_CTM_GetFunctionDetails(const wxFileName &fileName, int lineno, TagEntryPtr &tag, clFunction &func);
	WXDLLIMPEXP_CL void CLP_CTM_GetFiles(const wxString &partialName, std::vector<wxFileName> &files);
	WXDLLIMPEXP_CL void CLP_CTM_GetHoverTip(const wxFileName &fileName, int lineno, const wxString & expr, const wxString &word, const wxString & text, std::vector<wxString> & tips);
	WXDLLIMPEXP_CL bool CLP_CTM_GetMemberType(const wxString &scope, const wxString &name, wxString &type, wxString &typeScope);
	WXDLLIMPEXP_CL void CLP_CTM_GetScopesFromFile(const wxFileName &fileName, std::vector<wxString> &scopes);
	WXDLLIMPEXP_CL const wchar_t* CLP_CTM_GetScopeName(const wxString &scope);
	WXDLLIMPEXP_CL void CLP_CTM_GetTagsByKind(std::vector<TagEntryPtr> &tags, const wxArrayString &kind, const wxString &partName = wxEmptyString);
	WXDLLIMPEXP_CL void CLP_CTM_OpenType(std::vector<TagEntryPtr> &tags);
	WXDLLIMPEXP_CL bool CLP_CTM_ProcessExpression(const wxString &expression, wxString &type, wxString &typeScope);
	WXDLLIMPEXP_CL void CLP_CTM_RetagFiles(const std::vector<wxFileName> &files, bool quickRetag);
	WXDLLIMPEXP_CL void CLP_CTM_TagsByScope1(const wxString& scope, std::vector<TagEntryPtr> &tags);
	WXDLLIMPEXP_CL void CLP_CTM_TagsByScope2(const wxString &scopeName, const wxString &kind, std::vector<TagEntryPtr> &tags, bool includeInherits = false, bool onlyWorkspace = false);
	WXDLLIMPEXP_CL void CLP_CTM_TagsByScopeAndName(const wxString& scope, const wxString &name, std::vector<TagEntryPtr> &tags, size_t flags = PartialMatch);
	WXDLLIMPEXP_CL void CLP_CTM_TagsFromFileAndScope(const wxFileName &fileName, const wxString &scopeName, std::vector<TagEntryPtr> &tags);
	WXDLLIMPEXP_CL bool CLP_CTM_WordCompletionCandidates(const wxFileName &fileName, int lineno, const wxString& expr, const wxString& text, const wxString &word, std::vector<TagEntryPtr> &candidates);



WXDLLIMPEXP_CL void CLP_ProcessNativeEvents();
WXDLLIMPEXP_CL bool CLP_NativeInit(wxEvtHandler* handler, wxString& idxPath, wxString& databasePath);
WXDLLIMPEXP_CL void CLP_NativeEnd();


};
#endif