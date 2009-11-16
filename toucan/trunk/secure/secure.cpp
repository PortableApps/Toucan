/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include <wx/wfstream.h>
#include <wx/filefn.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>

#include "secure.h"
extern "C"{
	#include "../ccrypt/ccryptlib.h"
}
#include "../toucan.h"
#include "../rules.h"
#include "../variables.h"
#include "../basicfunctions.h"
#include "../data/securedata.h"
#include "../forms/frmmain.h"
#include "../controls/vdtc.h"

bool Secure(SecureData *data){
	wxArrayString arrLocation = data->GetLocations();
	//Iterate through the entries in the array
	for(unsigned int i = 0; i < arrLocation.Count(); i++)
	{
		if(wxGetApp().GetAbort()){
			return true;
		}
		//Need to add normalisation to SecureData
		if(arrLocation.Item(i) != wxEmptyString){
			if(wxDirExists(arrLocation.Item(i))){
				CryptDir(arrLocation.Item(i), data);
			}
			else if(wxFileExists(arrLocation.Item(i))){
				CryptFile(arrLocation.Item(i), data);
			}
		}
	}
	if(wxGetApp().GetUsesGUI()){
		wxGetApp().MainWindow->m_Secure_TreeCtrl->DeleteAllItems();
		wxGetApp().MainWindow->m_Secure_TreeCtrl->AddRoot(wxT("HiddenRoot"));
		for(unsigned int i = 0; i < wxGetApp().MainWindow->m_SecureLocations->GetCount(); i++){
			wxString path = Normalise(wxGetApp().MainWindow->m_SecureLocations->Item(i));
			if(wxDirExists(path) || wxFileExists(path)){
				wxGetApp().MainWindow->m_SecureLocations->Add(path);
				wxGetApp().MainWindow->m_SecureLocations->RemoveAt(i);
				wxGetApp().MainWindow->m_Secure_TreeCtrl->AddNewPath(path);
			}
			else{
				if(path.Right(3) == wxT("cpt")){
					wxGetApp().MainWindow->m_SecureLocations->Add(path.Left(path.Length() - 4));
					wxGetApp().MainWindow->m_SecureLocations->RemoveAt(i);
					wxGetApp().MainWindow->m_Secure_TreeCtrl->AddNewPath(path.Left(path.Length() - 4));
				}
				else{
					wxGetApp().MainWindow->m_SecureLocations->Add(path + wxT(".cpt"));
					wxGetApp().MainWindow->m_SecureLocations->RemoveAt(i);
					wxGetApp().MainWindow->m_Secure_TreeCtrl->AddNewPath(path + wxT(".cpt"));
				}
			}
		}
	}
	return true;
}


/*The main loop for the Secure process. It is called by Secure initially and then either calls itself when it reaches a
folder or CryptFile when it reaches a file.*/
bool CryptDir(wxString strPath, SecureData *data)
{   
	if(wxGetApp().GetAbort()){
		return true;
	}
	wxDir dir(strPath);
	wxString filename;
	bool blDir = dir.GetFirst(&filename);
	if (blDir)
	{
		do {
			if(wxGetApp().GetAbort()){
				return true;
			}
			if (wxDirExists(strPath + wxFILE_SEP_PATH + filename) ){
				CryptDir(strPath + wxFILE_SEP_PATH + filename, data);
			}
			else{
				CryptFile(strPath + wxFILE_SEP_PATH + filename, data);
			}
		}
		while (dir.GetNext(&filename) );
	}   
	return true;
}


