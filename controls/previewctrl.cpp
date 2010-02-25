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

void* PreviewTraverserThread::Entry(){
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
				wxString path = m_Path + wxFILE_SEP_PATH + filename;
				DirCtrlItem* item = new DirCtrlItem(path);
				if(m_Rules != NULL){
					if(m_Rules->ShouldExclude(path, wxDirExists(path))){
						item->SetColour(wxColour("Red"));
					}
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
	wxCommandEvent* event = new wxCommandEvent(TRAVERSER_FINISHED, ID_TRAVERSED);
	event->SetInt(GetId());
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

DirTraverserThread* PreviewDirCtrl::GetThread(const wxString& path, wxEvtHandler* handler){
	return new PreviewTraverserThread(path, m_Rules, handler);
}
