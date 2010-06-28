//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah                            
// file name            : cpp_scanner.h              
//                                                                          
// -------------------------------------------------------------------------
// A                                                                        
//              _____           _      _     _ _                            
//             /  __ \         | |    | |   (_) |                           
//             | /  \/ ___   __| | ___| |    _| |_ ___                      
//             | |    / _ \ / _  |/ _ \ |   | | __/ _ )                     
//             | \__/\ (_) | (_| |  __/ |___| | ||  __/                     
//              \____/\___/ \__,_|\___\_____/_|\__\___|                     
//                                                                          
//                                                  F i l e                 
//                                                                          
//    This program is free software; you can redistribute it and/or modify  
//    it under the terms of the GNU General Public License as published by  
//    the Free Software Foundation; either version 2 of the License, or     
//    (at your option) any later version.                                   
//                                                                          
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
 #ifndef CODELITE_CPPSCANNER_H
#define CODELITE_CPPSCANNER_H

#include "FlexLexer.h"
#include "smart_ptr.h"

#ifdef WXMAKINGDLL_CODELITE
#    define WXDLLIMPEXP_CL WXEXPORT
#elif defined(WXUSINGDLL_CODELITE)
#    define WXDLLIMPEXP_CL WXIMPORT
#else /* not making nor using FNB as DLL */
#    define WXDLLIMPEXP_CL
#endif // WXMAKINGDLL_CODELITE

class WXDLLIMPEXP_CL CppScanner : public flex::yyFlexLexer
{
public:
	CppScanner();
	~CppScanner(void);
	
	/// Override the LexerInput function
	int LexerInput(char *buf, int max_size);
	void SetText(const char* data);
	void Reset();

	 
	///	Note about comment and line number:
	///	If the last text consumed is a comment, the line number 
	///	returned is the line number of the last line of the comment
	///	incase the comment spans over number of lines
	///	(i.e. /* ... */ comment style)

	const int& LineNo() const { return yylineno; }
	inline void ClearComment() { m_comment = wxEmptyString; }
	inline const wxChar* GetComment() const { return m_comment.GetData(); }
	inline void KeepComment(const int& keep) { m_keepComments = keep; }
	inline void ReturnWhite(const int& rw) { m_returnWhite = rw; }

private:
	char *m_data;
	char *m_pcurr;
	int   m_total;
	int   m_curr;
};

typedef SmartPtr<CppScanner> CppScannerPtr;
#endif // CODELITE_CPPSCANNER_H
