/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2007 Steven Lamerton
// Licence:     GNU GPL 2 (See readme for more info
/////////////////////////////////////////////////////////////////////////////////

//The CryptFile function can either be called by the main Secure function, or by the CryptDir function.
bool CryptFile(wxString strFile, wxString strPass, wxString strFunction, frmProgress* window, wxString strRoutine)
{

    	if(wxGetApp().GetStrAbort() == wxT("ABORT")){
        	return false;
    	}
	//Make sure that a filename has been passed correctly and check that it is not a 0 byte file
    	wxFileName filename(strFile);
    	if(filename.IsOk() == true){
    		wxString size = filename.GetHumanReadableSize();
    		if(size == wxT("Not available")){
        			window->m_Progress_Text->AppendText(_("\nSkipped ") + strFile);
        			return false;
    		}
	}
	wxArrayString arrErrors;
	wxArrayString arrOutput;

	if(strFunction == _("Encrypt")){
		//Set an int to the file attributes to ensure that it can be encrypted	        
		int filearrtibs = GetFileAttributes(strFile);
		//Set the file to have no file attributes
        SetFileAttributes(strFile,FILE_ATTRIBUTE_NORMAL);  
        wxString command;
		if(strRoutine == wxT("AES"))  {       
			command = wxT("App\\toucan\\ccrypt -e -K\"") + strPass + wxT("\" \"") + strFile + wxT("\"");
		}
		else{		
			command = wxT("App\\toucan\\burp -eo -k\"") + strPass + wxT("\" \"") + strFile + wxT("\"");
		}
		//Execute the process 
		long lgReturn = wxExecute(command, arrErrors,arrOutput, wxEXEC_NODISABLE|wxEXEC_SYNC);
		//Put the original attributes back
		SetFileAttributes(strFile,filearrtibs);
		if(lgReturn == 0){        
        			window->m_Progress_Text->AppendText(_("\nEncrypted ") + strFile);
        			window->Update();
		}
		else{
        			window->m_Progress_Text->AppendText(_("\nFailed to encrypt ") + strFile);
        			window->Update();
		}
        		wxGetApp().Yield(true);
	}

	else if(strFunction == _("Decrypt"))
	{
        //Set an int to the file attributes to ensure that it can be decrypted
		int filearrtibs = GetFileAttributes(strFile);
        SetFileAttributes(strFile,FILE_ATTRIBUTE_NORMAL); 
        wxString command;
		if(strRoutine == wxT("Blowfish")){        
			command = wxT("App\\toucan\\burp -d -k\"") + strPass + wxT("\" \"") + strFile + wxT("\" \"") + wxPathOnly(strFile) + wxT("\\") + wxT("1.tmp\"");
		}	
		else{
			command = wxT("App\\toucan\\ccrypt -d -K\"") + strPass + wxT("\" \"") + strFile + wxT("\" ");
		}
		long lgReturn = wxExecute(command, arrErrors, arrOutput, wxEXEC_NODISABLE);
		//If Blowfish is used then the decryped file (1.tmp) is renamed to the correct name and then 1.tmp is removed
		if(strRoutine == wxT("Blowfish")){		
			wxCopyFile(wxPathOnly(strFile) + wxT("\\1.tmp"), strFile, true);
			wxRemoveFile(wxPathOnly(strFile) + wxT("\\1.tmp"));
		}
        SetFileAttributes(strFile,filearrtibs);
		if(lgReturn == 0){       
 			window->m_Progress_Text->AppendText(_("\nDecrypted ") + strFile);
        			window->Update();
		}
		else{
 			window->m_Progress_Text->AppendText(_("\nFailed to decrypt ") + strFile);
        			window->Update();
		}
        		wxGetApp().Yield(true);
	}
	//wxLog::Flush();
	return true;
}