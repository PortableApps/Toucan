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

#include "toucan.h"
#include "frmmain.h"
#include "backupprocess.h"
#include "cmdline.h"

class Settings;

//Because we actually have a consola app that is well hidden!
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
	SetSettingsPath(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Left(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Length() - 10) + wxT("Data") + wxFILE_SEP_PATH);
	
	m_Settings = new Settings();
	m_Settings->TransferFromFile();
	//Set the settings path
	SetLanguage(m_Settings->GetLanguageCode());
	wxInitAllImageHandlers();
	wxBitmap bitmap;
	MainWindow = new frmMain(NULL, ID_AUIFRAME);
			
	ProgressWindow = new frmProgress(NULL, ID_FRMPROGRESS, _("Progress"));
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
		if(MainWindow->GetSize() == wxSize(wxGetApp().m_Settings->GetWidth(), wxGetApp().m_Settings->GetHeight())){
			MainWindow->Maximize();
		}
		else{
			MainWindow->Iconize(false);
			MainWindow->SetSize(wxGetApp().m_Settings->GetWidth(), wxGetApp().m_Settings->GetHeight());
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
	return wxApp::OnExit();
}

int main(int argc, char* argv[])
{
	if(argc == 1){
		ShowWindow(GetConsoleWindow(), SW_HIDE); 
	}
	wxEntry(argc,argv); 
	return true;
}


