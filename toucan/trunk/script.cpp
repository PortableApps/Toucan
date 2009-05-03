/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include <wx/arrstr.h>
#include <wx/tokenzr.h>
#include <wx/fileconf.h>
#include <wx/listctrl.h>

#include "toucan.h"
#include "script.h"
#include "waitthread.h"
#include "backupprocess.h"
#include "secure.h"
#include "variables.h"
#include "basicfunctions.h"
#include "filecounter.h"
#include "data/rootdata.h"
#include "sync/syncdata.h"
#include "sync/syncthread.h"
#include "data/backupdata.h"
#include "data/securedata.h"
#include "controls/loglistctrl.h"
#include "forms/frmmain.h"
#include "forms/frmprogress.h"

bool ScriptManager::Execute(){
	StartUp();
	if(!Validate()){
		CleanUp();
		return false;
	}
	if(wxGetApp().GetUsesGUI()){
		ProgressBarSetup();
	}
	if(GetCount() != 0){
		SetCommand(1);
		ParseCommand(0);
	}
	return true;
}

bool ScriptManager::StartUp(){
	//Set up all of the form related stuff
	m_ProgressWindow = wxGetApp().ProgressWindow;
	m_ProgressWindow->m_List->DeleteAllItems();
	wxGetApp().MainWindow->m_Notebook->Disable();
	//Send all errors to the list control
	LogListCtrl* logList = new LogListCtrl(m_ProgressWindow->m_List);
	delete wxLog::SetActiveTarget(logList);
	//Set up the buttons on the progress box
	m_ProgressWindow->m_OK->Enable(false);
	m_ProgressWindow->m_Save->Enable(false);
	m_ProgressWindow->m_Cancel->Enable(true);
	
	//Send a blank item to get the item count up
	OutputBlank();
	m_Time = wxDateTime::Now();
	OutputProgress(m_Time.FormatTime(), _("Starting"));
	OutputBlank();
	
	SetGaugeValue(0);
	//Show the window
	if(wxGetApp().GetUsesGUI()){
		m_ProgressWindow->Refresh();
		m_ProgressWindow->Update();
		m_ProgressWindow->Show();
	}
	m_ProgressWindow->m_List->SetColumnWidth(1, m_ProgressWindow->m_List->GetClientSize().GetWidth() - m_ProgressWindow->m_List->GetColumnWidth(0));
	return true;
}

bool ScriptManager::Validate(){
	//Check the script to see if it is valid, check number of parameters and job name
	bool valid = true;
	for(unsigned int i = 0; i < m_Script.Count(); i++){
		//Split the script line up into tokens, quote mark limited
		wxStringTokenizer tkz(m_Script.Item(i), wxT("\""), wxTOKEN_STRTOK);
		wxString token = tkz.GetNextToken();
		token.Trim();
		if(token == wxT("Sync") || token == wxT("Secure") || token == _("Delete") || token == _("Execute") || token == wxT("Backup")){
			if(tkz.CountTokens() != 1){
				OutputProgress(wxString::Format(_("Line %d has an incorrect number of parameters"), i+1));
				valid = false;
			}
			//We have the correct number of parameters, check the job names
			if(token == wxT("Sync")){
				wxString job = tkz.GetNextToken();
				SyncData data;
				data.SetName(job);
				if(data.TransferFromFile()){
					//We dont yet need to do anything special for Sync
					;
				}
				else{
					OutputProgress(wxString::Format(job + _(" not recognised on line %d"), i+1));
					valid = false;
				}
			}
			else if(token == wxT("Backup")){
				wxString job = tkz.GetNextToken();
				BackupData data;
				data.SetName(job);
				if(data.TransferFromFile()){
					if(data.GetUsesPassword()){
						wxString pass = InputPassword();
						if(pass == wxEmptyString){
							valid = false;
						}
						else{
							m_Password = pass;
						}
					}
				}
				else{
					OutputProgress(wxString::Format(job + _(" not recognised on line %d"), i+1));
					valid = false;
				}
			}
			else if(token == wxT("Secure")){
				wxString job = tkz.GetNextToken();
				SecureData data;
				data.SetName(job);
				if(data.TransferFromFile()){
					wxString pass = InputPassword();
					if(pass == wxEmptyString){
						valid = false;
					}
					else{
						m_Password = pass;
					}
				}
				else{
					OutputProgress(wxString::Format(job + _(" not recognised on line %d"), i+1));
					valid = false;
				}
			}
		}
		else if(token == _("Move") || token == _("Copy") || token == _("Rename")){
			if(tkz.CountTokens() != 3){
				OutputProgress(wxString::Format(_("Line %d has an incorrect number of parameters"), i+1));
				valid = false;
			}
		}
		else{
			OutputProgress(wxString::Format(token + _(" not recognised on line %d"), i+1));
			valid = false;
		}
	}
	return valid;
}

