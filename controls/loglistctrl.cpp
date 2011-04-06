/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 - 2011 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include "loglistctrl.h"
#include "../basicfunctions.h"
#include <wx/datetime.h>
#include <wx/listctrl.h>

LogListCtrl::LogListCtrl(wxListCtrl* list){
	m_List = list;
}

void LogListCtrl::DoLogRecord(wxLogLevel level, const wxString& msg, const wxLogRecordInfo& info){
    //Discard verbose output, they should only be output to a file
    if(level <= wxLOG_Info)
        return;
    OutputProgress(msg, Error);
}
