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
	std::vector<wxString> source = FolderContentsToVector(m_Data.GetSource());
	std::vector<wxString> destination = FolderContentsToVector(m_Data.GetDest());
	wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, ID_SCRIPTFINISH);
	wxPostEvent(wxGetApp().ProgressWindow, event);
	return NULL;
}

std::vector<wxString> SyncThread::FolderContentsToVector(wxString path){
	std::vector<wxString> paths;
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

std::map<wxString, location> SyncThread::MergeVectorsToMap(std::vector<wxString> sourcevec, std::vector<wxString> destinationvec){
	
}

bool SyncThread::OperationCaller(std::map<wxString, location>){
	
}

bool SyncThread::OnSourceNotDestination(wxString path){
	
}

bool SyncThread::OnNotSourceDestination(wxString path){
		
}
bool SyncThread::OnSourceAndDestination(wxString path){
	
}