bool ScriptManager::ProgressBarSetup(){
	OutputProgress(_("Setting up progress bar"));
	long count = 0;
	FileCounter counter;
	for(unsigned int i = 0; i < GetScript().GetCount(); i++){
		wxString strLine = m_Script.Item(i);
		wxStringTokenizer tkz(strLine, wxT("\""), wxTOKEN_STRTOK);
		wxString strToken = tkz.GetNextToken();
		strToken.Trim();
		if(strToken == wxT("Sync")){
			SyncData data;
			strToken = tkz.GetNextToken();
			data.SetName(strToken);
			data.TransferFromFile();
			counter.AddPath(data.GetSource());
			//Add both paths for mirror and equalise as they look at both sides
			if(data.GetFunction() == _("Equalise") || data.GetFunction() == _("Mirror")){
				counter.AddPath(data.GetDest());			
			}
		}	
		else if(strToken == wxT("Backup")){
			BackupData data;
			strToken = tkz.GetNextToken();
			data.SetName(strToken);
			data.TransferFromFile();
			if(data.GetFunction() != _("Restore")){
				counter.AddPaths(data.GetLocations());
			}
			//Add an extra three for the message 7zip sends
			count += 3;
		}
		else if(strToken == wxT("Secure")){
			SecureData data;
			strToken = tkz.GetNextToken();
			data.SetName(strToken);
			data.TransferFromFile();
			counter.AddPaths(data.GetLocations());
		}
		else if(strToken == _("Delete")){
			count++;
		}
		else if(strToken == _("Move")){
			count++;
		}
		else if(strToken == _("Copy")){
			count++;
		}
		else if(strToken == _("Rename")){
			count++;
		}
		else if(strToken == _("Execute")){
			count++;
		}
	}
	counter.Count();
	count += counter.GetCount();
	SetGaugeValue(0);
	SetGaugeRange(count);
	return true;
}

