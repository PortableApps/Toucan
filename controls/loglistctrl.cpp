/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 - 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include "loglistctrl.h"
#include "../basicfunctions.h"
#include <wx/datetime.h>
#include <wx/listctrl.h>

LogListCtrl::LogListCtrl(wxListCtrl* list){
	m_List = list;
}

void LogListCtrl::DoLog(wxLogLevel WXUNUSED(level), const wxString& msg, time_t WXUNUSED(timestamp)){
    OutputProgress(msg, Error);
}
