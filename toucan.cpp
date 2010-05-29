/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006 - 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include <wx/stdpaths.h>
#include <wx/splash.h>
#include <wx/intl.h>
#include <wx/fileconf.h>
#include <wx/listctrl.h>
#include <wx/dir.h>
#include <wx/log.h>
#include <wx/gauge.h>
#include <wx/cmdline.h>
#include <wx/image.h> 
#include <cxxtest/ErrorPrinter.h>

#ifdef __MINGW32__
#define _WIN32_WINNT 0x0500
#endif

#ifdef __WXMSW__
	#include <windows.h>
	#include <tlhelp32.h>
	#include <process.h>
	#include <wx/msw/winundef.h>
#endif

#include "test.h"
#include "toucan.h"
#include "script.h"
#include "settings.h"
#include "luamanager.h"
#include "signalprocess.h"
#include "basicfunctions.h"
#include "forms/frmmain.h"
#include "backup/backupprocess.h"
#include "secure/secureprocess.h"
#include "forms/frmprogress.h"
#include "forms/frmpassword.h"

IMPLEMENT_APP_NO_MAIN(Toucan)

IMPLEMENT_CLASS(Toucan, wxApp)

BEGIN_EVENT_TABLE(Toucan, wxApp)
	EVT_COMMAND(ID_OUTPUT, wxEVT_COMMAND_BUTTON_CLICKED, Toucan::OnOutput)
	EVT_COMMAND(ID_FINISH, wxEVT_COMMAND_BUTTON_CLICKED, Toucan::OnFinish)
	EVT_COMMAND(ID_PROCESS, wxEVT_COMMAND_BUTTON_CLICKED, Toucan::OnProcess)
	EVT_COMMAND(ID_BACKUPPROCESS, wxEVT_COMMAND_BUTTON_CLICKED, Toucan::OnBackupProcess)
	EVT_COMMAND(ID_SECUREPROCESS, wxEVT_COMMAND_BUTTON_CLICKED, Toucan::OnSecureProcess)
	EVT_COMMAND(ID_GETPASSWORD, wxEVT_COMMAND_BUTTON_CLICKED, Toucan::OnGetPassword)
	EVT_COMMAND(ID_PROGRESS, wxEVT_COMMAND_BUTTON_CLICKED, Toucan::OnProgress)
	EVT_COMMAND(ID_PROGRESSSETUP, wxEVT_COMMAND_BUTTON_CLICKED, Toucan::OnProgressSetup)
END_EVENT_TABLE()

int main(int argc, char *argv[]){
	wxEntry(argc, argv);
}

