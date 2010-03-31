/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 - 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include "syncbase.h"
#include "syncpreview.h"
#include "../rules.h"
#include "../toucan.h"
#include "../data/syncdata.h"

#include <list>
#include <map>
#include <algorithm>
#include <wx/string.h>
#include <wx/wfstream.h>

bool isoriginal(DirCtrlItem *item){
	return item->GetColour() == wxColour("Black");
}

SyncPreview::SyncPreview(const wxString &syncsource, const wxString &syncdest, SyncData* syncdata, bool issource) 
            :sourcetree(issource), SyncBase(syncsource, syncdest, syncdata)
{}

DirCtrlItemArray SyncPreview::Execute(){
	std::list<wxString> sourcepaths = FolderContentsToList(sourceroot);
	std::list<wxString> destpaths = FolderContentsToList(destroot);
	std::map<wxString, int> mergeresult = MergeListsToMap(sourcepaths, destpaths);
	OperationCaller(mergeresult);
	//If needed we now filter out the unchanged items
	if(data->GetPreviewChanges()){
		if(sourcetree){
			DirCtrlIter ret = std::remove_if(sourceitems.begin(), sourceitems.end(), isoriginal);
			sourceitems.erase(ret, sourceitems.end());
			return sourceitems;
		}
		else{
			DirCtrlIter ret = std::remove_if(destitems.begin(), destitems.end(), isoriginal);
			destitems.erase(ret, destitems.end());
			return destitems;
		}
	}
	else{
		return sourcetree ? sourceitems : destitems;
	}
}

void SyncPreview::OperationCaller(std::map<wxString, int> paths){
	for(std::map<wxString, int>::iterator iter = paths.begin(); iter != paths.end(); ++iter){
		if(wxDirExists(sourceroot + wxFILE_SEP_PATH + (*iter).first) || wxDirExists(destroot + wxFILE_SEP_PATH + (*iter).first)){
			if((*iter).second == 1){
				sourceitems.push_back(new DirCtrlItem(sourceroot + wxFILE_SEP_PATH + (*iter).first, true));
				OnSourceNotDestFolder((*iter).first);
			}
			else if((*iter).second == 2){
				destitems.push_back(new DirCtrlItem(destroot + wxFILE_SEP_PATH + (*iter).first, true));
				OnNotSourceDestFolder((*iter).first);		
			}
			else if((*iter).second == 3){
				sourceitems.push_back(new DirCtrlItem(sourceroot + wxFILE_SEP_PATH + (*iter).first, true));
				destitems.push_back(new DirCtrlItem(destroot + wxFILE_SEP_PATH + (*iter).first, true));
				OnSourceAndDestFolder((*iter).first);
			}
		}
		//We have a file
		else{
			if((*iter).second == 1){
				sourceitems.push_back(new DirCtrlItem(sourceroot + wxFILE_SEP_PATH + (*iter).first));
				OnSourceNotDestFile((*iter).first);
			}
			else if((*iter).second == 2){
				destitems.push_back(new DirCtrlItem(destroot + wxFILE_SEP_PATH + (*iter).first));
				OnNotSourceDestFile((*iter).first);
			}
			else if((*iter).second == 3){
				sourceitems.push_back(new DirCtrlItem(sourceroot + wxFILE_SEP_PATH + (*iter).first));
				destitems.push_back(new DirCtrlItem(destroot + wxFILE_SEP_PATH + (*iter).first));
				OnSourceAndDestFile((*iter).first);
			}
		}
	}
	return;
}

void SyncPreview::OnSourceNotDestFile(const wxString &path){
	if(data->GetFunction() != _("Clean")){
		wxString source = sourceroot + wxFILE_SEP_PATH + path;
		wxString dest = destroot + wxFILE_SEP_PATH + path;
		DirCtrlItem* item = new DirCtrlItem(dest);
		if(!data->GetRules()->ShouldExclude(source, false)){
			item->SetColour(wxT("Blue"));
			destitems.push_back(item);
			if(data->GetFunction() == _("Move")){
				DirCtrlIter iter = std::find(sourceitems.begin(), sourceitems.end(), source);
				if(iter != sourceitems.end()){
					(*iter)->SetColour(wxT("Grey"));						
				}
			}
		}
		else{
			delete item;
		}	
	}
}

void SyncPreview::OnNotSourceDestFile(const wxString &path){
	wxString source = sourceroot + wxFILE_SEP_PATH + path;
	wxString dest = destroot + wxFILE_SEP_PATH + path;
	if(!data->GetRules()->ShouldExclude(dest, false)){
		if(data->GetFunction() == _("Mirror") || data->GetFunction() == _("Clean")){
			DirCtrlIter iter = std::find(destitems.begin(), destitems.end(), dest);
			if(iter != destitems.end()){
				if(!data->GetRules()->ShouldExclude(dest, false)){
					(*iter)->SetColour(wxT("Grey"));						
				}
			}
		}
		else if(data->GetFunction() == _("Equalise")){
			if(!data->GetRules()->ShouldExclude(dest, false)){
				DirCtrlItem* item = new DirCtrlItem(source);
				item->SetColour(wxT("Blue"));
				sourceitems.push_back(item);
			}
		}
	}
}

