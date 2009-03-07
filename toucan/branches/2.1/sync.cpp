/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "toucan.h"
#include "sync.h"
#include "forms/frmprogress.h"
#include <wx/dir.h>

void *SyncThread::Entry(){
	std::list<wxString> source = FolderContentsToList(m_Data.GetSource());
	std::list<wxString> destination = FolderContentsToList(m_Data.GetDest());
	wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, ID_SCRIPTFINISH);
	wxPostEvent(wxGetApp().ProgressWindow, event);
	return NULL;
}

std::list<wxString> SyncThread::FolderContentsToList(wxString path){
	std::list<wxString> paths;
	//Tidy up the path the make sure it is a directory
	if (path[path.length()-1] != wxFILE_SEP_PATH) {
		path += wxFILE_SEP_PATH;       
	}
	if(wxDirExists(path)){
		wxString filename;
		wxDir dir(path);
		if(dir.GetFirst(&filename)){
			do{
				paths.push_back(filename);
			}
			while(dir.GetNext(&filename));
		}
	}
	return paths;
}

std::map<wxString, location> SyncThread::MergeListsToMap(std::list<wxString> sourcelist, std::list<wxString> destlist){
	std::map<wxString, location> mergeresult;
	//Iterate through all of the paths in the source vector
	std::list<wxString>::iterator sourceiter;
	for(sourceiter = sourcelist.begin(); sourceiter != sourcelist.end(); ++sourceiter){
	}
	return mergeresult;
}

bool SyncThread::OperationCaller(std::map<wxString, location>){
	
}

bool SyncThread::OnSourceNotDest(wxString path){
	
}

bool SyncThread::OnNotSourceDest(wxString path){
		
}
bool SyncThread::OnSourceAndDest(wxString path){
	
}

