/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 - 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include "syncjob.h"
#include "../rules.h"
#include "../toucan.h"
#include "../data/syncdata.h"
#include "../basicfunctions.h"
#include "../fileops.h"
#include "../path.h"

#include <list>
#include <map>
#include <wx/string.h>
#include <wx/log.h>
#include <wx/dir.h>
#include <wx/filename.h>

SyncJob::SyncJob(SyncData *Data) : Job(Data){
	;
}

void* SyncJob::Entry(){
	SyncData *data = static_cast<SyncData*>(GetData());
	SyncFiles sync(data->GetSource(), data->GetDest(), data);
	sync.Execute();
	return NULL;
}

SyncFiles::SyncFiles(const wxFileName &syncsource, const wxFileName &syncdest, SyncData* syncdata) 
          : SyncBase(syncsource, syncdest, syncdata)
{
    Path::CreateDirectoryPath(sourceroot);
    Path::CreateDirectoryPath(destroot);
}

bool SyncFiles::Execute(){
	auto sourcepaths = FolderContentsToList(sourceroot);
	auto destpaths = FolderContentsToList(destroot);
	auto mergeresult = MergeListsToMap(sourcepaths, destpaths);
	OperationCaller(mergeresult);
	return true;
}

void SyncFiles::OperationCaller(std::map<wxString, int> paths){
	for(auto iter = paths.begin(); iter != paths.end(); ++iter){
		if(wxGetApp().GetAbort()){
			return;
		}
        wxFileName source = sourceroot.GetFullPath() + (*iter).first;
        wxFileName dest = destroot.GetFullPath() + (*iter).first;
		if(source.DirExists() || dest.DirExists()){
			if((*iter).second == 1){
				OnSourceNotDestFolder(source, dest);
			}
			else if((*iter).second == 2){
				OnNotSourceDestFolder(source, dest);				
			}
			else if((*iter).second == 3){
				OnSourceAndDestFolder(source, dest);
			}
		}
		//We have a file
		else{
			if((*iter).second == 1){
				OnSourceNotDestFile(source, dest);
			}
			else if((*iter).second == 2){
				OnNotSourceDestFile(source, dest);				
			}
			else if((*iter).second == 3){
				OnSourceAndDestFile(source, dest);
			}
		}
	}
	return;
}

void SyncFiles::OnSourceNotDestFile(const wxFileName &source, const wxFileName &dest){
	//Clean doesnt copy any files
	if(data->GetFunction() != _("Clean")){
		if(data->GetRules()->Matches(source) != Excluded){
			if(CopyIfNeeded(source, dest)){
				if(data->GetFunction() == _("Move")){
					RemoveFile(source);
				}
			}	
		}	
	}
}

void SyncFiles::OnNotSourceDestFile(const wxFileName &source, const wxFileName &dest){
	if(data->GetFunction() == _("Mirror") || data->GetFunction() == _("Clean")){
		if(data->GetRules()->Matches(dest) != Excluded){
			RemoveFile(dest);	
		}
	}
	else if(data->GetFunction() == _("Equalise")){
		if(data->GetRules()->Matches(dest) != Excluded){
			CopyIfNeeded(dest, source);
		}
	}
}

void SyncFiles::OnSourceAndDestFile(const wxFileName &source, const wxFileName &dest){
	if(data->GetFunction() == _("Copy") || data->GetFunction() == _("Mirror") || data->GetFunction() == _("Move")){
		if(data->GetRules()->Matches(source) != Excluded){
			if(CopyIfNeeded(source, dest)){
				if(data->GetFunction() == _("Move")){
					RemoveFile(source);
				}				
			}
		}
	}	
	else if(data->GetFunction() == _("Equalise")){
		SourceAndDestCopy(source, dest);
	}
}

