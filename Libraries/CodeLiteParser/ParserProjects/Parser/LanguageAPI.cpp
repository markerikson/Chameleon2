#include "language.h"

#include <wx/wx.h>
#include <wx/app.h>
#include <wx/dlimpexp.h>

#include "LanguageAPI.h"

static wxString tempString;
static ExpressionResult tempResult;

const wchar_t* CLP_L_OptimizeScope(const wxString& srcString)
{
	tempString = LanguageST::Get()->OptimizeScope(srcString);
	return tempString.wc_str();
}

const wchar_t* CLP_L_GetScopeName(const wxString &in, std::vector<wxString> *additionlNS)
{
	tempString = LanguageST::Get()->GetScopeName(in, additionlNS);
	return tempString.wc_str();
}


ExpressionResult* CLP_L_ParseExpression(const wxString &in)
{
	tempResult = LanguageST::Get()->ParseExpression(in);
	return &tempResult;
}

bool CLP_L_ProcessExpression(const wxString& stmt, const wxString& text, const wxString &fn, int lineno, wxString &typeName, wxString &typeScope, wxString &oper, wxString &scopeTemplateInitList)
{
	bool result = LanguageST::Get()->ProcessExpression(stmt, text, fn, lineno, typeName, typeScope, oper, scopeTemplateInitList);
	return result;
}

void CLP_L_GetLocalVariables(const wxString &in, std::vector<TagEntryPtr> &tags, const wxString &name, size_t flag)
{
	LanguageST::Get()->GetLocalVariables(in, tags, name, flag);
}

