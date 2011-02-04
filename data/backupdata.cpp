/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 - 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include "backupdata.h"
#include "../basicfunctions.h"
#include "../toucan.h"
#include "../rules.h"
#include "../controls/previewctrl.h"
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

void BackupData::TransferFromFile(){
	if(!wxGetApp().m_Jobs_Config->Exists(GetName())){
		throw std::invalid_argument(std::string(GetName() + " is not a valid job"));
	}

	SetFileLocation(Read<wxString>("BackupLocation"));
	SetLocations(Read("Locations"));
	SetFunction(ToLang(Read<wxString>("Function")));
	SetFormat(Read<wxString>("Format"));
	SetRatio(Read<int>("Ratio"));
	SetUsesPassword(Read<bool>("IsPass"));
	SetTest(Read<bool>("Test"));
	SetSolid(Read<bool>("Solid"));

	RuleSet *rules = new RuleSet(Read<wxString>("Rules"));
    rules->TransferFromFile(wxGetApp().GetSettingsPath());
	SetRules(rules);
}

void BackupData::TransferToFile(){
	wxGetApp().m_Jobs_Config->DeleteGroup(GetName());

	Write("BackupLocation", GetFileLocation());
	Write("Locations", GetLocations());
	Write("Function", ToEn(GetFunction()));
	Write("Format", GetFormat());
	Write("Ratio", GetRatio());
	Write("IsPass", GetUsesPassword());
	Write("Test", GetTest());
	Write("Solid", GetSolid());
	Write("Rules", GetRules() ? GetRules()->GetName() : wxT(""));
	Write("Type", "Backup");

	if(!wxGetApp().m_Jobs_Config->Flush()){
		throw std::runtime_error("There was an error flushing the jobs file");
	}
}

bool BackupData::TransferToForm(frmMain *window){
	if(!window){
		return false;
	}

	window->m_Backup_Rules->SetStringSelection(GetRules()->GetName());

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
	}

	//Notify the window that we have updated the rules this also addes them to the tree
	wxCommandEvent event;
	window->OnBackupRulesSelected(event);

	window->m_Backup_Function->SetStringSelection(GetFunction());
	window->m_Backup_Format->SetStringSelection(GetFormat());
	window->m_Backup_Ratio->SetValue(GetRatio());
	window->m_Backup_IsPass->SetValue(GetUsesPassword());
	window->m_Backup_Test->SetValue(GetTest());
	window->m_BackupSolid->SetValue(GetSolid());
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
	SetTest(window->m_Backup_Test->GetValue());
	SetSolid(window->m_BackupSolid->GetValue());

	RuleSet *rules = new RuleSet(window->m_Backup_Rules->GetStringSelection());
    rules->TransferFromFile(wxGetApp().GetSettingsPath());
	SetRules(rules);

	return true;	
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
		if(!GetSolid()){
			solid = " -ms=off";
		}
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
		commands.Add(exe + wxT(" a -t") + GetFormat() + GetPassword() + ratio + solid +  wxT(" \"") + GetFileLocation() + wxT("\"") + includes + tempdir + " -sccUTF-8");	
	}
	else if(GetFunction() == _("Update")){
		commands.Add(exe + wxT(" u -t") + GetFormat() + GetPassword() + ratio + solid + wxT(" \"") + GetFileLocation() + wxT("\"") + includes + tempdir + " -sccUTF-8"); 
	}
	else if(GetFunction() == _("Restore")){
		commands.Add(exe + wxT("  x -aoa \"") + GetLocation(0) + wxT("\" -o\"") + GetFileLocation() + wxT("\" * -r") + GetPassword() + " -sccUTF-8");	
	}
	//With the Differential type the first use creates a file called base file. On subsequent runs a file is created with a filename based on both the date and time.    
	else if(GetFunction() == _("Differential")){
		if (GetFileLocation()[GetFileLocation().length()-1] != wxFILE_SEP_PATH) {
			SetFileLocation(GetFileLocation() + wxFILE_SEP_PATH);       
		}
		if(wxFileExists(GetFileLocation() + wxFILE_SEP_PATH + wxT("BaseFile.") + GetFormat())){
			wxDateTime now = wxDateTime::Now();
			commands.Add(exe + wxT(" u") + GetPassword() + ratio + solid + wxT(" \"") + GetFileLocation() + wxT("BaseFile.") + GetFormat() + wxT("\" -u- -up0q3x2z0!\"") + GetFileLocation() + now.FormatISODate()+ wxT("-") + now.Format(wxT("%H")) + wxT("-") +  now.Format(wxT("%M")) + wxT(".") + GetFormat() + wxT("\"") + includes + tempdir + " -sccUTF-8");
		}
		else{
			commands.Add(exe + wxT(" a -t") + GetFormat() + GetPassword() + ratio + solid +  wxT(" \"") + GetFileLocation() + wxT("BaseFile.") + GetFormat() + wxT(" \"") + includes + tempdir + " -sccUTF-8");
		}
	}

	if(GetFormat() == "tar"){
		wxString tarpath = GetFileLocation();
		SetFileLocation(GetFileLocation().Left(GetFileLocation().Length() - 3) + originalext);
		commands.Add(exe + " a -tgzip " + GetPassword() + ratio + wxT(" \"") + GetFileLocation() + wxT("\" \"") + tarpath + wxT("\"") + tempdir + " -sccUTF-8");
	}

	if(GetTest()){
		commands.Add(exe + " t \"" + GetFileLocation() + "\" * -r" + " -sccUTF-8");
	}
	return commands;
}


bool BackupData::CreateList(wxTextFile *file, wxString path, int length){
	if(wxGetApp().GetAbort()){
		return true;
	}

	if(wxDirExists(path)){
		//Clean up the path passed
		if (path[path.length()-1] != wxFILE_SEP_PATH) {
			path += wxFILE_SEP_PATH;       
		}
		wxDir dir(path);
		wxString filename;
		//If the path is ok
		if(dir.GetFirst(&filename)){
			//Loop through all of the files and folders in the directory
			do {
                wxFileName location = wxFileName(path + filename);
                RuleResult result = GetRules()->Matches(location);

                if((result == Excluded && location.IsDir()) || 
                  ((result == Included || result == NoMatch) && !location.IsDir())){
                    //We recurse to check for included files or add the file to the list
                    CreateList(file, path + filename, length);
                }
                else if((result == Included || result == NoMatch) && location.IsDir()){
                    wxDir* dircheck = new wxDir(path + filename);
                    bool nosub = !dircheck->HasFiles() && !dircheck->HasSubDirs();
                    delete dircheck;
                    //If we has no subfile or subfolders add to the list, otherwise recurse
                    if(nosub){
                        file->AddLine((path + filename).Right((path + filename).length() - length));
					}
                    else{
                        CreateList(file, path + filename, length);
                    }
					
                }
                else{
                    //Do nothing as we are either an excluded file or an 
                    //absolutely excluded folder
                }
			}
			while (dir.GetNext(&filename));
		}  
	}
	//We have been passed a file
	else{
		RuleResult res = GetRules()->Matches(wxFileName::FileName(path));
        if(res != Excluded && res != AbsoluteExcluded){
			file->AddLine(path.Right(path.Length() - length));
		}		
	}
	return true;
}
