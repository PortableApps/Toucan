/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "backupdata.h"
#include "basicfunctions.h"
#include "toucan.h"
#include <wx\fileconf.h>
#include <wx\stdpaths.h>


bool BackupData::TransferFromFile(wxString strName){
	//Create a new fileconfig object
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""),  wxGetApp().GetSettingsPath()+ wxT("Jobs.ini"));
	
	bool blError = true;
	wxString strTemp;
	
	/*Read all of the values from the ini file, and if there are not read erros add them to the data
	Password is not stored in the text file*/
	if(config->Read(strName + wxT("/BackupLocation"), &strTemp)){
		SetBackupLocation(strTemp);
		blError = false;
	}

	if(config->Read(strName + wxT("/Locations"), &strTemp)){
		SetLocations(StringToArrayString(strTemp, wxT("#")));
		blError = false;
	}

	if(config->Read(strName + wxT("/Function"), &strTemp)){
		SetFunction(strTemp);
		blError = false;
	}

	if(config->Read(strName + wxT("/Format"), &strTemp)){
		SetFormat(strTemp);
		blError = false;
	}

	if(config->Read(strName + wxT("/Ratio"), &strTemp)){
		SetRatio(strTemp);
		blError = false;
	}

	//Delete the fileconfig object
	delete config;

	//Output an error massage if needed
	if(blError){
		ErrorBox(wxT("There was an error reading from the jobs file, \nplease check it is not set as read only or in use."));
		return false;
	}
	return true;
}

bool BackupData::TransferToFile(wxString strName){
	//Create a new fileconfig object
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""),  wxGetApp().GetSettingsPath()+ wxT("Jobs.ini"));
	
	bool blError;

	//Delete any existing jobs with this name to ensure correct settings are retained
	blError = config->DeleteGroup(strName);

	//Add the files to be written
	if(!config->Write(strName + wxT("/BackupLocation"),  GetBackupLocation())){
		blError = false;
	}
	if(!config->Write(strName + wxT("/Locations"),  ArrayStringToString(GetLocations(), wxT("#")))){
		blError = false;
	}		
	if(!config->Write(strName + wxT("/Function"), GetFunction())){
		blError = false;
	}
	if(!config->Write(strName + wxT("/Format"), GetFormat())){
		blError = false;
	}
	if(!config->Write(strName + wxT("/Ratio"), GetRatio())){
		blError = false;
	}
	
	//Write the files and delete the fileconfig object
	config->Flush();
	delete config;
	
	if(!blError){
		ErrorBox(wxT("There was an error saving to the jobs file, \nplease check it is not set as read only or in use."));
		return false;
	}
	return true;
}

/*This function takes the data in BackupData and fills in the GUI*/
void BackupData::TransferToForm(frmMain *window){
	//Set the value of the text control
	window->m_Backup_Location->SetValue(GetBackupLocation());
	//Delete all of the items in the treectrl and readd the root
	window->m_Backup_TreeCtrl->DeleteAllItems();
	window->m_Backup_TreeCtrl->AddRoot(_("Hidden root"));
	//Remove all of the items in the filepath list
	for(unsigned int i = 0; i < wxGetApp().GetBackupLocations().GetCount(); i++){
		wxGetApp().RemoveBackupLocation(i);
	}
	//Add the new locations to the treectrl and the list
	for(unsigned int j = 0; j < GetLocations().GetCount(); j++){
		wxGetApp().AppendBackupLocation(GetLocations().Item(j));
		window->m_Backup_TreeCtrl->AddNewPath(GetLocations().Item(j));
	}
	//Set the rest of the window up
	window->m_Backup_Function->SetStringSelection(GetFunction());
	window->m_Backup_Format->SetStringSelection(GetFormat());
	window->m_Backup_Ratio->SetStringSelection(GetRatio());
	window->m_Backup_Pass->SetValue(GetPass());
	window->m_Backup_Repass->SetValue(GetPass());
	return;
}

/* This function sets all of the fields in SyncData based on the user inputted data in the
Main program window.*/
bool BackupData::TransferFromForm(frmMain *window, bool blShowErrors){
	wxString strPass, strRepass = wxEmptyString;
	
	bool blNotFilled = false;
	//Set the intenal file list as the global list, ensuring that there are items to backup
	SetLocations(wxGetApp().GetBackupLocations());
	if(GetLocations().GetCount() == 0){ blNotFilled = true; }

	//Get the items from the form, ensuring that they are filled
	if(window->m_Backup_Location->GetValue() != wxEmptyString){ SetBackupLocation(window->m_Backup_Location->GetValue()); }
	else{ blNotFilled = true; }	
	if(window->m_Backup_Function->GetStringSelection() != wxEmptyString){ SetFunction(window->m_Backup_Function->GetStringSelection()); }
	else{ blNotFilled = true; }
	if(window->m_Backup_Format->GetStringSelection() != wxEmptyString){ SetFormat(window->m_Backup_Format->GetStringSelection()) ; }
	else{ blNotFilled = true; }
	if(window->m_Backup_Ratio->GetStringSelection() != wxEmptyString){ SetRatio(window->m_Backup_Ratio->GetStringSelection()); }
	else{ blNotFilled = true; }
	//Do not worry if a password is not added as it is not complusory
	if(window->m_Backup_Pass->GetValue() != wxEmptyString){ strPass = window->m_Backup_Pass->GetValue(); }
	if(window->m_Backup_Repass->GetValue() != wxEmptyString){ strRepass = window->m_Backup_Repass->GetValue(); }
	//Output an error message if the fields are not filled
	if(blNotFilled && blShowErrors){
		ErrorBox(_("Not all of the required fields are filled"));
		return false;
	}
	//Make sure that the two passwords are the same
	if(strPass == strRepass){
		SetPass(strPass);
	}
	else{
		if(blShowErrors){
			ErrorBox(_("The passwords to not match, please try again."));
		}
	}
	//wxMessageBox(_("Returning"));
	return true;	
}

