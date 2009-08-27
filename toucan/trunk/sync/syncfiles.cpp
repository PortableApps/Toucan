/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "syncfiles.h"
#include "../rules.h"
#include "../toucan.h"
#include "../basicfunctions.h"
#include "../data/syncdata.h"
#include <list>
#include <map>
#include <wx/string.h>
#include <wx/datetime.h>
#include <wx/filename.h>
#include <wx/wfstream.h>
#include <wx/dir.h>

SyncFiles::SyncFiles(wxString syncsource, wxString syncdest, SyncData* syncdata){
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
	this->data = syncdata;
	this->preview = false;
}

bool SyncFiles::Execute(){
	std::list<wxString> sourcepaths = FolderContentsToList(sourceroot);
	std::list<wxString> destpaths = FolderContentsToList(destroot);
	std::map<wxString, short> mergeresult = MergeListsToMap(sourcepaths, destpaths);
	OperationCaller(mergeresult);
	return true;
}

bool SyncFiles::OnSourceNotDestFile(wxString path){
	wxString source = sourceroot + wxFILE_SEP_PATH + path;
	wxString dest = destroot + wxFILE_SEP_PATH + path;
	//Clean doesnt copy any files
	if(data->GetFunction() != _("Clean")){
		if(!data->GetRules()->ShouldExclude(source, false)){
			if(CopyFilePlain(source, dest)){
				if(data->GetFunction() == _("Move")){
					RemoveFile(source);
				}
			}	
		}	
	}
	return true;
}
bool SyncFiles::OnNotSourceDestFile(wxString path){
	wxString source = sourceroot + wxFILE_SEP_PATH + path;
	wxString dest = destroot + wxFILE_SEP_PATH + path;
	if(data->GetFunction() == _("Mirror") || data->GetFunction() == _("Clean")){
		if(!data->GetRules()->ShouldExclude(dest, false)){
			RemoveFile(dest);			
		}
	}
	else if(data->GetFunction() == _("Equalise")){
		if(!data->GetRules()->ShouldExclude(dest, false)){
			//Swap them around as we are essentially in reverse
			CopyFilePlain(dest, source);
		}
	}
	return true;
}
bool SyncFiles::OnSourceAndDestFile(wxString path){
	wxString source = sourceroot + wxFILE_SEP_PATH + path;
	wxString dest = destroot + wxFILE_SEP_PATH + path;
	if(!data->GetRules()->ShouldExclude(source, false)){
		if(data->GetFunction() == _("Copy") || data->GetFunction() == _("Mirror") || data->GetFunction() == _("Move")){
			//Use the hash check version to minimise copying
			if(CopyFileHash(source, dest)){
				if(data->GetFunction() == _("Move")){
					RemoveFile(source);
				}				
			}
		}
		else if(data->GetFunction() == _("Update")){
			CopyFileTimestamp(source, dest);			
		}		
	}
	if(data->GetFunction() == _("Equalise")){
		SourceAndDestCopy(source, dest);
	}
	return true;
}
bool SyncFiles::OnSourceNotDestFolder(wxString path){
	//Call the function on the next subfolder
	wxString source = sourceroot + wxFILE_SEP_PATH + path;
	wxString dest = destroot + wxFILE_SEP_PATH + path;
	//Always recurse into the next directory
	SyncFiles sync(source, dest, data);
	sync.Execute();
	if(data->GetFunction() != _("Clean")){
		wxDir destdir(dest);
		wxDir sourcedir(source);
		if(!destdir.HasFiles() && !destdir.HasSubDirs() && data->GetRules()->ShouldExclude(source, true)){
			wxRmdir(dest);
			return false;
		}
		else{
			//Set the timestamps if needed
			if(data->GetTimeStamps()){
				CopyFolderTimestamp(source, dest);
			}	
		}
		if(!sourcedir.HasFiles() && !sourcedir.HasSubDirs() && data->GetFunction() == _("Move")){
			//If we are moving and there are no files left then we need to remove the folder
			wxRmdir(source);
		}
	}
	return true;
}
bool SyncFiles::OnNotSourceDestFolder(wxString path){
	wxString source = sourceroot + wxFILE_SEP_PATH + path;
	wxString dest = destroot + wxFILE_SEP_PATH + path;
	if(data->GetFunction() == _("Mirror") || data->GetFunction() == _("Clean")){
		if(!data->GetRules()->ShouldExclude(dest, true)){
			RemoveDirectory(dest);		
		}
	}
	else if(data->GetFunction() == _("Equalise")){
		SyncFiles sync(source, dest, data);
		sync.Execute();
		wxDir dir(source);
		if(!dir.HasFiles() && !dir.HasSubDirs() && data->GetRules()->ShouldExclude(dest, true)){
			wxRmdir(dest);
		}
		else{
			//Set the timestamps if needed
			if(data->GetTimeStamps()){
				CopyFolderTimestamp(dest, source);
			}	
		}
	}
	return true;
}
bool SyncFiles::OnSourceAndDestFolder(wxString path){
	wxString source = sourceroot + wxFILE_SEP_PATH + path;
	wxString dest = destroot + wxFILE_SEP_PATH + path;
	//Always recurse into the next directory
	SyncFiles sync(source, dest, data);
	sync.Execute();
	if(data->GetFunction() != _("Clean")){
		wxDir destdir(dest);
		wxDir sourcedir(source);
		if(!destdir.HasFiles() && !destdir.HasSubDirs() && data->GetRules()->ShouldExclude(source, true)){
			wxRmdir(dest);
			return false;
		}
		else{
			//Set the timestamps if needed
			if(data->GetTimeStamps()){
				CopyFolderTimestamp(source, dest);
			}	
		}
		if(!sourcedir.HasFiles() && !sourcedir.HasSubDirs() && data->GetFunction() == _("Move")){
			//If we are moving and there are no files left then we need to remove the folder
			wxRmdir(source);
		}
	}
	return true;
}

