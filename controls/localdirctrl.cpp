/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include "localdirctrl.h"
#include <wx/window.h>

#ifdef __WXMSW__
	#include <windows.h>
	#include <wx/msw/winundef.h>
#endif

LocalDirCtrl::LocalDirCtrl(wxWindow* parent, int id, const wxPoint& pos,
						   const wxSize& size, long style)
						   : DirCtrl(parent, id, pos, size, style){
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
			AddItem(new DirCtrlItem(wxFileName::DirName(drive)));
			drive += offset;
		}
	}
#elif __WXGTK__
	DirCtrlItem* item = new DirCtrlItem(wxFileName("/"), "root", DIRCTRL_ROOT, 3);
	AddItem(item);
#endif
}
