/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "syncjob.h"
#include "../rules.h"
#include "../toucan.h"
#include "../variables.h"
#include "../data/syncdata.h"
#include "../basicfunctions.h"

#include <list>
#include <map>
#include <wx/string.h>
#include <wx/datetime.h>
#include <wx/filename.h>
#include <wx/wfstream.h>
#include <wx/log.h>
#include <wx/dir.h>

SyncJob::SyncJob(SyncData *Data) : Job(Data){
	;
}

void* SyncJob::Entry(){
	SyncData *data = static_cast<SyncData*>(GetData());
	SyncFiles sync(data->GetSource(), data->GetDest(), data);
	sync.Execute();
	return NULL;
}

SyncFiles::SyncFiles(const wxString &syncsource, const wxString &syncdest, SyncData* syncdata) 
          : SyncBase(syncsource, syncdest, syncdata){
	this->preview = false;
}

bool SyncFiles::Execute(){
	std::list<wxString> sourcepaths = FolderContentsToList(sourceroot);
	std::list<wxString> destpaths = FolderContentsToList(destroot);
	std::map<wxString, int> mergeresult = MergeListsToMap(sourcepaths, destpaths);
	OperationCaller(mergeresult);
	return true;
}

void SyncFiles::OnSourceNotDestFile(const wxString &path){
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
}

void SyncFiles::OnNotSourceDestFile(const wxString &path){
	wxString source = sourceroot + wxFILE_SEP_PATH + path;
	wxString dest = destroot + wxFILE_SEP_PATH + path;
	if(data->GetFunction() == _("Mirror") || data->GetFunction() == _("Clean")){
		if(!data->GetRules()->ShouldExclude(dest, false)){
			RemoveFile(dest);	
		}
	}
	else if(data->GetFunction() == _("Equalise")){
		if(!data->GetRules()->ShouldExclude(dest, false)){
			CopyFilePlain(dest, source);
		}
	}
}

void SyncFiles::OnSourceAndDestFile(const wxString &path){
	wxString source = sourceroot + wxFILE_SEP_PATH + path;
	wxString dest = destroot + wxFILE_SEP_PATH + path;
	if(data->GetFunction() == _("Copy") || data->GetFunction() == _("Mirror") || data->GetFunction() == _("Move")){
		if(!data->GetRules()->ShouldExclude(source, false)){
			if(CopyFileStream(source, dest)){
				if(data->GetFunction() == _("Move")){
					RemoveFile(source);
				}				
			}
		}
	}
	else if(data->GetFunction() == _("Update")){
		if(!data->GetRules()->ShouldExclude(source, false)){
			CopyFileTimestamp(source, dest);
		}
	}		
	else if(data->GetFunction() == _("Equalise")){
		SourceAndDestCopy(source, dest);
	}
}

void SyncFiles::OnSourceNotDestFolder(const wxString &path){
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
}

void SyncFiles::OnNotSourceDestFolder(const wxString &path){
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
}

void SyncFiles::OnSourceAndDestFolder(const wxString &path){
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
}

bool SyncFiles::CopyFile(const wxString &source, const wxString &dest){
#ifdef __WXMSW__
	return CopyFileEx(source.fn_str(), dest.fn_str(), &CopyProgressRoutine, NULL, NULL, 0);
#else
	return wxCopyFile(source, dest, true);
#endif
}

bool SyncFiles::CopyFilePlain(const wxString &source, const wxString &dest){
	//ATTN : Needs linux support
	#ifdef __WXMSW__
		long destAttributes = 0;
		long sourceAttributes = 0;
		if(data->GetIgnoreRO() || data->GetAttributes()){
				destAttributes = wxFileExists(dest) ? GetFileAttributes(dest) : FILE_ATTRIBUTE_NORMAL;
				sourceAttributes = wxFileExists(source) ? GetFileAttributes(source) : FILE_ATTRIBUTE_NORMAL;
		}
		if(data->GetIgnoreRO()){
			SetFileAttributes(dest, FILE_ATTRIBUTE_NORMAL);
		}
	#endif

	wxString desttemp = wxPathOnly(dest) + wxFILE_SEP_PATH + wxT("Toucan.tmp");
	if(CopyFile(source, desttemp)){
		if(wxRenameFile(desttemp, dest, true)){
			OutputProgress(_("Copied ") + source);
		}
		else{
			OutputProgress(_("Failed to copy ") + source, true, true);
			if(wxFileExists(desttemp)){
				wxRemoveFile(desttemp);
			}
			#ifdef __WXMSW__
				if(data->GetIgnoreRO()){
					SetFileAttributes(source, sourceAttributes); 
				}
			#endif 
			return false;
		}
	}
	else{
		OutputProgress(_("Failed to copy ") + source, true, true);
		#ifdef __WXMSW__
			if(data->GetIgnoreRO()){

				SetFileAttributes(source, sourceAttributes); 
			} 
		#endif
		return false;
	}
	if(data->GetTimeStamps()){
		wxFileName from(source);
		wxFileName to(dest);
		wxDateTime access, mod, created;
		if(from.GetTimes(&access, &mod, &created)){
			to.SetTimes(&access, &mod, &created); 
		}
	}	
	#ifdef __WXMSW__
		if(data->GetIgnoreRO()){
			SetFileAttributes(dest, destAttributes); 
			SetFileAttributes(source, sourceAttributes); 
		} 
		if(data->GetAttributes()){                   
			SetFileAttributes(dest, sourceAttributes);
		}
	#endif
	return true;
}

