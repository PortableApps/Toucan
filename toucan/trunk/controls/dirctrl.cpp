/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "dirctrl.h"

#include <wx/artprov.h>

BEGIN_EVENT_TABLE(DirCtrl, wxTreeCtrl)
	EVT_TREE_ITEM_EXPANDING(-1, DirCtrl::OnNodeExpand)
END_EVENT_TABLE()


DirCtrl::DirCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
		: wxTreeCtrl(parent, id, pos, size, style)
{
	AddRoot(wxT("Hidden Root"));
	m_Image = new wxImageList(16, 16);
	m_Image->Add(wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16, 16)));
	m_Image->Add(wxArtProvider::GetBitmap(wxART_FOLDER, wxART_OTHER, wxSize(16, 16)));
	AssignImageList(m_Image);
}

DirCtrl::~DirCtrl(){
	DeleteAllItems();
}

void DirCtrl::AddItem(DirCtrlItem *item){
	if(OnAddItem(item)){
		wxTreeItemId id = this->AppendItem(this->GetRootItem(), item->GetCaption(), item->GetIcon(), item->GetIcon(), item);
		if(item->GetType() == DIRCTRL_FILE || item->GetType() == DIRCTRL_ROOT){
			AddDirectory(item, GetScanDepth());
			Expand(id);
		}
	}
}

bool DirCtrl::OnAddItem(DirCtrlItem *item){
	return true;
}

void DirCtrl::AddDirectory(DirCtrlItem *item, int depth){
	if(depth == -1 || depth > 0){
		//If we have not yet added this directory then do so
		if(GetChildrenCount(item->GetId()) == 0){
			DirCtrlItemArray items;
			wxArrayString files;
			DirCtrlTraverser traverser(&files);

			wxDir dir(item->GetFullPath());
			if(dir.IsOpened()){
				dir.Traverse(traverser);
			}
			else{
				return;
			}
			OnAddDirectory(&items);
		}
	}
}

void DirCtrl::OnAddDirectory(DirCtrlItemArray *items){
	
}

void DirCtrl::OnNodeExpand(wxTreeEvent &event){
	
}


