/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef _LOGLISTCTRL_H_
#define _LOGLISTCTRL_H_

#include <wx/log.h>

class wxListCtrl;

class LogListCtrl : public wxLog
{
public:
    LogListCtrl(wxListCtrl* list);
    virtual void DoLogRecord(wxLogLevel WXUNUSED(level), const wxString& msg, const wxLogRecordInfo& info);

private:
    wxListCtrl* m_List;
};

#endif
