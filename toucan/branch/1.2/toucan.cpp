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
#include <wx/wfstream.h>
#include <wx/cmdline.h>
#include <wx/textfile.h>
#include <wx/txtstrm.h>
#include <wx/stdpaths.h> 
#include <wx/snglinst.h>
#include <wx/splash.h>
#include <iostream>
#include <stdio.h>
using namespace std;

//declare the main wxWidgets application,
//but don't create an entry point for the application
IMPLEMENT_APP_NO_MAIN(Toucan)


//becyause I'm lazy
using namespace std; 

Toucan::Toucan()
{	////@begin toucan member initialisation
	////@end toucan member initialisation
}


static const wxCmdLineEntryDesc cmdLineDesc[] =
{	{ wxCMD_LINE_PARAM,  NULL, NULL, wxT("Password"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
	{ wxCMD_LINE_PARAM,  NULL, NULL, wxT("File"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
	{ wxCMD_LINE_PARAM,  NULL, NULL, wxT("Action"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
	{ wxCMD_LINE_PARAM,  NULL, NULL, wxT("Function"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
	{ wxCMD_LINE_NONE }

};

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
}
/*!
//* Cleanup for toucan
//*/
int Toucan::OnExit()
{    
	delete m_locale ;
	return wxApp::OnExit();
}

int main(int argc, char* argv[])
{


    //::wxPrintF(_("Test"));
    if(argc == 1){
        ShowWindow(GetConsoleWindow(), SW_HIDE ); 
        wxEntry(argc,argv); 
    }
    else{
        wxFFileOutputStream output( stderr );
        wxTextOutputStream cout( output );
        wxInitializer initializer; 
        cout << _("Welcome to the Toucan command line system.\n");
        
        wxCmdLineParser cmdParser(cmdLineDesc, argc, argv);
        int res;
        {
            wxLogNull log;
            // Pass false to suppress auto Usage() message
            res = cmdParser.Parse(false);
        }
        //Create new file config to read the jobs
        wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxT("\\Data\\Jobs.ini") );
        wxFileConfig::Set( config );
        if(config->Read(cmdParser.GetParam(0) + wxT("/Type")) == wxT("Sync")){
            //Split the exclusions using a string tokeniser
            wxString strToSplit = config->Read(cmdParser.GetParam(0) + wxT("/Exclusions"));
            wxStringTokenizer tkz(strToSplit, wxT("|"), wxTOKEN_STRTOK);
            wxString token;
            wxArrayString arrExclusions;
            while ( tkz.HasMoreTokens() ){
                token = tkz.GetNextToken();
                arrExclusions.Add(token);
            
            }
            if(Normalise(config->Read(cmdParser.GetParam(0) + wxT("/1"))) == wxEmptyString){
                cout<<_("There was an error processing the first portable variable");
                return false;
            }
            if(Normalise(config->Read(cmdParser.GetParam(0) + wxT("/2"))) == wxEmptyString){
                cout<<_("There was an error processing the second portable variable");
                return false;
            }            
            
            bool blAttribs = config->Read(cmdParser.GetParam(0) + wxT("/Attributes"));
            Sync(config->Read(cmdParser.GetParam(0) + wxT("/1")), config->Read(cmdParser.GetParam(0) + wxT("/2")), config->Read(cmdParser.GetParam(0) + wxT("/Function")), arrExclusions, false, blAttribs, false);
            return false;
        }
        else if(config->Read(cmdParser.GetParam(0) + wxT("/Type")) == wxT("Backup")){
            //Split the exclusions using a string tokeniser
            wxString strToSplit = config->Read(cmdParser.GetParam(0) + wxT("/Exclusions"));
            wxStringTokenizer tkz(strToSplit, wxT("|"), wxTOKEN_STRTOK);
            wxString token;
            wxArrayString arrExclusions;
            while ( tkz.HasMoreTokens() ){
                token = tkz.GetNextToken();
                arrExclusions.Add(token);
            
            }
            wxString strFirst = Normalise(config->Read(cmdParser.GetParam(0) + wxT("/1")));
            strFirst = Normalise(strFirst);
            if(strFirst == wxEmptyString){
                cout<<_("There was an error processing the second portable variable");
            }
            wxString strSecond = Normalise(config->Read(cmdParser.GetParam(0) + wxT("/2")));
            strSecond = Normalise(strSecond);
            if(strSecond == wxEmptyString){
                cout<<_("There was an error processing the second portable variable");
            }

            //Clears up text file for new exclusions
            wxString strPath = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("App") + wxFILE_SEP_PATH + wxT("temp-exclusions.txt");
            PrepareTextFile(strPath);
            //Generate the exclusion file list
            GenerateExclusions(strFirst, arrExclusions, false);
            //Cut the beginnings off the files so that they are 7zip compatible
            CutStart(strFirst, false);
            //Run the backup
            Backup(strFirst,strSecond, config->Read(cmdParser.GetParam(0) + wxT("/Function")), config->Read(cmdParser.GetParam(0) + wxT("/Format")), config->Read(cmdParser.GetParam(0) + wxT("/Ratio")), false, wxEmptyString);        
            //wxMessageBox(_("Returning Now"));
            return false;
        }
        else if(config->Read(cmdParser.GetParam(0) + wxT("/Type")) == wxT("Secure")){
            //Split the files and folders usig a string tokeniser
            wxString strToSplit = config->Read(cmdParser.GetParam(0) + wxT("/Files"));
            wxStringTokenizer tkz(strToSplit, wxT("|"), wxTOKEN_STRTOK);
            wxString token;
            wxArrayString arrFiles;
            while ( tkz.HasMoreTokens() ){
                token = tkz.GetNextToken();
                if(Normalise(token) != wxEmptyString){
                arrFiles.Add(token);
                }
                else{
                cout<< _("There was an error processing the following portable variable so it not be en/decrypted: ") + token;
                }
            }
            Secure(arrFiles, config->Read(cmdParser.GetParam(0) + wxT("/Function")), cmdParser.GetParam(1), false, wxT("AES"));
           // wxPrintF(_("Fired Secure"));
        return false;
       }
    }
    return 0;
}
