/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include "syncctrl.h"
#include "../rules.h"
#include "../path.h"
#include "../sync/syncpreview.h"
#include "../data/syncdata.h"

#include <wx/log.h>
#include <algorithm>

void* SyncPreviewThread::Entry(){
	SyncPreview preview(m_Path, m_OppPath, m_Data, (m_Type == SYNC_SOURCE) ? true : false);
	DirCtrlItemArray* items = new DirCtrlItemArray(preview.Execute());

	//Sort the items, perhaps in the future the comparison method shoulf move
	//to a seperare call so it can be easily changed in inherited classes
	std::sort(items->begin(), items->end(), DirCtrlItemComparison);

	//Send the results back to the calling DirCtrl which takes ownership 
	//of the vector
    wxTreeEvent *event = new wxTreeEvent(TRAVERSER_FINISHED, ID_TRAVERSED);
    event->SetItem(m_Parent);
    event->SetClientData(items);
	wxQueueEvent(m_Handler, event);

	return NULL;
}

SyncPreviewDirCtrl::SyncPreviewDirCtrl(wxWindow* parent, wxWindowID id, SyncType type,
									   const wxPoint& pos,
									   const wxSize& size, long style)
									   :m_Type(type), PreviewDirCtrl(parent, id, pos, size, style){
   m_Preview = false;  
   m_Root = wxEmptyString;
}

void SyncPreviewDirCtrl::AddItem(DirCtrlItem *item){
	m_Root = item->GetFullPath();
	DeleteChildren(GetRootItem());
	DirCtrl::AddItem(item);
}

void SyncPreviewDirCtrl::AddItem(const wxString &path){
	if(path == wxEmptyString){
		return;
	}
	//We only get directories in a sync control
	AddItem(new DirCtrlItem(wxFileName::DirName(path)));
}

void SyncPreviewDirCtrl::OnTraversed(wxTreeEvent &event){
	wxTreeItemId parent = event.GetItem();
	if(parent.IsOk()){
		DirCtrlItemArray* items = static_cast<DirCtrlItemArray*>(event.GetClientData());
        if(this->GetPreviewChanges() && items->empty()){
            wxTreeItemId newparent = this->GetItemParent(parent);
            this->Delete(parent);
            while(newparent.IsOk() && !this->ItemHasChildren(newparent)){
                parent = newparent;
                newparent = this->GetItemParent(parent);
                this->Delete(parent);
            }
            return;
        }
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

DirThread* SyncPreviewDirCtrl::GetThread(const wxString& path, wxTreeItemId parent){
	if(m_Preview){
		SyncData *data = new SyncData(wxT("PreviewJob"));
		data->TransferFromForm(wxGetApp().MainWindow);
        data->SetSource(Path::Normalise(data->GetSource()));
        data->SetDest(Path::Normalise(data->GetDest()));
		//Remove a trailing slash if we have one
		wxString trimmedpath = path;
		if(trimmedpath.EndsWith(wxFILE_SEP_PATH)){
			trimmedpath = path.BeforeLast(wxFILE_SEP_PATH);
		}
		//Likewise for the opposite path
		wxString opproot = m_Ctrl->GetRoot();
		if(opproot.EndsWith(wxFILE_SEP_PATH)){
			opproot = opproot.BeforeLast(wxFILE_SEP_PATH);
		}
		wxString opp = opproot + wxFILE_SEP_PATH + trimmedpath.Right(path.Length() - m_Root.Length());
		if(m_Type == SYNC_SOURCE){
			return new SyncPreviewThread(trimmedpath, opp , m_Type, data, parent, this);
		}
		else{
			return new SyncPreviewThread(opp, trimmedpath , m_Type, data, parent, this);			
		}
	}
	else{
		return new DirThread(path, parent, this);
	}
}
