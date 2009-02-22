/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "extendeddirctrl.h"
#include <wx/arrstr.h>
#include <wx/wx.h>

ExtendedDirCtrl::ExtendedDirCtrl(wxWindow* parent, const wxWindowID id = -1, const wxString& dir = wxDirDialogDefaultFolderStr,
								const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxBORDER_THEME){
									
		wxGenericDirCtrl::Create(parent, id, dir, pos, size, style);
		this->GetTreeCtrl()->SetWindowStyle(this->GetTreeCtrl()->GetWindowStyle()|wxTR_MULTIPLE );
}

wxArrayString ExtendedDirCtrl::GetSelectedPaths(){
	wxArrayString arrPaths;
	wxArrayTreeItemIds items;
	this->GetTreeCtrl()->GetSelections(items);
	for(unsigned int i = 0; i < items.Count(); i++){
		wxTreeItemId id = items.Item(i);
		wxDirItemData* data = (wxDirItemData*) this->GetTreeCtrl()->GetItemData(id);
		arrPaths.Add(data->m_path);
	}
	return arrPaths;
}
