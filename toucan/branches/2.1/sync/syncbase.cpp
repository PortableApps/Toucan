/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "syncbase.h"
#include <wx/dir.h>
#include <wx/filefn.h>

//Just a stub to allow this to correctly be compiled
SyncBase::SyncBase(){
	;
}

SyncBase::~SyncBase(){
	;
}
std::list<wxString> SyncBase::FolderContentsToList(wxString path){
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

std::map<wxString, location> SyncBase::MergeListsToMap(std::list<wxString> sourcelist, std::list<wxString> destlist){
	std::list<wxString>::iterator iter;
	std::list<wxString>::iterator destiter;
	std::map<wxString, location> mergeresult;
	for(iter = sourcelist.begin(); iter != sourcelist.end(); ++iter){
		bool destexists = false;
		for(destiter = destlist.begin(); destiter != destlist.end(); ++destiter){
			if(*destiter == *iter){
				destexists = true;
				destlist.erase(destiter);
				break;
			}
		}
		location where = SOURCE;
		if(destexists){
			where = location(SOURCE|DEST);
		}
		mergeresult[*iter] = where;
	}
	for(destiter = destlist.begin(); destiter != destlist.end(); ++destiter){
		mergeresult[*destiter] = location(DEST);
	}
	return mergeresult;
}

bool SyncBase::OperationCaller(std::map<wxString, location> paths){
	for(std::map<wxString, location>::iterator iter = paths.begin(); iter != paths.end(); ++iter){
		if(wxDirExists(sourceroot + (*iter).first)){
			if((*iter).second == SOURCE){
				OnSourceNotDestFolder((*iter).first);
			}
			else if((*iter).second == DEST){
				OnNotSourceDestFolder((*iter).first);				
			}
			else if((*iter).second == SOURCE|DEST){
				OnSourceAndDestFolder((*iter).first);
			}
		}
		//We have a file
		else{
			if((*iter).second == SOURCE){
				OnSourceNotDestFile((*iter).first);
			}
			else if((*iter).second == DEST){
				OnNotSourceDestFile((*iter).first);				
			}
			else if((*iter).second == SOURCE|DEST){
				OnSourceAndDestFile((*iter).first);
			}			
		}
	}
	return true;
}
