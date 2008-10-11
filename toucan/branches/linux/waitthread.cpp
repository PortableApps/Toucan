/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 Steven Lamerton
// Licence:     GNU GPL 2 (See readme for more info
/////////////////////////////////////////////////////////////////////////////////

#include "waitthread.h"
//#include "backupprocess.h"

void *WaitThread::Entry(){
	#ifdef __WXMSW__
		HANDLE hProcess=OpenProcess(PROCESS_QUERY_INFORMATION,FALSE,m_PID);
		DWORD lgReturn;
		GetExitCodeProcess(hProcess, &lgReturn);
		while(lgReturn == STILL_ACTIVE){
			m_Process->HasInput();
			GetExitCodeProcess(hProcess, &lgReturn);
		}
	#endif
	return NULL;
}
