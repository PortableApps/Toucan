/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 2009 Steven Lamerton
// Licence:     GNU GPL 2 (See readme for more info
/////////////////////////////////////////////////////////////////////////////////

#include "waitthread.h"
#include "backupprocess.h"
#ifdef __WXMSW__
	#include <windows.h>
#endif

void *WaitThread::Entry(){
	#ifdef __WXMSW__
		HANDLE hProcess=OpenProcess(PROCESS_QUERY_INFORMATION,FALSE,m_PID);
		DWORD lgReturn;
		GetExitCodeProcess(hProcess, &lgReturn);
		while(lgReturn == STILL_ACTIVE){
			m_Process->HasInput();
			GetExitCodeProcess(hProcess, &lgReturn);
		}
	#else
		while(wxProcess::Exists(m_PID)){
			m_Process->HasInput();
		}
	#endif
	return NULL;
}