void SyncPreview::OnSourceAndDestFile(const wxString &path){
	wxString source = sourceroot + wxFILE_SEP_PATH + path;
	wxString dest = destroot + wxFILE_SEP_PATH + path;
	if(!data->GetRules()->ShouldExclude(dest, false)){
		if(data->GetFunction() == _("Copy") || data->GetFunction() == _("Mirror") || data->GetFunction() == _("Move")){
			if(CopyIfNeeded(source, dest)){
				DirCtrlIter iter = std::find(destitems.begin(), destitems.end(), dest);
				if(iter != destitems.end()){
					(*iter)->SetColour(wxT("Green"));		
					if(data->GetFunction() == _("Move")){
						DirCtrlIter iter = std::find(sourceitems.begin(), sourceitems.end(), source);
						if(iter != sourceitems.end()){
							(*iter)->SetColour(wxT("Grey"));						
						}
					}
				}
			}		
		}
		else if(data->GetFunction() == _("Equalise")){
			wxDateTime tmTo, tmFrom;
			wxFileName flTo(dest);
			wxFileName flFrom(source);
			flTo.GetTimes(NULL, &tmTo, NULL);
			flFrom.GetTimes(NULL, &tmFrom, NULL);		

			if(data->GetIgnoreDLS()){
				tmFrom.MakeTimezone(wxDateTime::Local, true);
			}

			if(tmFrom.IsLaterThan(tmTo)){
				if(CopyIfNeeded(source, dest)){
					DirCtrlIter iter = std::find(destitems.begin(), destitems.end(), dest);
					if(iter != destitems.end()){
						(*iter)->SetColour(wxT("Green"));			
					}
				}	
			}
			else if(tmTo.IsLaterThan(tmFrom)){
				if(CopyIfNeeded(dest, source)){
					DirCtrlIter iter = std::find(sourceitems.begin(), sourceitems.end(), source);
					if(iter != sourceitems.end()){
						(*iter)->SetColour(wxT("Green"));			
					}
				}					
			}
		}
	}
}

void SyncPreview::OnSourceNotDestFolder(const wxString &path){
	if(data->GetFunction() != _("Clean")){
		wxString source = sourceroot + wxFILE_SEP_PATH + path;
		wxString dest = destroot + wxFILE_SEP_PATH + path;
		DirCtrlItem* item = new DirCtrlItem(dest, true);
		if(!data->GetRules()->ShouldExclude(source, true)){
			item->SetColour(wxT("Blue"));
		}
		else{
			item->SetColour(wxT("Red"));
		}
		destitems.push_back(item);
		if(data->GetFunction() == _("Move")){
			DirCtrlIter iter = std::find(sourceitems.begin(), sourceitems.end(), source);
			if(iter != sourceitems.end()){
				(*iter)->SetColour(wxT("Red"));						
			}
		}
	}
}

void SyncPreview::OnNotSourceDestFolder(const wxString &path){
	wxString source = sourceroot + wxFILE_SEP_PATH + path;
	wxString dest = destroot + wxFILE_SEP_PATH + path;
	if(data->GetFunction() == _("Mirror") || data->GetFunction() == _("Clean")){
		DirCtrlIter iter = std::find(destitems.begin(), destitems.end(), dest);
		if(iter != destitems.end()){
			if(!data->GetRules()->ShouldExclude(dest, true)){
				(*iter)->SetColour(wxT("Grey"));				
			}		
		}
	}
	else if(data->GetFunction() == _("Equalise")){
		DirCtrlItem* item = new DirCtrlItem(source, true);
		if(!data->GetRules()->ShouldExclude(dest, true)){
			item->SetColour(wxT("Blue"));

		}
		else{
			item->SetColour(wxT("Red"));
		}
		sourceitems.push_back(item);
	}
}

void SyncPreview::OnSourceAndDestFolder(const wxString &path){
	wxString source = sourceroot + wxFILE_SEP_PATH + path;
	wxString dest = destroot + wxFILE_SEP_PATH + path;
	if(data->GetFunction() == _("Move")){
		DirCtrlIter iter = std::find(sourceitems.begin(), sourceitems.end(), source);
		if(iter != sourceitems.end()){
			if(!data->GetRules()->ShouldExclude(source, true)){
				(*iter)->SetColour(wxT("Red"));				
			}		
		}
	}
}

bool SyncPreview::CopyIfNeeded(const wxString &source, const wxString &dest){
	//If the dest file doesn't exists then we must copy
	if(!wxFileExists(dest)){
		return true;
	}
	//If we fail any of the required tests then return false
	if((data->GetCheckSize() && !ShouldCopySize(source, dest))
		|| (data->GetCheckTime() && !ShouldCopyTime(source, dest))
		|| (data->GetCheckShort() && !ShouldCopyShort(source, dest))
		||(data->GetCheckFull() && !ShouldCopyFull(source, dest))){
		return false;
	}
	return true;
}
