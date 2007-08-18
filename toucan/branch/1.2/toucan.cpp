/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2007 Steven Lamerton
// Licence:     GNU GPL 2 (See readme for more info
/////////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "toucan.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "toucan.h"
#include "sync.h"
#include "backup.h"
#include "secure.h"
//#include "remove-start.h"
//#include "exclusions.h"
#include "basicops.h"
#include "frmmain.h"
#include "backup-func.h"

//#include <wx/intl.h>
#include <wx/cmdline.h>
#include <wx/textfile.h>
#include <wx/stdpaths.h> 
#include <wx/snglinst.h>
#include <wx/splash.h>
#include <iostream> 

//declare the main wxWidgets application,
//but don't create an entry point for the application
IMPLEMENT_APP_NO_MAIN(Toucan)


//becyause I'm lazy
using namespace std; 

Toucan::Toucan()
{	////@begin toucan member initialisation
	////@end toucan member initialisation
}

/*
static const wxCmdLineEntryDesc cmdLineDesc[] =
{	{ wxCMD_LINE_PARAM,  NULL, NULL, wxT("Password"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
	{ wxCMD_LINE_PARAM,  NULL, NULL, wxT("File"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
	{ wxCMD_LINE_PARAM,  NULL, NULL, wxT("Action"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
	{ wxCMD_LINE_PARAM,  NULL, NULL, wxT("Function"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
	{ wxCMD_LINE_NONE }

};*/

/*!
* Initialisation for toucan
*/

bool Toucan::OnInit()
{    
    wxGetApp().SelectLanguage();
	wxInitAllImageHandlers();
	wxBitmap bitmap;
    frmMain* mainWindow = new frmMain(NULL, ID_FRMMAIN, wxT("Toucan"));
    if(::wxFileExists(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("App") + wxFILE_SEP_PATH + wxT("toucan") + wxFILE_SEP_PATH + wxT("bitmaps") + wxFILE_SEP_PATH + wxT("Toucan.jpg")))
    {
        bitmap.LoadFile(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("App") + wxFILE_SEP_PATH + wxT("toucan") + wxFILE_SEP_PATH + wxT("bitmaps") + wxFILE_SEP_PATH + wxT("Toucan.jpg"), wxBITMAP_TYPE_JPEG);
        wxSplashScreen *scrn = new wxSplashScreen(bitmap, wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_TIMEOUT, 5000, mainWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP);
        wxYield();
        //Sleep for two seconds before destroying the splash screen and showing main frame
        wxSleep(2);
        //Now destroy the splashscreen
        scrn->Destroy(); 
    }
	//delete splash;
    mainWindow->ShowModal();
    mainWindow->Destroy();
	// A modal dialog application should return false to terminate the app.
	return false;
}
void Toucan::SelectLanguage()
{	
	wxFileConfig *programconfig = new wxFileConfig( wxT(""), wxT(""), wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxT("\\Data\\Settings.ini") );
	wxFileConfig::Set( programconfig );
	int lang; 
    programconfig->Read(wxT("General/Language"), &lang);
	if(lang != wxLANGUAGE_ENGLISH){
	m_locale = new wxLocale(lang);
	m_locale->AddCatalogLookupPathPrefix(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("Data"));
	m_locale->AddCatalog(wxT("toucan"));
	}
}/*!
* Cleanup for toucan
*/
int Toucan::OnExit()
{    
	//delete m_locale ;
	////@begin toucan cleanup
	return wxApp::OnExit();
}
int main(int argc, char* argv[])
{
        printf("boo");
        cout << "HELLO!" << endl;
        cout << "Please make a choice:" << endl;
        cout << "1) Start the GUI" << endl;
        cout << "2) Print some text" << endl;
        cout << "3) Quit" << endl;

        int choice;
        cin >> choice;

        switch(choice)
        {
                case 1:
                        cout << "Starting interface..." << endl;
                        wxEntry(argc,argv);
                break;
                case 2:
                        cout << "Blablabla..." << endl;
                break;

                case 3:
                default:
                        cout << "Bye!" << endl;
                break;

        };

        return 0;
}
