/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2007 Steven Lamerton
// Licence:     GNU GPL 2 (See readme for more info
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SYNCFILE
#define H_SYNCFILE

#include <wx/filefn.h>
#include <wx/dir.h>
#include <wx/datetime.h>
#include <wx/filename.h>

#include "basicops.h"
#include "syncdata.h"
#include "rules.h"

bool SyncFile(SyncData data, Rules rules)
{
	unsigned int i;
	if(!rules.Matches(data.GetDest(), false)){
		int iAttributes;
		if(blIgnoreRO){
			iAttributes = GetFileAttributes(data.GetDest());
			SetFileAttributes(data.GetDest(),FILE_ATTRIBUTE_NORMAL); 
		} 
		if(data.GetFunction() == _("Copy")){	
			if(wxCopyFile(data.GetSource(), data.GetDest(), true)){
				OutputProgress(data);
			}
			else{
				OutputError(data);
			}
                	}	
		if(strFunction == _("Update")){
			/*Check to see if the desination file exists, if it does then a time check is made, if not then 
			the source file is always copied*/
			if(wxFileExists(data.GetDest())){	
				wxDateTime tmTo = wxFileModificationTime(data.GetDest());
				wxDateTime tmFrom = wxFileModificationTime(data.GetSource());
				//I.E. strFrom is newer
				if(tmFrom.IsLaterThan(tmTo)){
					wxCopyFile(data.GetSource, data.GetDest(), true);
					OutputProgress(data);
				}
			}
			else{
				wxCopyFile(data.GetSource(), data.GetDest(), true);
				OutputProgress(data);
			}
		}
		if(data.GetFunction() == _("Mirror")){	
			if(!wxFileExists(data.GetDest()){
				wxRemoveFile(data.GetSource());
				OutputProgress(data);
			}
                	}
		//Set the old attrributes back
		if(blIgnoreRO){
			SetFileAttributes(data.GetDest(), iArributes); 
		} 
		//Code needs to be added for Linux, Mac also needs to be researched
		if(blAttributes == true){
			#indef(__wxMSW__)
			int filearrtibs = GetFileAttributes(data.GetSource());
			SetFileAttributes(data.GetDest(),FILE_ATTRIBUTE_NORMAL);                       
			SetFileAttributes(data.GetDest(),filearrtibs);
			#endif
		}
		if(blTimestamps){
			wxFileName from(data.GetSoh'urce());
			wxFileName to(data.GetDest());
			wxDateTime access, mod, created;
			from.GetTimes(&access ,&mod ,&created );
			to.SetTimes(&access ,&mod , &created); 
		}			
	}
	return true;
}

#endif
