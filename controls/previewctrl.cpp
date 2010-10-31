/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include "previewctrl.h"
#include "dirctrl.h"
#include "../rules.h"

#include <wx/log.h>
#include <algorithm>

void* PreviewThread::Entry(){
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
				//Simple check to see if we should be excluded, if so colour red
				wxString path = m_Path + filename;
				DirCtrlItem *item;
				if(wxDirExists(path)){
					item = new DirCtrlItem(wxFileName::DirName(path));
				}
				else{
					item = new DirCtrlItem(wxFileName::FileName(path));
				}
				if(m_Rules != NULL && m_Rules->ShouldExclude(path, wxDirExists(path))){
					item->SetColour(wxColour("Red"));
				}
				items->push_back(item);
			} while(dir.GetNext(&filename));
		}
	}

	//Sort the items, perhaps in the future the comparison method shoulf move
	//to a seperare call so it can be easily changed in inherited classes
	std::sort(items->begin(), items->end(), DirCtrlItemComparison);

	//Send the results back to the calling DirCtrl which takes ownership 
	//of the vector
    wxTreeEvent *event = new wxTreeEvent(EVT_TRAVERSER_FINISHED, ID_TRAVERSED);
    event->SetItem(m_Parent);
    event->SetClientData(items);
	wxQueueEvent(m_Handler, event);

	return NULL;
}

PreviewDirCtrl::PreviewDirCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos,
							   const wxSize& size, long style) 
							   : DirCtrl(parent, id, pos, size, style){
   m_Rules = NULL;
}

PreviewDirCtrl::~PreviewDirCtrl(){
	delete m_Rules;
}

void PreviewDirCtrl::AddDirectory(DirCtrlItem *item){
	//If we have not yet added this directory then do so
	if(GetChildrenCount(item->GetId()) == 0 && item->GetType() != DIRCTRL_FILE){
		PreviewThread *thread = new PreviewThread(item->GetFullPath(), item->GetId(), m_Rules, this);
        //Add our new thread to the threadpool
        m_Pool->schedule(*thread);
	}
}