void SyncFiles::OnSourceNotDestFolder(const wxFileName &source, const wxFileName &dest){
	//Always recurse into the next directory unless we have an absolute exclude
    RuleResult res = data->GetRules()->Matches(source);
    if(res != AbsoluteExcluded){
	    SyncFiles sync(source, dest, data);
	    sync.Execute();
    }
	if(data->GetFunction() != _("Clean")){
		wxDir destdir(dest.GetFullPath());
		wxDir sourcedir(source.GetFullPath());
		if(!destdir.HasFiles() && !destdir.HasSubDirs() && res != Excluded){
			RemoveDirectory(dest);
		}
		else{
			//Set the timestamps if needed
			if(data->GetTimeStamps()){
				CopyFolderTimestamp(source, dest);
			}	
		}
		if(!sourcedir.HasFiles() && !sourcedir.HasSubDirs() && data->GetFunction() == _("Move")){
			//If we are moving and there are no files left then we need to remove the folder
			RemoveDirectory(source);
		}
	}
}

void SyncFiles::OnNotSourceDestFolder(const wxFileName &source, const wxFileName &dest){
    RuleResult res = data->GetRules()->Matches(dest);
	if(data->GetFunction() == _("Mirror") || data->GetFunction() == _("Clean")){
		if(res != Excluded && res != AbsoluteExcluded){
			RemoveDirectory(dest);		
		}
	}
	else if(data->GetFunction() == _("Equalise")){
        if(res != AbsoluteExcluded){
		    SyncFiles sync(source, dest, data);
		    sync.Execute();
        }
		wxDir dir(source.GetFullPath());
		if(!dir.HasFiles() && !dir.HasSubDirs() && data->GetRules()->Matches(dest) != Excluded){
			RemoveDirectory(dest);
		}
		else{
			//Set the timestamps if needed
			if(data->GetTimeStamps()){
				CopyFolderTimestamp(dest, source);
			}	
		}
	}
}

void SyncFiles::OnSourceAndDestFolder(const wxFileName &source, const wxFileName &dest){
	//Always recurse into the next directory
    RuleResult res = data->GetRules()->Matches(source);
    if(res != AbsoluteExcluded){
	    SyncFiles sync(source, dest, data);
	    sync.Execute();
    }
	if(data->GetFunction() != _("Clean")){
		wxDir destdir(dest.GetFullPath());
		wxDir sourcedir(source.GetFullPath());
		if(!destdir.HasFiles() && !destdir.HasSubDirs() && res != Excluded && res != AbsoluteExcluded){
			RemoveDirectory(dest);
		}
		else{
			//Set the timestamps if needed
			if(data->GetTimeStamps()){
				CopyFolderTimestamp(source, dest);
			}	
		}
		if(!sourcedir.HasFiles() && !sourcedir.HasSubDirs() && data->GetFunction() == _("Move")){
			//If we are moving and there are no files left then we need to remove the folder
			RemoveDirectory(source);
		}
	}
}

bool SyncFiles::CopyFile(const wxFileName &source, const wxFileName &dest){
    wxString sourcepath = source.GetFullPath(), destpath = dest.GetFullPath();
	//ATTN : Needs linux support
	#ifdef __WXMSW__
		long destAttributes = 0;
		long sourceAttributes = 0;
		if(data->GetIgnoreRO() || data->GetAttributes()){
				destAttributes = dest.FileExists() ? GetFileAttributes(destpath.fn_str()) : FILE_ATTRIBUTE_NORMAL;
				sourceAttributes = source.FileExists() ? GetFileAttributes(sourcepath.fn_str()) : FILE_ATTRIBUTE_NORMAL;
		}
		if(data->GetIgnoreRO()){
			SetFileAttributes(destpath.fn_str(), FILE_ATTRIBUTE_NORMAL);
		}
	#endif

	wxString desttemp = dest.GetPathWithSep() + wxT("Toucan.tmp");
	if(File::Copy(source, desttemp)){
		if(File::Rename(desttemp, dest, true)){
			OutputProgress(_("Copied ") + sourcepath, Message);
		}
		else{
			OutputProgress(_("Failed to copy ") + sourcepath, Error);
			if(wxFileExists(desttemp)){
				wxRemoveFile(desttemp);
			}
			#ifdef __WXMSW__
				if(data->GetIgnoreRO()){
					SetFileAttributes(sourcepath, sourceAttributes); 
				}
			#endif 
			return false;
		}
	}
	else{
        OutputProgress(_("Failed to copy ") + sourcepath, Error);
		#ifdef __WXMSW__
			if(data->GetIgnoreRO()){
				SetFileAttributes(sourcepath.fn_str(), sourceAttributes); 
			} 
		#endif
		return false;
	}
	if(data->GetTimeStamps()){
		wxDateTime access, mod, created;
		if(source.GetTimes(&access, &mod, &created)){
			dest.SetTimes(&access, &mod, &created); 
		}
	}	
	#ifdef __WXMSW__
		if(data->GetIgnoreRO()){
			SetFileAttributes(destpath.fn_str(), destAttributes); 
			SetFileAttributes(sourcepath.fn_str(), sourceAttributes); 
		} 
		if(data->GetAttributes()){                   
			SetFileAttributes(destpath.fn_str(), sourceAttributes);
		}
	#endif
	return true;
}

