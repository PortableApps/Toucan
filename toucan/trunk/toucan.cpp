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


#include "signalprocess.h"
#include "backup/backupprocess.h"

#ifdef __WXMSW__
	#define _WIN32_WINNT 0x0500 
	#include <windows.h>
	#include <tlhelp32.h>
	#include <process.h>
	#include <wx/msw/winundef.h>
#endif

#include "toucan.h"
#include "forms/frmmain.h"
#include "forms/frmprogress.h"
#include "cmdline.h"
#include "basicfunctions.h"
#include "script.h"
#include "settings.h"
#include "luamanager.h"
#include "data/syncdata.h"
#include "data/backupdata.h"
#include "data/securedata.h"

IMPLEMENT_APP_NO_MAIN(Toucan)

IMPLEMENT_CLASS(Toucan, wxApp)

BEGIN_EVENT_TABLE(Toucan, wxApp)
	EVT_COMMAND(ID_PROGRESS, wxEVT_COMMAND_BUTTON_CLICKED, Toucan::OnProgress)
	EVT_COMMAND(ID_BACKUPPROCESS, wxEVT_COMMAND_BUTTON_CLICKED, Toucan::OnBackupProcess)
	EVT_COMMAND(ID_PROCESS, wxEVT_COMMAND_BUTTON_CLICKED, Toucan::OnProcess)
	EVT_COMMAND(ID_FINISH, wxEVT_COMMAND_BUTTON_CLICKED, Toucan::OnFinish)
END_EVENT_TABLE()

int main(int argc, char *argv[]){
	wxEntry(argc, argv);
}

//Toucan startup
bool Toucan::OnInit(){
	#ifdef __WXMSW__
		if(argc == 1){
			ShowWindow(GetConsoleWindow(), SW_HIDE);
		}
	#endif

 	//Set the splash screen going
	wxInitAllImageHandlers();
	wxSplashScreen *scrn = NULL;
	if(wxFileExists(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("splash.jpg")) && argc == 1){
		wxBitmap bitmap;
		bitmap.LoadFile(wxPathOnly(wxStandardPaths::Get().GetExecutablePath())  + wxFILE_SEP_PATH + wxT("splash.jpg"), wxBITMAP_TYPE_JPEG);
		scrn = new wxSplashScreen(bitmap, wxSPLASH_CENTRE_ON_PARENT|wxSPLASH_NO_TIMEOUT, 5000, NULL, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE|wxSTAY_ON_TOP|wxFRAME_NO_TASKBAR);
	}

	m_Abort = false;
	//Are we in gui mode?
	m_UsesGUI = argc == 1 ? true : false;

	//Work out where the settings dir is. Firstly get the exe dir
	wxFileName settingspath = wxFileName::DirName((wxPathOnly(wxStandardPaths::Get().GetExecutablePath())));
	//Next remove the \App\toucan
	settingspath.RemoveLastDir();
	settingspath.RemoveLastDir();
	//And the add \Data
	settingspath.AppendDir(wxT("Data"));

	m_SettingsPath = settingspath.GetFullPath();
	//Make sure the data directory is there
	if(!wxDirExists(GetSettingsPath())){
		wxMkdir(GetSettingsPath());
	}

	//Collect the dirve letters
	#ifdef __WXMSW__
		TCHAR drives[256];  
		if(GetLogicalDriveStrings(256, drives)){  
			LPTSTR drive = drives;  
			while(*drive){  
				wxString volumename = wxEmptyString;
				TCHAR label[256]; 
				if(GetVolumeInformation(drive, label, sizeof(label), NULL, 0, NULL, NULL, 0)){
					volumename.Printf(wxT("%s"),label); 
					if(volumename != wxEmptyString){
						m_DriveLabels[volumename] = wxString(drive).Left(2);
					}
				}
				int offset = _tcslen(drive) + 1;  
				drive += offset;  
			}
		}
	#endif

	//Create the config stuff and set it up
 	m_Jobs_Config = new wxFileConfig(wxT(""), wxT(""), wxGetApp().GetSettingsPath() + wxT("Jobs.ini"));
	m_Rules_Config = new wxFileConfig(wxT(""), wxT(""), wxGetApp().GetSettingsPath() + wxT("Rules.ini"));
	m_Scripts_Config = new wxFileConfig(wxT(""), wxT(""), wxGetApp().GetSettingsPath() + wxT("Scripts.ini"));
	m_Variables_Config = new wxFileConfig(wxT(""), wxT(""), wxGetApp().GetSettingsPath() + wxT("Variables.ini"));

	m_Jobs_Config->SetExpandEnvVars(false);
	m_Rules_Config->SetExpandEnvVars(false);
	m_Scripts_Config->SetExpandEnvVars(false);
	m_Variables_Config->SetExpandEnvVars(false);

	//Set the language and init the maps
 	wxFileConfig *settings = new wxFileConfig(wxT(""), wxT(""), GetSettingsPath() + wxT("Settings.ini"));
	SetLanguage(settings->Read(wxT("General/LanguageCode"), wxT("en")));
	delete settings;
	InitLangMaps();

	//Make sure the jobs file is up to date!
	int version = 1;
	m_Jobs_Config->Read(wxT("General/Version"), &version);
	if(version < 213){
		m_Jobs_Config->Write(wxT("General/Version"), 213);
		m_Jobs_Config->Flush();
		UpdateJobs(version);
	}

	//Update the script file
	version = 1;
	m_Scripts_Config->Read(wxT("General/Version"), &version);
	if(version < 202){
		m_Scripts_Config->Write(wxT("General/Version"), 202);
		m_Scripts_Config->Flush();
		UpdateRules(version);
	}

	//Update the rules file
	version = 1;
	m_Rules_Config->Read(wxT("General/Version"), &version);
	if(version < 202){
		m_Rules_Config->Write(wxT("General/Version"), 202);
		m_Rules_Config->Flush();
		UpdateScripts(version);
	}	
	
	//Update the settings file
	version = 1;
	m_Rules_Config->Read(wxT("General/Version"), &version);
	if(version < 213){
		m_Rules_Config->Write(wxT("General/Version"), 213);
		m_Rules_Config->Flush();
		UpdateSettings(version);
	}

	//Create the settings class amd load the settings
	m_Settings = new Settings(GetSettingsPath() + wxT("Settings.ini"));
	m_Settings->TransferFromFile();

	//Create the lua manager
	m_LuaManager = new LuaManager();

	//Set up the help system
	m_Help = new wxHtmlHelpController(wxHF_DEFAULT_STYLE|wxHF_EMBEDDED, MainWindow);

	wxFileSystem::AddHandler(new wxArchiveFSHandler);

	MainWindow = new frmMain();

	if(GetUsesGUI()){
		MainWindow->Show();
		if(m_Settings->GetWidth() < 1 && m_Settings->GetHeight() < 1){
			MainWindow->Maximize(false);
		}
		else{
			MainWindow->Maximize(true);
		}
		if(scrn != NULL){
			scrn->Destroy(); 
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
/*			for(int i = 0; i < ProgressWindow->m_List->GetItemCount(); i++){
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
			file.Write();*/
		}
		wxGetApp().MainWindow->Destroy();
	}
	return true;
}

