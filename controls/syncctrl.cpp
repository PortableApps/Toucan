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
	wxCommandEvent* event = new wxCommandEvent(TRAVERSER_FINISHED, ID_TRAVERSED);
	event->SetInt(GetId());
	event->SetClientData(items);
	wxQueueEvent(m_Handler, event);
	return NULL;
}

SyncPreviewDirCtrl::SyncPreviewDirCtrl(wxWindow* parent, wxWindowID id, SyncType type,
									   SyncPreviewDirCtrl *ctrl, const wxPoint& pos,
									   const wxSize& size, long style)
									   :m_Type(type), m_Ctrl(ctrl), PreviewDirCtrl(parent, id, pos, size, style){
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

DirThread* SyncPreviewDirCtrl::GetThread(const wxString& path){
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
			return new SyncPreviewThread(trimmedpath, opp , m_Type, data, this);
		}
		else{
			return new SyncPreviewThread(opp, trimmedpath , m_Type, data, this);			
		}
	}
	else{
		return new DirThread(path, this);
	}
}
