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
#include <vector>

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

std::list<wxString> SyncBase::FolderContentsToList(const wxString &path){
	std::list<wxString> paths;
	wxFileName fnpath = wxFileName::DirName(path);
	if(fnpath.DirExists()){
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
		
	dtsource.MakeTimezone(wxDateTime::UTC, true);
	dtdest.MakeTimezone(wxDateTime::UTC, true);

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
	//For more detailed comments see the ShouldCopyFull function
	std::auto_ptr<wxFileInputStream> sourcestream(new wxFileInputStream(source));
	std::auto_ptr<wxFileInputStream> deststream(new wxFileInputStream(dest));

	if(!sourcestream->IsOk() || !deststream->IsOk()){
		return false;
	}

	if(sourcestream->GetLength() != deststream->GetLength()){
		return true;
	}

	wxFileOffset size = sourcestream->GetLength();
	wxFileOffset bytesToRead = wxMin(1024, size);

	std::vector<char> sourcebuf(bytesToRead);
	std::vector<char> destbuf(bytesToRead);

	sourcestream->Read(&sourcebuf[0], bytesToRead);
	deststream->Read(&destbuf[0], bytesToRead);

	if(sourcestream->GetLastError() != wxSTREAM_NO_ERROR || deststream->GetLastError() != wxSTREAM_NO_ERROR){
		return false;
	}

	if(wxTmemcmp(&sourcebuf[0], &destbuf[0], bytesToRead) != 0){
		return true;
	}

	//Seek to the end
	sourcestream->SeekI(bytesToRead, wxFromEnd);
	deststream->SeekI(bytesToRead, wxFromEnd);

	sourcestream->Read(&sourcebuf[0], bytesToRead);
	deststream->Read(&destbuf[0], bytesToRead);

	if(sourcestream->GetLastError() != wxSTREAM_NO_ERROR || deststream->GetLastError() != wxSTREAM_NO_ERROR){
		return false;
	}

	if(wxTmemcmp(&sourcebuf[0], &destbuf[0], bytesToRead) != 0){
		return true;
	}
	return false;
}

bool SyncBase::ShouldCopyFull(const wxString &source, const wxString &dest){
	std::auto_ptr<wxFileInputStream> sourcestream(new wxFileInputStream(source));
	std::auto_ptr<wxFileInputStream> deststream(new wxFileInputStream(dest));

	//Something is wrong with our streams, return false as
	//it is not a good idea to copy in this case
	if(!sourcestream->IsOk() || !deststream->IsOk()){
		return false;
	}
	
	//If we have different lengths then we need to copy
	if(sourcestream->GetLength() != deststream->GetLength()){
		return true;
	}

	wxFileOffset size = sourcestream->GetLength();

	//We read in 4KB chunks as testing seems to show they are the fastest
	std::vector<char> sourcebuf(4096);
	std::vector<char> destbuf(4096);

	wxFileOffset bytesLeft = size;
	while(bytesLeft > 0){
		wxFileOffset bytesToRead = wxMin(4096, bytesLeft);
		sourcebuf.resize(bytesToRead);
		destbuf.resize(bytesToRead);
		sourcestream->Read(&sourcebuf[0], bytesToRead);
		deststream->Read(&destbuf[0], bytesToRead);

		//If we have a read error then return false as it is potentially 
		//unsafe to copy
		if(sourcestream->GetLastError() != wxSTREAM_NO_ERROR || deststream->GetLastError() != wxSTREAM_NO_ERROR){
			return false;
		}

		//Use a memcmp rather than a strncmp as certain binary files can 
		//contain embedded nulls
		if(wxTmemcmp(&sourcebuf[0], &destbuf[0], bytesToRead) != 0){
			return true;
		}
		bytesLeft-=bytesToRead;
	}
	//If we make it here then the files are the same
	return false;
}