void Toucan::CleanTemp(){
	wxArrayString files;
	wxDir::GetAllFiles(GetSettingsPath(), &files, wxT("*.tmp"), wxDIR_FILES);
	for(unsigned int i = 0; i < files.GetCount(); i++){
		wxRemoveFile(files.Item(i));
	}
	if(wxFileExists(wxGetApp().GetSettingsPath() + wxT("Includes.txt"))){
		wxRemoveFile(wxGetApp().GetSettingsPath() + wxT("Includes.txt"));
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
	KillConime();
	CleanTemp();
	delete m_Locale;
	delete m_Settings;
	delete m_Jobs_Config;
	delete m_Rules_Config;
	delete m_Scripts_Config;
	delete m_Variables_Config;
	return wxApp::OnExit();
}

void Toucan::RebuildForm(){
	MainWindow->Destroy();
	SetLanguage(m_Settings->GetLanguageCode());
	InitLangMaps();
	m_Settings->TransferFromFile();
	MainWindow = new frmMain();
	MainWindow->Show();
	if(m_Settings->GetWidth() < 1 && m_Settings->GetHeight() < 1){
		MainWindow->Maximize(false);
	}	
	else{
		MainWindow->Maximize(true);
	}
}

void Toucan::KillConime(){
	#ifdef __WXMSW__
		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if(snapshot != INVALID_HANDLE_VALUE){
			unsigned int toucanpid = _getpid();
			PROCESSENTRY32 pe;
			pe.dwSize = sizeof(PROCESSENTRY32);

			if(!Process32First(snapshot, &pe)){
				CloseHandle(snapshot);
				return;
			}

			do{
				//check if it is a conime process
				if(wcsncmp(pe.szExeFile, wxT("conime.exe"), sizeof(pe.szExeFile)) || wcsncmp(pe.szExeFile, wxT("conime"), sizeof(pe.szExeFile))){
					//then if toucan is its parent process
					if(pe.th32ParentProcessID == toucanpid){
						//then kill it
						wxProcess::Kill(pe.th32ProcessID, wxSIGKILL);
					}
				}
			}while(Process32Next(snapshot, &pe));
			  
			CloseHandle(snapshot);
		}
		
	#endif
}

void Toucan::OnProgress(wxCommandEvent &event){
	if(m_UsesGUI){
		frmProgress *window = m_LuaManager->GetProgressWindow();
		if(window){
			long index = window->m_List->InsertItem(window->m_List->GetItemCount(), wxEmptyString);
			window->m_List->SetItem(index, 1, event.GetString());
			if(event.GetInt() == 1){
				window->m_List->SetItem(index, 0, wxDateTime::Now().FormatISOTime());
				window->m_List->SetItemTextColour(index, wxColour(wxT("Red")));
			}
			else if(event.GetInt() == 2){
				window->m_List->SetItemTextColour(index, wxColour(wxT("Red")));
			}
			else if(event.GetInt() == 3){
				window->m_List->SetItem(index, 0, wxDateTime::Now().FormatISOTime());
			}
			window->m_List->EnsureVisible(index);
			window->Update();
		}
	}
	//TODO : Command line output
	else{

	}
}

void Toucan::OnProcess(wxCommandEvent &event){
	m_ProcessMap[event.GetInt()] = false;
	SignalProcess *process = new SignalProcess(event.GetInt());
	wxExecute(event.GetString(), wxEXEC_ASYNC|wxEXEC_NODISABLE, process);
}

void Toucan::OnBackupProcess(wxCommandEvent &event){
	m_ProcessMap[event.GetInt()] = false;
	wxExecute(event.GetString(), wxEXEC_ASYNC|wxEXEC_NODISABLE, static_cast<BackupProcess*>(event.GetEventObject()));
}

void Toucan::OnFinish(wxCommandEvent &WXUNUSED(event)){
	m_LuaManager->CleanUp();
}

void Toucan::InitLangMaps(){
	m_EnToLang.clear();
	m_LangToEn.clear();
	
	//Sync
	m_EnToLang[wxT("Copy")] = _("Copy");
	m_EnToLang[wxT("Update")] = _("Update");
	m_EnToLang[wxT("Mirror")] = _("Mirror");
	m_EnToLang[wxT("Equalise")] = _("Equalise");
	m_EnToLang[wxT("Move")] = _("Move");
	m_EnToLang[wxT("Clean")] = _("Clean");
	//Backup
	m_EnToLang[wxT("Complete")] = _("Complete");
	m_EnToLang[wxT("Update")] = _("Update");
	m_EnToLang[wxT("Differential")] = _("Differential");
	m_EnToLang[wxT("Restore")] = _("Restore");
	//Secure
	m_EnToLang[wxT("Encrypt")] = _("Encrypt");
	m_EnToLang[wxT("Decrypt")] = _("Decrypt");
	//Settings
	m_EnToLang[wxT("Icons and Text")] = _("Icons and Text");
	m_EnToLang[wxT("Text")] = _("Text");
	//Tab Labels
	m_EnToLang[wxT("Sync")] = _("Sync");
	m_EnToLang[wxT("Backup")] = _("Backup");
	m_EnToLang[wxT("Secure")] = _("Secure");
	m_EnToLang[wxT("Rules")] = _("Rules");
	m_EnToLang[wxT("Variables")] = _("Variables");
	m_EnToLang[wxT("Script")] = _("Script");
	m_EnToLang[wxT("Settings")] = _("Settings");
	m_EnToLang[wxT("Help")] = _("Help");
	
	//Sync
	m_LangToEn[_("Copy")] = wxT("Copy");
	m_LangToEn[_("Update")] = wxT("Update");
	m_LangToEn[_("Mirror")] = wxT("Mirror");
	m_LangToEn[_("Equalise")] = wxT("Eualise");
	m_LangToEn[_("Move")] = wxT("Move");
	m_LangToEn[_("Clean")] = wxT("Clean");
	//Backup
	m_LangToEn[_("Complete")] = wxT("Complete");
	m_LangToEn[_("Update")] = wxT("Update");
	m_LangToEn[_("Differential")] = wxT("Differential");
	m_LangToEn[_("Restore")] = wxT("Restore");
	//Secure
	m_LangToEn[_("Encrypt")] = wxT("Encrypt");
	m_LangToEn[_("Decrypt")] = wxT("Decrypt");
	//Settings
	m_LangToEn[_("Icons and Text")] = wxT("Icons and Text");
	m_LangToEn[_("Text")] = wxT("Text");
	//Tab Labels
	m_LangToEn[_("Sync")] = wxT("Sync");
	m_LangToEn[_("Backup")] = wxT("Backup");
	m_LangToEn[_("Secure")] = wxT("Secure");
	m_LangToEn[_("Rules")] = wxT("Rules");
	m_LangToEn[_("Variables")] = wxT("Variables");
	m_LangToEn[_("Script")] = wxT("Script");
	m_LangToEn[_("Settings")] = wxT("Settings");
	m_LangToEn[_("Help")] = wxT("Help");
}
