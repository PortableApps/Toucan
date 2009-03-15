/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "syncbase.h"
#include "syncpreview.h"
#include "../md5.h"
#include <list>
#include <map>
#include <wx/string.h>

SyncPreview::SyncPreview(wxString syncsource, wxString syncdest, SyncData* syncdata, Rules syncrules, bool issource) : SyncFiles(syncsource, syncdest, syncdata, syncrules){
	this->sourcetree = issource;
	this->preview = true;
}

VdtcTreeItemBaseArray SyncPreview::Execute(){
	std::list<wxString> sourcepaths = FolderContentsToList(sourceroot);
	std::list<wxString> destpaths = FolderContentsToList(destroot);
	std::map<wxString, short> mergeresult = MergeListsToMap(sourcepaths, destpaths);
	OperationCaller(mergeresult);
	if(sourcetree){
		return sourceitems;
	}
	else{
		return destitems;		
	}

}

bool SyncPreview::OperationCaller(std::map<wxString, short> paths){
	for(std::map<wxString, short>::iterator iter = paths.begin(); iter != paths.end(); ++iter){
		if(wxDirExists(sourceroot + wxFILE_SEP_PATH + (*iter).first) || wxDirExists(destroot + wxFILE_SEP_PATH + (*iter).first)){
			if((*iter).second == 1){
				sourceitems.Add(new VdtcTreeItemBase(VDTC_TI_DIR, (*iter).first));
				OnSourceNotDestFolder((*iter).first);
			}
			else if((*iter).second == 2){
				destitems.Add(new VdtcTreeItemBase(VDTC_TI_DIR, (*iter).first));
				OnNotSourceDestFolder((*iter).first);		
			}
			else if((*iter).second == 3){
				sourceitems.Add(new VdtcTreeItemBase(VDTC_TI_DIR, (*iter).first));
				destitems.Add(new VdtcTreeItemBase(VDTC_TI_DIR, (*iter).first));
				OnSourceAndDestFolder((*iter).first);
			}
		}
		//We have a file
		else{
			if((*iter).second == 1){
				sourceitems.Add(new VdtcTreeItemBase(VDTC_TI_FILE, (*iter).first));
				OnSourceNotDestFile((*iter).first);
			}
			else if((*iter).second == 2){
				destitems.Add(new VdtcTreeItemBase(VDTC_TI_FILE, (*iter).first));
				OnNotSourceDestFile((*iter).first);
			}
			else if((*iter).second == 3){
				sourceitems.Add(new VdtcTreeItemBase(VDTC_TI_FILE, (*iter).first));
				destitems.Add(new VdtcTreeItemBase(VDTC_TI_FILE, (*iter).first));
				OnSourceAndDestFile((*iter).first);
			}
		}
	}
	return true;
}

bool SyncPreview::OnSourceNotDestFile(wxString path){
	wxString source = sourceroot + wxFILE_SEP_PATH + path;
	wxString dest = destroot + wxFILE_SEP_PATH + path;
	if(!rules.ShouldExclude(source, false)){
		VdtcTreeItemBase* item = new VdtcTreeItemBase(VDTC_TI_FILE, path);
		item->SetColour(wxT("Blue"));
		destitems.Add(item);
	}
	return true;
}

bool SyncPreview::OnNotSourceDestFile(wxString path){
	wxString source = sourceroot + wxFILE_SEP_PATH + path;
	wxString dest = destroot + wxFILE_SEP_PATH + path;
	if(!rules.ShouldExclude(dest, false)){
		if(data->GetFunction() == _("Mirror")){
			int pos = GetItemLocation(path, &destitems);
			if(pos != -1){
				destitems.Item(pos)->SetColour(wxT("Grey"));			
			}
		}
		else if(data->GetFunction() == _("Equalise")){
			VdtcTreeItemBase* item = new VdtcTreeItemBase(VDTC_TI_FILE, path);
			item->SetColour(wxT("Blue"));
			sourceitems.Add(item);
		}
	}
	return true;
}

bool SyncPreview::OnSourceAndDestFile(wxString path){
	wxString source = sourceroot + wxFILE_SEP_PATH + path;
	wxString dest = destroot + wxFILE_SEP_PATH + path;
	if(!rules.ShouldExclude(dest, false)){
		if(data->GetFunction() == _("Copy") || data->GetFunction() == _("Mirror")){
			wxMD5* md5 = new wxMD5();
			if(md5->GetFileMD5(source) != md5->GetFileMD5(dest)){
				int pos = GetItemLocation(path, &destitems);
				if(pos != -1){
					destitems.Item(pos)->SetColour(wxT("Green"));			
				}
			}		
		}
		else if(data->GetFunction() == _("Update")){
			wxDateTime tmTo, tmFrom;
			wxFileName flTo(dest);
			wxFileName flFrom(source);
			flTo.GetTimes(NULL, &tmTo, NULL);
			flFrom.GetTimes(NULL, &tmFrom, NULL);		

			if(data->GetIgnoreDLS()){
				tmFrom.MakeTimezone(wxDateTime::Local, true);
			}

			if(tmFrom.IsLaterThan(tmTo)){
				wxMD5* md5 = new wxMD5();
				if(md5->GetFileMD5(source) != md5->GetFileMD5(dest)){
					int pos = GetItemLocation(path, &destitems);
					if(pos != -1){
						destitems.Item(pos)->SetColour(wxT("Green"));			
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
				wxMD5* md5 = new wxMD5();
				if(md5->GetFileMD5(source) != md5->GetFileMD5(dest)){
					int pos = GetItemLocation(path, &destitems);
					if(pos != -1){
						destitems.Item(pos)->SetColour(wxT("Green"));			
					}
				}	
			}
			else if(tmTo.IsLaterThan(tmFrom)){
				wxMD5* md5 = new wxMD5();
				if(md5->GetFileMD5(source) != md5->GetFileMD5(dest)){
					int pos = GetItemLocation(path, &sourceitems);
					if(pos != -1){
						sourceitems.Item(pos)->SetColour(wxT("Green"));			
					}
				}					
			}
		}
	}
	return true;
}
bool SyncPreview::OnSourceNotDestFolder(wxString path){
	return true;
}
bool SyncPreview::OnNotSourceDestFolder(wxString path){
	if(data->GetFunction() == _("Mirror")){
		int pos = GetItemLocation(path, &destitems);
		if(pos != -1){
			destitems.Item(pos)->SetColour(wxT("Green"));			
		}
	}
	return true;
}
bool SyncPreview::OnSourceAndDestFolder(wxString path){
	/*wxString source = sourceroot + wxFILE_SEP_PATH + path;
	wxString dest = destroot + wxFILE_SEP_PATH + path;
	//Always recurse into the next directory
	SyncFiles sync(source, dest, data, rules);
	sync.Execute();
	if(data->GetTimeStamps()){
		CopyFolderTimestamp(source, dest);
	}*/
	return true;
}

int SyncPreview::GetItemLocation(wxString path, VdtcTreeItemBaseArray* array){
	for(unsigned int i = 0; i < array->GetCount(); i++){
		if(array->Item(i)->GetName() == path){
			return i;
		}
	}
	return -1;
}
