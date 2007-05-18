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
#include "basicops.h"

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

	wxSingleInstanceChecker* m_checker = new wxSingleInstanceChecker(wxT("Toucan"));
	if ( m_checker->IsAnotherRunning() )
	{
		wxLogError(_("Another program instance is already running, aborting."));

		return false;
	} 
    
    wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxT("\\Data\\Jobs.ini") );
    wxFileConfig::Set( config );
    
    if(config->Read(cmdParser.GetParam(0) + wxT("/Type")) == _("Sync"))
    {
    
        wxString strToSplit = config->Read(cmdParser.GetParam(0) + wxT("/Exclusions"));
        wxStringTokenizer tkz(strToSplit, wxT("|"), wxTOKEN_STRTOK);
        wxString token;
        wxArrayString arrExclusions;
        while ( tkz.HasMoreTokens() )
        {
            token = tkz.GetNextToken();
            arrExclusions.Add(token);
        
        }
        wxString strAttribs = config->Read(cmdParser.GetParam(0) + wxT("/Attributes"));
        if(strAttribs == wxT("0"))
        {
            Sync(config->Read(cmdParser.GetParam(0) + wxT("/1")), config->Read(cmdParser.GetParam(0) + wxT("/2")), config->Read(cmdParser.GetParam(0) + wxT("/Function")), arrExclusions, false, false);
        }
        else if(strAttribs == wxT("1"))
        {
            Sync(config->Read(cmdParser.GetParam(0) + wxT("/1")), config->Read(cmdParser.GetParam(0) + wxT("/2")), config->Read(cmdParser.GetParam(0) + wxT("/Function")), arrExclusions, false, true);
        }
      
		return false;
    
    }
    else if(config->Read(cmdParser.GetParam(0) + wxT("/Type")) == _("Backup"))
    {
        wxString strToSplit = config->Read(cmdParser.GetParam(0) + wxT("/Exclusions"));
        wxStringTokenizer tkz(strToSplit, wxT("|"), wxTOKEN_STRTOK);
        wxString token;
        wxArrayString arrExclusions;
        while ( tkz.HasMoreTokens() )
        {
            token = tkz.GetNextToken();
            arrExclusions.Add(token);
        
        }
        //Clears up text file for new exclusions
        wxString strPath = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("App") + wxFILE_SEP_PATH + wxT("temp-exclusions.txt");
		PrepareTextFile(strPath);
		//Generate the exclusion file list
		GenerateExclusions(config->Read(cmdParser.GetParam(0) + wxT("/1")), arrExclusions, false);
		//Cut the beginnings off the files so that they are 7zip compatible
		CutStart(config->Read(cmdParser.GetParam(0) + wxT("/1")), false);
		//Run the backup
		Backup(config->Read(cmdParser.GetParam(0) + wxT("/1")), config->Read(cmdParser.GetParam(0) + wxT("/2")), config->Read(cmdParser.GetParam(0) + wxT("/Function")), config->Read(cmdParser.GetParam(0) + wxT("/Format")), config->Read(cmdParser.GetParam(0) + wxT("/Ratio")), false);
        return false;
    }
    else if(config->Read(cmdParser.GetParam(0) + wxT("/Type")) == _("Secure"))
    {
        wxString strToSplit = config->Read(cmdParser.GetParam(0) + wxT("/Files"));
        wxStringTokenizer tkz(strToSplit, wxT("|"), wxTOKEN_STRTOK);
        wxString token;
        wxArrayString arrFiles;
        while ( tkz.HasMoreTokens() )
        {
            token = tkz.GetNextToken();
            arrFiles.Add(token);
        
        }
        Secure(arrFiles, config->Read(cmdParser.GetParam(0) + wxT("/Function")), cmdParser.GetParam(1), false);
    return false;
   }
    
	wxInitAllImageHandlers();
	wxBitmap bitmap;
    frmMain* mainWindow = new frmMain(NULL, ID_FRMMAIN, wxT("Toucan"));
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
