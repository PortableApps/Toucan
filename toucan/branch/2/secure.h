/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2007 Steven Lamerton
// Licence:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include <wx/file.h>
#include <wx/filefn.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/log.h>
#include <wx/stdpaths.h>

#include "securedata.h"




bool CryptFile(wxString strFile, SecureData data, Rules rules, frmProgress *window);
bool CryptDir(wxString strPath, SecureData data, Rules rules, frmProgress* window);


bool Secure(SecureData data, Rules rules, frmProgress *window){
	OutputProgress(_("Starting..."), window);
	//wxGetApp().SetStrAbort(wxEmptyString);
	unsigned int i;
	wxArrayString arrLocation = data.GetLocations();
	//Iterate through the entries in the array
	for(i = 0; i < arrLocation.Count(); i++)
	{
		//if(wxGetApp().GetStrAbort() == wxT("ABORT")){
		//return false;
		//}
		//Need to add normalisation to SecureData
		if(arrLocation.Item(i) != wxEmptyString){
			if(wxDirExists(arrLocation.Item(i))){
				//MessageBox(arrLocation.Item(i), _("Boo"));
				CryptDir(arrLocation.Item(i), data, rules, window);
			}
			else{
				CryptFile(arrLocation.Item(i), data, rules, window);
            			}
		}
	}
	OutputProgress(_("Finished..."), window);
	return true;
}


/*The main loop for the Secure process. It is called by Secure initially and then either calls itself when it reaches a
folder or CryptFile when it reaches a file.*/
bool CryptDir(wxString strPath, SecureData data, Rules rules, frmProgress* window)
{   
	//if(rules.ShouldExclude(strPath, true)){
	//	return true;
	//}
	wxDir dir(strPath);
	wxString filename;
	bool blDir = dir.GetFirst(&filename);
	if (blDir)
	{
		do {
			if (wxDirExists(strPath + wxFILE_SEP_PATH + filename) ){
				CryptDir(strPath + wxFILE_SEP_PATH + filename, data, rules, window);
			}
			else{
				//MessageBox(_("Called CryptFile"), _("2"));
				CryptFile(strPath + wxFILE_SEP_PATH + filename, data, rules, window);
			}

		}
		while (dir.GetNext(&filename) );
	}   
	return true;
}


bool CryptFile(wxString strFile, SecureData data, Rules rules, frmProgress* window)
{
	if(rules.ShouldExclude(strFile, false)){
		return true;
	}
	//wxMessageBox(_("Cont"));
	//Make sure that it is a 'real' file
	wxFileName filename(strFile);
	if(filename.IsOk() == true){
		wxString size = filename.GetHumanReadableSize();
		if(size == wxT("Not available")){
//			OutputProgress(_("Skipped ") + strFile, window, blVisible);
			return false;
		}
	}
	if(filename.GetExt() == wxT("cpt") && data.GetFunction() == _("Encrypt") && data.GetFormat() == _T("Rijndael")){
		return false;
	}
	if(filename.GetExt() != wxT("cpt") && data.GetFunction() == _("Decrypt") && data.GetFormat() == _T("Rijndael")){
		return false;
	}	
	
	wxArrayString arrErrors;
	wxArrayString arrOutput;

	if(data.GetFunction() == _("Encrypt")){
		//wxMessageBox(_("Encryption"));
		//Set an int to the file attributes to ensure that it can be encrypted	        
		int filearrtibs = GetFileAttributes(strFile);
		//Set the file to have no file attributes
		SetFileAttributes(strFile,FILE_ATTRIBUTE_NORMAL);  
		wxString command;
		if(data.GetFormat() == wxT("Rijndael"))  {       
			command = wxT("ccrypt -e -K\"") + data.GetPass() + wxT("\" \"") + strFile + wxT("\"");
		}
		else{		
			command = wxT("burp -eo -k\"") + data.GetPass() + wxT("\" \"") + strFile + wxT("\"");
		}
		//Execute the process 
		//wxMessageBox(_("About to execute"));
		
		long lgReturn = 1;
		lgReturn = wxExecute(command, arrErrors, arrOutput, wxEXEC_SYNC|wxEXEC_NODISABLE);
		
		//wxMessageBox(_("Finished the process"));
		//Put the original attributes back
		SetFileAttributes(strFile,filearrtibs);
		if(lgReturn == 0){        
			OutputProgress(_("Encrypted ") + strFile, window);
		}
		else{
			OutputProgress(_("Failed to encrypt ") + strFile, window);
		}
		//wxMessageBox(_("After"));
	}

	else if(data.GetFunction() == _("Decrypt")){
		//Set an int to the file attributes to ensure that it can be decrypted
		int filearrtibs = GetFileAttributes(strFile);
		SetFileAttributes(strFile,FILE_ATTRIBUTE_NORMAL); 
		wxString command;
		if(data.GetFormat() == wxT("Blowfish")){        
			command = wxT("burp -d -k\"") + data.GetPass() + wxT("\" \"") + strFile + wxT("\" \"") + wxPathOnly(strFile) + wxT("\\") + wxT("1.tmp\"");
		}	
		else{
			command = wxT("ccrypt -d -K\"") + data.GetPass() + wxT("\" \"") + strFile + wxT("\" ");
		}
		
		//wxMessageBox(_("About to execute"));
		
		long lgReturn = 1;
		lgReturn = wxExecute(command, arrErrors, arrOutput, wxEXEC_SYNC|wxEXEC_NODISABLE);
		
		//wxMessageBox(_("Finished the process"));
		
		//If Blowfish is used then the decryped file (1.tmp) is renamed to the correct name and then 1.tmp is removed
		if(data.GetFormat() == wxT("Blowfish")){		
			wxCopyFile(wxPathOnly(strFile) + wxT("\\1.tmp"), strFile, true);
			wxRemoveFile(wxPathOnly(strFile) + wxT("\\1.tmp"));
		}
		SetFileAttributes(strFile,filearrtibs);
		if(lgReturn == 0){       
 			OutputProgress(_("Decrypted ") + strFile, window);
		}
		else{
 			OutputProgress(_("Failed to decrypt ") + strFile, window);
		}
	}

	return true;
}
