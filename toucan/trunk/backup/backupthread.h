/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 - 2009 Steven Lamerton
// Licence:     GNU GPL 2 (See readme for more info
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_BACKUPTHREAD
#define H_BACKUPTHREAD

class BackupProcess;
class BackupData;
#include <wx/thread.h>

class BackupThread : public wxThread
{
public:
	//Constructor
	BackupThread(long pid, BackupProcess *process, BackupData *data):wxThread(wxTHREAD_DETACHED){
		m_PID = pid;
		m_Process = process;
		m_Data = data;
	}
	virtual void *Entry();

private:
	long m_PID;
	BackupProcess *m_Process;
	BackupData *m_Data;
};

#endif