void SyncFiles::CopyFileTimestamp(const wxString &source, const wxString &dest){
	wxDateTime tmTo, tmFrom;
	wxFileName flTo(dest);
	wxFileName flFrom(source);
	flTo.GetTimes(NULL, &tmTo, NULL);
	flFrom.GetTimes(NULL, &tmFrom, NULL);		

	if(data->GetIgnoreDLS()){
		tmFrom.MakeTimezone(wxDateTime::Local, true);
	}
	//If they are within two seconds of each other then they are 
	//likely the same due to filesystem differences (esp ext3)
	if(tmFrom.IsEqualUpTo(tmTo, wxTimeSpan(0, 0, 2, 0))){
		return;
	}
	else if(tmFrom.IsLaterThan(tmTo)){
		CopyFileStream(source, dest);
	}
}

//Returns true is there were no errors
bool SyncFiles::CopyFileStream(const wxString &source, const wxString &dest){
	if(disablestreams){
		return CopyFilePlain(source, dest);
	}

	wxFileInputStream *sourcestream = new wxFileInputStream(source);
	wxFileInputStream *deststream = new wxFileInputStream(dest);

	//Something is wrong with our streams, return error
	if(!sourcestream->IsOk() || !deststream->IsOk()){
		OutputProgress(_("Failed to copy ") + source, true, true);
		delete sourcestream;
		delete deststream;
		return false;
	}
	
	if(sourcestream->GetLength() != deststream->GetLength()){
		delete sourcestream;
		delete deststream;
		return CopyFilePlain(source, dest);
	}

	wxFileOffset size = sourcestream->GetLength();

	//We read in 4KB chunks as testing seems to show they are the fastest
	char *sourcebuf = new char[4096];
	char *destbuf = new char [4096];
	wxFileOffset bytesLeft = size;
	while(bytesLeft > 0){
		wxFileOffset bytesToRead = wxMin(4096, bytesLeft);
		sourcestream->Read(sourcebuf, bytesToRead);
		deststream->Read(destbuf, bytesToRead);
		if(sourcestream->GetLastError() != wxSTREAM_NO_ERROR || deststream->GetLastError() != wxSTREAM_NO_ERROR){
			OutputProgress(_("Failed to copy ") + source, true, true);
			delete sourcestream;
			delete deststream;
			delete[] sourcebuf;
			delete[] destbuf;
			return false;
		}
		if(strncmp(sourcebuf, destbuf, bytesToRead) != 0){
			delete sourcestream;
			delete deststream;
			delete[] sourcebuf;
			delete[] destbuf;
			return CopyFilePlain(source, dest);
		}
		bytesLeft-=bytesToRead;
	}
	delete sourcestream;
	delete deststream;
	delete[] sourcebuf;
	delete[] destbuf;
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
				if(RemoveFile(path + filename)){
					//We have to increment the gauge for ourself here
					if(wxGetApp().IsGui()){
						IncrementGauge();					
					}
                }
            }
	
		}
		while (dir->GetNext(&filename) );
	} 
	delete dir;
	{
  		wxLogNull log;
		if(wxFileName::Rmdir(path)){
			OutputProgress(_("Removed ") + path);
		}
	}
	return true;
}

bool SyncFiles::CopyFolderTimestamp(const wxString &source, const wxString &dest){
	wxFileName from(source);
	wxFileName to(dest);
	wxDateTime access, mod, created;
	from.GetTimes(&access, &mod, &created);
	to.SetTimes(&access, &mod, &created); 
	return true;
}

bool SyncFiles::RemoveFile(const wxString &path){
#ifdef __WXMSW__
	wxString tmppath(path);
	tmppath += wxT('\0');
	
	int flags = data->GetRecycle() ? FOF_ALLOWUNDO : 0;
	SHFILEOPSTRUCT opstruct;
	ZeroMemory(&opstruct, sizeof(opstruct));
	opstruct.wFunc = FO_DELETE;
	opstruct.pFrom = tmppath.fn_str();
	opstruct.fFlags = flags | FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI;
	if(SHFileOperation(&opstruct) == 0){
		OutputProgress(_("Removed ") + path);
		return true;
	}
#else
	if(wxRemoveFile(path)){
		OutputProgress(_("Removed ") + path);		
		return true;
	}
#endif
	return false;
}

bool SyncFiles::SourceAndDestCopy(const wxString &source, const wxString &dest){
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
			CopyFileStream(source, dest);			
		}
	}
	else if(tmTo.IsLaterThan(tmFrom)){
		if(!data->GetRules()->ShouldExclude(dest, false)){
			CopyFileStream(dest, source);
		}
	}
	return true;	
}

#ifdef __WXMSW__
DWORD CALLBACK CopyProgressRoutine(LARGE_INTEGER WXUNUSED(TotalFileSize), LARGE_INTEGER WXUNUSED(TotalBytesTransferred), 
									LARGE_INTEGER WXUNUSED(StreamSize), LARGE_INTEGER WXUNUSED(StreamBytesTransferred), 
									DWORD WXUNUSED(dwStreamNumber), DWORD WXUNUSED(dwCallbackReason),
									HANDLE WXUNUSED(hSourceFile), HANDLE WXUNUSED(hDestinationFile), 
									LPVOID WXUNUSED(lpData)){
	if(wxGetApp().GetAbort()){
		return PROGRESS_CANCEL;
	}
	else{
		return PROGRESS_CONTINUE;
	}
}
#endif
