/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "backupfunctions.h"
#include "backupprocess.h"
#include "toucan.h"
#include "backupdata.h"
#include <wx/textfile.h>

bool PipedProcess::HasInput()
{
	bool hasInput = false;
	if (IsInputAvailable()){
		if(wxGetApp().ShouldAbort()){
			HANDLE hProcess=OpenProcess(PROCESS_ALL_ACCESS,TRUE,wxGetApp().GetPID());
			TerminateProcess(hProcess,0);
		}
		wxTextInputStream tis(*GetInputStream());
		wxString msg = tis.ReadLine();
		//Need to change this to OutputProgress
		m_Window->m_Text->AppendText(msg + wxT("\n"));
		//Need a window update or refresh in here
		wxMilliSleep(50);
		hasInput = true;
    	}
	return hasInput;
}

void PipedProcess::OnTerminate(int pid, int status)
{
	// show the rest of the output
	while(HasInput())
        ;
		
	//Check to see if there are any more jobs to run
	if(wxGetApp().GetBackup() <  wxGetApp().MainWindow->m_BackupLocations->Count()){ 
		//Grab a copy of the data needed
		BackupData data;
		data.TransferFromForm(wxGetApp().MainWindow, false);
		Rules rules;
		rules.TransferFromFile(wxGetApp().MainWindow->m_Backup_Rules->GetStringSelection());
		
		wxString strCommand;
		//Open the text file for the file paths and clear it
		wxTextFile *file = new wxTextFile(wxGetApp().GetSettingsPath() + wxT("Exclusions.txt"));
		file->Open();
		file->Clear();
		file->Write();
		//Create the command to execute
		strCommand = data.CreateCommand(wxGetApp().GetBackup());
		wxString strPath = data.GetLocations().Item(wxGetApp().GetBackup());
		if (strPath[strPath.length()-1] != wxFILE_SEP_PATH) {
			strPath += wxFILE_SEP_PATH;       
		}
		strPath = strPath.BeforeLast(wxFILE_SEP_PATH);
		strPath = strPath.BeforeLast(wxFILE_SEP_PATH);
		//Create the list of files to backup
		CreateList(file, rules, data.GetLocations().Item(wxGetApp().GetBackup()), strPath.Length());
		//Commit the file changes
		file->Write();
		//Cretae the process, execute it and register it
		PipedProcess *process = new PipedProcess(m_Window);
		wxGetApp().RegisterProcess(process);
		long lgPID = wxExecute(strCommand, wxEXEC_ASYNC|wxEXEC_NODISABLE, process);
		wxGetApp().SetPID(lgPID);
		wxGetApp().SetBackup(wxGetApp().GetBackup() + 1);
	}
	else{
		//Set the correct buttons
		m_Window->m_OK->Enable(true);
		m_Window->m_Save->Enable(true);
		m_Window->m_Cancel->Enable(false);
		wxDateTime now = wxDateTime::Now();
		m_Window->m_Text->AppendText(_("Time: ") + now.FormatISOTime() + wxT("\n"));
		m_Window->m_Text->AppendText(_("Finished"));
		wxGetApp().SetAbort(false);
		//Unregister the process as we are now done
		wxGetApp().UnregisterProcess(this);
	}
}
