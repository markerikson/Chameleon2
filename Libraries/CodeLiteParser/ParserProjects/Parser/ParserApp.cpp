// CodeLiteParser.cpp : Defines the exported functions for the DLL application.
//

//#include "stdafx.h"


#include <iostream>
#include <fstream>
#include <string>

#include <wx/msw/msvcrt.h>

#include <wx/wx.h>
#include <wx/init.h>
#include <wx/string.h>
#include <wx/textfile.h>
#include <wx/msw/private.h>
#include <wx/timer.h>
#include <wx/dlimpexp.h>

using namespace std;

#pragma unmanaged

extern bool wxInitialized;

class EventReceiver : public wxEvtHandler
{
public:

	//bool waitingOnThread;
	//wxString filename;

	void OnCommandEvent(wxCommandEvent& e)
	{
		cout << "Command event received" << endl;
	}

protected:

	DECLARE_EVENT_TABLE()

};

EventReceiver receiver;

class MyWorkerThread : public wxThread
{
public:
	MyWorkerThread()
	{

	}

	// thread execution starts here
	virtual wxThread::ExitCode Entry();

	// called when the thread exits - whether it terminates normally or is
	// stopped with Delete() (but not when it is Kill()ed!)
	virtual void OnExit()
	{
		cout << "Thread exiting" << endl;
	}

public:
};





BEGIN_EVENT_TABLE(EventReceiver, wxEvtHandler)
	EVT_COMMAND(wxID_ANY, wxEVT_COMMAND_BUTTON_CLICKED, EventReceiver::OnCommandEvent)
END_EVENT_TABLE()


MyWorkerThread* thread;

void WXEXPORT RunApp()
{
	//cout << "RunApp: starting thread" << endl;
	//wxTheApp->OnRun();
	//thread = new MyWorkerThread();
	//thread->Run();
}


class CodeLiteParserApp : public wxAppConsole
{
	
	virtual void ProcessPendingEvents()
	{
		wxAppConsole::ProcessPendingEvents();
	}
	
	virtual bool OnInit()
	{
		//timer.Start(1000, false);
		cout << "CLPA::OnInit" << endl;
		//RunApp();
		return true;
	}

	
	
};
IMPLEMENT_APP_NO_MAIN(CodeLiteParserApp)

	/*
static wxAppConsole *wxCreateApp()
{
	wxAppConsole::CheckBuildOptions(WX_BUILD_OPTIONS_SIGNATURE,"my app");
	return new CodeLiteParserApp;
}
*/
// the only public function, declared also in library's header


void WXEXPORT AddCommandEvent()
{
	wxCommandEvent e;
	e.SetId(1);
	e.SetEventType(wxEVT_COMMAND_BUTTON_CLICKED);

	cout << "Adding command event" << endl;
	receiver.AddPendingEvent(e);
}

void WXEXPORT ProcessEvents()
{
	//cout << "In ProcessEvents()" << endl;
	if(wxTheApp && wxTheApp->HasPendingEvents())
	{
		cout << "Processing events" << endl;
		wxTheApp->ProcessPendingEvents();
		//receiver.ProcessPendingEvents();
	}
}


wxThread::ExitCode MyWorkerThread::Entry()
{
	//wxApp::SetInitializerFunction(wxCreateApp);
	cout << "MyWorkerThread::Entry" << endl;
	wxInitialized = true;
	//wxEntry(0,NULL);
	wxTheApp->OnInit();
	wxTheApp->MainLoop();

	return 0;
}

