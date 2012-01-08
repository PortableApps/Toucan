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

void SyncPreviewThread(const wxString& path, const wxString &opppath, SyncType type, SyncData *data, wxTreeItemId parent, wxEvtHandler* handler)
{
	SyncPreview preview(path, opppath, data, (type == SYNC_SOURCE) ? true : false);
	DirCtrlItemArray* items = new DirCtrlItemArray(preview.Execute());

	//Sort the items, perhaps in the future the comparison method shoulf move
	//to a seperare call so it can be easily changed in inherited classes
	std::sort(items->begin(), items->end(), DirCtrlItemComparison);

	//Send the results back to the calling DirCtrl which takes ownership 
	//of the vector
    wxTreeEvent *event = new wxTreeEvent(EVT_TRAVERSER_FINISHED, ID_TRAVERSED);
    event->SetItem(parent);
    event->SetClientData(items);
	wxQueueEvent(handler, event);
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
	if(parent){
		DirCtrlItemArray* items = static_cast<DirCtrlItemArray*>(event.GetClientData());
        if(this->GetPreviewChanges() && items->empty()){
            wxTreeItemId newparent = this->GetItemParent(parent);
            this->Delete(parent);
            while(newparent && !this->ItemHasChildren(newparent)){
                parent = newparent;
                newparent = this->GetItemParent(parent);
                this->Delete(parent);
            }
            return;
        }
		for(DirCtrlItemArray::iterator iter = items->begin(); iter != items->end(); ++iter){
			wxTreeItemId id = AppendItem(parent, (*iter)->GetCaption(), (*iter)->GetIcon(), (*iter)->GetIcon(), *iter);
			if(id){
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
}

void SyncPreviewDirCtrl::AddThread(const wxString& path, wxTreeItemId parent, boost::threadpool::pool* pool){
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
			pool->schedule(boost::bind(SyncPreviewThread, trimmedpath, opp , m_Type, data, parent, this));
		}
		else{
			pool->schedule(boost::bind(SyncPreviewThread, opp, trimmedpath , m_Type, data, parent, this));			
		}
	}
	else{
		pool->schedule(boost::bind(DirThread, path, parent, this));
	}
}