bool SyncFiles::CopyFilePlain(wxString source, wxString dest){
	bool ShouldTimeStamp = false;
	#ifdef __WXMSW__
		long destAttributes = 0;
		long sourceAttributes = 0;
	#endif
	if(data->GetIgnoreRO()){
		#ifdef __WXMSW__
			destAttributes = GetFileAttributes(dest);
			if(destAttributes == -1){
				destAttributes = FILE_ATTRIBUTE_NORMAL;					
			}
			SetFileAttributes(dest,FILE_ATTRIBUTE_NORMAL); 
			sourceAttributes = GetFileAttributes(source);
			if(sourceAttributes == -1){
				sourceAttributes = FILE_ATTRIBUTE_NORMAL;					
			}
			SetFileAttributes(source,FILE_ATTRIBUTE_NORMAL); 
		#endif
	} 

	if(wxCopyFile(source, wxPathOnly(dest) + wxFILE_SEP_PATH + wxT("Toucan.tmp"), true)){
		if(wxRenameFile(wxPathOnly(dest) + wxFILE_SEP_PATH + wxT("Toucan.tmp"), dest, true)){
			OutputProgress(_("Copied ") + source);
			ShouldTimeStamp = true;
		}
		else{
			return false;
		}
	}
	else{
		return false;
	}
	if(wxFileExists(wxPathOnly(dest) + wxFILE_SEP_PATH + wxT("Toucan.tmp"))){
		if(!wxRemoveFile(wxPathOnly(dest) + wxFILE_SEP_PATH + wxT("Toucan.tmp"))){
			return false;
		}
	}
	if(data->GetTimeStamps() && ShouldTimeStamp){
		wxFileName from(source);
		wxFileName to(dest);
		wxDateTime access, mod, created;
		from.GetTimes(&access ,&mod ,&created );
		to.SetTimes(&access ,&mod , &created); 
	}	
	//Set the old attributes back
	if(data->GetIgnoreRO()){
		#ifdef __WXMSW__
			SetFileAttributes(dest, destAttributes); 
			SetFileAttributes(source, sourceAttributes); 
		#endif
	} 
	if(data->GetAttributes()){
		#ifdef __WXMSW__
			int filearrtibs = GetFileAttributes(source);
			SetFileAttributes(dest,FILE_ATTRIBUTE_NORMAL);                       
			SetFileAttributes(dest,filearrtibs);
		#endif
	}
	return true;
}

bool SyncFiles::CopyFileTimestamp(wxString source, wxString dest){
	wxDateTime tmTo, tmFrom;
	wxFileName flTo(dest);
	wxFileName flFrom(source);
	flTo.GetTimes(NULL, &tmTo, NULL);
	flFrom.GetTimes(NULL, &tmFrom, NULL);		

	if(data->GetIgnoreDLS()){
		tmFrom.MakeTimezone(wxDateTime::Local, true);
	}
	if(tmFrom.IsLaterThan(tmTo)){
		return CopyFileHash(source, dest);
	}
	return false;
}

