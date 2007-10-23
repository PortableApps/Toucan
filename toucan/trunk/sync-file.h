/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2007 Steven Lamerton
// Licence:     GNU GPL 2 (See readme for more info
/////////////////////////////////////////////////////////////////////////////////

#include <wx/filefn.h>
#include <wx/dir.h>
#include <wx/datetime.h>
#include <wx/filename.h>

#include "basicops.h"

bool SyncFile(wxString strFrom, wxString strTo, wxString strFunction, wxArrayString arrExclusions, bool blAttributes, frmProgress* window, int length1, int length2, bool blPreview, bool blVisible)
{
	wxFileName flName(strTo);
	unsigned int i;
	bool blEqual = false;
	//Check that the file does not have an excluded extension
	//for(i = 0; i <arrExclusions.GetCount(); i++)
	//{
		//if(flName.GetExt() != wxEmptyString  &&  wxT(".") + flName.GetExt() == arrExclusions.Item(i)){
		//	blEqual = true;
		//}
	//}
	//I.E. it is not equal to one of the excluded filenames
	if(!IsExcluded(strTo, arrExclusions, false)){
		if(strFunction == _("Copy")){
			if(!blPreview){
				if(wxFileExists(strFrom)){
					wxRemoveFile(strTo);
					wxCopyFile(strFrom, strTo, true);
                }
			}	
			wxString both1 = strFrom ;
			both1 = both1.Right(both1.Length() - length1);
			OutputProgress(_("Copied \t") + both1, window, blVisible);
		}
		if(strFunction == _("Update")){
			if(wxFileExists(strTo)){
				wxDateTime tmTo = wxFileModificationTime(strTo);
				wxDateTime tmFrom = wxFileModificationTime(strFrom);
				//I.E. strFrom is newer
				if(tmFrom > tmTo){
					if(!blPreview){
						wxRemoveFile(strTo);
						wxCopyFile(strFrom, strTo, true);
					}
					wxString both1 = strTo;
					both1 = both1.Right(both1.Length() - length2);
					OutputProgress(_("Overwrote \t") + both1, window, blVisible);
				}
			}
			else{
				if(!blPreview){
					wxCopyFile(strFrom, strTo, true);
				}
				wxString both1 = strFrom ;
				both1 = both1.Right(both1.Length() - length1);
				OutputProgress(_("Copied \t") + both1, window, blVisible);
			}
		}
		//Copy the file attributes and timestamps as required
		if(blAttributes == true){
			if(!blPreview){
				wxFileName from(strFrom);
				wxFileName to(strTo);
				int filearrtibs = GetFileAttributes(strFrom);
				SetFileAttributes(strTo,FILE_ATTRIBUTE_NORMAL);                       
				wxDateTime access, mod, created;
				from.GetTimes(&access ,&mod ,&created );
				to.SetTimes(&access ,&mod , &created); 
				SetFileAttributes(strTo,filearrtibs);
			}
		}
		wxGetApp().Yield(true);			
	}
	return true;
}