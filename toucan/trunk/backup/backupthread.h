/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 - 2009 Steven Lamerton
// Licence:     GNU GPL 2 (See readme for more info
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_BACKUPTHREAD
#define H_BACKUPTHREAD

class BackupProcess;
#include <wx/thread.h>

class BackupThread : public wxThread
{
public:
	//Constructor
	BackupThread(long pid, BackupProcess *process):wxThread(wxTHREAD_DETACHED){
		m_PID = pid;
		m_Process = process;
	}
	virtual void *Entry();

private:
	long m_PID;
	BackupProcess *m_Process;
};

#endif
