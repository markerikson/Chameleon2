#include "ParserProjects/Parser/language.h"
#include "ParserProjects/Parser/LanguageAPI.h"

#include <vector>
#include <queue>
#include "Utilities.h"

#include <wx/wx.h>
#include <wx/init.h>
#include <wx/string.h>
#include <wx/textfile.h>
#include <wx/msw/private.h>

#pragma managed

#include <vcclr.h>
#include <msclr\marshal.h>

#using <System.dll>
#using <mscorlib.dll>

using namespace System;
using namespace System::Collections::Generic;
using namespace msclr::interop;
using namespace System::Runtime::InteropServices;



namespace CodeLite
{

	public ref class ExpressionInfo
	{
	public:

		bool        m_isFunc;
		String^ m_name;
		bool        m_isThis;
		bool        m_isaType;
		bool        m_isPtr;
		String^ m_scope;
		bool        m_isTemplate;
		String^ m_templateInitList;
		bool        m_isGlobalScope;

	};

	public ref class LanguageWrapper
	{
	public:

		LanguageWrapper()
		{
		}
		
	private:


	public:
		String^ OptimizeScope(String^ source)
		{
			marshal_context context;
			wxString sSource = ConvertString(source);

			wxString scope = CLP_L_OptimizeScope(sSource);

			return ConvertString(scope);
		}

		String^ GetScopeName(String^ input, List<String^>^ additionalNamespaces)
		{
			marshal_context context;
			wxString sInput = ConvertString(input);

			wxString sScope;

			if(additionalNamespaces == nullptr)
			{
				sScope = CLP_L_GetScopeName(sInput, NULL);				
			}
			else
			{
				vector<wxString> additionalNS;

				sScope = CLP_L_GetScopeName(sInput, &additionalNS);

				for(int i = 0; i < additionalNS.size(); i++)
				{
					String^ ns = ConvertString(additionalNS[i]);
					additionalNamespaces->Add(ns);
				}
			}

			String^ scope = ConvertString(sScope);

			return scope;
		}

		bool ProcessExpression( String^ stmt,  String^ text,  String^ filename, int lineNum, [Out]String^% typeName, 
								[Out]String^% typeScope, [Out]String^% oper, [Out]String^% scopeTemplateInitList)
		{
			marshal_context context;

			wxString sStmt = ConvertString(stmt);
			wxString sText = ConvertString(text);
			wxString sFilename = ConvertString(filename);


			wxString sTypeName, sTypeScope, sOper, sScopeTemplate;

			bool result = CLP_L_ProcessExpression(sStmt, sText, sFilename, lineNum, sTypeName, sTypeScope, sOper, sScopeTemplate);

			typeName = ConvertString(sTypeName);
			typeScope = ConvertString(sTypeScope);
			oper = ConvertString(sOper);
			scopeTemplateInitList = ConvertString(sScopeTemplate);

			return result;
		}

		ExpressionInfo^ ParseExpression(String^ expr)
		{
			wxString sExpr = ConvertString(expr);

			ExpressionResult* er = CLP_L_ParseExpression(sExpr);
			ExpressionInfo^ info = ConvertExpressionInfo(*er);

			return info;
		}


		List<Tag^>^ GetLocalVariables(String^ scope, String^ name, int flags)
		{
			marshal_context context;
			wxString sScope = ConvertString(scope);
			wxString sName = ConvertString(name);

			vector<TagEntryPtr> tags;


			CLP_L_GetLocalVariables(sScope, tags, sName, flags);

			return TagVectorToTagList(tags);
		}

		static ExpressionInfo^ ConvertExpressionInfo(ExpressionResult& er)
		{
			ExpressionInfo^ info = gcnew ExpressionInfo();

			info->m_isFunc = er.m_isFunc;
			info->m_name = ConvertString(er.m_name);
			info->m_isThis = er.m_isThis;
			info->m_isaType = er.m_isaType;
			info->m_isPtr = er.m_isPtr;
			info->m_scope = ConvertString(er.m_scope);
			info->m_isTemplate = er.m_isTemplate;
			info->m_templateInitList = ConvertString(er.m_templateInitList);
			info->m_isGlobalScope = er.m_isGlobalScope;

			return info;
		}
	};
}