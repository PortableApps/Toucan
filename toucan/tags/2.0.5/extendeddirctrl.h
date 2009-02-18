/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_EXTENDEDDIRCTRL
#define H_EXTENDEDDIRCTRL

#include <wx/arrstr.h>
#include <wx/dirctrl.h>

class ExtendedDirCtrl : public wxGenericDirCtrl
{
	
public:
	ExtendedDirCtrl(wxWindow* parent, const wxWindowID id, const wxString& dir, const wxPoint& pos, const wxSize& size, long style);
	wxArrayString GetSelectedPaths();

};

#endif