bool CryptFile(const wxString &path, SecureData *data)
{
	if(wxGetApp().GetAbort()){
		return true;
	}
	//Check to see it the file should be excluded	
	if(data->GetRules()->ShouldExclude(path, false)){
		return true;
	}
	//Make sure that it is a 'real' file
	wxFileName filename(path);
	if(filename.IsOk() == true){
		wxString size = filename.GetHumanReadableSize();
		if(size == wxT("Not available")){
			return false;
		}
	}

	//Ensure that we are not encrypting an already encrypted file or decrypting a non encrypted file
	if(filename.GetExt() == wxT("cpt") && data->GetFunction() == _("Encrypt")){
		return false;
	}
	if(filename.GetExt() != wxT("cpt") && data->GetFunction() == _("Decrypt")){
		return false;
	}	

	wxFileStream *filestream = new wxFileStream(path);
/*	wxFileOffset size = filestream->GetLength();

	char *in = new char[4096];
	char *out = new char[4096];
	ccrypt_stream_s stream;
	ccencrypt_init(&stream, data->GetPassword().mb_str());

	wxFileOffset bytesLeft = size;
	while(bytesLeft > 0){
		wxFileOffset bytesToRead = wxMin(4096, bytesLeft);
		filestream->Read(in, bytesToRead);
		if(filestream->GetLastError() != wxSTREAM_NO_ERROR){
			//Error, need to add error handling code here
		}
		strm.avail_in = bytesToRead;
		strm.next_in = in;
        do{
            strm.avail_out = 4096;
            strm.next_out = out;
            ccencrypt(&strm);
            filestream->SeekO(-bytesToRead, wxFromCurrent);
            filestream->Write(
        }while(strm.avail_out == 0);
	}

*/
	/*//A couple of arrays to catch the output and surpress the command window
	wxArrayString arrErrors;
	wxArrayString arrOutput;

	if(data->GetFunction() == _("Encrypt")){
		//Set the file to have normal attributes so it can be encrypted
		#ifdef __WXMSW__   
			int filearrtibs = GetFileAttributes(strFile);
			SetFileAttributes(strFile,FILE_ATTRIBUTE_NORMAL);  
		#endif

		wxSetWorkingDirectory(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()));
		//Create and execute the command
		wxString command = wxT("./ccrypt -e -K\"") + data->GetPassword() + wxT("\" \"") + strFile + wxT("\"");
		long lgReturn = wxExecute(command, arrErrors, arrOutput, wxEXEC_SYNC|wxEXEC_NODISABLE);

		//Put the old attributed back
		#ifdef __WXMSW__
			SetFileAttributes(strFile,filearrtibs);
		#endif

		if(lgReturn == 0){        
			OutputProgress(_("Encrypted ") + strFile);
		}
		else{
			OutputProgress(_("Failed to encrypt ") + strFile + wxString::Format(wxT(" : %i"), lgReturn), true, true);
		}
	}

	else if(data->GetFunction() == _("Decrypt")){
		//Set the file to have normal attributes so it can be decrypted
		#ifdef __WXMSW__
			int filearrtibs = GetFileAttributes(strFile);
			SetFileAttributes(strFile,FILE_ATTRIBUTE_NORMAL);
		#endif
		
		if(wxFileExists(strFile.Left(strFile.Length() - 4))){
			//We have a file with the decryped name already there, skip it
			OutputProgress(_("Failed to decrypt ") + strFile, true, true);
			return true;
		}

		wxSetWorkingDirectory(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()));

		//Create and execute the command
		wxString command = wxT("./ccrypt -f -d -K\"") + data->GetPassword() + wxT("\" \"") + strFile + wxT("\"");
		long lgReturn = wxExecute(command, arrErrors, arrOutput, wxEXEC_SYNC|wxEXEC_NODISABLE);

		//Put the old attributed back
		#ifdef __WXMSW__
			SetFileAttributes(strFile,filearrtibs);
		#endif

		if(lgReturn == 0){       
 			OutputProgress(_("Decrypted ") + strFile);
		}
		else{
 			OutputProgress(_("Failed to decrypt ") + strFile + wxString::Format(wxT(" : %i"), lgReturn), true, true);
		}
	}
	IncrementGauge();*/
	return true;
}
