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
	return (item->GetColour() == wxColour("Black") && item->GetType() != DIRCTRL_FOLDER);
}

SyncPreview::SyncPreview(const wxString &syncsource, const wxString &syncdest, SyncData* syncdata, bool issource) 
            :sourcetree(issource), SyncBase(wxFileName::DirName(syncsource), wxFileName::DirName(syncdest), syncdata)
{}

DirCtrlItemArray SyncPreview::Execute(){
	auto sourcepaths = FolderContentsToList(sourceroot);
	auto destpaths = FolderContentsToList(destroot);
	auto mergeresult = MergeListsToMap(sourcepaths, destpaths);
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
	for(auto iter = paths.begin(); iter != paths.end(); ++iter){
        wxFileName source, dest;
        if(wxDirExists(sourceroot.GetPathWithSep() + (*iter).first)
        || wxDirExists(destroot.GetPathWithSep() + (*iter).first)){
            source = wxFileName::DirName(sourceroot.GetPathWithSep() + (*iter).first);
            dest = wxFileName::DirName(destroot.GetPathWithSep() + (*iter).first); 
			if((*iter).second == 1){
				sourceitems.push_back(new DirCtrlItem(source));
				OnSourceNotDestFolder(source, dest);
			}
			else if((*iter).second == 2){
				destitems.push_back(new DirCtrlItem(dest));
				OnNotSourceDestFolder(source, dest);		
			}
			else if((*iter).second == 3){
				sourceitems.push_back(new DirCtrlItem(source));
				destitems.push_back(new DirCtrlItem(dest));
				OnSourceAndDestFolder(source, dest);
			}
		}
		//We have a file
        else{
            source = wxFileName::FileName(sourceroot.GetPathWithSep() + (*iter).first);
            dest = wxFileName::FileName(destroot.GetPathWithSep() + (*iter).first); 

			if((*iter).second == 1){
				sourceitems.push_back(new DirCtrlItem(source));
				OnSourceNotDestFile(source, dest);
			}
			else if((*iter).second == 2){
				destitems.push_back(new DirCtrlItem(dest));
				OnNotSourceDestFile(source, dest);
			}
			else if((*iter).second == 3){
				sourceitems.push_back(new DirCtrlItem(source));
				destitems.push_back(new DirCtrlItem(dest));
				OnSourceAndDestFile(source, dest);
			}
		}
	}
	return;
}

