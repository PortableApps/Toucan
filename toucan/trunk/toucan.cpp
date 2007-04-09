/////////////////////////////////////////////////////////////////////////////
// Name:        toucan.cpp
// Purpose:     
// Author:      Steven Lamerton
// Modified by: 
// Created:     24/11/2006 14:30:47
// RCS-ID:      
// Copyright:   Copyright (C)  2006 Steven Lamerton
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "puss.h"
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
#include "remove-start.h"
#include "exclusions.h"

//#include <wx/intl.h>
#include <wx/cmdline.h>
#include <wx/textfile.h>
#include <wx/stdpaths.h> 
#include <wx/snglinst.h>
#include <wx/splash.h>

////@begin XPM images
////@end XPM images

/*!
* Application instance implementation
*/

////@begin implement app
IMPLEMENT_APP( Toucan )
////@end implement app

/*!
* PUSS type definition
*/

IMPLEMENT_CLASS( Toucan, wxApp )

/*!
* PUSS event table definition
*/

BEGIN_EVENT_TABLE( Toucan, wxApp )

////@begin PUSS event table entries
////@end PUSS event table entries

END_EVENT_TABLE()

/*!
* Constructor for PUSS
*/

Toucan::Toucan()
{	////@begin PUSS member initialisation
	////@end PUSS member initialisation
}static const wxCmdLineEntryDesc cmdLineDesc[] =
{	{ wxCMD_LINE_PARAM,  NULL, NULL, wxT("Password"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
	{ wxCMD_LINE_PARAM,  NULL, NULL, wxT("File"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
	{ wxCMD_LINE_PARAM,  NULL, NULL, wxT("Action"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
	{ wxCMD_LINE_PARAM,  NULL, NULL, wxT("Function"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
	{ wxCMD_LINE_NONE }

};

/*!
* Initialisation for PUSS
*/

bool Toucan::OnInit()
{    
	if(wxFileExists(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("Data") + wxFILE_SEP_PATH + wxT("fr") + wxFILE_SEP_PATH + wxT("puss.mo")))
	{
		//wxMessageBox(wxT("boo"));
		//Load language if one exists from puss.h
		SelectLanguage(wxLANGUAGE_FRENCH);
	}
	// m_locale = NULL ;
	// wxGetApp().SelectLanguage(wxLANGUAGE_FRENCH);
	//wxSystemOptions::SetOption(wxT("msw.remap"), 0); 
	wxCmdLineParser cmdParser(cmdLineDesc, argc, argv);
	//wxMessageBox(wxT("Parsing"));
	int res;
	{
		wxLogNull log;
		// Pass false to suppress auto Usage() message
		res = cmdParser.Parse(false);
	}
		// Check for a project filename
	if (cmdParser.GetParam(0).MakeLower() == wxT("secure"))
	{
		//wxMessageBox(cmdParser.GetParam(0));
		wxTextFile file;
		file.Open(cmdParser.GetParam(1));
		unsigned int i;
		wxArrayString arrFiles;
		for(i=0; i < file.GetLineCount();i++)
		{
			
			arrFiles.Add(file.GetLine(i));
			
		}
		Secure(arrFiles, cmdParser.GetParam(2), cmdParser.GetParam(3), false);
		return false;
		
	}
	if (cmdParser.GetParam(0).MakeLower() == wxT("sync"))
	{
		//wxMessageBox(cmdParser.GetParam(0));
		wxTextFile file;
		wxArrayString arrExclusions;
		file.Open(cmdParser.GetParam(1));
		if(file.GetLineCount() >= 3)
		{
			unsigned int i;
			for(i = 3;i < file.GetLineCount(); i++)
			{
				arrExclusions.Add(file.GetLine(i));
			}
			
		}
		Sync(file.GetLine(0), file.GetLine(1), file.GetLine(2), arrExclusions, false);
		return false;
		
	}
		if (cmdParser.GetParam(0).MakeLower() == wxT("backup"))
	{
		wxTextFile file;
		file.Open(cmdParser.GetParam(1));
		wxArrayString arrExclusions;
		if(file.GetLineCount() >= 5)
		{
			unsigned int i;
			for(i = 5;i < file.GetLineCount(); i++)
			{
				//wxMessageBox(file.GetLine(i));
				arrExclusions.Add(file.GetLine(i));
			}
			
		}
		wxString strPath = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("App") + wxFILE_SEP_PATH + wxT("temp-exclusions.txt");
		wxTextFile file2;
		if(wxFile::Exists(strPath))
		{	
			file2.Open(strPath);
			file2.Clear();
			file2.Write();
		}
		else
		{
			file2.Create(strPath);
		}
		GenerateExclusions(file.GetLine(0), arrExclusions, false);
		CutStart(file.GetLine(0), false);
		Backup(file.GetLine(0), file.GetLine(1), file.GetLine(2),file.GetLine(3),file.GetLine(4), false);
		return false;
	}

	wxSingleInstanceChecker* m_checker = new wxSingleInstanceChecker(wxT("Toucan"));
	if ( m_checker->IsAnotherRunning() )
	{
		wxLogError(_("Another program instance is already running, aborting."));

		return false;
	} 
	wxInitAllImageHandlers();
	wxBitmap bitmap;
        frmMain* mainWindow = new frmMain(NULL, ID_DIALOG_MAIN, wxT("Toucan"));
    if(::wxFileExists(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("App") + wxFILE_SEP_PATH + wxT("bitmaps") + wxFILE_SEP_PATH + wxT("Toucan.jpg")))
    {
   // wxMessageBox(_("Boo2"));
	bitmap.LoadFile(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("App") + wxFILE_SEP_PATH + wxT("bitmaps") + wxFILE_SEP_PATH + wxT("Toucan.jpg"), wxBITMAP_TYPE_JPEG);
 

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

	return true;
}
void Toucan::SelectLanguage(int lang)
{	m_locale = new wxLocale(wxLANGUAGE_FRENCH);
	m_locale->AddCatalogLookupPathPrefix(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("Data"));
	m_locale->AddCatalog(wxT("toucan"));
}/*!
* Cleanup for PUSS
*/
int Toucan::OnExit()
{    
	//delete m_locale ;
	////@begin PUSS cleanup
	return wxApp::OnExit();
}
