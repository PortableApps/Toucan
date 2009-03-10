// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "syncbase.h"
#include "syncfiles.h"
#include "../basicfunctions.h"
#include <list>
#include <map>
#include <wx/string.h>

SyncFiles::SyncFiles(wxString syncsource, wxString syncdest, SyncData* syncdata){
	this->sourceroot = syncsource;
	this->destroot = syncdest;
	this->data = syncdata;
}

bool SyncFiles::Execute(){
	std::list<wxString> sourcepaths = FolderContentsToList(sourceroot);
	std::list<wxString> destpaths = FolderContentsToList(destroot);
	std::map<wxString, short> mergeresult = MergeListsToMap(sourcepaths, destpaths);
	OperationCaller(mergeresult);
	return true;
}

bool SyncFiles::OnSourceNotDestFile(wxString path){
	wxString source = sourceroot + wxFILE_SEP_PATH + path;
	wxString dest = destroot + wxFILE_SEP_PATH + path;
	if(data->GetFunction() == _("Copy")){
		CopyFile(source, dest);
	}
	return true;
}
bool SyncFiles::OnNotSourceDestFile(wxString path){
	wxString source = sourceroot + wxFILE_SEP_PATH + path;
	wxString dest = destroot + wxFILE_SEP_PATH + path;
	if(data->GetFunction() == _("Copy")){
		//Do nothing
	}
	return true;
}
bool SyncFiles::OnSourceAndDestFile(wxString path){
	wxString source = sourceroot + wxFILE_SEP_PATH + path;
	wxString dest = destroot + wxFILE_SEP_PATH + path;
	if(data->GetFunction() == _("Copy")){
		CopyFile(source, dest);
	}
	return true;
}
bool SyncFiles::OnSourceNotDestFolder(wxString path){
	//Call the function on the next subfolder
	wxString source = sourceroot + wxFILE_SEP_PATH + path;
	wxString dest = destroot + wxFILE_SEP_PATH + path;
	if(data->GetFunction() == _("Copy")){
		SyncFiles sync(source, dest, data);
		sync.Execute();
	}
	return true;
}
bool SyncFiles::OnNotSourceDestFolder(wxString path){
	wxString source = sourceroot + wxFILE_SEP_PATH + path;
	wxString dest = destroot + wxFILE_SEP_PATH + path;
	if(data->GetFunction() == _("Copy")){
		//Do nothing
	}
	return true;
}
bool SyncFiles::OnSourceAndDestFolder(wxString path){
	wxString source = sourceroot + wxFILE_SEP_PATH + path;
	wxString dest = destroot + wxFILE_SEP_PATH + path;
	if(data->GetFunction() == _("Copy")){
		SyncFiles sync(source, dest, data);
		sync.Execute();
	}
	return true;
}

bool SyncFiles::CopyFile(wxString source, wxString dest){
	if(wxCopyFile(source, wxPathOnly(dest) + wxFILE_SEP_PATH + wxT("Toucan.tmp"), true)){
		if(wxRenameFile(wxPathOnly(dest) + wxFILE_SEP_PATH + wxT("Toucan.tmp"), dest, true)){
			OutputProgress(wxT("Copied  ") + data->GetPreText() +  source.Right(source.Length() - data->GetLength()));
		}
	}
	if(wxFileExists(wxPathOnly(dest) + wxFILE_SEP_PATH + wxT("Toucan.tmp"))){
		wxRemoveFile(wxPathOnly(dest) + wxFILE_SEP_PATH + wxT("Toucan.tmp"));
	}
}