/*This is a debugging tool only, not for use in release  versions of Toucan */
void BackupData::Output(){
	MessageBox(GetBackupLocation(), wxT("Backup Location"));
	for(unsigned int i = 0; i < GetLocations().GetCount(); i++){
		MessageBox(GetLocations().Item(i), wxT("Location"));
	}
	MessageBox(GetFunction(), wxT("Function"));
	MessageBox(GetFormat(), wxT("Format"));
	MessageBox(GetRatio(), wxT("Ratio"));
	MessageBox(GetPass(), wxT("Pass"));
}

wxString BackupData::CreateCommand(int i){
	
	wxString strCommand;
	//Setting up to use the first item in the array.
	wxString strSecond = GetLocations().Item(i);
	
	//strSolid is only used if 7zip is chosen to allow updating
	wxString strSolid = wxEmptyString;

	//Turn the inputted format into one 7zip will understand
	if (GetFormat() == wxT("Zip")){
		SetFormat(wxT("zip"));
	}
   	else if (GetFormat() == wxT("7-Zip")){
		SetFormat(wxT("7z"));
		//Make sure solid mode is turned off so updating works
		strSolid = wxT(" -ms=off");
	}
	//Ratio stuff, will need to be updated for new sliding scale
	if (GetRatio() == _("Normal")){
		SetRatio(wxT(" -mx5"));
	}
	else if (GetRatio() == _("Maximum")){
		SetRatio(wxT(" -mx9"));
	}
	//Checking to see if there is a password used
	if(GetPass() != wxEmptyString){
		SetPass(wxT(" -p") + GetPass());
		//If 7zip is used then make sure headers are encrypted
		if(GetFormat() == wxT("7z")){
			SetPass(GetPass() + wxT(" -mhe=on"));
		}
	}
   
	if(GetFunction() == _("Complete")){
		strCommand = wxT("7za.exe a -t") + GetFormat() + GetPass() + GetRatio() + strSolid +  wxT(" \"") + GetBackupLocation() + wxT("\" ") + wxT(" \"") + GetLocations().Item(i) + wxT("\" ") + wxT(" -x@\"") + wxGetApp().GetSettingsPath() + wxT("Exclusions.txt") + wxT("\"");
	}
	else if(GetFunction() == _("Update")){
		strCommand = wxT("7za.exe u -t") + GetFormat() + GetPass() + GetRatio() + strSolid + wxT(" \"") + GetBackupLocation() + wxT("\"") + wxT(" -x@\"are") + wxGetApp().GetSettingsPath() + wxT("Exclusions.txt") + wxT("\""); 
	}
	/*else if(GetFunction() == _("Restore")){
		strCommand = wxT("7za.exe  x  \"") + strFirst + wxT("\" -o\"") + strSecond + wxT("\" * -r")  + wxT(" -x@\"") + wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH  + wxT("temp-exclusions.txt") + wxT("\"") + strPass + strSolid;
	}*/
	//With the incremental type the first use creates a file called base file. On subsequent runs a file is created with a filename based on both the date and time.    
	else if(GetFunction() == _("Incremental")){
		if (GetBackupLocation()[GetBackupLocation().length()-1] != wxFILE_SEP_PATH) {
			GetBackupLocation() += wxFILE_SEP_PATH;       
		}
		if(wxFileExists(GetBackupLocation() + wxFILE_SEP_PATH + wxT("BaseFile.") + GetFormat())){
			wxDateTime now = wxDateTime::Now();
			strCommand = wxT("7za.exe u") + GetPass() + strSolid + wxT(" \"") + GetBackupLocation() + wxT("BaseFile.") + strFormat + wxT("\" -u- -up0q3x2z0!\"") + GetBackupLocation() + wxFILE_SEP_PATH + now.FormatISODate()+ wxT("-") + now.Format(wxT("%H")) + wxT("-") +  now.Format(wxT("%M")) + wxT(".") + strFormat + wxT("\" \"") + GetLocations().Item(i) + wxT("\"") + wxT(" -x@\"") + wxGetApp().GetSettingsPath() + wxT("Exclusions.txt") + wxT("\"");
		}
		else{
			strCommand = wxT("7za.exe a -t") + GetFormat() + GetPass() + GetRatio() + strSolid +  wxT(" \"") + GetBackupLocation() + wxT("BaseFile.") + strFormat + wxT("\" ") + wxT(" \"") + GetLocations().Item(i) + wxT("\" ") + wxT(" -x@\"") + wxGetApp().GetSettingsPath() + wxT("Exclusions.txt") + wxT("\"");
		}
	}
	return strCommand;
}
