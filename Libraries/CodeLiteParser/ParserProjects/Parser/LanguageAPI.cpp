#include "language.h"

#include <wx/wx.h>
#include <wx/app.h>
#include <wx/dlimpexp.h>

#include "LanguageAPI.h"

static wxString tempString;
static ExpressionResult tempResult;

const wchar_t* CLP_L_OptimizeScope(const wxString& srcString)
{
	return tempString.wc_str();
}

const wchar_t* CLP_L_GetScopeName(const wxString &in, std::vector<wxString> *additionlNS)
{
	return tempString.wc_str();
}


ExpressionResult* CLP_L_ParseExpression(const wxString &in)
{
	return &tempResult;
}

bool CLP_L_ProcessExpression(const wxString& stmt, const wxString& text, const wxString &fn, int lineno, wxString &typeName, wxString &typeScope, wxString &oper, wxString &scopeTemplateInitList)
{
	return false;
}

void CLP_L_GetLocalVariables(const wxString &in, std::vector<TagEntryPtr> &tags, const wxString &name, size_t flag)
{

}

