/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include <wx/stdpaths.h>
#include <wx/splash.h>
#include <wx/intl.h>
#include <wx/fileconf.h>

#include "toucan.h"
#include "frmmain.h"
#include "backupprocess.h"
#include "cmdline.h"
#include "basicfunctions.h"
#include "script.h"
#include "settings.h"

//Because we actually have a console app that is well hidden!
IMPLEMENT_APP_NO_MAIN(Toucan)

//Toucan startup
bool Toucan::OnInit(){    
	if(argc == 1){
		blGUI = true;
	}
	else{
		blGUI = false;
	}
	
	//Make sure the data directory is there
	if(!wxDirExists(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Left(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Length() - 10) + wxT("Data"))){
		wxMkdir(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Left(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Length() - 10) + wxT("Data"));
	}
	SetSettingsPath(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Left(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Length() - 10) + wxT("Data") + wxFILE_SEP_PATH);
	
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
	m_Settings = new Settings();
	m_Settings->TransferFromFile();
	
	//Create the script manager
	m_Script = new ScriptManager();
	
	//Make sure the jobs file is up to date!
	int version = 1;
	m_Jobs_Config->Read(wxT("General/Version"), &version);
	m_Jobs_Config->Write(wxT("General/Version"), 202);
	m_Jobs_Config->Flush();
	if(version < 202){
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
	
	
	//Set the settings path
	SetLanguage(m_Settings->GetLanguageCode());
	
	//Set up the sizes and so forth
	int height, width, x, y;
	wxClientDisplayRect(&x, &y, &width, &height);

	wxPoint position((int)(m_Settings->GetX() * width), (int)(m_Settings->GetY() * height));
	wxSize size((int)(m_Settings->GetWidth() * width), (int)(m_Settings->GetHeight() * height));
	long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxMAXIMIZE|wxMINIMIZE_BOX|wxMAXIMIZE_BOX|wxCLOSE_BOX;
	
	wxInitAllImageHandlers();
	wxBitmap bitmap;
	MainWindow = new frmMain(NULL, ID_AUIFRAME, wxT("Toucan"), position, size, style);
			
	ProgressWindow = new frmProgress(NULL, ID_FRMPROGRESS, _("Progress"), wxDefaultPosition, wxSize(640, 480), wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxMAXIMIZE_BOX|wxMINIMIZE_BOX);
	if(blGUI){
		if(wxFileExists(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("Splash.jpg"))){
			bitmap.LoadFile(wxPathOnly(wxStandardPaths::Get().GetExecutablePath())  + wxFILE_SEP_PATH + wxT("splash.jpg"), wxBITMAP_TYPE_JPEG);
			wxSplashScreen *scrn = new wxSplashScreen(bitmap, wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_TIMEOUT, 5000, MainWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE|wxSTAY_ON_TOP|wxFRAME_NO_TASKBAR);
			wxYield();
			//Sleep for two seconds before destroying the splash screen and showing main frame
			wxSleep(2);
			//Now destroy the splashscreen
			scrn->Destroy(); 
		}
		if(m_Settings->GetWidth() < 1 && m_Settings->GetHeight() < 1){
			MainWindow->Iconize(false);
		}	
		MainWindow->Show();
	
	}
	else{
		ParseCommandLine();
		//We need to wait for the script manager to finish
		while(m_Script->GetCommand() <= m_Script->GetCount()){
			wxMilliSleep(200);
			wxYield();
			;
		}
		delete MainWindow->m_BackupLocations;
		delete MainWindow->m_SecureLocations;
		wxGetApp().MainWindow->Destroy();
		wxGetApp().ProgressWindow->Destroy();
	}
	return true;
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
	delete m_Locale;
	delete m_Settings;
	delete m_Script;
	return wxApp::OnExit();
}

int main(int argc, char* argv[]){
	wxEntry(argc,argv); 
	return true;
}


