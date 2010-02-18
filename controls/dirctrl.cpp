/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009-2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include "dirctrl.h"
#include <algorithm>
#include <wx/event.h>
#include <wx/log.h>
#include <wx/artprov.h>

wxDEFINE_EVENT(TRAVERSER_FINISHED, wxCommandEvent);

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

DirCtrlTraverser* DirTraverserThread::GetTraverser(){
	DirCtrlItemArray* items = new std::vector<DirCtrlItem*>();
	return new DirCtrlTraverser(items);
}

void* DirTraverserThread::Entry(){
	//Get the traverser and traverse
	DirCtrlTraverser* traverser = GetTraverser();
	wxDir dir(m_Path);
	if(dir.IsOpened()){
		//To stop errors on failing to parse some folders
		wxLogNull null;
		dir.Traverse(*traverser);
	}

	//Sort the items, perhaps in the future the comparison method shoulf move
	//to a seperare call so it can be easily changed in inherited classes
	std::sort(traverser->GetItems()->begin(), traverser->GetItems()->end(), DirCtrlItemComparison);

	//Send the results back to the calling DirCtrl which takes ownership 
	//of the vector
	wxCommandEvent* event = new wxCommandEvent(TRAVERSER_FINISHED, ID_TRAVERSED);
	event->SetExtraLong(this->GetId());
	event->SetInt(m_Depth);
	event->SetExtraLong(GetId());
	event->SetClientData(traverser->GetItems());
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
	SetScanDepth(2);
	AddRoot(wxT("Hidden Root"));
	m_Image = new wxImageList(16, 16);
	m_Image->Add(wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER));
	m_Image->Add(wxArtProvider::GetBitmap(wxART_FOLDER, wxART_OTHER));
	m_Image->Add(wxArtProvider::GetBitmap(wxART_HARDDISK, wxART_OTHER));
	m_Image->Add(wxArtProvider::GetBitmap(wxART_EXECUTABLE_FILE , wxART_OTHER));
	AssignImageList(m_Image);
}

DirCtrl::~DirCtrl(){
	DeleteAllItems();
}

void DirCtrl::AddItem(DirCtrlItem *item){
	wxTreeItemId id = this->AppendItem(this->GetRootItem(), item->GetCaption(), item->GetIcon(), item->GetIcon(), item);
	if(item->GetType() == DIRCTRL_FOLDER || item->GetType() == DIRCTRL_ROOT){
		AddDirectory(item, 0);
		Expand(id);
	}
}

void DirCtrl::AddItem(const wxString &path){
	wxFileName name(path);
	DirCtrlItem *item = new DirCtrlItem(name);
	AddItem(item);
}

void DirCtrl::AddDirectory(DirCtrlItem *item, int depth){
	if(depth == -1 || depth < m_ScanDepth){
		//If we have not yet added this directory then do so
		if(GetChildrenCount(item->GetId()) == 0 && item->GetType() != DIRCTRL_FILE){
			DirTraverserThread *thread = new DirTraverserThread(item->GetFullPath(), depth, this);
			thread->Create();
			m_IdMap[thread->GetId()] = item->GetId();
			thread->Run();
		}
	}
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
				AddDirectory(item, 0);
			}
			child = GetNextChild(parent, cookie);
		}

	}
}

void DirCtrl::OnTraversed(wxCommandEvent &event){
	DirCtrlItemArray* items = static_cast<DirCtrlItemArray*>(event.GetClientData());
	for(DirCtrlItemArray::iterator iter = items->begin(); iter != items->end(); ++iter){
		wxTreeItemId temp = m_IdMap[event.GetExtraLong()];
		wxTreeItemId id = AppendItem(m_IdMap[event.GetExtraLong()], (*iter)->GetCaption(), (*iter)->GetIcon(), (*iter)->GetIcon(), *iter);
		DirCtrlItem* item = static_cast<DirCtrlItem*>(GetItemData(id));
		if(item->GetType() == DIRCTRL_FOLDER){
			AddDirectory(item, (event.GetInt() == -1 ? -1 : event.GetInt() + 1));
		}
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

void DirCtrl::ExpandAll(){
	wxBusyCursor busy;
	Freeze();
	wxTreeCtrl::ExpandAll();
	Thaw();
}
