//#include "stdafx.h"

#include "ParserProjects/Parser/language.h"
#include <vcclr.h>
#include <msclr\marshal.h>
#include <vector>
#include <queue>

#include <wx/wx.h>
#include <wx/init.h>
#include <wx/string.h>
#include <wx/textfile.h>
#include <wx/msw/private.h>
#include "Utilities.h"

#using <System.dll>
#using <mscorlib.dll>

using namespace System;
using namespace System::Collections::Generic;
using namespace msclr::interop;
using namespace System::Runtime::InteropServices;


namespace CodeLite
{

	public ref class LanguageWrapper
	{
	public:

		LanguageWrapper()
		{
			m_lang = LanguageST::Get();
		}



	private:
		Language* m_lang;


	public:
		String^ OptimizeScope(String^ source)
		{
			marshal_context context;
			wxString sSource = context.marshal_as<const wchar_t*>(source);

			wxString scope = m_lang->OptimizeScope(sSource);

			return  marshal_as<String^>(scope.wc_str());
		}

		String^ GetScopeName(String^ input, List<String^>^ additionalNamespaces)
		{
			marshal_context context;
			wxString sInput = context.marshal_as<const wchar_t*>(input);

			wxString sScope;

			if(additionalNamespaces == nullptr)
			{
				sScope = m_lang->GetScopeName(sInput, NULL);				
			}
			else
			{
				vector<wxString> additionalNS;

				sScope = m_lang->GetScopeName(sInput, &additionalNS);

				for(int i = 0; i < additionalNS.size(); i++)
				{
					String^ ns = marshal_as<String^>(additionalNS[i].wc_str());
					additionalNamespaces->Add(ns);
				}
			}

			String^ scope = marshal_as<String^>(sScope.wc_str());

			return scope;
		}

		bool ProcessExpression( String^ stmt,  String^ text,  String^ filename, int lineNum, [Out]String^% typeName, 
								[Out]String^% typeScope, [Out]String^% oper, [Out]String^% scopeTemplateInitList)
		{
			marshal_context context;

			wxString sStmt = context.marshal_as<const wchar_t*>(stmt);
			wxString sText = context.marshal_as<const wchar_t*>(text);
			wxString sFilename = context.marshal_as<const wchar_t*>(filename);

			wxFileName fn = sFilename;

			wxString sTypeName, sTypeScope, sOper, sScopeTemplate;

			bool result = m_lang->ProcessExpression(sStmt, sText, fn, lineNum, sTypeName, sTypeScope, sOper, sScopeTemplate);

			typeName = marshal_as<String^>(sTypeName.wc_str());
			typeScope = marshal_as<String^>(sTypeScope.wc_str());
			oper = marshal_as<String^>(sOper.wc_str());
			scopeTemplateInitList = marshal_as<String^>(sScopeTemplate.wc_str());

			return result;
		}


		List<Tag^>^ GetLocalVariables(String^ scope, String^ name, int flags)
		{
			marshal_context context;
			wxString sScope = context.marshal_as<const wchar_t*>(scope);
			wxString sName = context.marshal_as<const wchar_t*>(name);

			vector<TagEntryPtr> tags;


			m_lang->GetLocalVariables(sScope, tags, sName, flags);

			return ParserUtilities::TagVectorToTagList(tags);
		}
	};
}