bool SyncFiles::CopyIfNeeded(const wxFileName &source, const wxFileName &dest){
	//If the dest file doesn't exists then we must copy
	if(!dest.FileExists()){
        return CopyFile(source, dest);
	}
	//If copy if anything says copy
	if((data->GetCheckSize() && ShouldCopySize(source, dest))
    || (data->GetCheckTime() && ShouldCopyTime(source, dest))
    || (data->GetCheckShort() && ShouldCopyShort(source, dest))
    || (data->GetCheckFull() && ShouldCopyFull(source, dest))
    || (!data->GetCheckSize() && !data->GetCheckTime() 
    &&  !data->GetCheckShort() && !data->GetCheckFull())){
		return CopyFile(source, dest);
	}
    OutputProgress(_("Skipped ") + source.GetFullPath(), Message);
	return false;
}

bool SyncFiles::RemoveDirectory(const wxFileName &path){
	if(wxGetApp().GetAbort()){
		return true;
	}
	//If we have a root directory then return
	if (path.GetDirCount() ==0)
		return false;

	wxDir* dir = new wxDir(path.GetFullPath());
	wxString filename;
	if(dir->GetFirst(&filename)){
		do {
			if(wxGetApp().GetAbort()){
				return true;
			}
			if(wxDirExists(path.GetFullPath() + filename)){
				RemoveDirectory(wxFileName(path.GetFullPath() + filename));
			}
			else{
				if(RemoveFile(wxFileName(path.GetFullPath() + filename))){
                    OutputProgress(_("Removed ") + path.GetFullPath() + filename, Message);
                }
            }
	
		}
		while (dir->GetNext(&filename) );
	} 
	delete dir;
	{
  		wxLogNull log;
		if(RemoveDirectory(path)){
            OutputProgress(_("Removed ") + path.GetFullPath(), Message);
		}
	}
	return true;
}

bool SyncFiles::CopyFolderTimestamp(const wxFileName &source, const wxFileName &dest){
	wxDateTime access, mod, created;
	source.GetTimes(&access, &mod, &created);
	dest.SetTimes(&access, &mod, &created); 
	return true;
}

bool SyncFiles::RemoveFile(const wxFileName &path){
	if(File::Delete(path, data->GetRecycle(), data->GetIgnoreRO())){
        OutputProgress(_("Removed ") + path.GetFullPath(), Message);		
		return true;
	}
	return false;
}

bool SyncFiles::SourceAndDestCopy(const wxFileName &source, const wxFileName &dest){
	wxDateTime to, from;
	dest.GetTimes(NULL, &to, NULL);
	source.GetTimes(NULL, &from, NULL);		

	from.MakeTimezone(wxDateTime::UTC, true);
	to.MakeTimezone(wxDateTime::UTC, true);

	if(from.IsLaterThan(to)){
		if(data->GetRules()->Matches(source) != Excluded){
			CopyIfNeeded(source, dest);			
		}
	}
	else if(to.IsLaterThan(from)){
		if(data->GetRules()->Matches(source) != Excluded){
			CopyIfNeeded(dest, source);
		}
	}
	return true;	
}