void SyncPreview::OnSourceNotDestFile(const wxFileName &source, const wxFileName &dest){
	if(data->GetFunction() != _("Clean")){
		DirCtrlItem* item = new DirCtrlItem(dest);
		if(data->GetRules()->Matches(source) != Excluded){
			item->SetColour(wxT("Blue"));
			destitems.push_back(item);
			if(data->GetFunction() == _("Move")){
				DirCtrlIter iter = FindPath(&sourceitems, source);
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

void SyncPreview::OnNotSourceDestFile(const wxFileName &source, const wxFileName &dest){
	if(data->GetRules()->Matches(dest) != Excluded){
		if(data->GetFunction() == _("Mirror") || data->GetFunction() == _("Clean")){
	        DirCtrlIter iter = FindPath(&destitems, dest);
			if(iter != destitems.end()){
				if(data->GetRules()->Matches(dest) != Excluded){
					(*iter)->SetColour(wxT("Grey"));						
				}
			}
		}
		else if(data->GetFunction() == _("Equalise")){
			if(data->GetRules()->Matches(dest) != Excluded){
				DirCtrlItem* item = new DirCtrlItem(source);
				item->SetColour(wxT("Blue"));
				sourceitems.push_back(item);
			}
		}
	}
}

void SyncPreview::OnSourceAndDestFile(const wxFileName &source, const wxFileName &dest){
	if(data->GetRules()->Matches(dest) != Excluded){
		if(data->GetFunction() == _("Copy") || data->GetFunction() == _("Mirror") || data->GetFunction() == _("Move")){
			if(CopyIfNeeded(source, dest)){
	            DirCtrlIter iter = FindPath(&destitems, dest);
				if(iter != destitems.end()){
					(*iter)->SetColour(wxT("Green"));		
					if(data->GetFunction() == _("Move")){
				        DirCtrlIter iter = FindPath(&sourceitems, source);
						if(iter != sourceitems.end()){
							(*iter)->SetColour(wxT("Grey"));						
						}
					}
				}
			}		
		}
		else if(data->GetFunction() == _("Equalise")){
			wxDateTime to, from;
			dest.GetTimes(NULL, &to, NULL);
			source.GetTimes(NULL, &from, NULL);		

			from.MakeTimezone(wxDateTime::UTC, true);
			to.MakeTimezone(wxDateTime::UTC, true);

			if(from.IsLaterThan(to)){
				if(CopyIfNeeded(source, dest)){
				DirCtrlIter iter = FindPath(&destitems, dest);
					if(iter != destitems.end()){
						(*iter)->SetColour(wxT("Green"));			
					}
				}	
			}
			else if(to.IsLaterThan(from)){
				if(CopyIfNeeded(dest, source)){
				    DirCtrlIter iter = FindPath(&sourceitems, source);
					if(iter != sourceitems.end()){
						(*iter)->SetColour(wxT("Green"));			
					}
				}					
			}
		}
	}
}

void SyncPreview::OnSourceNotDestFolder(const wxFileName &source, const wxFileName &dest){
	if(data->GetFunction() != _("Clean")){
		DirCtrlItem* item = new DirCtrlItem(dest);
        RuleResult res = data->GetRules()->Matches(source);
		if(res != Excluded && res != AbsoluteFolderExclude){
			item->SetColour(wxT("Blue"));
		}
		else{
			item->SetColour(wxT("Red"));
		}
		destitems.push_back(item);
		if(data->GetFunction() == _("Move")){
			DirCtrlIter iter = FindPath(&sourceitems, source);
			if(iter != sourceitems.end()){
				(*iter)->SetColour(wxT("Red"));						
			}
		}
	}
}

void SyncPreview::OnNotSourceDestFolder(const wxFileName &source, const wxFileName &dest){
	if(data->GetFunction() == _("Mirror") || data->GetFunction() == _("Clean")){
		DirCtrlIter iter = FindPath(&destitems, dest);
		if(iter != destitems.end()){
            RuleResult res = data->GetRules()->Matches(dest);
			if(res != Excluded && res != AbsoluteExcluded){
				(*iter)->SetColour(wxT("Grey"));				
			}		
		}
	}
	else if(data->GetFunction() == _("Equalise")){
        RuleResult res = data->GetRules()->Matches(dest);
		DirCtrlItem* item = new DirCtrlItem(source);
		if(res != Excluded && res != AbsoluteExcluded){
			item->SetColour(wxT("Blue"));
		}
		else{
			item->SetColour(wxT("Red"));
		}
		sourceitems.push_back(item);
	}
}

void SyncPreview::OnSourceAndDestFolder(const wxFileName &source, const wxFileName &dest){
	if(data->GetFunction() == _("Move")){
	    DirCtrlIter iter = FindPath(&sourceitems, source);
		if(iter != sourceitems.end()){
            RuleResult res = data->GetRules()->Matches(source);
			if(res != Excluded && res != AbsoluteExcluded){
				(*iter)->SetColour(wxT("Red"));				
			}		
		}
	}
}

bool SyncPreview::CopyIfNeeded(const wxFileName &source, const wxFileName &dest){
	//If the dest file doesn't exists then we must copy
	if(!dest.FileExists()){
		return true;
	}
	//If copy if anything says copy
	if(    (data->GetCheckSize() && ShouldCopySize(source, dest))
		|| (data->GetCheckTime() && ShouldCopyTime(source, dest))
		|| (data->GetCheckShort() && ShouldCopyShort(source, dest))
		|| (data->GetCheckFull() && ShouldCopyFull(source, dest))
        || (!data->GetCheckSize() && !data->GetCheckTime() && 
            !data->GetCheckShort() && !data->GetCheckFull())){
		return true;
	}
	return false;
}

DirCtrlIter SyncPreview::FindPath(DirCtrlItemArray* items, const wxFileName &path){
    for(DirCtrlIter iter = items->begin(); iter != items->end(); ++iter){
        if((*iter)->GetPath() == path){
            return iter;
        }
    }
    return items->end();
}