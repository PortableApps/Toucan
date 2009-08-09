/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 - 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "backupdata.h"
#include "../basicfunctions.h"
#include "../toucan.h"
#include "../variables.h"
#include "../backupprocess.h"
#include "../waitthread.h"
#include "../forms/frmmain.h"
#include "../forms/frmprogress.h"
#include "../controls/vdtc.h"
#include <wx/textfile.h>
#include <wx/fileconf.h>
#include <wx/stdpaths.h>
#include <wx/dir.h>

bool BackupData::TransferFromFile(){
	bool error = false;
	bool btemp;
	wxString stemp;
	int itemp;

	if(!wxGetApp().m_Jobs_Config->Exists(GetName())){
		return false;
	}

	if(wxGetApp().m_Jobs_Config->Read(GetName() + wxT("/BackupLocation"), &stemp)) SetFileLocation(stemp);
		else error = true;
	if(wxGetApp().m_Jobs_Config->Read(GetName() + wxT("/Locations"), &stemp)) SetLocations(StringToArrayString(stemp, wxT("|")));
		else error = true;
	if(wxGetApp().m_Jobs_Config->Read(GetName() + wxT("/Function"), &stemp)) SetFunction(stemp);
		else error = true;
	if(wxGetApp().m_Jobs_Config->Read(GetName() + wxT("/Format"), &stemp)) SetFormat(stemp);
		else error = true;
	if(wxGetApp().m_Jobs_Config->Read(GetName() + wxT("/Ratio"), &itemp)) SetRatio(itemp);
		else error = true;
	if(wxGetApp().m_Jobs_Config->Read(GetName() + wxT("/IsPass"), &btemp)) SetUsesPassword(btemp);
		else error = true;

	if(error){
		wxMessageBox(_("There was an error reading from the jobs file"), _("Error"), wxICON_ERROR);
		return false;
	}
	return true;
}

bool BackupData::TransferToFile(){
	bool error = false;

	if(!wxGetApp().m_Jobs_Config->DeleteGroup(GetName())) error = true;
	if(!wxGetApp().m_Jobs_Config->Write(GetName() + wxT("/BackupLocation"),  GetFileLocation())) error = true;
	if(!wxGetApp().m_Jobs_Config->Write(GetName() + wxT("/Locations"),  ArrayStringToString(GetLocations(), wxT("|")))) error = true;
	if(!wxGetApp().m_Jobs_Config->Write(GetName() + wxT("/Function"), GetFunction())) error = true;
	if(!wxGetApp().m_Jobs_Config->Write(GetName() + wxT("/Format"), GetFormat())) error = true;
	if(!wxGetApp().m_Jobs_Config->Write(GetName() + wxT("/Ratio"), GetRatio())) error = true;
	if(!wxGetApp().m_Jobs_Config->Write(GetName() + wxT("/IsPass"), GetUsesPassword())) error = true;

	wxGetApp().m_Jobs_Config->Flush();

	if(error){
		wxMessageBox(_("There was an error saving to the jobs file, \nplease check it is not set as read only or in use"), _("Error"), wxICON_ERROR);
		return false;
	}
	return true;
}

bool BackupData::TransferToForm(frmMain *window){
	if(!window){
		return false;
	}
	window->m_Backup_Location->SetValue(GetFileLocation());
	window->m_Backup_TreeCtrl->DeleteChildren(window->m_Backup_TreeCtrl->GetRootItem());
	
	window->m_BackupLocations->Clear();
	for(unsigned int j = 0; j < GetLocations().GetCount(); j++){
		window->m_BackupLocations->Add(GetLocation(j));
		window->m_Backup_TreeCtrl->AddNewPath(Normalise(GetLocation(j)));
	}
	window->m_Backup_Function->SetStringSelection(GetFunction());
	window->m_Backup_Format->SetStringSelection(GetFormat());
	window->m_Backup_Ratio->SetValue(GetRatio());
	window->m_Backup_IsPass->SetValue(GetUsesPassword());
	return false;
}

bool BackupData::TransferFromForm(frmMain *window){
	if(!window){
		return false;
	}
	SetLocations(*window->m_BackupLocations);
	SetFileLocation(window->m_Backup_Location->GetValue()); 
	SetFunction(window->m_Backup_Function->GetStringSelection()); 
	SetFormat(window->m_Backup_Format->GetStringSelection()) ; 
	SetRatio(window->m_Backup_Ratio->GetValue());
	SetUsesPassword(window->m_Backup_IsPass->GetValue());
	SetRules(new Rules(window->m_Backup_Rules->GetStringSelection(), true));
	return true;	
}

