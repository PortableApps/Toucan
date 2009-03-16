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
	//Constructor
    LogListCtrl(wxListCtrl* list);
    //Catch the logs
    virtual void DoLog(wxLogLevel level, const wxChar *msg, time_t timestamp);

private:
    wxListCtrl* m_List;
};

#endif
