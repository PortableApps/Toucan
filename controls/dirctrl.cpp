/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009-2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include "dirctrl.h"
#include "../forms/frmmain.h"
#include "syncctrl.h"
#include <algorithm>
#include <wx/event.h>
#include <wx/log.h>
#include <wx/artprov.h>
#include <wx/stdpaths.h>

wxDEFINE_EVENT(TRAVERSER_FINISHED, wxCommandEvent);

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

DirCtrlItem::DirCtrlItem(const wxFileName &path){
	m_Colour = wxColour("Black");
	m_Path = path;
	if(path.IsDir()){
		/*We add 2 because GetVolume returns C and we expect C:\ */
		if(path.GetVolume().Length() + 2 == path.GetFullPath().Length()){
			m_Caption = path.GetFullPath();
			m_Type = DIRCTRL_ROOT;
#ifdef __WXMSW__
			unsigned int type = GetDriveType(path.GetFullPath());
			if(type == DRIVE_REMOVABLE){
				m_Icon = 5;
			}
			else if(type == DRIVE_CDROM){
				m_Icon = 4;
			}
			else{
				//Default to using the harddisk icon
				m_Icon = 3;
			}
#else
			m_Icon = 3;
#endif
		}
		else{
			m_Caption = path.GetDirs().Last();
			m_Type = DIRCTRL_FOLDER;
			m_Icon = 0;
		}
	}
	else{
		m_Caption = path.GetFullName();
		m_Type = DIRCTRL_FILE;
		m_Icon = (path.GetExt() == "exe") ? 2 : 1;
	}
};

void* DirThread::Entry(){
	DirCtrlItemArray* items = new DirCtrlItemArray();
	//Traverse though the directory and add each file and folder
	wxDir dir(m_Path);
	if(dir.IsOpened()){
		wxString filename;
		//Supress any warning we might get here about folders we cant open
		wxLogNull null;
		bool ok = dir.GetFirst(&filename);
		if(ok){
			do {
				const wxString path = m_Path + filename;
				if(wxDirExists(path)){
					items->push_back(new DirCtrlItem(wxFileName::DirName(path)));
				}
				else{
					items->push_back(new DirCtrlItem(wxFileName::FileName(path)));
				}
			} while(dir.GetNext(&filename));
		}
	}

	//Sort the items, perhaps in the future the comparison method shoulf move
	//to a seperare call so it can be easily changed in inherited classes
	std::sort(items->begin(), items->end(), DirCtrlItemComparison);

	//Send the results back to the calling DirCtrl which takes ownership 
	//of the vector
	wxCommandEvent* event = new wxCommandEvent(TRAVERSER_FINISHED, ID_TRAVERSED);
	event->SetInt(GetId());
	event->SetClientData(items);
	wxQueueEvent(m_Handler, event);
	return NULL;
}

BEGIN_EVENT_TABLE(DirCtrl, wxTreeCtrl)
	EVT_TREE_ITEM_EXPANDING(-1, DirCtrl::OnNodeExpand)
	EVT_COMMAND(ID_TRAVERSED, TRAVERSER_FINISHED, DirCtrl::OnTraversed)
END_EVENT_TABLE()

DirCtrl::DirCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
		: wxTreeCtrl(parent, id, pos, size, style)
{
    m_Expand = false;
	AddRoot(wxT("Hidden Root"));

	const wxString bitmapdir = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + "bitmaps" + wxFILE_SEP_PATH;
	m_Image = new wxImageList(16, 16);
	m_Image->Add(wxBitmap(bitmapdir + wxT("folder.png"), wxBITMAP_TYPE_PNG));
	m_Image->Add(wxBitmap(bitmapdir + wxT("file.png"), wxBITMAP_TYPE_PNG));
	m_Image->Add(wxBitmap(bitmapdir + wxT("file-exe.png"), wxBITMAP_TYPE_PNG));
	m_Image->Add(wxBitmap(bitmapdir + wxT("drive-harddisk.png"), wxBITMAP_TYPE_PNG));
	m_Image->Add(wxBitmap(bitmapdir + wxT("drive-optical.png"), wxBITMAP_TYPE_PNG));
	m_Image->Add(wxBitmap(bitmapdir + wxT("drive-removable-media.png"), wxBITMAP_TYPE_PNG));
	m_Image->Add(wxBitmap(bitmapdir + wxT("folder-open.png"), wxBITMAP_TYPE_PNG));
	AssignImageList(m_Image);
}

