/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 - 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include "syncbase.h"
#include "../rules.h"
#include "../basicfunctions.h"
#include "../data/syncdata.h"
#include <wx/dir.h>
#include <wx/filefn.h>
#include <wx/datetime.h>
#include <wx/filename.h>
#include <wx/wfstream.h>

SyncBase::SyncBase(const wxString &syncsource, const wxString &syncdest, SyncData* syncdata) 
         :data(syncdata)
{
	//Strip trailing slashes
	if(syncsource[syncsource.Length() - 1] == wxFILE_SEP_PATH){
		this->sourceroot = syncsource.Left(syncsource.Length() - 1);
	}
	else{
		sourceroot = syncsource;
	}
	if(syncdest[syncdest.Length() - 1] == wxFILE_SEP_PATH){
		this->destroot = syncdest.Left(syncdest.Length() - 1);
	}
	else{
		destroot = syncdest;
	}
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

std::map<wxString, int> SyncBase::MergeListsToMap(std::list<wxString> sourcelist, std::list<wxString> destlist){
	std::list<wxString>::iterator iter;
	std::list<wxString>::iterator destiter;
	std::map<wxString, int> mergeresult;
	for(iter = sourcelist.begin(); iter != sourcelist.end(); ++iter){
		bool destexists = false;
		for(destiter = destlist.begin(); destiter != destlist.end(); ++destiter){
			if(*destiter == *iter){
				destexists = true;
				destlist.erase(destiter);
				break;
			}
		}
		int where = 1;
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

bool SyncBase::OperationCaller(std::map<wxString, int> paths){
	for(std::map<wxString, int>::iterator iter = paths.begin(); iter != paths.end(); ++iter){
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
			if(wxGetApp().IsGui() && !preview){
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

bool SyncBase::ShouldCopySize(const wxString &source, const wxString &dest){
	wxFileName fnsource = wxFileName::FileName(source);
	wxFileName fndest = wxFileName::FileName(dest);
	return !(fnsource.GetSize() == fndest.GetSize());
}

bool SyncBase::ShouldCopyTime(const wxString &source, const wxString &dest){
	wxDateTime dtsource, dtdest;
	wxFileName fnsource = wxFileName::FileName(source);
	wxFileName fndest = wxFileName::FileName(dest);
	fnsource.GetTimes(NULL, &dtsource, NULL);
	fndest.GetTimes(NULL, &dtdest, NULL);		

	if(data->GetIgnoreDLS()){
		dtsource.MakeTimezone(wxDateTime::Local, true);
		dtdest.MakeTimezone(wxDateTime::Local, true);
	}
	//If they are within two seconds of each other then they are 
	//likely the same due to filesystem differences (esp ext3 and FAT)
	if(dtsource.IsEqualUpTo(dtdest, wxTimeSpan(0, 0, 2, 0)) || dtsource.IsEarlierThan(dtdest)){
		return false;
	}
	else{
		return true;
	}
}

bool SyncBase::ShouldCopyShort(const wxString &source, const wxString &dest){
	wxFileInputStream *sourcestream = new wxFileInputStream(source);
	wxFileInputStream *deststream = new wxFileInputStream(dest);

	//Something is wrong with our streams, return false as
	//it is not a good idea to copy in this case
	if(!sourcestream->IsOk() || !deststream->IsOk()){
		delete sourcestream;
		delete deststream;
		return false;
	}

	//If we have different lengths then we need to copy
	if(sourcestream->GetLength() != deststream->GetLength()){
		return true;
	}

	wxFileOffset size = sourcestream->GetLength();

	//We are just testing the start and the end so we need a small buffer
	char sourcebufstart[100], sourcebufend[100], destbufstart[100], destbufend[100];

	//Read the start
	wxFileOffset bytesToRead = wxMin(size, 100);
	sourcestream->Read(sourcebufstart, bytesToRead);
	deststream->Read(destbufstart, bytesToRead);

	//If we have a read error then return false as it is potentially 
	//unsafe to copy
	if(sourcestream->GetLastError() != wxSTREAM_NO_ERROR || deststream->GetLastError() != wxSTREAM_NO_ERROR){
		delete sourcestream;
		delete deststream;
		return false;
	}

	//Seek to the end
	sourcestream->SeekI(bytesToRead, wxFromEnd);
	deststream->SeekI(bytesToRead, wxFromEnd);

	sourcestream->Read(sourcebufend, bytesToRead);
	deststream->Read(destbufend, bytesToRead);

	//If we have a read error then return false as it is potentially 
	//unsafe to copy
	if(sourcestream->GetLastError() != wxSTREAM_NO_ERROR || deststream->GetLastError() != wxSTREAM_NO_ERROR){
		delete sourcestream;
		delete deststream;
		return false;
	}

	//Use a memcmp rather than a strncmp as certain binary files can 
	//contain embedded nulls
	if(wxTmemcmp(sourcebufstart, destbufstart, bytesToRead) != 0 && wxTmemcmp(sourcebufend, destbufend, bytesToRead) != 0){
		delete sourcestream;
		delete deststream;
		return true;
	}

	delete sourcestream;
	delete deststream;
	//If we make it here then the files are the same
	return false;
}

bool SyncBase::ShouldCopyFull(const wxString &source, const wxString &dest){
	wxFileInputStream *sourcestream = new wxFileInputStream(source);
	wxFileInputStream *deststream = new wxFileInputStream(dest);

	//Something is wrong with our streams, return false as
	//it is not a good idea to copy in this case
	if(!sourcestream->IsOk() || !deststream->IsOk()){
		delete sourcestream;
		delete deststream;
		return false;
	}
	
	//If we have different lengths then we need to copy
	if(sourcestream->GetLength() != deststream->GetLength()){
		return true;
	}

	wxFileOffset size = sourcestream->GetLength();

	//We read in 4KB chunks as testing seems to show they are the fastest
	char *sourcebuf = new char[4096];
	char *destbuf = new char[4096];
	wxFileOffset bytesLeft = size;
	while(bytesLeft > 0){
		wxFileOffset bytesToRead = wxMin(4096, bytesLeft);
		sourcestream->Read(sourcebuf, bytesToRead);
		deststream->Read(destbuf, bytesToRead);

		//If we have a read error then return false as it is potentially 
		//unsafe to copy
		if(sourcestream->GetLastError() != wxSTREAM_NO_ERROR || deststream->GetLastError() != wxSTREAM_NO_ERROR){
			delete sourcestream;
			delete deststream;
			delete[] sourcebuf;
			delete[] destbuf;
			return false;
		}

		//Use a memcmp rather than a strncmp as certain binary files can 
		//contain embedded nulls
		if(wxTmemcmp(sourcebuf, destbuf, bytesToRead) != 0){
			delete sourcestream;
			delete deststream;
			delete[] sourcebuf;
			delete[] destbuf;
			return true;
		}
		bytesLeft-=bytesToRead;
	}
	delete sourcestream;
	delete deststream;
	delete[] sourcebuf;
	delete[] destbuf;
	//If we make it here then the files are the same
	return false;
}
