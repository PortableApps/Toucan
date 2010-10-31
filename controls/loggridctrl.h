/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 - 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_LOGGRIDCTRL
#define H_LOGGRIDCTRL

#include <wx/log.h>

class wxGrid;

class LogGridCtrl : public wxLog
{
public:
	//Constructor
    LogGridCtrl(wxGrid* grid);
    //Catch the logs
    virtual void DoLog(wxLogLevel WXUNUSED(level), const wxString& msg, time_t timestamp);

private:
    wxGrid* Grid;
};

#endif