void BackupData::Output(){
	wxMessageBox(GetFileLocation(), wxT("Backup Location"));
	for(unsigned int i = 0; i < GetLocations().GetCount(); i++){
		wxMessageBox(GetLocation(i), wxString::Format(wxT("Location %d"), i));
	}
	wxMessageBox(GetFunction(), wxT("Function"));
	wxMessageBox(GetFormat(), wxT("Format"));
	wxMessageBox(wxString::Format(wxT("%d"), GetRatio()), wxT("Compression Ratio"));
}

wxString BackupData::CreateCommand(int i){
	
	wxString strCommand, strTempDir;
	//Setting up to use the first item in the array.
	wxString strSecond = GetLocation(i);
	
	//strSolid is only used if 7zip is chosen to allow updating
	wxString strSolid = wxEmptyString;

	wxString exe = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("7za");

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
	if(i == 0){
		//Checking to see if there is a password used
		if(GetPassword() != wxEmptyString){
			SetPassword(wxT(" -p") + GetPassword());
			//If 7zip is used then make sure headers are encrypted
			if(GetFormat() == wxT("7z")){
				SetPassword(GetPassword() + wxT(" -mhe=on"));
			}
		}
	}
   
	strTempDir = wxT(" -w\"") + wxPathOnly(GetFileLocation()) + wxT("\"");

	if(GetFunction() == _("Complete")){
		strCommand = exe + wxT(" a -t") + GetFormat() + GetPassword() + strRatio + strSolid +  wxT(" \"") + GetFileLocation() + wxT("\"") +  wxT(" @\"") + wxGetApp().GetSettingsPath() + wxT("Includes.txt") + wxT("\" ") + strTempDir;	
	}
	else if(GetFunction() == _("Update")){
		strCommand = exe + wxT(" u -t") + GetFormat() + GetPassword() + strRatio + strSolid + wxT(" \"") + GetFileLocation() + wxT("\"") +  wxT(" @\"") + wxGetApp().GetSettingsPath() + wxT("Includes.txt") + wxT("\" ") + strTempDir; 
	}
	else if(GetFunction() == _("Restore")){
		strCommand =  exe + wxT("  x -aoa \"") + GetLocation(i) + wxT("\" -o\"") + GetFileLocation() + wxT("\" * -r") + GetPassword();	
	}
	//With the Differential type the first use creates a file called base file. On subsequent runs a file is created with a filename based on both the date and time.    
	else if(GetFunction() == _("Differential")){
		if (GetFileLocation()[GetFileLocation().length()-1] != wxFILE_SEP_PATH) {
			SetFileLocation(GetFileLocation() + wxFILE_SEP_PATH);       
		}
		if(wxFileExists(GetFileLocation() + wxFILE_SEP_PATH + wxT("BaseFile.") + GetFormat())){
			wxDateTime now = wxDateTime::Now();
			strCommand = exe + wxT(" u") + GetPassword() + strRatio + strSolid + wxT(" \"") + GetFileLocation() + wxT("BaseFile.") + GetFormat() + wxT("\" -u- -up0q3x2z0!\"") + GetFileLocation() + now.FormatISODate()+ wxT("-") + now.Format(wxT("%H")) + wxT("-") +  now.Format(wxT("%M")) + wxT(".") + GetFormat() + wxT("\"") +  wxT(" @\"") + wxGetApp().GetSettingsPath() + wxT("Includes.txt") + wxT("\" ") + strTempDir;
		}
		else{
			strCommand = exe + wxT(" a -t") + GetFormat() + GetPassword() + strRatio + strSolid +  wxT(" \"") + GetFileLocation() + wxT("BaseFile.") + GetFormat() + wxT(" \"") +  wxT(" @\"") + wxGetApp().GetSettingsPath() + wxT("Includes.txt") + wxT("\" ") + strTempDir;
		}
	}
	return strCommand;
}


