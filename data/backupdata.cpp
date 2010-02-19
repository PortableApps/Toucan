/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 - 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "backupdata.h"
#include "../basicfunctions.h"
#include "../toucan.h"
#include "../variables.h"
#include "../controls/dirctrlspec.h"
#include "../forms/frmmain.h"
#include "../forms/frmprogress.h"
#include "../backup/backupprocess.h"
#include <wx/textfile.h>
#include <wx/fileconf.h>
#include <wx/stdpaths.h>
#include <wx/dir.h>
#include <wx/msgdlg.h>
#include <wx/combobox.h>
#include <wx/radiobox.h>
#include <wx/slider.h>
#include <wx/checkbox.h>

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
	if(wxGetApp().m_Jobs_Config->Read(GetName() + wxT("/Function"), &stemp)) SetFunction(ToLang(stemp));
		else error = true;
	if(wxGetApp().m_Jobs_Config->Read(GetName() + wxT("/Format"), &stemp)) SetFormat(stemp);
		else error = true;
	if(wxGetApp().m_Jobs_Config->Read(GetName() + wxT("/Ratio"), &itemp)) SetRatio(itemp);
		else error = true;
	if(wxGetApp().m_Jobs_Config->Read(GetName() + wxT("/IsPass"), &btemp)) SetUsesPassword(btemp);
		else error = true;
	if(wxGetApp().m_Jobs_Config->Read(GetName() + wxT("/Test"), &btemp)) SetTest(btemp);
		else error = true;
	if(wxGetApp().m_Jobs_Config->Read(GetName() + wxT("/Rules"), &stemp))  SetRules(new Rules(stemp, true));
		else error = true;

	if(error){
		wxMessageBox(_("There was an error reading from the jobs file"), _("Error"), wxICON_ERROR);
		return false;
	}
	return true;
}

