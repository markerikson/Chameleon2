// dllmain.cpp : Defines the entry point for the DLL application.
//#include "stdafx.h"

#include <wx/wx.h>
#include <wx/init.h>
#include <wx/msw/private.h>

#include <iostream>

using namespace std;


WXDLLIMPEXP_BASE void wxSetInstance(HINSTANCE instance);

static void initWx(HINSTANCE instance)
{
	int argc = 0;
	char ** argv = NULL;
	wxSetInstance(instance);
	wxEntryStart(argc, argv);
}


DWORD WINAPI MyThreadFunction( LPVOID lpParam ) 
{ 
	cout << "Calling wxEntry()" << endl;
	wxEntry(0,NULL);
	cout << "wxEntry() complete" << endl;
	/*
	if ( !wxTheApp || !wxTheApp->CallOnInit() )
	{
		return FALSE;
	}

	cout << "Calling App::Run" << endl;
	wxTheApp->OnRun();
	return 0;
	*/

	return 0;
}


bool wxInitialized;

extern void RunApp();

BOOL APIENTRY DllMain( HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			initWx(hModule);

			if ( !wxTheApp || !wxTheApp->CallOnInit() )
			{
				return FALSE;
			}

			/*
			DWORD   id = 0;
			cout << "Creating thread" << endl;
			CreateThread(NULL,0, MyThreadFunction,NULL, 0,&id);
			cout << "Thread created" << endl;
			
			
			
			
			*/


			break;
		}
	case DLL_PROCESS_DETACH:
		{
			//cout << "Cleaning up" << endl;
			wxEntryCleanup();
			/*
			if(wxInitialized)
			{
				cout << "App exiting" << endl;
				wxTheApp->Exit();
			}
			*/
			cout << "Detaching" << endl;
			break;
		}
	}
	return TRUE;
}

