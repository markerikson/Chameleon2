#ifndef LANGUAGEAPI_H
#define LANGUAGEAPI_H

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
	WXDLLIMPEXP_CL const wchar_t* CLP_L_OptimizeScope(const wxString& srcString);
	WXDLLIMPEXP_CL const wchar_t* CLP_L_GetScopeName(const wxString &in, std::vector<wxString> *additionlNS);

	WXDLLIMPEXP_CL ExpressionResult* CLP_L_ParseExpression(const wxString &in);
	WXDLLIMPEXP_CL bool CLP_L_ProcessExpression(const wxString& stmt, const wxString& text, const wxString &fn, int lineno, wxString &typeName, wxString &typeScope, wxString &oper, wxString &scopeTemplateInitList);
	WXDLLIMPEXP_CL void CLP_L_GetLocalVariables(const wxString &in, std::vector<TagEntryPtr> &tags, const wxString &name = wxEmptyString, size_t flag = PartialMatch);
}


#endif