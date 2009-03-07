/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "toucan.h"
#include "sync.h"
#include "forms/frmprogress.h"

void *SyncThread::Entry(){

	wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, ID_SCRIPTFINISH);
	wxPostEvent(wxGetApp().ProgressWindow, event);
	return NULL;
}

std::vector<wxString> SyncThread::FolderContentsToVector(wxString path){
	
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

