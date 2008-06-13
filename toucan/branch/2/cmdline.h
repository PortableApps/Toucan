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

	int res;
	wxCmdLineParser cmdParser(wxGetApp().argc, wxGetApp().argv);
	int iArgs = wxGetApp().argc;
	//Simple job name
	if(iArgs == 2){
		cmdParser.AddParam(_("Job name"));
	}
	//Job name with password
	else if(iArgs == 4){
		cmdParser.AddParam(_("Job name"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
		cmdParser.AddParam(_("Password"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
		cmdParser.AddParam(_("Repeated password"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
	}
	//Sync job all specified
	else if(iArgs == 10){
		cmdParser.AddParam(_("Operation"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
		cmdParser.AddParam(_("Source"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
		cmdParser.AddParam(_("Destination"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
		cmdParser.AddParam(_("Function"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
		cmdParser.AddParam(_("Rules"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
		cmdParser.AddParam(_("Read-only"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
		cmdParser.AddParam(_("Daylight savings"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
		cmdParser.AddParam(_("Timestamps"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
		cmdParser.AddParam(_("Attributes"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
	}
	//Backup job all specified
	else if(iArgs == 7){
		cmdParser.AddParam(_("Operation"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
		cmdParser.AddParam(_("Backup file"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
		cmdParser.AddParam(_("File of paths"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
		cmdParser.AddParam(_("Format"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
		cmdParser.AddParam(_("Compression level"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
		cmdParser.AddParam(_("Rules"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
	}
	//Backup job all specified with password
	else if(iArgs == 9){
		cmdParser.AddParam(_("Operation"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
		cmdParser.AddParam(_("Backup file"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
		cmdParser.AddParam(_("File of paths"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
		cmdParser.AddParam(_("Format"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
		cmdParser.AddParam(_("Compression level"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
		cmdParser.AddParam(_("Rules"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
		cmdParser.AddParam(_("Password"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
		cmdParser.AddParam(_("Repeated password"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
	}
	//Secure all specified
	else if(iArgs == 8){
		cmdParser.AddParam(_("Operation"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
		cmdParser.AddParam(_("File of paths"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
		cmdParser.AddParam(_("Function"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
		cmdParser.AddParam(_("Format"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
		cmdParser.AddParam(_("Rules"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);		
		cmdParser.AddParam(_("Password"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
		cmdParser.AddParam(_("Repeated password"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
	}
	else{
		OutputProgress(_("You have specified the wrong number of options"));
		return false;
	}
	wxLogNull log;
	{
		res = cmdParser.Parse(false);
	}
	//Create new file config to read the jobs
	wxFileConfig *config = new wxFileConfig(wxT(""), wxT(""), wxGetApp().GetSettingsPath() + wxT("Jobs.ini"));
	if(config->Read(cmdParser.GetParam(0) + wxT("/Type")) == wxT("Sync")){
		wxArrayString arrScript;
		arrScript.Add(wxT("Sync \"") + cmdParser.GetParam(0) + wxT("\""));
		ParseScript(arrScript);
	}
	else if(config->Read(cmdParser.GetParam(0) + wxT("/Type")) == wxT("Backup")){
		wxArrayString arrScript;
		arrScript.Add(wxT("Backup \"") + cmdParser.GetParam(0) + wxT("\""));
		ParseScript(arrScript);
	}
	else if(config->Read(cmdParser.GetParam(0) + wxT("/Type")) == wxT("Secure")){
		wxArrayString arrScript;
		arrScript.Add(wxT("Secure \"") + cmdParser.GetParam(0) + wxT("\""));
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
		data.SetSource(cmdParser.GetParam(1));
		data.SetDest(cmdParser.GetParam(2));
		data.SetFunction(cmdParser.GetParam(3));
		data.SetTimeStamps(cmdParser.GetParam(5));
		data.SetAttributes(cmdParser.GetParam(6));
		data.SetIgnoreRO(cmdParser.GetParam(7));
		data.SetIgnoreDLS(cmdParser.GetParam(8));
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
	else if(cmdParser.GetParam(0) == wxT("Backup") && cmdParser.GetParamCount() == 7){
		BackupData data;
		data.SetBackupLocation(cmdParser.GetParam(1));
		wxTextFile file;
		wxArrayString arrLocations;
		file.Open(cmdParser.GetParam(2));
		for(unsigned int i = 0; i < file.GetLineCount(); i++){
			arrLocations.Add(file.GetLine(i));
		}
		data.SetLocations(arrLocations);
		data.SetFunction(cmdParser.GetParam(3));
		data.SetFormat(cmdParser.GetParam(5));
		wxVariant varTemp = cmdParser.GetParam(6);
		data.SetRatio(varTemp.GetInteger());
		if(data.TransferToFile(wxT("LastBackupJob"))){
			wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxGetApp().GetSettingsPath() + wxT("Jobs.ini") );
			config->Write(wxT("LastBackupJob/Rules"),  cmdParser.GetParam(6));
			config->Write(wxT("LastBackupJob/Type"),  _("Backup"));
			config->Flush();
			wxArrayString arrScript;
			arrScript.Add(wxT("Sync \"LastBackupJob\""));
			wxGetApp().SetAbort(false);
			ParseScript(arrScript);
			config->DeleteGroup(wxT("LastBackupJob"));
			delete config;
		}
	}
	else if(cmdParser.GetParam(0) == wxT("Secure") && cmdParser.GetParamCount() == 8){
		SecureData data;
		wxTextFile file;
		wxArrayString arrLocations;
		file.Open(cmdParser.GetParam(1));
		for(unsigned int i = 0; i < file.GetLineCount(); i++){
			arrLocations.Add(file.GetLine(i));
		}
		data.SetLocations(arrLocations);
		data.SetFunction(cmdParser.GetParam(2));
		data.SetFormat(cmdParser.GetParam(3));
		if(data.TransferToFile(wxT("LastSecureJob"))){
			wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxGetApp().GetSettingsPath() + wxT("Jobs.ini") );
			config->Write(wxT("LastSecureJob/Rules"),  cmdParser.GetParam(4));
			config->Write(wxT("LastSecureJob/Type"),  _("Secure"));
			config->Flush();
			wxArrayString arrScript;
			arrScript.Add(wxT("Sync \"LastSecureJob\""));
			wxGetApp().SetAbort(false);
			ParseScript(arrScript);
			config->DeleteGroup(wxT("LastSecureJob"));
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
