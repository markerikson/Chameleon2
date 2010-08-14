#ifndef EXPRESSION_RESULT_H
#define EXPRESSION_RESULT_H

#include "string"
#include <stdio.h>

#include <wx/dlimpexp.h>

#ifdef WXMAKINGDLL_CODELITE
#    define WXDLLIMPEXP_CL WXEXPORT
#elif defined(WXUSINGDLL_CODELITE)
#    define WXDLLIMPEXP_CL WXIMPORT
#else /* not making nor using FNB as DLL */
#    define WXDLLIMPEXP_CL
#endif // WXMAKINGDLL_CODELITE

class WXDLLIMPEXP_CL ExpressionResult
{
public:

	bool        m_isFunc;
	std::string m_name;
	bool        m_isThis;
	bool        m_isaType;
	bool        m_isPtr;
	std::string m_scope;
	bool        m_isTemplate;
	std::string m_templateInitList;
	bool        m_isGlobalScope;

public:
	ExpressionResult();
	virtual ~ExpressionResult();
	void Reset();
	void Print();
	std::string ToString() const;
};
#endif //EXPRESSION_RESULT_H
