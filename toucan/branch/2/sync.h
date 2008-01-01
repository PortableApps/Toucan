/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2007 Steven Lamerton
// Licence:     GNU GPL 2 (See readme for more info
/////////////////////////////////////////////////////////////////////////////////

#include "basicfunctions.h"
#include "frmprogress.h"
#include <wx/dir.h>

bool SyncFile(SyncData data, Rules rules, frmProgress *window);

/*The main SyncLoop is initially called from the Sync function, and calls itself when it reaches a folder and calls SyncFille when a file is reached*/
bool SyncLoop(SyncData data, Rules rules, frmProgress *window)
{
	wxString strTo = data.GetDest();
	wxString strFrom = data.GetSource();
	//This section need to be updated for the new data stuff
	//Append a slash in case of an incorrect pass
	if (strTo[strTo.length()-1] != wxFILE_SEP_PATH) {
		strTo += wxFILE_SEP_PATH;       
	}
	// for both dirs
	if (strFrom[strFrom.length()-1] != wxFILE_SEP_PATH) {
		strFrom += wxFILE_SEP_PATH;       
	}
	if (!wxDirExists(strTo)){
            	if(!rules.ShouldExclude(strTo, true)){
            		wxMkdir(strTo);
		}
		else{
			return false;
		}
	}
	wxDir dir(strFrom);
	wxString strFilename;
	bool blDir = dir.GetFirst(&strFilename);
	if(blDir){
		do {
			if(wxDirExists(strFrom + strFilename))
			{
				if(!rules.ShouldExclude(strTo + strFilename, true))
				{ 
					if (wxDirExists(strTo + strFilename)){
						data.SetSource(strFrom + strFilename);
						data.SetDest(strTo + strFilename);
						SyncLoop(data, rules, window);
					}
					else{
						if(data.GetFunction() == _("Mirror")){
							RemoveDirectory(strTo + strFilename);
						}
						else{
							wxMkdir(strTo + strFilename);
							data.SetSource(strFrom + strFilename);
							data.SetDest(strTo + strFilename);
							SyncLoop(data, rules, window);
						}
					}
				}
			}
			else{
				data.SetSource(strFrom + strFilename);
				data.SetDest(strTo + strFilename);
				SyncFile(data, rules, window);
			}
		}
		while (dir.GetNext(&strFilename) );
	}  
	return true;
	
}

bool SyncFile(SyncData data, Rules rules, frmProgress *window)
{
	int iAttributes = FILE_ATTRIBUTE_NORMAL;
	if(!rules.ShouldExclude(data.GetDest(), false)){
		if(data.GetIgnoreRO()){
			iAttributes = GetFileAttributes(data.GetDest());
			SetFileAttributes(data.GetDest(),FILE_ATTRIBUTE_NORMAL); 
		} 
		if(data.GetFunction() == _("Copy")){	
			if(wxCopyFile(data.GetSource(), data.GetDest(), true)){
				OutputProgress(_("Copied ") + data.GetSource().AfterLast(wxFILE_SEP_PATH) + _(" to ") + data.GetSource(), window);
			}
		}	
		if(data.GetFunction() == _("Update")){
			/*Check to see if the desination file exists, if it does then a time check is made, if not then 
			the source file is always copied*/
			if(wxFileExists(data.GetDest())){	
				wxDateTime tmTo = wxFileModificationTime(data.GetDest());
				wxDateTime tmFrom = wxFileModificationTime(data.GetSource());
				if(data.GetIgnoreDLS()){
					tmFrom.MakeTimezone(wxDateTime::Local, true);
				}
				//I.E. strFrom is newer
				if(tmFrom.IsLaterThan(tmTo)){
					wxCopyFile(data.GetSource(), data.GetDest(), true);
					OutputProgress(_("Updated ") + data.GetSource().AfterLast(wxFILE_SEP_PATH) + _(" in ") + data.GetSource(), window);
				}
			}
			else{
				wxCopyFile(data.GetSource(), data.GetDest(), true);
				OutputProgress(_("Copied ") + data.GetSource().AfterLast(wxFILE_SEP_PATH)+  _(" to ") + data.GetSource(), window);
			}
		}
		if(data.GetFunction() == _("Mirror")){	
			if(!wxFileExists(data.GetDest())){
				wxRemoveFile(data.GetSource());
				OutputProgress(_("Removed ") + data.GetSource(), window);
			}
		}
		//Set the old attrributes back
		if(data.GetIgnoreRO()){
			SetFileAttributes(data.GetDest(), iAttributes); 
		} 
		//Code needs to be added for Linux, Mac also needs to be researched
		if(data.GetAttributes() == true){
			//#ifdef(__WXMSW__)
			int filearrtibs = GetFileAttributes(data.GetSource());
			SetFileAttributes(data.GetDest(),FILE_ATTRIBUTE_NORMAL);                       
			SetFileAttributes(data.GetDest(),filearrtibs);
			//#endif
		}
		if(data.GetTimeStamps()){
			wxFileName from(data.GetSource());
			wxFileName to(data.GetDest());
			wxDateTime access, mod, created;
			from.GetTimes(&access ,&mod ,&created );
			to.SetTimes(&access ,&mod , &created); 
		}			
	}
	return true;
}
