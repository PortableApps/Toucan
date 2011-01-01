/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 - 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include "syncbase.h"
#include "../rules.h"
#include "../path.h"
#include "../basicfunctions.h"
#include "../data/syncdata.h"
#include <wx/dir.h>
#include <wx/filefn.h>
#include <wx/datetime.h>
#include <wx/filename.h>
#include <wx/wfstream.h>
#include <vector>
#include <memory>

SyncBase::SyncBase(const wxFileName &source, const wxFileName &dest, SyncData* syncdata) 
         :data(syncdata)
{
    this->sourceroot = Path::Normalise(source);
    this->destroot = Path::Normalise(dest);
}

SyncBase::~SyncBase(){
	;
}

std::list<wxString> SyncBase::FolderContentsToList(const wxFileName &path){
	std::list<wxString> paths;
	if(path.IsOk() && path.DirExists()){
		wxString filename;
		wxDir dir(path.GetFullPath());
        if(!dir.IsOpened())
            return paths;
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
	std::map<wxString, int> mergeresult;
	for(auto iter = sourcelist.begin(); iter != sourcelist.end(); ++iter){
		bool destexists = false;
		for(auto destiter = destlist.begin(); destiter != destlist.end(); ++destiter){
			if(wxFileName(*iter).SameAs(wxFileName(*destiter))){
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
	for(auto destiter = destlist.begin(); destiter != destlist.end(); ++destiter){
		mergeresult[*destiter] = 2;
	}
	return mergeresult;
}

void SyncBase::OperationCaller(std::map<wxString, int> paths){
    for(auto iter = paths.begin(); iter != paths.end(); ++iter){
        if(wxGetApp().GetAbort())
			return;

        wxFileName source, dest;
        if(wxDirExists(sourceroot.GetPathWithSep() + (*iter).first)
        || wxDirExists(destroot.GetPathWithSep() + (*iter).first)){
            source = wxFileName::DirName(sourceroot.GetPathWithSep() + (*iter).first);
            dest = wxFileName::DirName(destroot.GetPathWithSep() + (*iter).first); 

            if((*iter).second == 1)
                OnSourceNotDestFolder(source, dest);
            else if((*iter).second == 2)
                OnNotSourceDestFolder(source, dest);
            else if((*iter).second == 3)
                OnSourceAndDestFolder(source, dest);
        }
        else{
            source = wxFileName::FileName(sourceroot.GetPathWithSep() + (*iter).first);
            dest = wxFileName::FileName(destroot.GetPathWithSep() + (*iter).first); 

            if((*iter).second == 1)
                OnSourceNotDestFile(source, dest);
            else if((*iter).second == 2)
                OnNotSourceDestFile(source, dest);				
            else if((*iter).second == 3)
                OnSourceAndDestFile(source, dest);
        }
    }
    return;
}

bool SyncBase::ShouldCopySize(const wxFileName &source, const wxFileName &dest){
	return !(source.GetSize() == dest.GetSize());
}

bool SyncBase::ShouldCopyTime(const wxFileName &source, const wxFileName &dest){
	wxDateTime dtsource, dtdest;
	source.GetTimes(NULL, &dtsource, NULL);
	dest.GetTimes(NULL, &dtdest, NULL);		
		
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

bool SyncBase::ShouldCopyShort(const wxFileName &source, const wxFileName &dest){
	//For more detailed comments see the ShouldCopyFull function
	std::unique_ptr<wxFileInputStream> sourcestream(new wxFileInputStream(source.GetFullPath()));
	std::unique_ptr<wxFileInputStream> deststream(new wxFileInputStream(dest.GetFullPath()));

	if(!sourcestream->IsOk() || !deststream->IsOk()){
		return false;
	}

	if(sourcestream->GetLength() != deststream->GetLength()){
		return true;
	}

    //Handle zero length files correctly
    if(sourcestream->GetLength() == 0){
        return false;
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

bool SyncBase::ShouldCopyFull(const wxFileName &source, const wxFileName &dest){
	std::unique_ptr<wxFileInputStream> sourcestream(new wxFileInputStream(source.GetFullPath()));
	std::unique_ptr<wxFileInputStream> deststream(new wxFileInputStream(dest.GetFullPath()));

	//Something is wrong with our streams, return false as
	//it is not a good idea to copy in this case
	if(!sourcestream->IsOk() || !deststream->IsOk()){
		return false;
	}
	
	//If we have different lengths then we need to copy
	if(sourcestream->GetLength() != deststream->GetLength()){
		return true;
	}

    //Handle zero length files correctly
    if(sourcestream->GetLength() == 0){
        return false;
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
