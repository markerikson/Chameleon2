/*
*DebuggerGDB Wrapper
*
* Takes the CodeLite(c)2008 by Eran Ifrah version, and wraps it into .NET
* for use in C#.
*
*Start: 7/7/2010
*ver: 0.01
*auth: Benjamin Carhart
*/

//Chameleon Utils
#include "Utilities.h"

//CodeLite libs
#include "debuggergdb.h"
#include "debugger.h"

//DOTNET
#include <msclr\marshal.h>
#using <System.dll> //.NET library
#using <mscorlib.dll>

//WX Widgets Libs
#include <wx/wx.h>
#include <wx/init.h>
#include <wx/string.h>
#include <wx/textfile.h>
#include <wx/msw/private.h>

using namespace System;
using namespace System::Collections::Generic;
using namespace msclr::interop;
using namespace System::Runtime::InteropServices;
using namespace System::Timers;

using namespace std;

//Creating a class with only 1 instance
typedef Singleton<PerserMessageReceiver> ParserMessageReceiverST;

namespace CodeLite	//for the actual codelite object
{
	//objects used by the CodeLite debugger go here

	//My Class
	public ref class CdebuggerWrapper	//'ref' means this is a .NET class
	{
	public:
		//allocate native object on C++ heap
		CdebuggerWrapper()
		{
			//constructor stuff goes here
		}

		//deallocate native object 
		~CdebuggerWrapper()
		{
			//deconstructor stuff goes here
		}

	protected:
		//deallocate object on the finalizer run in case deconstructor was missed
		!CdebuggerWrapper()
		{
			//deconstructor stuff goes here; possibly a reference above?
		}

	private:
		//put the codelite debugger object here
		IDebugger * m_debugger;
		DebuggerInfo m_debuggerInfo;

		//NOTES:
		//CodeLite instantiates the GDB Debugger by a class called IDebugger which
		// returns a reference to the instantiated object.
	}
}

//My attempt at converting the classes from debuggergdb.cpp to .NET classes
	bool Start(String^ path, String^ name, String^ cwd, List<BreakPointInfo^>^ bpList, List<String^>^ cmds)
	{
		wxString sPath = ConvertString(path);
		wxString sName = ConvertString(name);
		wxString sCwd = ConvertString(cwd);

		vector<BreakPointInfo^> vBpList;

		for(int c = 0; c < bpList.size(); c++)
		{
			BPEntryPtr pBP = bpList[c];
			BreakPointInfo^ newBP = BPInfoPointerToBP(pBP);

			vBpList->Add(newBP);	//does this work with vector?
		}

		vector<wxString> vCmds;

		for(int d = 0; d < cmds.size(); d++)
		{
			wxString newCmd = ConvertString(cmds[d]);
			vCmds->Add(newCmd);
		}

		return m_debugger->Start(sPath, sName, sCwd, vBpList, vCmds);
	}

	//need the rest of the 'Start' items here

	bool Run(const String^ args, cost String^ comm)
	{
		//marshal_context context;
		wxString sArgs = ConvertString(args);
		wxString sComm = ConvertString(comm);

		return m_debugger->Run(sArgs, sComm);
	}

	bool Stop()
	{
		return m_debugger->Stop();
	}

	bool Break(const BreakpointInfo& bp)
	{
		//not sure if a .NET conversion is needed?  If so, need to .NET convert
		//BreakpointInfo in [debugger.h]
	}

	bool SetEnabledState(const int bid, const bool enable)
	{
		return m_debugger->SetEnabledState(bid, enable);
	}

	bool SetIgnoreLevel(const int bid, const int ignorecount)
	{
		return m_debugger->SetIgnoreLevel(bid, ignorecount);
	}

	bool SetCondition(const BreakpointInfo& bp)
	{
		//see BREAK above
	}

	bool SetCommands(const BreakpointInfo& bp)
	{
		//see BREAK above
	}

	bool RemoveBreak(int bid)
	{
		return m_debugger->RemoveBreak(bid);
	}

	bool RemoveAllBreaks()
	{return m_debugger->RemoveAllBreaks();}

	bool StepIn()
	{return m_debugger->StepIn();}

	bool StepOut()
	{return m_debugger->StepOut();}

	bool Next()
	{return m_debugger->Next();}

	bool Continue()
	{return m_debugger->Continue();}

	bool QueryFileLine()
	{return m_debugger->QueryFileLine();}

	bool Interrupt()
	{return m_debugger->Interrupt();}

	bool IsRunning()
	{return m_debugger->IsRunning();}

	bool ExecuteCmd(const String^ cmd)
	{
	}

//EXAMPLE 1
//====================================
//Original-
//bool EvaluateExpressionToString(const wxString &expression, const wxString &format);
	bool EvaluateExpressionToString(String^ expression, String^ format)
	{
		wxString sExpression = ConvertString(expression);
		wxString sFormat = ConvertString(format);

		//m_debugger refers to CodeLite's debugger instance (debugger GDB object)
		return m_debugger->EvaluateExpressionToString(sExpression, sFormat);
	}

	bool QueryLocals()
	{return m_debugger->QueryLocals();}

	bool ListFrames()
	{return m_debugger->ListFrames();}

	bool ListThreads()
	{return m_debugger->ListThreads();}