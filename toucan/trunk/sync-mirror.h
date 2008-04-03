/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2007 Steven Lamerton
// Licence:     GNU GPL 2 (See readme for more info
/////////////////////////////////////////////////////////////////////////////////

#include <wx/filefn.h>
#include <wx/dir.h>

bool RemoveDirectory(wxString strLocation, bool blVisible, frmProgress* window)
{
	// if it's a possible root directory
	if (strLocation.length() <= 3) {
		wxLogError(_("Toucan tried to delete a root directory. This has been forbidden, /nif you really need to delete ") + strLocation + _(" please contact me at www.portableapps.com"));
		return false;
	}
	if(wxGetApp().GetStrAbort() == wxT("ABORT")){
		return false;
	}
	//Make sure that the correct ending is appended
	if (strLocation[strLocation.length()-1] != wxFILE_SEP_PATH) {
		strLocation += wxFILE_SEP_PATH;       
	}
	
	wxDir* dir = new wxDir(strLocation);
	wxString strFilename;
	bool blDir = dir->GetFirst(&strFilename);
	if(blDir){
		do {
			if(wxGetApp().GetStrAbort() == wxT("ABORT")){
				return false;	
			}
			if(wxDirExists(strLocation + strFilename)){
				RemoveDirectory(strLocation + strFilename, blVisible, window);
			}
			else{
				if(wxRemoveFile(strLocation + strFilename)){
                    OutputProgress(_("Removed ") + strLocation + strFilename, window, blVisible);
                }
            }
	
		}
		while (dir->GetNext(&strFilename) );
	} 
	delete dir;
  	wxLogNull log;
    //wxMessageBox(_("About to try and remove ") + strLocation);
	if(wxFileName::Rmdir(strLocation)){
        OutputProgress(_("Removed ") + strLocation, window, blVisible);
    }
    
	return true;
}

bool MirrorDir(wxString strOriginal, wxString strNew, frmProgress* window, int length1, int length2, bool blPreview, bool blVisible)
{
    	if(wxGetApp().GetStrAbort() == wxT("ABORT")){
        		return false;
    	}
        //Make sure that the correct ending is appended
        if (strOriginal[strOriginal.length()-1] != wxFILE_SEP_PATH) {
            		strOriginal += wxFILE_SEP_PATH;       
    	}
    	// for both dirs
    	if (strNew[strNew.length()-1] != wxFILE_SEP_PATH) {
            		strNew += wxFILE_SEP_PATH;       
    	}

    	wxDir dir(strNew);
    	wxString strFilename;
    	bool blDir = dir.GetFirst(&strFilename);
    	if (blDir){
        		do {
            			if(wxGetApp().GetStrAbort() == wxT("ABORT")){
            				return false;
            			}
            			if (wxDirExists(strNew + strFilename)){
                			if(!wxDirExists(strOriginal + strFilename)){
                    				if(!blPreview){
                                        //wxMessageBox(_("Remove dir on ") + strNew + strFilename);
                    					RemoveDirectory(strNew + strFilename, blVisible, window);
                                        
                    				}
                    				wxString both1 = strNew + strFilename;
                    				both1 = both1.Right(both1.Length() - length1);
                    				OutputProgress(_("\nRemoved \t") + both1, window, blVisible);
                			}
                			else{
                    				MirrorDir(strOriginal+ strFilename, strNew+ strFilename,window, length1, length2, blPreview, blVisible);
                			}  
            			}
            			else{
                			if(wxGetApp().GetStrAbort() == wxT("ABORT")){
                				return false;
               			}
                			if(!wxFileExists(strOriginal + strFilename)){
                    				if(!blPreview){
                    					wxRemoveFile(strNew + strFilename);
                    				}
                    				wxString both1 = strNew + strFilename;
                    				both1 = both1.Right(both1.Length() - length1);
                    				OutputProgress(_("\nRemoved \t") + both1, window, blVisible);
                			}
            			}
        		} while (dir.GetNext(&strFilename) );
    	}  
    	return true;
}