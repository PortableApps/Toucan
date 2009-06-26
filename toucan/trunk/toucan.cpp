/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include <wx/stdpaths.h>
#include <wx/splash.h>
#include <wx/intl.h>
#include <wx/fileconf.h>
#include <wx/html/helpctrl.h>
#include <wx/listctrl.h>
#include <wx/fs_arc.h>
#include <wx/dir.h>

#include "toucan.h"
#include "forms/frmmain.h"
#include "forms/frmprogress.h"
#include "backupprocess.h"
#include "cmdline.h"
#include "basicfunctions.h"
#include "script.h"
#include "settings.h"
#include "sync/syncdata.h"
#include "data/backupdata.h"
#include "data/securedata.h"

IMPLEMENT_APP(Toucan)

//Toucan startup
bool Toucan::OnInit(){
	#ifdef __WXMSW__
		if(argc == 1){
			if(wxGetOsVersion() != wxOS_WINDOWS_9X){
				ShowWindow(GetConsoleWindow(), SW_HIDE); 			
			}			
		}
	#endif
	//Set the splash screen going
	wxInitAllImageHandlers();
	wxSplashScreen *scrn = NULL;
	if(wxFileExists(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("Splash.jpg")) && argc == 1){
		wxBitmap bitmap;
		bitmap.LoadFile(wxPathOnly(wxStandardPaths::Get().GetExecutablePath())  + wxFILE_SEP_PATH + wxT("splash.jpg"), wxBITMAP_TYPE_JPEG);
		scrn = new wxSplashScreen(bitmap, wxSPLASH_CENTRE_ON_PARENT|wxSPLASH_NO_TIMEOUT, 5000, NULL, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE|wxSTAY_ON_TOP|wxFRAME_NO_TASKBAR);
	}

	if(argc == 1){
		SetUsesGUI(true);
	}
	else{
		SetUsesGUI(false);
	}
	//Work out where the settings dir is. Firstly get the exe dir
	wxFileName settingspath = wxFileName::DirName((wxPathOnly(wxStandardPaths::Get().GetExecutablePath())));
	//Next remove the \App\toucan
	settingspath.RemoveLastDir();
	settingspath.RemoveLastDir();
	//And the add \Data
	settingspath.AppendDir(wxT("Data"));

	SetSettingsPath(settingspath.GetFullPath());
	//Make sure the data directory is there
	if(!wxDirExists(GetSettingsPath())){
		wxMkdir(GetSettingsPath());
	}

	//Create the config stuff and set it up
 	m_Jobs_Config = new wxFileConfig(wxT(""), wxT(""), wxGetApp().GetSettingsPath() + wxT("Jobs.ini"));
	m_Rules_Config = new wxFileConfig(wxT(""), wxT(""), wxGetApp().GetSettingsPath() + wxT("Rules.ini"));
	m_Scripts_Config = new wxFileConfig(wxT(""), wxT(""), wxGetApp().GetSettingsPath() + wxT("Scripts.ini"));
	m_Variables_Config = new wxFileConfig(wxT(""), wxT(""), wxGetApp().GetSettingsPath() + wxT("Variables.ini"));

	m_Jobs_Config->SetExpandEnvVars(false);
	m_Rules_Config->SetExpandEnvVars(false);
	m_Scripts_Config->SetExpandEnvVars(false);
	m_Variables_Config->SetExpandEnvVars(false);

	//Create the settings class amd load the settings
	m_Settings = new Settings(GetSettingsPath() + wxT("Settings.ini"));
	m_Settings->TransferFromFile();

	//Create the script manager
	m_Script = new ScriptManager();

	//Set up the help system
	m_Help = new wxHtmlHelpController(wxHF_DEFAULT_STYLE|wxHF_EMBEDDED, MainWindow);

	//Make sure the jobs file is up to date!
	int version = 1;
	m_Jobs_Config->Read(wxT("General/Version"), &version);
	m_Jobs_Config->Write(wxT("General/Version"), 204);
	m_Jobs_Config->Flush();
	if(version < 204){
		UpdateJobs(version);
	}

	//Update the script file
	version = 1;
	m_Scripts_Config->Read(wxT("General/Version"), &version);
	m_Scripts_Config->Write(wxT("General/Version"), 202);
	m_Scripts_Config->Flush();
	if(version < 202){
		UpdateRules(version);
	}

	//Update the rules file
	version = 1;
	m_Rules_Config->Read(wxT("General/Version"), &version);
	m_Rules_Config->Write(wxT("General/Version"), 202);
	m_Rules_Config->Flush();
	if(version < 202){
		UpdateScripts(version);
	}	

	SetLanguage(m_Settings->GetLanguageCode());

	wxFileSystem::AddHandler(new wxArchiveFSHandler);

	MainWindow = new frmMain();
	ProgressWindow = new frmProgress(NULL, ID_FRMPROGRESS, _("Progress"), wxDefaultPosition, wxSize(640, 480), wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxMAXIMIZE_BOX|wxMINIMIZE_BOX);

	if(GetUsesGUI()){
		MainWindow->Show();
		if(scrn != NULL){
			scrn->Destroy(); 
		}
		else{
			delete scrn;
		}
	}
	else{
		ParseCommandLine();
		if(!m_Settings->GetDisableLog()){
			//Write out a log so we know what happened
			wxTextFile file;
			file.Create(GetSettingsPath() + wxDateTime::Now().FormatISODate() + wxT(" - ") + wxDateTime::Now().Format(wxT("%H")) + wxT("-")+ wxDateTime::Now().Format(wxT("%M")) + wxT("-") +  wxDateTime::Now().Format(wxT("%S")) + wxT(".txt"));
			//Yield to make sure all of the output has reached the progress dialog
			Yield();
			for(int i = 0; i < ProgressWindow->m_List->GetItemCount(); i++){
				wxListItem itemcol1, itemcol2;

				itemcol1.m_itemId = i;
				itemcol1.m_col = 0;
				itemcol1.m_mask = wxLIST_MASK_TEXT;
				ProgressWindow->m_List->GetItem(itemcol1);
				itemcol2.m_itemId = i;
				itemcol2.m_col = 1;
				itemcol2.m_mask = wxLIST_MASK_TEXT;
				ProgressWindow->m_List->GetItem(itemcol2);
				file.AddLine(itemcol1.m_text + wxT("\t") + itemcol2.m_text);
			}
			file.Write();
		}
		delete MainWindow->m_BackupLocations;
		delete MainWindow->m_SecureLocations;
		wxGetApp().MainWindow->Destroy();
		wxGetApp().ProgressWindow->Destroy();
	}
	return true;
}

void Toucan::CleanTemp(){
	wxArrayString files;
	wxDir::GetAllFiles(GetSettingsPath(), &files, wxT("*.tmp"), wxDIR_FILES);
	for(unsigned int i = 0; i < files.GetCount(); i++){
		wxRemoveFile(files.Item(i));
	}
}

//Language setup
void Toucan::SetLanguage(wxString strLanguage){
	int LangCode = wxLocale::FindLanguageInfo(strLanguage)->Language;
	m_Locale = new wxLocale();
	m_Locale->Init(LangCode);
	m_Locale->AddCatalogLookupPathPrefix(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + _T("lang"));
	m_Locale->AddCatalog(wxT("toucan"));
}

//Cleanup
int Toucan::OnExit(){   
	CleanTemp();
	delete m_Locale;
	delete m_Settings;
	delete m_Script;
	return wxApp::OnExit();
}

void Toucan::RebuildForm(){
	MainWindow->Destroy();
	MainWindow = new frmMain();
	MainWindow->Show();
}
