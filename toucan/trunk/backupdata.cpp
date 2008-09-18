/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "backupdata.h"
#include "basicfunctions.h"
#include "toucan.h"
#include "variables.h"
#include "backupprocess.h"
#include "waitthread.h"
#include <wx\fileconf.h>
#include <wx\stdpaths.h>
#include <wx\dir.h>


bool BackupData::TransferFromFile(){
	/*wxString strName = GetName();
	//Create a new fileconfig object
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""),  wxGetApp().GetSettingsPath()+ wxT("Jobs.ini"));
	
	bool blError = true;
	wxString strTemp;
	int iTemp;

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

	if(config->Read(strName + wxT("/Ratio"), &iTemp)){
		SetRatio(iTemp);
		blError = false;
	}
	
	if(config->Read(strName + wxT("/IsPass"), &IsPassword)){
		blError = false;
	}

	//Delete the fileconfig object
	delete config;

	//Output an error massage if needed
	if(blError){
		ErrorBox(_("There was an error reading from the jobs file, \nplease check it is not set as read only or in use."));
		return false;
	}
	return true;*/
	
	wxString strName = GetName();
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""),  wxGetApp().GetSettingsPath()+ wxT("Jobs.ini"));
	
	bool blError;
	int iTemp;
	wxString strTemp;
	bool blTemp;

	blError = config->Read(strName + wxT("/BackupLocation"), &strTemp);
	if(blError){ SetBackupLocation(strTemp); }	
	blError = config->Read(strName + wxT("/Locations"), &strTemp);
	if(blError){ SetLocations(StringToArrayString(strTemp, wxT("#"))); }	
	blError = config->Read(strName + wxT("/Function"), &strTemp);
	if(blError){ SetFunction(strTemp); }
	blError = config->Read(strName + wxT("/Format"), &strTemp);
	if(blError){ SetFormat(strTemp); }
	blError = config->Read(strName + wxT("/Ratio"), &iTemp);
	if(blError){ SetRatio(iTemp); }
	blError = config->Read(strName + wxT("/IsPass"), &blTemp);
	if(blError){ IsPassword = blTemp; }
	delete config;
	
	if(!blError){
		ErrorBox(_("There was an error reading from the jobs file, \nplease check it is not set as read only or in use."));
		return false;
	}
}

bool BackupData::TransferToFile(){
	wxString strName = GetName();
	//Create a new fileconfig object
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""),  wxGetApp().GetSettingsPath()+ wxT("Jobs.ini"));
	bool blError = false;

	//Delete any existing jobs with this name to ensure correct settings are retained
	config->DeleteGroup(strName);
	config->Flush();

	//Add the files to be written
	if(!config->Write(strName + wxT("/BackupLocation"),  GetBackupLocation())){
		blError = true;
	}
	if(!config->Write(strName + wxT("/Locations"),  ArrayStringToString(GetLocations(), wxT("#")))){
		blError = true;
	}		
	if(!config->Write(strName + wxT("/Function"), GetFunction())){
		blError = true;
	}
	if(!config->Write(strName + wxT("/Format"), GetFormat())){
		blError = true;
	}
	if(!config->Write(strName + wxT("/Ratio"), GetRatio())){
		blError = true;
	}
	if(!config->Write(strName + wxT("/IsPass"), IsPassword)){
		blError = true;
	}
	
	//Write the files and delete the fileconfig object
	config->Flush();
	delete config;
	
	if(blError){
		ErrorBox(_("There was an error saving to the jobs file, \nplease check it is not set as read only or in use."));
		return false;
	}
	return true;
}

bool BackupData::TransferToForm(){
	frmMain *window = wxGetApp().MainWindow;
	if(window == NULL){
		return false;
	}
	window->m_Backup_Location->SetValue(GetBackupLocation());
	window->m_Backup_TreeCtrl->DeleteAllItems();
	window->m_Backup_TreeCtrl->AddRoot(wxT("Hidden root"));
	
	//Remove all of the items in the filepath list
	wxGetApp().MainWindow->m_BackupLocations->Clear();
	//Add the new locations to the treectrl and the list
	for(unsigned int j = 0; j < GetLocations().GetCount(); j++){
		wxGetApp().MainWindow->m_BackupLocations->Add(GetLocations().Item(j));
		window->m_Backup_TreeCtrl->AddNewPath(Normalise(Normalise(GetLocations().Item(j))));
	}
	//Set the rest of the window up
	window->m_Backup_Function->SetStringSelection(GetFunction());
	window->m_Backup_Format->SetStringSelection(GetFormat());
	window->m_Backup_Ratio->SetValue(GetRatio());
	window->m_Backup_IsPass->SetValue(IsPassword);
	return false;
}