DirCtrl::~DirCtrl(){
	DeleteAllItems();
}

void DirCtrl::AddItem(DirCtrlItem *item){
    //As we only ever expand by calling ExpandAll we can turn it off here
    m_Expand = false;
	wxTreeItemId id = this->AppendItem(this->GetRootItem(), item->GetCaption(), item->GetIcon(), item->GetIcon(), item);
	if(item->GetType() == DIRCTRL_FOLDER){
		SetItemImage(id, 6, wxTreeItemIcon_Expanded);
	}
	if(item->GetType() == DIRCTRL_FOLDER || item->GetType() == DIRCTRL_ROOT){
		AddDirectory(item);
	}
}

void DirCtrl::AddItem(const wxString &path){
	if(wxDirExists(path)){
		AddItem(new DirCtrlItem(wxFileName::DirName(path)));
	}
	else{
		AddItem(new DirCtrlItem(wxFileName::FileName(path)));
	}
}

void DirCtrl::AddDirectory(DirCtrlItem *item){
	//If we have not yet added this directory then do so
	if(GetChildrenCount(item->GetId()) == 0 && item->GetType() != DIRCTRL_FILE){
		DirThread *thread = GetThread(item->GetFullPath());
		thread->Create();
		m_IdMap[thread->GetId()] = item->GetId();
		thread->Run();
	}
}

DirThread* DirCtrl::GetThread(const wxString& path){
	return new DirThread(path, this);
}

void DirCtrl::OnNodeExpand(wxTreeEvent &event){
	wxBusyCursor busy;
	wxTreeItemId parent = event.GetItem();
	wxTreeItemIdValue cookie;
	if(parent.IsOk()){
		wxTreeItemId child = GetFirstChild(parent, cookie);
		while(child.IsOk()){
			DirCtrlItem *item = static_cast<DirCtrlItem*>(GetItemData(child));
			if(item->GetType() == DIRCTRL_FOLDER){
				AddDirectory(item);
			}
			child = GetNextChild(parent, cookie);
		}

	}
}

void DirCtrl::OnTraversed(wxCommandEvent &event){
	wxTreeItemId parent = m_IdMap[event.GetInt()];
	if(parent.IsOk()){
		DirCtrlItemArray* items = static_cast<DirCtrlItemArray*>(event.GetClientData());
		for(DirCtrlItemArray::iterator iter = items->begin(); iter != items->end(); ++iter){
			wxTreeItemId id = AppendItem(parent, (*iter)->GetCaption(), (*iter)->GetIcon(), (*iter)->GetIcon(), *iter);
			if((*iter)->GetType() == DIRCTRL_FOLDER){
				SetItemImage(id, 6, wxTreeItemIcon_Expanded);
			}
			SetItemTextColour(id, (*iter)->GetColour());
		}
	}
    if(GetItemParent(parent) == this->GetRootItem() || m_Expand)
    {
        Expand(parent);
    }
}

wxArrayString DirCtrl::GetSelectedPaths(){
	wxArrayString paths;
	wxArrayTreeItemIds items;
	GetSelections(items);
	for(unsigned int i = 0; i < items.Count(); i++){
		wxTreeItemId id = items.Item(i);
		DirCtrlItem* data = static_cast<DirCtrlItem*>(GetItemData(id));
		paths.Add(data->GetFullPath());
	}
	return paths;
}

wxString DirCtrl::GetPath(wxTreeItemId item){
    return static_cast<DirCtrlItem*>(GetItemData(item))->GetFullPath();
}

void DirCtrl::ExpandAll(){
    m_Expand = true;
	wxTreeCtrl::ExpandAll();
}
