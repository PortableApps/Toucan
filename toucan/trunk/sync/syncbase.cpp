/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "syncbase.h"
#include "../rules.h"
#include "../toucan.h"
#include "../settings.h"
#include "../basicfunctions.h"
#include "../data/syncdata.h"
#include <wx/dir.h>
#include <wx/filefn.h>

SyncBase::SyncBase(){
	this->disablestreams = wxGetApp().m_Settings->GetDisableStream();
}

SyncBase::~SyncBase(){
	;
}

std::list<const wxString> SyncBase::FolderContentsToList(wxString path){
	std::list<const wxString> paths;
	//Tidy up the path the make sure it is a directory
	if (path[path.length()-1] != wxFILE_SEP_PATH) {
		path += wxFILE_SEP_PATH;       
	}
	if(!preview){
		if(!wxDirExists(path)){
			wxMkdir(path);
		}
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

std::map<const wxString, short> SyncBase::MergeListsToMap(std::list<const wxString> sourcelist, std::list<const wxString> destlist){
	std::list<const wxString>::iterator iter;
	std::list<const wxString>::iterator destiter;
	std::map<const wxString, short> mergeresult;
	for(iter = sourcelist.begin(); iter != sourcelist.end(); ++iter){
		bool destexists = false;
		for(destiter = destlist.begin(); destiter != destlist.end(); ++destiter){
			if(*destiter == *iter){
				destexists = true;
				destlist.erase(destiter);
				break;
			}
		}
		short where = 1;
		if(destexists){
			where = 3;
		}
		mergeresult[*iter] = where;
	}
	for(destiter = destlist.begin(); destiter != destlist.end(); ++destiter){
		mergeresult[*destiter] = 2;
	}
	return mergeresult;
}

bool SyncBase::OperationCaller(std::map<const wxString, short> paths){
	for(std::map<const wxString, short>::iterator iter = paths.begin(); iter != paths.end(); ++iter){
		if(wxGetApp().GetAbort()){
			return true;
		}
		if(wxDirExists(sourceroot + wxFILE_SEP_PATH + (*iter).first) || wxDirExists(destroot + wxFILE_SEP_PATH + (*iter).first)){
			if((*iter).second == 1){
				OnSourceNotDestFolder((*iter).first);
			}
			else if((*iter).second == 2){
				OnNotSourceDestFolder((*iter).first);				
			}
			else if((*iter).second == 3){
				OnSourceAndDestFolder((*iter).first);
			}
		}
		//We have a file
		else{
			if((*iter).second == 1){
				OnSourceNotDestFile((*iter).first);
			}
			else if((*iter).second == 2){
				OnNotSourceDestFile((*iter).first);				
			}
			else if((*iter).second == 3){
				OnSourceAndDestFile((*iter).first);
			}
			//Update the progress bar for files only
			if(wxGetApp().GetUsesGUI()){
				IncrementGauge();
				//If we have a file in both folders then increment again as we only do one pass
				if((*iter).second == 3){
					//But only if we are in a two way sync
					if(data->GetFunction() == _("Mirror") || data->GetFunction() == _("Equalise")){
						IncrementGauge();	
					}
				}
			}
		}
	}
	return true;
}