bool SyncFiles::CopyFileHash(wxString source, wxString dest){
	if(disablestreams){
		return CopyFilePlain(source, dest);
	}

	wxFileInputStream sourcestream(source);
	wxFileInputStream deststream(dest);

	//Something is wrong with our streams, return error
	if(!sourcestream.IsOk() || !deststream.IsOk()){
		OutputProgress(_("Failed to copy ") + source, wxDateTime::Now().FormatTime(), true);
		return false;
	}
	
	if(sourcestream.GetLength() != deststream.GetLength()){
		return CopyFilePlain(source, dest);			
	}

	//Large files take forever to read (I think the boundary is 2GB), better off just to copy
	wxFileOffset size = sourcestream.GetLength();
	if(size > 2147483648UL){
		return CopyFilePlain(source, dest);			
	}

	//We read in 1MB chunks
	char *sourcebuf = new char[1048576];
	char *destbuf = new char [1048576];
	wxFileOffset bytesLeft=size;
	while(bytesLeft > 0){
		wxFileOffset bytesToRead=wxMin((wxFileOffset) sizeof(sourcebuf),bytesLeft);
		sourcestream.Read((void*)sourcebuf,bytesToRead);
		deststream.Read((void*)destbuf,bytesToRead);
		if(sourcestream.GetLastError() != wxSTREAM_NO_ERROR || deststream.GetLastError() != wxSTREAM_NO_ERROR){
			OutputProgress(_("Failed to copy ") + source, wxDateTime::Now().FormatTime(), true);
			return false;
		}
		if(strncmp(sourcebuf, destbuf, bytesToRead) != 0){
			//Our strings differ, so copy the files
			//ATTN : In future update the files in place
			return CopyFilePlain(source, dest);
		}
		bytesLeft-=bytesToRead;
	}
	//The two files are actually the same, but update the timestamps
	if(data->GetTimeStamps()){
		wxFileName from(source);
		wxFileName to(dest);
		wxDateTime access, mod, created;
		from.GetTimes(&access ,&mod ,&created );
		to.SetTimes(&access ,&mod , &created); 
	}	
	return true;
}

bool SyncFiles::RemoveDirectory(wxString path){
	if(wxGetApp().GetAbort()){
		return true;
	}
	//Make sure that the correct ending is appended
	if (path[path.length()-1] != wxFILE_SEP_PATH) {
		path += wxFILE_SEP_PATH;       
	}
	//If we have a root directory then return
	if (path.length() <= 3) {
		return false;
	}
	wxDir* dir = new wxDir(path);
	wxString filename;
	bool blDir = dir->GetFirst(&filename);
	if(blDir){
		do {
			if(wxGetApp().GetAbort()){
				return true;
			}
			if(wxDirExists(path + filename)){
				RemoveDirectory(path + filename);
			}
			else{
				if(wxRemoveFile(path + filename)){
                    OutputProgress(_("Removed ") + path + filename);
					//We have to increment the gauge for ourself here
					if(wxGetApp().GetUsesGUI()){
						IncrementGauge();					
					}
                }
            }
	
		}
		while (dir->GetNext(&filename) );
	} 
	delete dir;
  	wxLogNull log;
	if(wxFileName::Rmdir(path)){
        OutputProgress(_("Removed ") + path);
    }
	return true;
}

bool SyncFiles::CopyFolderTimestamp(wxString source, wxString dest){
	wxFileName from(source);
	wxFileName to(dest);
	wxDateTime access, mod, created;
	from.GetTimes(&access ,&mod ,&created );
	to.SetTimes(&access ,&mod , &created); 
	return true;
}

bool SyncFiles::RemoveFile(wxString path){
	if(wxRemoveFile(path)){
		OutputProgress(_("Removed ") + path);			
	}
	return true;
}

bool SyncFiles::SourceAndDestCopy(wxString source, wxString dest){
	wxDateTime tmTo, tmFrom;
	wxFileName flTo(dest);
	wxFileName flFrom(source);
	flTo.GetTimes(NULL, &tmTo, NULL);
	flFrom.GetTimes(NULL, &tmFrom, NULL);		

	if(data->GetIgnoreDLS()){
		tmFrom.MakeTimezone(wxDateTime::Local, true);
	}

	if(tmFrom.IsLaterThan(tmTo)){
		if(!data->GetRules()->ShouldExclude(source, false)){
			CopyFileHash(source, dest);			
		}
	}
	else if(tmTo.IsLaterThan(tmFrom)){
		if(!data->GetRules()->ShouldExclude(dest, false)){
			CopyFileHash(dest, source);
		}
	}
	return true;	
}
