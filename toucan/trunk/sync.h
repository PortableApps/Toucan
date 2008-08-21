/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SYNC
#define H_SHYC

#include <wx/thread.h>

#include "syncdata.h"

class SyncThread : public wxThread
{
public:
	//Constructor
	SyncThread(SyncData data, Rules rules, frmMain *main):wxThread(wxTHREAD_JOINABLE){
		m_Data = data;
		m_Rules = rules;
		m_Main = main;
	}
	//Main function, all stuff goes in here
	virtual void *Entry();

private:
	SyncData m_Data;
	Rules m_Rules;
	frmMain* m_Main;
};

bool SyncLoop(SyncData data, Rules rules);
bool SyncFile(SyncData data, Rules rules);

bool DirectoryRemove(wxString strLocation);

bool FolderTimeLoop(wxString strFrom, wxString strTo);

#endif