bool ScriptManager::ParseCommand(int i){
	if(wxGetApp().GetAbort()){
		return false;
	}
	wxDateTime now = wxDateTime::Now();
	
	wxString strLine = m_Script.Item(i);
	
	wxStringTokenizer tkz(strLine, wxT("\""), wxTOKEN_STRTOK);
	wxString strToken = tkz.GetNextToken();
	
	strToken.Trim();
	
	RootData* data;
	
	if(strToken == wxT("Sync")){
		data = new SyncData();
		strToken = tkz.GetNextToken();
		data->SetName(strToken);
	}	
	else if(strToken == wxT("Backup")){
		data = new BackupData();
		strToken = tkz.GetNextToken();
		data->SetName(strToken);
	}
	else if(strToken == wxT("Secure")){
		data = new SecureData();
		strToken = tkz.GetNextToken();
		data->SetName(strToken);
	}
	else if(strToken == _("Delete")){
		wxString strSource = Normalise(Normalise(tkz.GetNextToken()));
		if(wxRemoveFile(strSource)){
			OutputProgress(_("Deleted ") + strSource);	
		}
		else{
			OutputProgress(_("Failed to delete ") + strSource);				
		}
		IncrementGauge();
		wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, ID_SCRIPTFINISH);
		wxPostEvent(wxGetApp().ProgressWindow, event);	
		return true;
	}
	else if(strToken == _("Move")){
		wxString strSource = Normalise(Normalise(tkz.GetNextToken()));
		tkz.GetNextToken();
		wxString strDest = Normalise(Normalise(tkz.GetNextToken()));
		if(wxCopyFile(strSource, strDest, true)){
			if(wxRemoveFile(strSource)){
				OutputProgress(_("Moved") + strSource);	
			}
			else{
				OutputProgress(_("Failed to move ") + strSource);
			}
		}
		else{
			OutputProgress(_("Failed to move ") + strSource);		
		}
		IncrementGauge();
		wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, ID_SCRIPTFINISH);
		wxPostEvent(wxGetApp().ProgressWindow, event);	
		return true;
	}
	else if(strToken == _("Copy")){
		wxString strSource = Normalise(Normalise(tkz.GetNextToken()));
		tkz.GetNextToken();
		wxString strDest = Normalise(Normalise(tkz.GetNextToken()));
		if(wxCopyFile(strSource, strDest, true)){
			OutputProgress(_("Copied ") + strSource);	
		}
		else{
			OutputProgress(_("Failed to copy ") +strSource);
		}
		IncrementGauge();
		wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, ID_SCRIPTFINISH);
		wxPostEvent(wxGetApp().ProgressWindow, event);	
		return true;
	}
	else if(strToken == _("Rename")){
		wxString strSource = Normalise(Normalise(tkz.GetNextToken()));
		tkz.GetNextToken();
		wxString strDest = Normalise(Normalise(tkz.GetNextToken()));
		if(wxRenameFile(strSource, strDest, true)){
			OutputProgress(_("Renamed ") + strSource);	
		}
		else{
			OutputProgress(_("Failed to rename ") + strSource);
		}
		IncrementGauge();
		wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, ID_SCRIPTFINISH);
		wxPostEvent(wxGetApp().ProgressWindow, event);	
		return true;
	}
	else if(strToken == _("Execute")){
		wxString strExecute = Normalise(Normalise(tkz.GetNextToken()));
		wxExecute(strExecute, wxEXEC_SYNC|wxEXEC_NODISABLE);
		OutputProgress(_("Executed ") + strExecute);
		IncrementGauge();
		wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, ID_SCRIPTFINISH);
		wxPostEvent(wxGetApp().ProgressWindow, event);	
		return true;
	}
	else{
		return false;
	}
	
	if(!data->TransferFromFile()){
		CleanUp();
	}
	if(!data->NeededFieldsFilled()){
		wxMessageBox(_("Not all of the required fields are filled"), _("Error"), wxICON_ERROR);
		CleanUp();
	}
	if(data->NeedsPassword()){
		data->SetPassword(m_Password);
	}
	
	Rules rules;
	if (wxGetApp().m_Jobs_Config->Read(data->GetName() + wxT("/Rules")) != wxEmptyString) {
		rules.TransferFromFile(wxGetApp().m_Jobs_Config->Read(data->GetName() + wxT("/Rules")));
	}
	
	if(!data->Execute(rules)){
		CleanUp();
	}
	
	return true;	
}

bool ScriptManager::CleanUp(){
	m_ProgressWindow->m_OK->Enable(true);
	m_ProgressWindow->m_Save->Enable(true);
	m_ProgressWindow->m_Cancel->Enable(false);
	wxDateTime now = wxDateTime::Now();
	if(wxGetApp().ProgressWindow->m_Gauge->IsEnabled()){
		SetGaugeValue(wxGetApp().ProgressWindow->m_Gauge->GetRange());	
	}
	OutputBlank();
	OutputProgress(now.Subtract(m_Time).Format(), _("Elapsed"));
	OutputProgress(now.FormatTime(), _("Finished"));

	//Yield here to make sure all output is shown
	wxGetApp().Yield();
	//Resize the second column to show all of the text
	m_ProgressWindow->m_List->SetColumnWidth(1, -1);

	//Remove finished jobs
	if (wxGetApp().m_Jobs_Config->HasGroup(wxT("LastSyncJob"))){
		wxGetApp().m_Jobs_Config->DeleteGroup(wxT("LastSyncJob"));
	}
	if (wxGetApp().m_Jobs_Config->HasGroup(wxT("LastBackupJob"))){
		wxGetApp().m_Jobs_Config->DeleteGroup(wxT("LastBackupJob"));
	}
	if (wxGetApp().m_Jobs_Config->HasGroup(wxT("LastSecureJob"))){
		wxGetApp().m_Jobs_Config->DeleteGroup(wxT("LastSecureJob"));
	}
	if (wxGetApp().m_Jobs_Config->HasGroup(wxT("LastRestoreJob"))){
		wxGetApp().m_Jobs_Config->DeleteGroup(wxT("LastRestoreJob"));
	}
	wxGetApp().m_Jobs_Config->Flush();
	wxGetApp().MainWindow->m_Notebook->Enable();
	return true;	
}
