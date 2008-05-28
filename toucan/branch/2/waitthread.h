/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 Steven Lamerton
// Licence:     GNU GPL 2 (See readme for more info
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_WAIT
#define H_WAIT

#include "backupprocess.h"

class WaitThread : public wxThread
{
public:
	//Constructor
	WaitThread(long pid, PipedProcess *process):wxThread(wxTHREAD_JOINABLE){
		m_PID = pid;
		m_Process = process;
	}
	//Main function, all stuff goes in here
	virtual void *Entry();

private:
	long m_PID;
	PipedProcess *m_Process;
};

void *WaitThread::Entry(){
	HANDLE hProcess=OpenProcess(PROCESS_QUERY_INFORMATION,FALSE,m_PID);
	DWORD lgReturn;
	GetExitCodeProcess(hProcess, &lgReturn);
	while(lgReturn == STILL_ACTIVE){
	//	wxMutexGuiEnter();
		m_Process->HasInput();
	//	wxMutexGuiLeave();
		GetExitCodeProcess(hProcess, &lgReturn);
	}
	return NULL;
}

#endif
