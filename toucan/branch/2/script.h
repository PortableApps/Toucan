/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SCRIPT
#define H_SCRIPT

#include <wx/arrstr.h>

class SyncData;

bool ParseScript(wxArrayString arrScript){
	
	//First check that the whole script is valid (basic number of parameters check)
	wxString strLine;
	bool blParseError = false;
	for(unsigned int i = 0; i < arrScript.Count(); i++){
		strLine = arrScript.Item(i); 
		wxStringTokenizer tkz(strLine, wxT("\""), wxTOKEN_STRTOK);
		wxString strToken = tkz.GetNextToken();
		strToken.Trim();
		if(strToken == _("Sync") || strToken == _("Secure") || strToken == _("Delete") || strToken == _("Execute")){
			if(tkz.CountTokens() != 1){
				wxLogError(_("Line %d has an incorrect number of parameters"), i+1);
				blParseError = true;
			}
		}
		else if(strToken == _("Move") || strToken == _("Copy")){
			if(tkz.CountTokens() != 3){
				wxLogError(_("Line %d has an incorrect number of parameters"), i+1);
				blParseError = true;
			}
		}
		else if(strToken == _("Backup")){
			wxLogError(_("Backup is not currently supported in scripts, sorry :("), i+1);	
			blParseError = true;
		}
		else{
			wxLogError(strToken + _(" not recognised on line %d"), i+1);
			blParseError = true;
		}
	}
	if(blParseError){
		return false;
	}

	
	//End of parsing for errors, now execute!

	frmProgress *window = wxGetApp().ProgressWindow;
	window->m_Text->Clear();
	//Send all errors to the text control
	wxLogTextCtrl* logTxt = new wxLogTextCtrl(window->m_Text);
	delete wxLog::SetActiveTarget(logTxt);
	//Set up the buttons on the progress box
	window->m_OK->Enable(false);
	window->m_Save->Enable(false);
	window->m_Cancel->Enable(true);
			
	window->m_Text->AppendText(_("Starting...\n"));
	wxDateTime now = wxDateTime::Now();
	window->m_Text->AppendText(_("Time: ") + now.FormatISOTime() + wxT("\n"));
	//Show the window
	window->Update();
	window->Show();

	for(unsigned int i = 0; i < arrScript.Count(); i++){
		strLine = arrScript.Item(i);
		wxStringTokenizer tkz(strLine, wxT("\""), wxTOKEN_STRTOK);
		wxString strToken = tkz.GetNextToken();
		strToken.Trim();
		if(strToken == _("Sync")){
			//Create the data sets and fill them		
			SyncData data;
			//Ensure that the data is filled
			wxString strJob = tkz.GetNextToken();
			if(!data.TransferFromFile(strJob)){
				window->m_OK->Enable(true);
				window->m_Save->Enable(true);
				window->m_Cancel->Enable(false);
				now = wxDateTime::Now();
				window->m_Text->AppendText(_("Time: ") + now.FormatISOTime() + wxT("\n"));
				window->m_Text->AppendText(_("Finished"));
				return false;
					
			}
			Rules rules;
			wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxGetApp().GetSettingsPath() + wxT("Jobs.ini"));
			if (config->Read(strJob + wxT("/Rules")) != wxEmptyString) {
				rules.TransferFromFile(config->Read(strJob + wxT("/Rules")));
			}
			delete config;
			//Create a new Sync thread and run it (needs to use Wait())
			SyncThread *thread = new SyncThread(data, rules, window, wxGetApp().MainWindow);
			thread->Create();
			thread->Run();
			thread->Wait();
			delete thread;
		}
		else if(strToken == _("Backup")){
			//Not yet working, give it time
		}
		else if(strToken == _("Secure")){
			SecureData data;
			//Ensure the data is filled
			wxString strJob = tkz.GetNextToken();
			if(!data.TransferFromFile(strJob)){
				window->m_OK->Enable(true);
				window->m_Save->Enable(true);
				window->m_Cancel->Enable(false);
				now = wxDateTime::Now();
				window->m_Text->AppendText(_("Time: ") + now.FormatISOTime() + wxT("\n"));
				window->m_Text->AppendText(_("Finished"));
				return false;
			}
			Rules rules;
			wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxGetApp().GetSettingsPath() + wxT("Jobs.ini"));
			if (config->Read(strJob + wxT("/Rules")) != wxEmptyString) {
				rules.TransferFromFile(config->Read(strJob + wxT("/Rules")));
			}
			//Call the secure function
			Secure(data, rules, window);
		}
		else if(strToken == _("Delete")){
			wxString strSource = tkz.GetNextToken();
			if(wxRemoveFile(strSource)){
				window->m_Text->AppendText(_("Deleted ") +strSource + wxT("\n"));	
			}
			else{
				window->m_Text->AppendText(_("Failed to delete ") +strSource + wxT("\n"));				
			}
		}
		else if(strToken == _("Move")){
			wxString strSource = tkz.GetNextToken();
			tkz.GetNextToken();
			wxString strDest = tkz.GetNextToken();
			if(wxCopyFile(strSource, strDest, true)){
				if(wxRemoveFile(strSource)){
					window->m_Text->AppendText(_("Moved") +strSource + wxT("\n"));	
				}
				else{
					window->m_Text->AppendText(_("Failed to move ") +strSource + wxT("\n"));
				}
			}
			else{
				window->m_Text->AppendText(_("Failed to move ") +strSource + wxT("\n"));		
			}
		}
		else if(strToken == _("Copy")){
			wxString strSource = tkz.GetNextToken();
			tkz.GetNextToken();
			wxString strDest = tkz.GetNextToken();
			if(wxCopyFile(strSource, strDest, true)){
				window->m_Text->AppendText(_("Copied ") +strSource + wxT("\n"));	
			}
			else{
				window->m_Text->AppendText(_("Failed to copy ") +strSource + wxT("\n"));
			}
		}
		else if(strToken == _("Execute")){
			wxString strExecute = tkz.GetNextToken();
			wxExecute(strExecute, wxEXEC_SYNC|wxEXEC_NODISABLE)){
			window->m_Text->AppendText(_("Executed ") + strExecute) + wxT("\n");
		}
	}
	window->m_OK->Enable(true);
	window->m_Save->Enable(true);
	window->m_Cancel->Enable(false);
	now = wxDateTime::Now();
	window->m_Text->AppendText(_("Time: ") + now.FormatISOTime() + wxT("\n"));
	window->m_Text->AppendText(_("Finished"));
	return true;
}
		
#endif
