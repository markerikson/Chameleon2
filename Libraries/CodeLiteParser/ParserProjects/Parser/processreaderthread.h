//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah
// file name            : worker_thread.h
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
#ifndef _ProcessReaderThread_H_
#define _ProcessReaderThread_H_

#include <deque>
#include "wx/thread.h"
#include "wx/event.h"

class IProcess;
/**
 * @class ProcessEventData
 * @author eran
 * @date 10/09/09
 * @file i_process.h
 * @brief
 */
class ProcessEventData
{
	wxString  m_data;
	IProcess *m_process;
	int       m_exitCode;

public:
	ProcessEventData() : m_exitCode(0) {}
	virtual ~ProcessEventData() {}

	void SetData(const wxString& data) {
		this->m_data = data.c_str();
	}
	const wxString& GetData() const {
		return m_data;
	}
	void SetProcess(IProcess* process) {
		this->m_process = process;
	}
	IProcess* GetProcess() {
		return m_process;
	}
	void SetExitCode(int exitCode) {
		this->m_exitCode = exitCode;
	}
	int GetExitCode() const {
		return m_exitCode;
	}
};

/**
 * Worker Thread class
 * usually user should define the ProcessRequest method
 */
class ProcessReaderThread : public wxThread
{
protected:
	wxEvtHandler *   m_notifiedWindow;
	IProcess*        m_process;
public:
	/**
	 * Default constructor.
	 */
	ProcessReaderThread();

	/**
	 * Destructor.
	 */
	virtual ~ProcessReaderThread();

	/**
	 * Thread execution point.
	 */
	virtual void *Entry();

	/**
	 * Called when the thread exits
	 * whether it terminates normally or is stopped with Delete() (but not when it is Kill()'ed!)
	 */
	virtual void OnExit() {};


	/**
	 * Set the window to be notified when a change was done
	 * between current source file tree and the actual tree.
	 * \param evtHandler
	 */
	void SetNotifyWindow( wxEvtHandler* evtHandler ) {
		m_notifiedWindow  = evtHandler;
	}

	/**
	 * Stops the thread
	 * This function returns only when the thread is terminated.
	 * \note This call must be called from the context of other thread (e.g. main thread)
	 */
	void Stop();

	/**
	 * Start the thread as joinable thread.
	 * \note This call must be called from the context of other thread (e.g. main thread)
	 */
	void Start(int priority = WXTHREAD_DEFAULT_PRIORITY);

	void SetProcess( IProcess *proc ) {
		m_process = proc;
	}
};

extern const wxEventType wxEVT_PROC_DATA_READ;
extern const wxEventType wxEVT_PROC_TERMINATED;

#endif // _ProcessReaderThread_H_
