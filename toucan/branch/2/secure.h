/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2007 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include <wx/file.h>
#include <wx/filefn.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>

#include "securedata.h"

//Forward declarations
bool CryptFile(wxString strFile, SecureData data, Rules rules, frmProgress *window); 
bool CryptDir(wxString strPath, SecureData data, Rules rules, frmProgress* window);

bool Secure(SecureData data, Rules rules, frmProgress *window){
	wxArrayString arrLocation = data.GetLocations();
	wxMessageBox(_("Here3"));
	//Iterate through the entries in the array
	for(unsigned int i = 0; i < arrLocation.Count(); i++)
	{
		if(wxGetApp().ShouldAbort()){
			return true;
		}
		//Need to add normalisation to SecureData
		if(arrLocation.Item(i) != wxEmptyString){
			if(wxDirExists(arrLocation.Item(i))){
				CryptDir(arrLocation.Item(i), data, rules, window);
			}
			else if(wxFileExists(arrLocation.Item(i))){
				CryptFile(arrLocation.Item(i), data, rules, window);
			}
		}
	}
	wxGetApp().MainWindow->m_Secure_TreeCtrl->DeleteAllItems();
	wxGetApp().MainWindow->m_Secure_TreeCtrl->AddRoot(wxT("HiddenRoot"));
	for(unsigned int i = 0; i < wxGetApp().MainWindow->m_SecureLocations->GetCount(); i++){
		wxGetApp().MainWindow->m_Secure_TreeCtrl->AddNewPath(wxGetApp().MainWindow->m_SecureLocations->Item(i));
	}
	return true;
}


/*The main loop for the Secure process. It is called by Secure initially and then either calls itself when it reaches a
folder or CryptFile when it reaches a file.*/
bool CryptDir(wxString strPath, SecureData data, Rules rules, frmProgress* window)
{   
	if(wxGetApp().ShouldAbort()){
		return true;
	}
	wxDir dir(strPath);
	wxString filename;
	bool blDir = dir.GetFirst(&filename);
	if (blDir)
	{
		do {
			if(wxGetApp().ShouldAbort()){
				return true;
			}
			if (wxDirExists(strPath + wxFILE_SEP_PATH + filename) ){
				CryptDir(strPath + wxFILE_SEP_PATH + filename, data, rules, window);
			}
			else{
				CryptFile(strPath + wxFILE_SEP_PATH + filename, data, rules, window);
			}
		}
		while (dir.GetNext(&filename) );
	}   
	return true;
}


bool CryptFile(wxString strFile, SecureData data, Rules rules, frmProgress* window)
{
	if(wxGetApp().ShouldAbort()){
		return true;
	}
	//Check to see it the file should be excluded	
	if(rules.ShouldExclude(strFile, false)){
		return true;
	}
	//Make sure that it is a 'real' file
	wxFileName filename(strFile);
	if(filename.IsOk() == true){
		wxString size = filename.GetHumanReadableSize();
		if(size == wxT("Not available")){
			return false;
		}
	}

	//Ensure that we are not encrypting an already encrypted file or decrypting a non encrypted file
	if(filename.GetExt() == wxT("cpt") && data.GetFunction() == _("Encrypt") && data.GetFormat() == _T("Rijndael")){
		return false;
	}
	if(filename.GetExt() != wxT("cpt") && data.GetFunction() == _("Decrypt") && data.GetFormat() == _T("Rijndael")){
		return false;
	}	
	
	//A couple of arrays to catch the output and surpress the command window
	wxArrayString arrErrors;
	wxArrayString arrOutput;

	if(data.GetFunction() == _("Encrypt")){
		//Set an int to the file attributes to ensure that it can be encrypted	        
		int filearrtibs = GetFileAttributes(strFile);
		//Set the file to have no file attributes
		SetFileAttributes(strFile,FILE_ATTRIBUTE_NORMAL);  

		//Create the command
		wxString command = wxT("ccrypt -e -K\"") + data.GetPass() + wxT("\" \"") + strFile + wxT("\"");

		//Execute the process 
		long lgReturn = wxExecute(command, arrErrors, arrOutput, wxEXEC_SYNC|wxEXEC_NODISABLE);
		
		//Put the original attributes back
		SetFileAttributes(strFile,filearrtibs);

		//Output the progress message
		if(lgReturn == 0){        
			OutputProgress(_("Encrypted ") + strFile);
		}
		else{
			OutputProgress(_("Failed to encrypt ") + strFile);
		}
	}

	else if(data.GetFunction() == _("Decrypt")){
		//Set an int to the file attributes to ensure that it can be decrypted
		int filearrtibs = GetFileAttributes(strFile);
		SetFileAttributes(strFile,FILE_ATTRIBUTE_NORMAL); 

		//Create the command (blowfish is allowed)
		wxString command;
		if(data.GetFormat() == wxT("Blowfish")){        
			command = wxT("burp -d -k\"") + data.GetPass() + wxT("\" \"") + strFile + wxT("\" \"") + wxPathOnly(strFile) + wxT("\\") + wxT("1.tmp\"");
		}	
		else{
			command = wxT("ccrypt -d -K\"") + data.GetPass() + wxT("\" \"") + strFile + wxT("\" ");
		}
		
		//Execute the process
		long lgReturn = wxExecute(command, arrErrors, arrOutput, wxEXEC_SYNC|wxEXEC_NODISABLE);
		
		//If Blowfish is used then the decryped file (1.tmp) is renamed to the correct name and then 1.tmp is removed
		if(data.GetFormat() == wxT("Blowfish")){		
			wxCopyFile(wxPathOnly(strFile) + wxT("\\1.tmp"), strFile, true);
			wxRemoveFile(wxPathOnly(strFile) + wxT("\\1.tmp"));
		}

		//Put the attributes back
		SetFileAttributes(strFile,filearrtibs);

		//Output the the progress message
		if(lgReturn == 0){       
 			OutputProgress(_("Decrypted ") + strFile);
		}
		else{
 			OutputProgress(_("Failed to decrypt ") + strFile);
		}
	}
	return true;
}
