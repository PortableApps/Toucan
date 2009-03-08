/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "../basicfunctions.h"
#include "../toucan.h"
#include "../forms/frmprogress.h"
#include "syncthread.h"
#include <wx/dir.h>

void *SyncThread::Entry(){
	std::list<wxString> source = FolderContentsToList(m_Data.GetSource());
	std::list<wxString> dest = FolderContentsToList(m_Data.GetDest());
	std::map<wxString, location> paths = MergeListsToMap(source, dest);
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

bool SyncThread::OperationCaller(std::map<wxString, location> paths){
	for(std::map<wxString, location>::iterator iter = paths.begin(); iter != paths.end(); ++iter){
		if((*iter).second == location(SOURCE)){
			OnSourceNotDest((*iter).first);
		}
		else if((*iter).second == DEST){
			OnNotSourceDest((*iter).first);			
		}
		else if((*iter).second == SOURCE|DEST){
			OnSourceAndDest((*iter).first);		
		}
	}
	return true;
}

bool SyncThread::OnSourceNotDest(wxString path){
	if(wxDirExists(m_Data.GetSource() + path)){
		//We need to call the function again with our new folder
		
	}
	else{
		//About if needed
		if(wxGetApp().ShouldAbort()){
			return true;
		}
		//Only copy timestamp if file was copied
		bool ShouldTimeStamp = false;
		#ifdef __WXMSW__
			long iAttributes = 0;
		#endif
		if(m_Rules.ShouldExclude(m_Data.GetDest(), false)){
			return false;
		}
		//Ignore read only if needed
		if(m_Data.GetIgnoreRO()){
			#ifdef __WXMSW__
				iAttributes = GetFileAttributes(m_Data.GetDest());
				if(iAttributes == -1){
					iAttributes = FILE_ATTRIBUTE_NORMAL;					
				}
				SetFileAttributes(m_Data.GetDest(),FILE_ATTRIBUTE_NORMAL); 
			#endif
		}
		//Copy the file using a temp file to help reduce corruption
		if(wxCopyFile(m_Data.GetSource(), wxPathOnly(m_Data.GetDest()) + wxFILE_SEP_PATH + wxT("Toucan.tmp"), true)){
			if(wxRenameFile(wxPathOnly(m_Data.GetDest()) + wxFILE_SEP_PATH + wxT("Toucan.tmp"), m_Data.GetDest(), true)){
				OutputProgress(m_Data.GetSource() + _("\t copied to \t") + m_Data.GetDest());
				ShouldTimeStamp = true;
			}
			else{
				return false;
			}
		}
		else{
			return false;
		}
		//Remove the temp file
		if(wxFileExists(wxPathOnly(m_Data.GetDest()) + wxFILE_SEP_PATH + wxT("Toucan.tmp"))){
			wxRemoveFile(wxPathOnly(m_Data.GetDest()) + wxFILE_SEP_PATH + wxT("Toucan.tmp"));
		}
		//Set the old attributes back
		if(m_Data.GetIgnoreRO()){
			#ifdef __WXMSW__
				SetFileAttributes(m_Data.GetDest(), iAttributes); 
			#endif
		} 
		//Set the attributes if needed
		if(m_Data.GetAttributes() == true){
			#ifdef __WXMSW__
				int filearrtibs = GetFileAttributes(m_Data.GetSource());
				SetFileAttributes(m_Data.GetDest(),FILE_ATTRIBUTE_NORMAL);                       
				SetFileAttributes(m_Data.GetDest(),filearrtibs);
			#endif
		}
		//Set the timestamps if needed
		if(m_Data.GetTimeStamps() && ShouldTimeStamp){
			wxFileName from(m_Data.GetSource());
			wxFileName to(m_Data.GetDest());
			wxDateTime access, mod, created;
			from.GetTimes(&access ,&mod ,&created );
			to.SetTimes(&access ,&mod , &created); 
		}			
		return true;
	}
}

bool SyncThread::OnNotSourceDest(wxString path){
		
}
bool SyncThread::OnSourceAndDest(wxString path){
	
}