bool BackupData::TransferToFile(){
	bool error = false;

	wxGetApp().m_Jobs_Config->DeleteGroup(GetName());

	if(!wxGetApp().m_Jobs_Config->Write(GetName() + wxT("/BackupLocation"),  GetFileLocation())) error = true;
	if(!wxGetApp().m_Jobs_Config->Write(GetName() + wxT("/Locations"),  ArrayStringToString(GetLocations(), wxT("|")))) error = true;
	if(!wxGetApp().m_Jobs_Config->Write(GetName() + wxT("/Function"), ToEn(GetFunction()))) error = true;
	if(!wxGetApp().m_Jobs_Config->Write(GetName() + wxT("/Format"), GetFormat())) error = true;
	if(!wxGetApp().m_Jobs_Config->Write(GetName() + wxT("/Ratio"), GetRatio())) error = true;
	if(!wxGetApp().m_Jobs_Config->Write(GetName() + wxT("/IsPass"), GetUsesPassword())) error = true;
	if(!wxGetApp().m_Jobs_Config->Write(GetName() + wxT("/Test"), GetTest())) error = true;
	if(!wxGetApp().m_Jobs_Config->Write(GetName() + wxT("/Rules"), GetRules() ? GetRules()->GetName() : wxT(""))) error = true;
	if(!wxGetApp().m_Jobs_Config->Write(GetName() + wxT("/Type"),  wxT("Backup"))) error = true;

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

	if(GetName() != wxT("BackupRemember") && window->m_Backup_Job_Select->GetStringSelection() != GetName()){
		if(window->m_Backup_Job_Select->FindString(GetName(), true) == wxNOT_FOUND){
			window->m_Backup_Job_Select->AppendString(GetName());
		}
		window->m_Backup_Job_Select->SetStringSelection(GetName());
	}

	window->m_Backup_Location->SetValue(GetFileLocation());
	window->m_Backup_TreeCtrl->DeleteChildren(window->m_Backup_TreeCtrl->GetRootItem());
	
	window->m_BackupLocations->Clear();
	for(unsigned int j = 0; j < GetLocations().GetCount(); j++){
		window->m_BackupLocations->Add(GetLocation(j));
		window->m_Backup_TreeCtrl->AddItem(Normalise(GetLocation(j)));
	}
	window->m_Backup_Function->SetStringSelection(GetFunction());
	window->m_Backup_Format->SetStringSelection(GetFormat());
	window->m_Backup_Ratio->SetValue(GetRatio());
	window->m_Backup_IsPass->SetValue(GetUsesPassword());
	window->m_Backup_Rules->SetStringSelection(GetRules()->GetName());

	//Notify the window that we have updated the rules
	wxCommandEvent event;
	window->OnBackupRulesSelected(event);
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

wxArrayString BackupData::CreateCommands(){
	wxArrayString commands;
	wxString ratio, originalext;
	wxString tempdir = wxT(" -w\"") + wxPathOnly(GetFileLocation()) + wxT("\"");	
	wxString solid = wxEmptyString;
	wxString exe = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("7za");
	wxString includes = wxT(" @\"") + wxGetApp().GetSettingsPath() + wxT("Includes.txt") + wxT("\" ");

	//Turn the inputted format into one 7zip will understand
	if (GetFormat() == wxT("Zip")){
		SetFormat(wxT("zip"));
	}
   	else if (GetFormat() == wxT("7-Zip")){
		SetFormat(wxT("7z"));
		//Make sure solid mode is turned off so updating works
		solid = wxT(" -ms=off");
	}
	else if(GetFormat() == wxT("GZip")){
		SetFormat(wxT("tar"));
		originalext = GetFileLocation().AfterLast('\\').AfterFirst('.');
		SetFileLocation(GetFileLocation().Left(GetFileLocation().Length() - originalext.Length()) + "tar");
	}

	if (GetRatio() == 0){
		ratio = wxT(" -mx0");
	}
	else if (GetRatio() == 1){
		ratio = wxT(" -mx1");
	}
	else if (GetRatio() == 2){
		ratio = wxT(" -mx3");
	}
	else if (GetRatio() == 3){
		ratio = wxT(" -mx5");
	}
	else if (GetRatio() == 4){
		ratio = wxT(" -mx7");
	}
	else if (GetRatio() == 5){
		ratio = wxT(" -mx9");
	}

	//Checking to see if there is a password used
	if(GetPassword() != wxEmptyString && GetPassword().Left(3) != " -p"){
		SetPassword(wxT(" -p") + GetPassword());
		//If 7zip is used then make sure headers are encrypted
		if(GetFormat() == wxT("7z")){
			SetPassword(GetPassword() + wxT(" -mhe=on"));
		}
	}

	if(GetFunction() == _("Complete")){
		commands.Add(exe + wxT(" a -t") + GetFormat() + GetPassword() + ratio + solid +  wxT(" \"") + GetFileLocation() + wxT("\"") + includes + tempdir);	
	}
	else if(GetFunction() == _("Update")){
		commands.Add(exe + wxT(" u -t") + GetFormat() + GetPassword() + ratio + solid + wxT(" \"") + GetFileLocation() + wxT("\"") + includes + tempdir); 
	}
	else if(GetFunction() == _("Restore")){
		commands.Add(exe + wxT("  x -aoa \"") + GetLocation(0) + wxT("\" -o\"") + GetFileLocation() + wxT("\" * -r") + GetPassword());	
	}
	//With the Differential type the first use creates a file called base file. On subsequent runs a file is created with a filename based on both the date and time.    
	else if(GetFunction() == _("Differential")){
		if (GetFileLocation()[GetFileLocation().length()-1] != wxFILE_SEP_PATH) {
			SetFileLocation(GetFileLocation() + wxFILE_SEP_PATH);       
		}
		if(wxFileExists(GetFileLocation() + wxFILE_SEP_PATH + wxT("BaseFile.") + GetFormat())){
			wxDateTime now = wxDateTime::Now();
			commands.Add(exe + wxT(" u") + GetPassword() + ratio + solid + wxT(" \"") + GetFileLocation() + wxT("BaseFile.") + GetFormat() + wxT("\" -u- -up0q3x2z0!\"") + GetFileLocation() + now.FormatISODate()+ wxT("-") + now.Format(wxT("%H")) + wxT("-") +  now.Format(wxT("%M")) + wxT(".") + GetFormat() + wxT("\"") + includes + tempdir);
		}
		else{
			commands.Add(exe + wxT(" a -t") + GetFormat() + GetPassword() + ratio + solid +  wxT(" \"") + GetFileLocation() + wxT("BaseFile.") + GetFormat() + wxT(" \"") + includes + tempdir);
		}
	}

	if(GetFormat() == "tar"){
		wxString tarpath = GetFileLocation();
		SetFileLocation(GetFileLocation().Left(GetFileLocation().Length() - 3) + originalext);
		commands.Add(exe + " a -tgzip " + GetPassword() + ratio + wxT(" \"") + GetFileLocation() + wxT("\" \"") + tarpath + wxT("\"") + tempdir);
	}

	if(GetTest()){
		commands.Add(exe + " t " + GetFileLocation() + " * -r");
	}
	return commands;
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

bool BackupData::IsReady(){
	if(GetLocations().Count() == 0 || GetFileLocation() == wxEmptyString
	|| GetFunction() == wxEmptyString || GetFormat() == wxEmptyString){
		return false;
	}
	else{
		return true;
	}
}
