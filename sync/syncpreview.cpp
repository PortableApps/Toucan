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

void SyncPreview::OnSourceNotDestFile(const wxFileName &source, const wxFileName &dest){
    DirCtrlItem *sourceitem = new DirCtrlItem(source);
    sourceitems.push_back(sourceitem);
    if(data->GetFunction() != _("Clean") && data->GetRules()->Matches(source) != Excluded){
        DirCtrlItem* destitem = new DirCtrlItem(dest);
        destitem->SetColour("Blue");
        destitems.push_back(destitem);
		if(data->GetFunction() == _("Move")){
            sourceitem->SetColour(wxT("Grey"));
        }
    }
}

void SyncPreview::OnNotSourceDestFile(const wxFileName &source, const wxFileName &dest){
    DirCtrlItem *destitem = new DirCtrlItem(dest);
    destitems.push_back(destitem);
    if(data->GetRules()->Matches(dest) != Excluded){
        if(data->GetFunction() == _("Mirror") || data->GetFunction() == _("Clean")){
            destitem->SetColour(wxT("Grey"));						
        }
        else if(data->GetFunction() == _("Equalise")){
            DirCtrlItem* item = new DirCtrlItem(source);
            item->SetColour(wxT("Blue"));
            sourceitems.push_back(item);
        }
    }
}

void SyncPreview::OnSourceAndDestFile(const wxFileName &source, const wxFileName &dest){
    DirCtrlItem *sourceitem = new DirCtrlItem(source);
    DirCtrlItem *destitem = new DirCtrlItem(dest);
	sourceitems.push_back(sourceitem);
    destitems.push_back(destitem);
    if(data->GetRules()->Matches(dest) != Excluded){
        if(data->GetFunction() == _("Copy") || data->GetFunction() == _("Mirror") || data->GetFunction() == _("Move")){
            if(CopyIfNeeded(source, dest)){
                destitem->SetColour(wxT("Green"));		
                if(data->GetFunction() == _("Move")){
                    sourceitem->SetColour(wxT("Grey"));
                }
            }		
        }
        else if(data->GetFunction() == _("Equalise")){
            wxDateTime to, from;
            dest.GetTimes(NULL, &to, NULL);
            source.GetTimes(NULL, &from, NULL);		

            from.MakeTimezone(wxDateTime::UTC, true);
            to.MakeTimezone(wxDateTime::UTC, true);

            if(from.IsLaterThan(to) && CopyIfNeeded(source, dest))
                destitem->SetColour(wxT("Green"));
            else if(to.IsLaterThan(from) && CopyIfNeeded(dest, source))
                sourceitem->SetColour(wxT("Green"));			
        }
    }
}

void SyncPreview::OnSourceNotDestFolder(const wxFileName &source, const wxFileName &dest){
    DirCtrlItem *sourceitem = new DirCtrlItem(source);
    sourceitems.push_back(sourceitem);
    if(data->GetFunction() != _("Clean")){
        DirCtrlItem* destitem = new DirCtrlItem(dest);
        RuleResult res = data->GetRules()->Matches(source);
        if(res != Excluded && res != AbsoluteFolderExclude)
            destitem->SetColour(wxT("Blue"));
        else
            destitem->SetColour(wxT("Red"));
        destitems.push_back(destitem);
        if(data->GetFunction() == _("Move"))
            sourceitem->SetColour(wxT("Red"));
    }
}

void SyncPreview::OnNotSourceDestFolder(const wxFileName &source, const wxFileName &dest){
    DirCtrlItem *destitem = new DirCtrlItem(dest);
    destitems.push_back(destitem);
    if(data->GetFunction() == _("Mirror") || data->GetFunction() == _("Clean")){
        RuleResult res = data->GetRules()->Matches(dest);
        if(res != Excluded && res != AbsoluteExcluded)
            destitem->SetColour(wxT("Grey"));		
    }
    else if(data->GetFunction() == _("Equalise")){
        RuleResult res = data->GetRules()->Matches(dest);
        DirCtrlItem* sourceitem = new DirCtrlItem(source);
        if(res != Excluded && res != AbsoluteExcluded)
            sourceitem->SetColour(wxT("Blue"));
        else
            sourceitem->SetColour(wxT("Red"));
        sourceitems.push_back(sourceitem);
    }
}

void SyncPreview::OnSourceAndDestFolder(const wxFileName &source, const wxFileName &dest){
    DirCtrlItem *sourceitem = new DirCtrlItem(source);
    sourceitems.push_back(sourceitem);
    destitems.push_back(new DirCtrlItem(dest));
    if(data->GetFunction() == _("Move")){
        RuleResult res = data->GetRules()->Matches(source);
        if(res != Excluded && res != AbsoluteExcluded){
            sourceitem->SetColour(wxT("Red"));						
        }
    }
}

bool SyncPreview::CopyIfNeeded(const wxFileName &source, const wxFileName &dest){
    //If the dest file doesn't exists then we must copy
    if(!dest.FileExists()){
        return true;
    }
    //If copy if anything says copy
    if((data->GetCheckSize() && ShouldCopySize(source, dest))
    || (data->GetCheckTime() && ShouldCopyTime(source, dest))
    || (data->GetCheckShort() && ShouldCopyShort(source, dest))
    || (data->GetCheckFull() && ShouldCopyFull(source, dest))
    || (!data->GetCheckSize() && !data->GetCheckTime() && 
        !data->GetCheckShort() && !data->GetCheckFull())){
        return true;
    }
    return false;
}
