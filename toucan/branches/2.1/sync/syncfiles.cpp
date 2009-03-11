// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "syncbase.h"
#include "syncfiles.h"
#include "../md5.h"
#include "../toucan.h"
#include "../basicfunctions.h"
#include <list>
#include <map>
#include <wx/string.h>
#include <wx/datetime.h>
#include <wx/filename.h>
#include <wx/dir.h>

// ATTN : All of the options still need to be implemented

SyncFiles::SyncFiles(wxString syncsource, wxString syncdest, SyncData* syncdata, Rules syncrules){
	this->sourceroot = syncsource;
	this->destroot = syncdest;
	this->data = syncdata;
	this->rules = syncrules;
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
	//Whatever function we have we always copy in this case
	CopyFile(source, dest);
	IncrementGauge();
	return true;
}
bool SyncFiles::OnNotSourceDestFile(wxString path){
	wxString source = sourceroot + wxFILE_SEP_PATH + path;
	wxString dest = destroot + wxFILE_SEP_PATH + path;
	if(data->GetFunction() == _("Mirror")){
		//We have a file that is not in the soure, so delete it
		if(wxRemoveFile(dest)){
			OutputProgress(_("Removed  ") + data->GetPreText() + source.Right(source.Length() - data->GetLength()));			
		}
	}
	else if(data->GetFunction() == _("Equalise")){
		//Swap them around as we are essentially in reverse
		UpdateFile(dest, source);
	}
	IncrementGauge();
	return true;
}
bool SyncFiles::OnSourceAndDestFile(wxString path){
	wxString source = sourceroot + wxFILE_SEP_PATH + path;
	wxString dest = destroot + wxFILE_SEP_PATH + path;
	if(data->GetFunction() == _("Copy") || data->GetFunction() == _("Update")){
		//Use the hash check version to minimise copying
		CopyFileHash(source, dest);
	}
	else if(data->GetFunction() == _("Update")){
		UpdateFile(source, dest);
	}
	IncrementGauge();
	return true;
}
bool SyncFiles::OnSourceNotDestFolder(wxString path){
	//Call the function on the next subfolder
	wxString source = sourceroot + wxFILE_SEP_PATH + path;
	wxString dest = destroot + wxFILE_SEP_PATH + path;
	//Always recurse into the next directory
	SyncFiles sync(source, dest, data, rules);
	sync.Execute();
	return true;
}
bool SyncFiles::OnNotSourceDestFolder(wxString path){
	wxString source = sourceroot + wxFILE_SEP_PATH + path;
	wxString dest = destroot + wxFILE_SEP_PATH + path;
	if(data->GetFunction() == _("Mirror")){
		RemoveDirectory(dest);
	}
	if(data->GetFunction() == _("Equalise")){
		SyncFiles sync(source, dest, data, rules);
		sync.Execute();
	}
	return true;
}
bool SyncFiles::OnSourceAndDestFolder(wxString path){
	wxString source = sourceroot + wxFILE_SEP_PATH + path;
	wxString dest = destroot + wxFILE_SEP_PATH + path;
	//Always recurse into the next directory
	SyncFiles sync(source, dest, data, rules);
	sync.Execute();
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
	return true;
}

bool SyncFiles::UpdateFile(wxString source, wxString dest){
	wxDateTime tmTo, tmFrom;
	wxFileName flTo(dest);
	wxFileName flFrom(source);
	flTo.GetTimes(NULL, &tmTo, NULL);
	flFrom.GetTimes(NULL, &tmFrom, NULL);		

	if(tmFrom.IsLaterThan(tmTo)){
		CopyFileHash(source, dest);
	}
	return true;
}

bool SyncFiles::CopyFileHash(wxString source, wxString dest){
	wxMD5* md5 = new wxMD5();
	if(md5->GetFileMD5(source) != md5->GetFileMD5(dest)){
		CopyFile(source, dest);
	}
	delete md5;
	return true;
}

bool SyncFiles::RemoveDirectory(wxString path){
	// ATTN : Can be replaced when move to wxWidgets 2.9
	if(wxGetApp().ShouldAbort()){
		return true;
	}
	//Make sure that the correct ending is appended
	if (path[path.length()-1] != wxFILE_SEP_PATH) {
		path += wxFILE_SEP_PATH;       
	}
	// if it's a possible root directory
	if (path.length() <= 3) {
		wxLogError(_("Toucan tried to delete a root directory. This has been forbidden for your own safety"));
		return false;
	}
	wxDir* dir = new wxDir(path);
	wxString filename;
	bool blDir = dir->GetFirst(&filename);
	if(blDir){
		do {
			if(wxDirExists(path + filename)){
				RemoveDirectory(path + filename);
			}
			else{
				if(wxRemoveFile(path + filename)){
                    OutputProgress(_("Removed ") + path + filename);
                }
            }
	
		}
		while (dir->GetNext(&filename) );
	} 
	delete dir;
  	wxLogNull log;
	if(wxFileName::Rmdir(path)){
        OutputProgress(_("Removed ") + path);
    }
	return true;
}
