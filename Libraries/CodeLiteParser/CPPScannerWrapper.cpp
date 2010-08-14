#include <wx/wx.h>
#include <wx/init.h>
#include <wx/string.h>
#include <wx/textfile.h>
#include <wx/msw/private.h>
#include "Utilities.h"

#include "ParserProjects/Parser/language.h"

#include <vector>
#include <queue>


#include <vcclr.h>
#include <msclr\marshal.h>

#using <System.dll>
#using <mscorlib.dll>

using namespace System;
using namespace System::Collections::Generic;
using namespace msclr::interop;
using namespace System::Runtime::InteropServices;


// TODO Will this work now that the parser code is in a separate DLL?


namespace CodeLite
{	

	public ref class CPPScannerWrapper
	{
	public:
		CPPScannerWrapper()
		{
			m_scanner = new CppScanner();
		}

		~CPPScannerWrapper()
		{
			Destroy();
		}


		void SetText(String^ text)
		{
			wxString sText = ConvertString(text);
			m_scanner->SetText(sText.c_str());
		}

		void Reset()
		{
			m_scanner->Reset();
		}

		int Lex()
		{
			int lexResult = m_scanner->yylex();
			return lexResult;
		}

		String^ TokenText()
		{
			wxString sText = m_scanner->YYText();
			String^ text = ConvertString(sText);
			return text;
		}

		int LineNumber()
		{
			return m_scanner->LineNo();
		}

		void ClearComment()
		{
			m_scanner->ClearComment();
		}

		void KeepComments(bool keep)
		{
			m_scanner->KeepComment(keep);
		}

		void ReturnWhitespace(bool whitespace)
		{
			m_scanner->ReturnWhite(whitespace);
		}

	protected:
		!CPPScannerWrapper()
		{
			Destroy();
		}

	private:
		CppScanner* m_scanner;


		void Destroy()
		{
			delete m_scanner;
		}
	};

}