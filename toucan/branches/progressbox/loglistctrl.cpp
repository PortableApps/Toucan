/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "loglistctrl.h"
#include <wx/datetime.h>
#include <wx/listctrl.h>

LogListCtrl::LogListCtrl(wxListCtrl* list){
	m_List = list;
}

void LogListCtrl::DoLog(wxLogLevel level, const wxChar *msg, time_t timestamp){
	wxDateTime date(timestamp);
	m_List->InsertItem(m_List->GetItemCount(), wxT("NewItem"));
	m_List->SetItem(m_List->GetItemCount() - 1, 1, msg);
	m_List->SetItem(m_List->GetItemCount() - 1, 0, date.FormatTime());
}
