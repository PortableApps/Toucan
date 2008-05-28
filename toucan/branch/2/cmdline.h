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
	else{
		OutputProgress(_("Sorry the command is not recognised"));
	}
	
	delete config;
	return true;
}


#endif
