/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_COMMANDLINE
#define H_COMMANDLINE

#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/cmdline.h>
#include <wx/fileconf.h>
#include "script.h"

bool ParseCommandLine(){
	OutputProgress(_("Welcome to the Toucan command line system.\n"));
	wxCmdLineParser cmdParser(wxGetApp().argc, wxGetApp().argv);
	int res;
	{
		wxLogNull log;
		res = cmdParser.Parse(false);
	}
	//Create new file config to read the jobs
	wxFileConfig *config = new wxFileConfig(wxT(""), wxT(""), wxGetApp().GetSettingsPath() + wxT("Jobs.ini"));
	
	if(config->Read(cmdParser.GetParam(0) + wxT("/Type")) == wxT("Sync")){
		wxArrayString arrScript;
		wxMessageBox(_("I am here"));
		arrScript.Add(wxT("Sync ") + cmdParser.GetParam(1) + wxT("\""));
		ParseScript(arrScript);
	}
	else if(config->Read(cmdParser.GetParam(0) + wxT("/Type")) == wxT("Backup")){
		wxArrayString arrScript;
		arrScript.Add(wxT("Backup \"") + cmdParser.GetParam(1) + wxT("\""));
		ParseScript(arrScript);
	}
	else if(config->Read(cmdParser.GetParam(0) + wxT("/Type")) == wxT("Secure")){
		wxArrayString arrScript;
		arrScript.Add(wxT("Secure ") + cmdParser.GetParam(1) + wxT("\""));
		ParseScript(arrScript);	
	}
	else if(cmdParser.GetParam(0) == _("Script")){
		wxFileConfig *config = new wxFileConfig(wxT(""), wxT(""), wxGetApp().GetSettingsPath() + wxT("Scripts.ini"));
		wxString strFile = config->Read(cmdParser.GetParam(1) + wxT("/") + wxT("Script"));
		wxArrayString arrContents = StringToArrayString(strFile, wxT("#"));
		ParseScript(arrContents);
	}
	else if(cmdParser.GetParam(0) == wxT("Sync") && cmdParser.GetParamCount() == 9){
		
		SyncData data;
		data.SetFunction(cmdParser.GetParam(1));
		data.SetTimeStamps(cmdParser.GetParam(2));
		data.SetAttributes(cmdParser.GetParam(3));
		data.SetIgnoreRO(cmdParser.GetParam(4));
		data.SetIgnoreDLS(cmdParser.GetParam(5));
		data.SetSource(cmdParser.GetParam(7));
		data.SetDest(cmdParser.GetParam(8));
		wxMessageBox(_("Here"));
		if(data.TransferToFile(wxT("LastSyncJob"))){
			wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxGetApp().GetSettingsPath() + wxT("Jobs.ini") );
			config->Write(wxT("LastSyncJob/Rules"),  cmdParser.GetParam(6));
			config->Write(wxT("LastSyncJob/Type"),  _("Sync"));
			config->Flush();
			wxArrayString arrScript;
			arrScript.Add(wxT("Sync \"LastSyncJob\""));
			wxGetApp().SetAbort(false);
			ParseScript(arrScript);
			config->DeleteGroup(wxT("LastSyncJob"));
			delete config;
		}
	}
	else{
		OutputProgress(_("Sorry the command is not recognised"));
	}
	
	delete config;
	return true;
}


#endif
