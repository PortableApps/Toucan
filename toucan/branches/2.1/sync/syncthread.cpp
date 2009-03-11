/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "syncfiles.h"
#include "syncthread.h"
#include "../toucan.h"
#include "../forms/frmprogress.h"

void *SyncThread::Entry(){
	SyncFiles sync(m_Data.GetSource(), m_Data.GetDest(), &m_Data, m_Rules);
	sync.Execute();
	wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, ID_SCRIPTFINISH);
	wxPostEvent(wxGetApp().ProgressWindow, event);
	return NULL;
}
