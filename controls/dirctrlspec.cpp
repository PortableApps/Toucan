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


PreviewDirCtrl::PreviewDirCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos,
						   const wxSize& size, long style) : DirCtrl(parent, id, pos, size, style){
   m_Rules = NULL;
}

PreviewDirCtrl::~PreviewDirCtrl(){
	if(m_Rules != NULL){
		delete m_Rules;
	}
}

void PreviewDirCtrl::OnTraversed(wxCommandEvent &event){
	DirCtrlItemArray* items = static_cast<DirCtrlItemArray*>(event.GetClientData());
	for(DirCtrlItemArray::iterator iter = items->begin(); iter != items->end(); ++iter){
		wxTreeItemId temp = m_IdMap[event.GetExtraLong()];
		wxTreeItemId id = AppendItem(m_IdMap[event.GetExtraLong()], (*iter)->GetCaption(), (*iter)->GetIcon(), (*iter)->GetIcon(), *iter);
		//If the item should be excluded then colour it if applicable
		if(m_Rules != NULL){
			if(m_Rules->ShouldExclude((*iter)->GetFullPath(), ((*iter)->GetType() == DIRCTRL_FOLDER))){
				SetItemTextColour(id, wxColour(wxT("Red")));
			}
		}
		DirCtrlItem* item = static_cast<DirCtrlItem*>(GetItemData(id));
		if(item->GetType() == DIRCTRL_FOLDER){
			AddDirectory(item, (event.GetInt() == -1 ? -1 : event.GetInt() + 1));
		}
	}
}