/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "syncbase.h"
#include "syncpreview.h"
#include "../toucan.h"
#include "../data/syncdata.h"

#include <list>
#include <map>
#include <wx/string.h>
#include <wx/wfstream.h>

SyncPreview::SyncPreview(const wxString &syncsource, const wxString &syncdest, SyncData* syncdata, bool issource) 
            :SyncBase(syncsource, syncdest, syncdata)
{
	this->sourcetree = issource;
	this->preview = true;
}

VdtcTreeItemBaseArray SyncPreview::Execute(){
	std::list<wxString> sourcepaths = FolderContentsToList(sourceroot);
	std::list<wxString> destpaths = FolderContentsToList(destroot);
	std::map<wxString, int> mergeresult = MergeListsToMap(sourcepaths, destpaths);
	OperationCaller(mergeresult);
	if(sourcetree){
		return sourceitems;
	}
	else{
		return destitems;		
	}

}

bool SyncPreview::OperationCaller(std::map<wxString, int> paths){
	for(std::map<wxString, int>::iterator iter = paths.begin(); iter != paths.end(); ++iter){
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

void SyncPreview::OnSourceNotDestFile(const wxString &path){
	if(data->GetFunction() != _("Clean")){
		wxString source = sourceroot + wxFILE_SEP_PATH + path;
		wxString dest = destroot + wxFILE_SEP_PATH + path;
		VdtcTreeItemBase* item = new VdtcTreeItemBase(VDTC_TI_FILE, path);
		if(!data->GetRules()->ShouldExclude(source, false)){
			item->SetColour(wxT("Blue"));
			destitems.Add(item);
			if(data->GetFunction() == _("Move")){
				int pos = GetItemLocation(path, &sourceitems);
				if(pos != -1){
					sourceitems.Item(pos)->SetColour(wxT("Grey"));						
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
			int pos = GetItemLocation(path, &destitems);
			if(pos != -1){
				if(!data->GetRules()->ShouldExclude(dest, false)){
					destitems.Item(pos)->SetColour(wxT("Grey"));						
				}
			}
		}
		else if(data->GetFunction() == _("Equalise")){
			VdtcTreeItemBase* item = new VdtcTreeItemBase(VDTC_TI_FILE, path);
			if(!data->GetRules()->ShouldExclude(dest, false)){
				item->SetColour(wxT("Blue"));
				sourceitems.Add(item);
			}
			else{
				delete item;
			}
		}
	}
}

void SyncPreview::OnSourceAndDestFile(const wxString &path){
	wxString source = sourceroot + wxFILE_SEP_PATH + path;
	wxString dest = destroot + wxFILE_SEP_PATH + path;
	if(!data->GetRules()->ShouldExclude(dest, false)){
		if(data->GetFunction() == _("Copy") || data->GetFunction() == _("Mirror") || data->GetFunction() == _("Move")){
			if(ShouldCopy(source, dest)){
				int pos = GetItemLocation(path, &destitems);
				if(pos != -1){
					destitems.Item(pos)->SetColour(wxT("Green"));		
					if(data->GetFunction() == _("Move")){
						int pos = GetItemLocation(path, &sourceitems);
						if(pos != -1){
							sourceitems.Item(pos)->SetColour(wxT("Grey"));						
						}
					}
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
			if(tmFrom.IsEqualUpTo(tmTo, wxTimeSpan(0, 0, 2, 0))){
				return;
			}
			else if(tmFrom.IsLaterThan(tmTo)){
				if(ShouldCopy(source, dest)){
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
				if(ShouldCopy(source, dest)){
					int pos = GetItemLocation(path, &destitems);
					if(pos != -1){
						destitems.Item(pos)->SetColour(wxT("Green"));			
					}
				}	
			}
			else if(tmTo.IsLaterThan(tmFrom)){
				if(ShouldCopy(dest, source)){
					int pos = GetItemLocation(path, &sourceitems);
					if(pos != -1){
						sourceitems.Item(pos)->SetColour(wxT("Green"));			
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
		VdtcTreeItemBase* item = new VdtcTreeItemBase(VDTC_TI_DIR, path);
		if(!data->GetRules()->ShouldExclude(source, true)){
			item->SetColour(wxT("Blue"));
		}
		else{
			item->SetColour(wxT("Red"));
		}
		destitems.Add(item);
		if(data->GetFunction() == _("Move")){
			int pos = GetItemLocation(path, &sourceitems);
			if(pos != -1){
				sourceitems.Item(pos)->SetColour(wxT("Red"));						
			}
		}
	}
}

void SyncPreview::OnNotSourceDestFolder(const wxString &path){
	wxString source = sourceroot + wxFILE_SEP_PATH + path;
	wxString dest = destroot + wxFILE_SEP_PATH + path;
	if(data->GetFunction() == _("Mirror") || data->GetFunction() == _("Clean")){
		int pos = GetItemLocation(path, &destitems);
		if(pos != -1){
			if(!data->GetRules()->ShouldExclude(dest, true)){
				destitems.Item(pos)->SetColour(wxT("Grey"));				
			}		
		}
	}
	else if(data->GetFunction() == _("Equalise")){
		VdtcTreeItemBase* item = new VdtcTreeItemBase(VDTC_TI_DIR, path);
		if(!data->GetRules()->ShouldExclude(dest, true)){
			item->SetColour(wxT("Blue"));

		}
		else{
			item->SetColour(wxT("Red"));
		}
		sourceitems.Add(item);
	}
}

void SyncPreview::OnSourceAndDestFolder(const wxString &path){
	wxString source = sourceroot + wxFILE_SEP_PATH + path;
	wxString dest = destroot + wxFILE_SEP_PATH + path;
	if(data->GetFunction() == _("Move")){
		int pos = GetItemLocation(path, &sourceitems);
		if(pos != -1){
			if(!data->GetRules()->ShouldExclude(source, true)){
				sourceitems.Item(pos)->SetColour(wxT("Red"));				
			}		
		}
	}
}

int SyncPreview::GetItemLocation(const wxString &path, VdtcTreeItemBaseArray* array){
	for(unsigned int i = 0; i < array->GetCount(); i++){
		if(array->Item(i)->GetName() == path){
			return i;
		}
	}
	return -1;
}

bool SyncPreview::ShouldCopy(const wxString &source, const wxString &dest){
	if(disablestreams){
		return true;
	}

	//See the real CopyFileStream for more info
	wxFileInputStream sourcestream(source);
	wxFileInputStream deststream(dest);

	//Something is wrong with our streams, return error
	if(!sourcestream.IsOk() || !deststream.IsOk()){
		return false;
	}

	if(sourcestream.GetLength() != deststream.GetLength()){
		return true;	
	}

	wxFileOffset size = sourcestream.GetLength();

	//We read in 4KB chunks as testing seems to show they are the fastest
	char *sourcebuf = new char[4096];
	char *destbuf = new char [4096];
	wxFileOffset bytesLeft = size;
	while(bytesLeft > 0){
		wxGetApp().Yield();
		wxFileOffset bytesToRead = wxMin(4096, bytesLeft);
		sourcestream.Read(sourcebuf, bytesToRead);
		deststream.Read(destbuf, bytesToRead);
		if(sourcestream.GetLastError() != wxSTREAM_NO_ERROR || deststream.GetLastError() != wxSTREAM_NO_ERROR){
			delete[] sourcebuf;
			delete[] destbuf;
			return false;
		}
		if(strncmp(sourcebuf, destbuf, bytesToRead) != 0){
			delete[] sourcebuf;
			delete[] destbuf;
			return true;
		}
		bytesLeft -= bytesToRead;
	}
	//The two files are actually the same
	delete[] sourcebuf;
	delete[] destbuf;
	return false;
}
