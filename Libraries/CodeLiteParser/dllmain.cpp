// dllmain.cpp : Defines the entry point for the DLL application.
//#include "stdafx.h"

#include <wx/wx.h>
#include <wx/init.h>
#include <wx/msw/private.h>


WXDLLIMPEXP_BASE void wxSetInstance(HINSTANCE instance);

static void initWx(HINSTANCE instance)
{
	int argc = 0;
	char ** argv = NULL;
	wxSetInstance(instance);
	wxEntryStart(argc, argv);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			//initWx(hModule);
			break;
		}
	case DLL_PROCESS_DETACH:
		{
			//wxEntryCleanup();
			break;
		}
	}
	return TRUE;
}

