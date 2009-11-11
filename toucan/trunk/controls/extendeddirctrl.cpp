/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "extendeddirctrl.h"
#include <wx/arrstr.h>
//#include <wx/wx.h>
#include <wx/tokenzr.h>

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

TreeStateSaver::TreeStateSaver(wxTreeCtrl *tree){
	m_Tree = tree;
	m_Paths = SaveChildren(wxEmptyString, tree->GetRootItem());
}

TreeStateSaver::~TreeStateSaver(){
	for(unsigned int i = 0; i < m_Paths.Count(); i++){
		LoadChildren(m_Paths.Item(i), m_Tree->GetRootItem());
	}
}

wxArrayString TreeStateSaver::SaveChildren(const wxString &path, wxTreeItemId parent){
	wxTreeItemIdValue cookie;
	wxArrayString paths;
	wxTreeItemId child = m_Tree->GetFirstChild(parent, cookie);
	bool anyexpanded = false;
	while(child.IsOk()){
		if(m_Tree->IsExpanded(child)){
			anyexpanded = true;
			wxArrayString newpaths = SaveChildren(path + wxT("|") + m_Tree->GetItemText(child), child);
			for(unsigned int i = 0; i < newpaths.Count(); i++){
				paths.Add(newpaths.Item(i));
			}
		}
		child = m_Tree->GetNextChild(parent, cookie);
	}
	if(!anyexpanded){
		paths.Add(path);
	}
	return paths;
}

void TreeStateSaver::LoadChildren(wxString path, wxTreeItemId parent){
	wxTreeItemIdValue cookie;
	wxTreeItemId child = m_Tree->GetFirstChild(parent, cookie);
	while(child.IsOk()){
		if(m_Tree->GetItemText(child) == path.AfterFirst(wxT('|')).BeforeFirst(wxT('|'))){
			m_Tree->Expand(child);
			LoadChildren(path.AfterFirst(wxT('|')), child);
			break;
		}
		child = m_Tree->GetNextChild(parent, cookie);
	}
	return;
}