//Toucan startup
bool Toucan::OnInit(){
	static const wxCmdLineEntryDesc desc[] =
	{
		{wxCMD_LINE_SWITCH, "h", "disablesplash", "Disables the splashscreen"},
		{wxCMD_LINE_SWITCH, "u", "unittests", "Runs the unittests"},
		{wxCMD_LINE_OPTION, "d", "datadirectory", "Location of the Data folder", wxCMD_LINE_VAL_STRING},
		{wxCMD_LINE_OPTION, "s", "script", "Script to run", wxCMD_LINE_VAL_STRING},
		{wxCMD_LINE_OPTION, "l", "logfile", "Path to save log", wxCMD_LINE_VAL_STRING},
		{wxCMD_LINE_OPTION, "j", "job", "Job to run", wxCMD_LINE_VAL_STRING},
		{wxCMD_LINE_NONE}
	};
	wxCmdLineParser parser(desc, argc, argv);

	wxMessageOutput *old = wxMessageOutput::Set(new wxMessageOutputStderr);
	if(parser.Parse() > 0){
		return false;
	}
	delete wxMessageOutput::Set(old);

	//If no script is found then we are in gui mode
	if(!parser.Found("script") && !parser.Found("job") && !parser.Found("unittests")){
		m_IsGui = true;
		#ifdef __WXMSW__
			ShowWindow(GetConsoleWindow(), SW_HIDE);
		#endif
	}
	else{
		m_IsGui = false;
	}

	const wxString exedir = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH;

	wxSplashScreen *scrn = NULL;
	wxInitAllImageHandlers();

	if(m_IsGui && !parser.Found("disablesplash")){
		if(wxFileExists(exedir + wxT("splash.jpg"))){
			wxBitmap bitmap;
			bitmap.LoadFile(exedir + wxT("splash.jpg"), wxBITMAP_TYPE_JPEG);
			scrn = new wxSplashScreen(bitmap, wxSPLASH_CENTRE_ON_PARENT|wxSPLASH_NO_TIMEOUT, 5000, NULL, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE|wxSTAY_ON_TOP|wxFRAME_NO_TASKBAR);
		}
	}

	m_Abort = false;
	m_IsLogging = false;
	m_LogFile = NULL;

	//Set the read only flag if needed
	wxTextFile writetest(exedir + wxT("writetest"));
	{
		wxLogNull null;
		m_IsReadOnly = !writetest.Create();
		wxRemoveFile(exedir + wxT("writetest"));
	}

	if(parser.Found("datadirectory")){
		wxString temp;
		parser.Found("datadirectory", &temp);
		wxFileName name(temp);
		name.Normalize();
		m_SettingsPath = name.GetFullPath();
	}
	else{
		//Work out where the settings dir is. Firstly get the exe dir
		wxFileName settingspath = wxFileName::DirName(exedir);
		//Next remove the \App\toucan
		settingspath.RemoveLastDir();
		settingspath.RemoveLastDir();
		//And the add \Data
		settingspath.AppendDir(wxT("Data"));
		m_SettingsPath = settingspath.GetFullPath();

	}
	//Make sure the data directory is there
	if(!wxDirExists(GetSettingsPath())){
		wxMkdir(GetSettingsPath());
	}

	//Create the config stuff and set it up
 	m_Jobs_Config = new wxFileConfig(wxT(""), wxT(""), m_SettingsPath + wxT("Jobs.ini"));
	m_Rules_Config = new wxFileConfig(wxT(""), wxT(""), m_SettingsPath + wxT("Rules.ini"));
	m_Scripts_Config = new wxFileConfig(wxT(""), wxT(""), m_SettingsPath + wxT("Scripts.ini"));
	m_Variables_Config = new wxFileConfig(wxT(""), wxT(""), m_SettingsPath + wxT("Variables.ini"));

	m_Jobs_Config->SetExpandEnvVars(false);
	m_Rules_Config->SetExpandEnvVars(false);
	m_Scripts_Config->SetExpandEnvVars(false);
	m_Variables_Config->SetExpandEnvVars(false);

	//Set the language and init the maps
 	wxFileConfig *settings = new wxFileConfig(wxT(""), wxT(""), m_SettingsPath + wxT("Settings.ini"));
	SetLanguage(settings->Read(wxT("General/LanguageCode"), wxT("en")));
	delete settings;
	InitLangMaps();

	if(!UpdateJobs() || !UpdateRules() /*|| !UpdateScripts()*/ || !UpdateSettings()){
		return false;
	}

	//Create the settings class and load the settings
	m_Settings = new Settings(GetSettingsPath() + wxT("Settings.ini"));
	m_Settings->TransferFromFile();

	//Create the lua manager
	m_LuaManager = new LuaManager();

	if(parser.Found("logfile")){
		m_IsLogging = true;
		wxString path;
		parser.Found("logfile", &path);
        Path::Normalise(path);
		m_LogFile = new wxTextFile(path);
		if(wxFileExists(path)){
			m_LogFile->Open();
			m_LogFile->Clear();
		}
		else{
			m_LogFile->Create();
		}
	}

	//Run the unit tests if needed and then exit
	if(parser.Found("unittests")){
		CxxTest::ErrorPrinter().run();
		exit(0);
	}

	if(m_IsGui){
		//Set up the main form
		MainWindow = new frmMain();
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
		if(parser.Found("script")){
			wxString script;
			parser.Found("script", &script);
			m_LuaManager->Run(script);
		}
		else if(parser.Found("job")){
			wxString name;
			parser.Found("job", &name);
			if(m_Jobs_Config->Exists(name)){
				wxString type = m_Jobs_Config->Read(name + "/Type");
				m_LuaManager->Run(type.Lower() + "([[" + name + "]])");
			}
			else{
				OutputProgress(_("The job does not exist"), true, true);
			}
		}
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
void Toucan::SetLanguage(const wxString &lang){
	if(wxLocale::FindLanguageInfo(lang)){
		m_Locale = new wxLocale();
		m_Locale->Init(wxLocale::FindLanguageInfo(lang)->Language);
		m_Locale->AddCatalogLookupPathPrefix(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + "lang");
		m_Locale->AddCatalog("toucan");
	}
	else{
		m_Locale = NULL;
	}
}

//Cleanup
int Toucan::OnExit(){
	if(m_IsLogging){
		m_LogFile->Write();
	}
	KillConime();
	CleanTemp();
	delete m_LogFile;
	delete m_Locale;
	delete m_Settings;
	//Deletion causes a flush which warns on read only devices
	if(!wxGetApp().IsReadOnly()){
		delete m_Jobs_Config;
		delete m_Rules_Config;
		delete m_Scripts_Config;
		delete m_Variables_Config;
	}
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
			size_t length = wxStrnlen(pe.szExeFile, 10);
			//check if it is a conime process
			if(wcsncmp(pe.szExeFile, wxT("conime.exe"), length) == 0 || wcsncmp(pe.szExeFile, wxT("conime"), length) == 0){
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

void Toucan::OnOutput(wxCommandEvent &event){
	if(m_IsLogging){
		wxString line = event.GetString();
		if(event.GetInt() == 1 || event.GetInt() == 3){
			line << "    " << wxDateTime::Now().FormatISOTime();
		}
		m_LogFile->AddLine(line);
	}
	if(m_IsGui){
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
	else{
		std::cout << event.GetString();
		if(event.GetInt() == 1 || event.GetInt() == 3){
			std::cout << "    " << wxDateTime::Now().FormatISOTime();
		}
		std::cout << std::endl;
	}
}

void Toucan::OnProcess(wxCommandEvent &event){
	m_StatusMap[event.GetInt()] = false;
	SignalProcess *process = new SignalProcess(event.GetInt());
	long pid = wxExecute(event.GetString(), wxEXEC_ASYNC|wxEXEC_NODISABLE, process);
	if(pid == 0){
		//We have an error
		OutputProgress(_("Could not run ") + event.GetString());
		m_StatusMap[event.GetInt()] = true;
	}
}

void Toucan::OnBackupProcess(wxCommandEvent &event){
	m_StatusMap[event.GetInt()] = false;
	long pid = wxExecute(event.GetString(), wxEXEC_ASYNC|wxEXEC_NODISABLE, static_cast<BackupProcess*>(event.GetEventObject()));
	static_cast<BackupProcess*>(event.GetEventObject())->SetRealPid(pid);
	if(pid == 0){
		//We have an error
		OutputProgress(_("Could not run ") + event.GetString());
		m_StatusMap[event.GetInt()] = true;
	}
}

void Toucan::OnSecureProcess(wxCommandEvent &event){
	m_StatusMap[event.GetInt()] = false;
	wxSetWorkingDirectory(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()));
	SecureProcess *process = new SecureProcess(event.GetInt());
	long pid = wxExecute(event.GetString(), wxEXEC_ASYNC|wxEXEC_NODISABLE, process);
	if(pid == 0){
		//We have an error
		OutputProgress(_("Could not run ") + event.GetString());
		m_StatusMap[event.GetInt()] = true;
	}
}

void Toucan::OnFinish(wxCommandEvent &WXUNUSED(event)){
	m_LuaManager->CleanUp();
	if(m_IsGui){
		wxCommandEvent event;
		MainWindow->OnSyncRefresh(event);
		MainWindow->OnBackupRefresh(event);
		MainWindow->OnSecureRefresh(event);
	}
	else{
		OnExit();
		exit(0);
	}
}

void Toucan::OnGetPassword(wxCommandEvent &event){
	m_StatusMap[event.GetInt()] = false;
	if(m_IsGui){
		frmPassword password(m_LuaManager->GetProgressWindow());
		if(password.ShowModal() == wxID_OK){
			m_Password = password.GetPassword();
			m_StatusMap[event.GetInt()] = true;
			return;
		}
	}
	else{
		std::string password, repeated;
		std::cout << _("Please input your password: ");
		std::getline(std::cin, password);
		std::cout << _("Please repeat your password: ");
		std::getline(std::cin, repeated);
		if(password != repeated){
			std::cout << _("Sorry the passwords do not match");
			m_Password = wxEmptyString;
			m_StatusMap[event.GetInt()] = true;
			return;
		}
		m_Password = wxString(password.c_str(), wxConvUTF8); 
	}
	m_StatusMap[event.GetInt()] = true;
}

void Toucan::OnProgressSetup(wxCommandEvent &event){
	frmProgress *window = m_LuaManager->GetProgressWindow();
	if(window){
		window->m_Gauge->SetRange(event.GetInt());
	}
}

void Toucan::OnProgress(wxCommandEvent &WXUNUSED(event)){
	frmProgress *window = m_LuaManager->GetProgressWindow();
	if(window){
		window->IncrementGauge();
	}
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
	m_LangToEn[_("Equalise")] = wxT("Equalise");
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
