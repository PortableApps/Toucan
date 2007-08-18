/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2007 Steven Lamerton
// Licence:     GNU GPL 2 (See readme for more info
/////////////////////////////////////////////////////////////////////////////////

#include <wx/filefn.h>
#include <wx/dir.h>

bool RemoveDir(wxString strLocation)
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
				RemoveDirectory(strLocation + strFilename);
			}
			else{
				wxRemoveFile(strLocation + strFilename);
			}
	
		}
		while (dir->GetNext(&strFilename) );
	} 
	delete dir;
  	wxLogNull log;
	wxFileName::Rmdir(strLocation);
	return true;
}

bool MirrorDir(wxString strOriginal, wxString strNew, frmProgress* window, int length1, int length2, bool blPreview)
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
                    					RemoveDir(strNew + strFilename);
                    				}
                			}
                			else{
                    				MirrorDir(strOriginal+ strFilename, strNew+ strFilename,window, length1, length2, blPreview);
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
                    				window->m_Progress_Text->AppendText(_("\nRemoved \t") + both1);
                			}
            			}
        		} while (dir.GetNext(&strFilename) );
    	}  
    	return true;
}