/* This function sets all of the fields in SyncData based on the user inputted data in the
Main program window.*/
bool BackupData::TransferFromForm(){
	frmMain *window = wxGetApp().MainWindow;
	wxString strPass, strRepass = wxEmptyString;

	bool blNotFilled = false;
	//Set the intenal file list as the global list, ensuring that there are items to backup
	SetLocations(*wxGetApp().MainWindow->m_BackupLocations);
	if(GetLocations().GetCount() == 0){ blNotFilled = true; }

	//Get the items from the form, ensuring that they are filled
	if(window->m_Backup_Location->GetValue() != wxEmptyString){ SetBackupLocation(window->m_Backup_Location->GetValue()); }
	else{ blNotFilled = true; }	
	if(window->m_Backup_Function->GetStringSelection() != wxEmptyString){ SetFunction(window->m_Backup_Function->GetStringSelection()); }
	else{ blNotFilled = true; }
	if(window->m_Backup_Format->GetStringSelection() != wxEmptyString){ SetFormat(window->m_Backup_Format->GetStringSelection()) ; }
	else{ blNotFilled = true; }
	SetRatio(window->m_Backup_Ratio->GetValue());
	IsPassword = window->m_Backup_IsPass->GetValue();
	//Output an error message if the fields are not filled
	if(blNotFilled){
		ErrorBox(_("Not all of the required fields are filled"));
		return false;
	}
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
}

wxString BackupData::CreateCommand(int i){
	
	wxString strCommand, strTempDir;
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
	wxString strRatio = wxT(" -mx5");
	if (GetRatio() == 0){
		strRatio = wxT(" -mx0");
	}
	else if (GetRatio() == 1){
		strRatio = wxT(" -mx1");
	}
	else if (GetRatio() == 2){
		strRatio = wxT(" -mx3");
	}
	else if (GetRatio() == 3){
		strRatio = wxT(" -mx5");
	}
	else if (GetRatio() == 4){
		strRatio = wxT(" -mx7");
	}
	else if (GetRatio() == 5){
		strRatio = wxT(" -mx9");
	}
	//Checking to see if there is a password used
	if(GetPassword() != wxEmptyString){
		SetPassword(wxT(" -p") + GetPass());
		//If 7zip is used then make sure headers are encrypted
		if(GetFormat() == wxT("7z")){
			SetPass(GetPass() + wxT(" -mhe=on"));
		}
	}
   
	strTempDir = wxT(" -w\"") + wxPathOnly(GetBackupLocation()) + wxT("\"");

	if(GetFunction() == _("Complete")){
		strCommand = wxT("7za.exe a -t") + GetFormat() + GetPass() + strRatio + strSolid +  wxT(" \"") + GetBackupLocation() + wxT("\" ") + wxT(" \"") + GetLocations().Item(i) + wxT("\" ") + wxT(" -x@\"") + wxGetApp().GetSettingsPath() + wxT("Exclusions.txt") + wxT("\"") + strTempDir;	
	}
	else if(GetFunction() == _("Update")){
		strCommand = wxT("7za.exe u -t") + GetFormat() + GetPass() + strRatio + strSolid + wxT(" \"") + GetBackupLocation() + wxT("\"") + wxT(" \"") + GetLocations().Item(i) + wxT("*\"") + wxT(" -x@\"") + wxGetApp().GetSettingsPath() + wxT("Exclusions.txt") + wxT("\"") + strTempDir; 
	}
	//With the Differential type the first use creates a file called base file. On subsequent runs a file is created with a filename based on both the date and time.    
	else if(GetFunction() == _("Differential")){
		if (GetBackupLocation()[GetBackupLocation().length()-1] != wxFILE_SEP_PATH) {
			SetBackupLocation(GetBackupLocation() + wxFILE_SEP_PATH);       
		}
		if(wxFileExists(GetBackupLocation() + wxFILE_SEP_PATH + wxT("BaseFile.") + GetFormat())){
			wxDateTime now = wxDateTime::Now();
			strCommand = wxT("7za.exe u") + GetPass() + strRatio + strSolid + wxT(" \"") + GetBackupLocation() + wxT("BaseFile.") + strFormat + wxT("\" -u- -up0q3x2z0!\"") + GetBackupLocation() + now.FormatISODate()+ wxT("-") + now.Format(wxT("%H")) + wxT("-") +  now.Format(wxT("%M")) + wxT(".") + strFormat + wxT("\" \"") + GetLocations().Item(i) + wxT("*\"") + wxT(" -x@\"") + wxGetApp().GetSettingsPath() + wxT("Exclusions.txt") + wxT("\"") + strTempDir;
		}
		else{
			strCommand = wxT("7za.exe a -t") + GetFormat() + GetPass() + strRatio + strSolid +  wxT(" \"") + GetBackupLocation() + wxT("BaseFile.") + strFormat + wxT("\" ") + wxT(" \"") + GetLocations().Item(i) + wxT("*\" ") + wxT(" -x@\"") + wxGetApp().GetSettingsPath() + wxT("Exclusions.txt") + wxT("\"") + strTempDir;
		}
	}
	wxMessageBox(strCommand);
	return strCommand;
}


