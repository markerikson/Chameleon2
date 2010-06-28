// CodeLiteParser.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"


#include <iostream>
#include <fstream>
#include <string>

#include <wx/msw/msvcrt.h>

#include <wx/wx.h>
#include <wx/init.h>
#include <wx/string.h>
#include <wx/textfile.h>
#include <wx/msw/private.h>

using namespace std;

#pragma unmanaged



class CodeLiteParserApp : public wxAppConsole
{
	virtual void ProcessPendingEvents()
	{
		wxAppConsole::ProcessPendingEvents();
	}

};
IMPLEMENT_APP_NO_MAIN(CodeLiteParserApp)