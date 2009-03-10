// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "syncfiles.h"

SyncFiles::SyncFiles(wxString syncsource, wxString syncdest, SyncData* syncdata){
	this->sourceroot = syncsource;
	this->destroot = syncdest;
	this->data = syncdata;
}

bool SyncFiles::Execute(){
	
}

bool SyncFiles::OnSourceNotDestFile(wxString path){
	
}
bool SyncFiles::OnNotSourceDestFile(wxString path){
	
}
bool SyncFiles::OnSourceAndDestFile(wxString path){
	
}
bool SyncFiles::OnSourceNotDestFolder(wxString path){
	
}
bool SyncFiles::OnNotSourceDestFolder(wxString path){
	
}
bool SyncFiles::OnSourceAndDestFolder(wxString path){
	
}
