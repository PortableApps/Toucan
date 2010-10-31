/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 - 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////
#include "loggridctrl.h"
#include "../basicfunctions.h"
#include <wx/datetime.h>
#include <wx/grid.h>

LogGridCtrl::LogGridCtrl(wxGrid* grid){
	Grid = grid;
}

void LogGridCtrl::DoLog(wxLogLevel WXUNUSED(level), const wxString& msg, time_t WXUNUSED(timestamp)){
	OutputProgress(msg, true, true);
}