bool BackupData::CreateList(wxTextFile *file, Rules rules, wxString strPath, int iRootLength){
	//Clean up the path passed
	if (strPath[strPath.length()-1] != wxFILE_SEP_PATH) {
		strPath += wxFILE_SEP_PATH;       
	}
	wxGetApp().Yield();
	wxDir dir(strPath);
	wxString strFilename;
	bool blDir = dir.GetFirst(&strFilename);
	//If the path is ok
	if(blDir){
		//Loop through all of the files and folders in the directory
		do {
			//If it is a directory
			if(wxDirExists(strPath + strFilename))
			{
				//Always call the function again to ensure that ALL files and folders are processed
				CreateList(file, rules, strPath + strFilename, iRootLength);
			}
			//If it is a file
			else{
				if(rules.ShouldExclude(strPath + strFilename, false)){
					wxString strCombined = strPath + strFilename;
					strCombined = strCombined.Right(strCombined.Length() - iRootLength - 1);
					file->AddLine(strCombined);
				}
			}
		}
		while (dir.GetNext(&strFilename) );
	}  
	return true;
}

bool BackupData::Execute(Rules rules){

	SetBackupLocation(Normalise(Normalise(GetBackupLocation())));
	SetLocation(0, Normalise(Normalise(GetLocation(0))));
	
	wxString strCommand;
	//Open the text file for the file paths and clear it
	wxTextFile *file = new wxTextFile(wxGetApp().GetSettingsPath() + wxT("Exclusions.txt"));
	if(wxFileExists(wxGetApp().GetSettingsPath() + wxT("Exclusions.txt"))){
		file->Open();
		file->Clear();
		file->Write();
	}
	else{
		file->Create();
	}
	//Create the command to execute
	strCommand = CreateCommand(0);
	wxString strPath = GetLocations().Item(0);
	if (strPath[strPath.length()-1] != wxFILE_SEP_PATH) {
		strPath += wxFILE_SEP_PATH;       
	}
	strPath = strPath.BeforeLast(wxFILE_SEP_PATH);
	strPath = strPath.BeforeLast(wxFILE_SEP_PATH);
	
	//Create the list of files to backup
	OutputProgress(_("Creating an exclusions list, this may take some time."));
	CreateList(file, rules, GetLocations().Item(0), strPath.Length());
	file->Write();
	//Create the process and execute it
	PipedProcess *process = new PipedProcess(wxGetApp().ProgressWindow);
	long lgPID = wxExecute(strCommand, wxEXEC_ASYNC|wxEXEC_NODISABLE, process);
	process->SetRealPid(lgPID);
	WaitThread *thread = new WaitThread(lgPID, process);
	thread->Create();
	thread->Run();	
	return true;
}
