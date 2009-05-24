/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 - 2009 Steven Lamerton
// Licence:     GNU GPL 2 (See readme for more info
/////////////////////////////////////////////////////////////////////////////////

#include "waitthread.h"
#include "backupprocess.h"
#include "toucan.h"
#ifdef __WXMSW__
	#include <windows.h>
#endif

void *WaitThread::Entry(){
	#ifdef __WXMSW__
		HANDLE hProcess=OpenProcess(PROCESS_QUERY_INFORMATION,FALSE,m_PID);
		DWORD lgReturn;
		GetExitCodeProcess(hProcess, &lgReturn);
		while(lgReturn == STILL_ACTIVE){
			if(!m_Process->HasInput()){
				//If there was no input then sleep for a while so we don't thrash the CPU
				wxMilliSleep(500);
				//Also yield for input incase it is backing up a large file
				wxGetApp().Yield();
			}
			GetExitCodeProcess(hProcess, &lgReturn);
		}
	#else
		while(wxProcess::Exists(m_PID)){
			if(!m_Process->HasInput()){
				//If there was no input then sleep for a while so we don't thrash the CPU
				wxMilliSleep(500);
				//Also yield for input incase it is backing up a large file
				wxGetApp().Yield();
			}
		}
	#endif
	return NULL;
}