bool BackupData::CreateList(wxTextFile *file, wxString path, int length){
	if(wxGetApp().GetAbort()){
		return true;
	}

	//Yield so the display remains responsive
	wxGetApp().Yield();

	if(wxDirExists(path)){
		//Clean up the path passed
		if (path[path.length()-1] != wxFILE_SEP_PATH) {
			path += wxFILE_SEP_PATH;       
		}
		wxDir dir(path);
		wxString filename;
		bool blDir = dir.GetFirst(&filename);
		//If the path is ok
		if(blDir){
			//Loop through all of the files and folders in the directory
			do {
				//If it is a directory
				if(wxDirExists(path + filename))
				{
					//If the direcotry doesnt have any subfiles then we can safely add it to the file list
					wxDir* dircheck = new wxDir(path + filename);
					if(!dircheck->HasFiles() && !dircheck->HasSubDirs()){
						if(!GetRules()->ShouldExclude(path + filename, true)){
							wxString strCombined = path + filename;
							strCombined = strCombined.Right(strCombined.Length() - length);
							file->AddLine(strCombined);
						}
					}
					delete dircheck;
					//Always call the function again to ensure that ALL files and folders are processed
					CreateList(file, path + filename, length);
				}
				//If it is a file
				else{
					if(!GetRules()->ShouldExclude(path + filename, false)){
						wxString strCombined = path + filename;
						strCombined = strCombined.Right(strCombined.Length() - length);
						file->AddLine(strCombined);
					}
				}
			}
			while (dir.GetNext(&filename));
		}  
	}
	//We have been passed a file
	else{
		if(!GetRules()->ShouldExclude(path, false)){
			path = path.Right(path.Length() - length);
			file->AddLine(path);
		}		
	}
	return true;
}

/*bool BackupData::Execute(Rules rules){
	//Expand all of the variables
	for(unsigned int i = 0; i < GetLocations().Count(); i++){
		SetLocation(i, Normalise(GetLocation(i)));
	}
	SetFileLocation(Normalise(GetFileLocation()));
	for(unsigned int i = 0; i < GetLocations().Count(); i++){
		wxString path = GetLocation(i);
		bool isDir = false;
		//If we have a directory clean it up
		if(wxDirExists(path)){
			if (path[path.length()-1] != wxFILE_SEP_PATH) {
				path += wxFILE_SEP_PATH;       
			}
			isDir = true;
		}
		//If we are not running a restore job we need to create a list file
		if(GetFunction() != _("Restore")){
			//Set up the include file
			wxTextFile *file = new wxTextFile(wxGetApp().GetSettingsPath() + wxT("Includes.txt"));
			if(wxFileExists(wxGetApp().GetSettingsPath() + wxT("Includes.txt"))){
				file->Open();
				file->Clear();
				file->Write();
			}
			else{
				file->Create();
			}
			wxFileName filename(path);
			int length; 
			//If we have a directory then take of the last dir and take off one for the remaining slash
			if(isDir){
				filename.RemoveLastDir();
				length = filename.GetFullPath().Length();
				path = filename.GetFullPath();
			}
			//For files remove the filename and the remaining slash
			else{
				length = filename.GetPath().Length() - 1;
				path = filename.GetPath();
				if(path.Length() != 3){
					length += 2;
				}
			}
			OutputProgress(_("Creating file list, this may take some time."));
			if(!CreateList(file, rules, GetLocation(i), length)){
				return false;
			}
			file->Write();
			//Set up the progress bar
			EnableGauge(true);
		}
		else{
			EnableGauge(false);
		}
		wxString strCommand = CreateCommand(i);
		wxSetWorkingDirectory(path);
		
		PipedProcess *process = new PipedProcess();
		long lgPID = wxExecute(strCommand, wxEXEC_ASYNC|wxEXEC_NODISABLE, process);
	
		process->SetRealPid(lgPID);
		WaitThread *thread = new WaitThread(lgPID, process);

		thread->Create();
		thread->Run();
		
		if(!wxGetApp().GetUsesGUI()){
			while(!wxGetApp().GetFinished()){
				//So we dont thrash the processor
				wxMilliSleep(10);
				wxGetApp().Yield();
			}
		}
	}
	return true;
}*/

bool BackupData::IsReady(){
	if(GetLocations().Count() == 0 || GetFileLocation() == wxEmptyString
	|| GetFunction() == wxEmptyString || GetFormat() == wxEmptyString){
		return false;
	}
	else{
		return true;
	}
}
