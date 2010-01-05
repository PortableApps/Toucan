/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "dirctrl.h"
#include <algorithm>
#include <wx/artprov.h>

bool DirCtrlItemComparison(DirCtrlItem *a, DirCtrlItem *b){
	if(a->GetType() == DIRCTRL_FOLDER && b->GetType() == DIRCTRL_FILE){
		return true;
	}
	else if(a->GetType() == DIRCTRL_FILE && b->GetType() == DIRCTRL_FOLDER){
		return false;
	}
	else{
		if(a->GetFullPath().CmpNoCase(b->GetFullPath()) >= 0){
			return false;
		}
		else{
			return true;
		}
	}
}

BEGIN_EVENT_TABLE(DirCtrl, wxTreeCtrl)
	EVT_TREE_ITEM_EXPANDING(-1, DirCtrl::OnNodeExpand)
END_EVENT_TABLE()

DirCtrl::DirCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
		: wxTreeCtrl(parent, id, pos, size, style)
{
	SetScanDepth(2);
	AddRoot(wxT("Hidden Root"));
	m_Image = new wxImageList(16, 16);
	m_Image->Add(wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER));
	m_Image->Add(wxArtProvider::GetBitmap(wxART_FOLDER, wxART_OTHER));
	m_Image->Add(wxArtProvider::GetBitmap(wxART_FOLDER, wxART_OTHER));
	AssignImageList(m_Image);
}

DirCtrl::~DirCtrl(){
	DeleteAllItems();
}

void DirCtrl::AddItem(DirCtrlItem *item){
	wxTreeItemId id = this->AppendItem(this->GetRootItem(), item->GetCaption(), item->GetIcon(), item->GetIcon(), item);
	if(item->GetType() == DIRCTRL_FOLDER || item->GetType() == DIRCTRL_ROOT){
		AddDirectory(item, GetScanDepth());
		Expand(id);
	}
}

void DirCtrl::AddItem(const wxString &path){
	wxFileName name(path);
	DirCtrlItem *item = new DirCtrlItem(name);
	AddItem(item);
}

void DirCtrl::AddDirectory(DirCtrlItem *item, int depth){
	if(depth == -1 || depth <= m_ScanDepth){
		//If we have not yet added this directory then do so
		if(GetChildrenCount(item->GetId()) == 0){
			DirCtrlItemArray items;
			DirCtrlTraverser traverser(&items);

			wxDir dir(item->GetFullPath());
			if(dir.IsOpened()){
				dir.Traverse(traverser);
			}
			else{
				return;
			}
			//Sort the remaining items
			std::sort(items.begin(), items.end(), DirCtrlItemComparison);
			//Add them to the tree
			for(DirCtrlItemArray::iterator iter = items.begin(); iter != items.end(); ++iter){
				wxTreeItemId id = AppendItem(item->GetId(), (*iter)->GetCaption(), (*iter)->GetIcon(), (*iter)->GetIcon(), *iter);
			}
		}
		//Now iterate through the nodes to expand any directories that need expanding
		wxTreeItemIdValue cookie = 0;
		DirCtrlItem *newitem;
		wxTreeItemId child = GetFirstChild(item->GetId(), cookie);
		while(child.IsOk()){
			newitem = static_cast<DirCtrlItem*>(GetItemData(child));
			if(newitem->GetType() == DIRCTRL_FOLDER) {
				AddDirectory(newitem, (depth == -1 ? -1 : depth++));
			}
			child = GetNextChild(item->GetId(), cookie);
		}
	}
}

void DirCtrl::OnNodeExpand(wxTreeEvent &event){
	wxTreeItemId id = event.GetItem();
	if (id.IsOk()) {
		DirCtrlItem *item = static_cast<DirCtrlItem*>(GetItemData(id));
		if (item->GetType() == DIRCTRL_FOLDER){
			AddDirectory(item, GetScanDepth());
		}
	}	
}
