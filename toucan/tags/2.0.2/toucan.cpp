/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include <wx/stdpaths.h>
#include <wx/list.h>
#include <wx/listimpl.cpp> 
#include <wx/snglinst.h>
#include <wx/splash.h>
#include <wx/intl.h>
#include <wx/fileconf.h>

#include "toucan.h"
#include "frmmain.h"
#include "backupprocess.h"
#include "cmdline.h"
#include "basicfunctions.h"

class Settings;
class ScriptManager;

//Because we actually have a console app that is well hidden!
IMPLEMENT_APP_NO_MAIN(Toucan)

/*!
* Constructor for Toucan
*/

Toucan::Toucan()
{
	Init();
}


/*!
* Member initialisation
*/

void Toucan::Init()
{
	blAbort = false;
}

/*!
* Initialisation for Toucan
*/

bool Toucan::OnInit()
{    
	
	if(argc == 1){
		blGUI = true;
	}
	else{
		blGUI = false;
	}
	if(!wxDirExists(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Left(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Length() - 10) + wxT("Data"))){
		wxMkdir(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Left(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Length() - 10) + wxT("Data"));
	}
	SetSettingsPath(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Left(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Length() - 10) + wxT("Data") + wxFILE_SEP_PATH);
	
	m_Settings = new Settings();
	m_Settings->TransferFromFile();
	
	m_Script = new ScriptManager();
	
	//Make a backup of the files
	{
		wxLogNull null;
		wxCopyFile(wxGetApp().GetSettingsPath() + wxT("Jobs.ini"), wxGetApp().GetSettingsPath() + wxT("Jobs.old"), true);
		wxCopyFile(wxGetApp().GetSettingsPath() + wxT("Rules.ini"), wxGetApp().GetSettingsPath() + wxT("Rules.old"), true);
		wxCopyFile(wxGetApp().GetSettingsPath() + wxT("Scripts.ini"), wxGetApp().GetSettingsPath() + wxT("Scripts.old"), true);
	}
	
	//Make sure the jobs file is up to date!
	wxFileConfig *jobconfig = new wxFileConfig(wxT(""), wxT(""), wxGetApp().GetSettingsPath() + wxT("Jobs.ini"));
	int version = 1;
	jobconfig->Read(wxT("General/Version"), &version);
	jobconfig->Write(wxT("General/Version"), 202);
	delete jobconfig;
	if(version < 202){
		UpdateJobs(version);
	}	
	
	//Update the script file
	wxFileConfig *scriptconfig = new wxFileConfig(wxT(""), wxT(""), wxGetApp().GetSettingsPath() + wxT("Rules.ini"));
	version = 1;
	scriptconfig->Read(wxT("General/Version"), &version);
	scriptconfig->Write(wxT("General/Version"), 202);
	delete scriptconfig;
	if(version < 202){
		UpdateRules(version);
	}
	
	
	//Update the rules file
	wxFileConfig *rulesconfig = new wxFileConfig(wxT(""), wxT(""), wxGetApp().GetSettingsPath() + wxT("Scripts.ini"));
	version = 1;
	rulesconfig->Read(wxT("General/Version"), &version);
	rulesconfig->Write(wxT("General/Version"), 202);
	delete rulesconfig;
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
		delete MainWindow->m_BackupLocations;
		delete MainWindow->m_SecureLocations;
		wxGetApp().MainWindow->Destroy();
		wxGetApp().ProgressWindow->Destroy();
	}
	return true;
}

void Toucan::SetLanguage(wxString strLanguage){
	int LangCode = wxLocale::FindLanguageInfo(strLanguage)->Language;
	m_Locale = new wxLocale();
	m_Locale->Init(LangCode);
	m_Locale->AddCatalogLookupPathPrefix(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + _T("lang"));
	m_Locale->AddCatalog(wxT("toucan"));
}

/*!
* Cleanup for Toucan
*/

int Toucan::OnExit()
{    
	delete m_Locale;
	delete m_Settings;
	delete m_Script;
	return wxApp::OnExit();
}

int main(int argc, char* argv[])
{
	#ifdef __WXMSW__
	if(argc == 1){
		ShowWindow(GetConsoleWindow(), SW_HIDE); 
	}
	#endif
	wxEntry(argc,argv); 
	return true;
}


