/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "restoredata.h"
#include "basicfunctions.h"
#include "toucan.h"
#include "backupprocess.h"
#include "waitthread.h"

bool RestoreData::TransferFromFile(){
	wxString strName = GetName();

	bool blError = false;
	wxString strTemp;
	bool blTemp;

	blError = wxGetApp().m_Jobs_Config->Read(strName + wxT("/BackupFile"), &strTemp);
	if(blError){ SetBackupFile(strTemp); }	
	blError = wxGetApp().m_Jobs_Config->Read(strName + wxT("/BackupFolder"), &strTemp);
	if(blError){ SetBackupFolder(strTemp); }	
	blError = wxGetApp().m_Jobs_Config->Read(strName + wxT("/IsPass"), &blTemp);
	if(blError){ IsPassword = blTemp; }
	
	if(!blError){
		ErrorBox(_("There was an error reading from the jobs file, \nplease check it is not set as read only or in use."));
		return false;
	}
	return true;
}

bool RestoreData::TransferToFile(){
	wxString strName = GetName();
	bool blError = false;

	//Delete any existing jobs with this name to ensure correct settings are retained
	wxGetApp().m_Jobs_Config->DeleteGroup(strName);
	wxGetApp().m_Jobs_Config->Flush();

	//Add the files to be written
	if(!wxGetApp().m_Jobs_Config->Write(strName + wxT("/BackupFile"),  GetBackupFile())){
		blError = true;
	}
	if(!wxGetApp().m_Jobs_Config->Write(strName + wxT("/BackupFolder"),  GetBackupFolder())){
		blError = true;
	}		
	if(!wxGetApp().m_Jobs_Config->Write(strName + wxT("/IsPass"), IsPassword)){
		blError = true;
	}
	
	//Write the files
	wxGetApp().m_Jobs_Config->Flush();
	
	if(blError){
		ErrorBox(_("There was an error saving to the jobs file, \nplease check it is not set as read only or in use."));
		return false;
	}
	return true;
}

//As restore is not a valid job no need for these functions
bool RestoreData::TransferToForm(){
	return false;
}

bool RestoreData::TransferFromForm(){
	return false;
}

/*This is a debugging tool only, not for use in release  versions of Toucan */
void RestoreData::Output(){
	MessageBox(GetBackupFile(), wxT("Backup File"));
	MessageBox(GetBackupFolder(), wxT("Backup Location"));
}

bool RestoreData::Execute(Rules rules){
	if(GetPassword() != wxEmptyString){
		SetPassword(wxT(" -p") + GetPassword());
	}
	wxString strCommand =  wxT("7za.exe  x") + GetPassword() + wxT(" -aoa \"") + GetBackupFile() + wxT("\" -o\"") + GetBackupFolder() + wxT("\" * -r");	
	PipedProcess *process = new PipedProcess(wxGetApp().ProgressWindow);
	long lgPID = wxExecute(strCommand, wxEXEC_ASYNC|wxEXEC_NODISABLE, process);
	process->SetRealPid(lgPID);
	WaitThread *thread = new WaitThread(lgPID, process);
	thread->Create();
	thread->Run();
	thread->Wait();
	while(process->HasInput())
		;
	wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, ID_SCRIPTFINISH);
	wxPostEvent(wxGetApp().ProgressWindow, event);	
	return true;
}

bool RestoreData::NeededFieldsFilled(){
	bool blFilled = true;
	if(GetBackupFile() == wxEmptyString){
		blFilled = false;
	}
	if(GetBackupFolder() == wxEmptyString){
		blFilled = false;
	}
	return blFilled;
}
