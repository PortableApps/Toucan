/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include "dirctrlspec.h"
#ifdef __WXMSW__
	#include <windows.h>
	#include <wx/msw/winundef.h>
#endif

LocalDirCtrl::LocalDirCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos,
						   const wxSize& size, long style) : DirCtrl(parent, id, pos, size, style){
	ReCreateTree();
}

void LocalDirCtrl::ReCreateTree(){
	DeleteAllItems();
	AddRoot(wxT("Hidden Root"));
#ifdef __WXMSW__
	TCHAR drives[256];
	if(GetLogicalDriveStrings(256, drives)){
		LPTSTR drive = drives;
		int offset = _tcslen(drive) + 1;
		while(*drive){
			AddItem(wxString(drive));
			drive += offset;
		}
	}
#elif __WXGTK__
	AddItem(wxT("/"));
#endif
}
