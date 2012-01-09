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

void PreviewThread(const wxString& path, wxTreeItemId parent, RuleSet *rules, wxEvtHandler* handler)
{
	DirCtrlItemArray* items = new DirCtrlItemArray();
	//Traverse though the directory and add each file and folder
	wxDir dir(path);
	if(dir.IsOpened()){
		wxString filename;
		//Supress any warning we might get here about folders we cant open
		wxLogNull null;
		if(dir.GetFirst(&filename)){
			do {
				//Simple check to see if we should be excluded, if so colour red
				wxString fullpath = path + filename;
				DirCtrlItem *item;
                wxFileName name;
                bool dir = wxDirExists(fullpath);

				if(dir)
                    name = wxFileName::DirName(fullpath);
				else
					name = wxFileName::FileName(fullpath);
                item = new DirCtrlItem(name);
				if(rules != NULL && (rules->Matches(name) == Excluded || rules->Matches(name) == AbsoluteExcluded)){
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
    event->SetItem(parent);
    event->SetClientData(items);
	wxQueueEvent(handler, event);
}

PreviewDirCtrl::PreviewDirCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos,
							   const wxSize& size, long style) 
							   : DirCtrl(parent, id, pos, size, style){
   m_Rules = NULL;
}

PreviewDirCtrl::~PreviewDirCtrl(){
	delete m_Rules;
}

void PreviewDirCtrl::AddThread(const wxString& path, wxTreeItemId parent, threadpool* pool){
	pool->schedule(boost::bind(PreviewThread, path, parent, m_Rules, this));
}
