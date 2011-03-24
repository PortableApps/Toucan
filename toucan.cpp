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

#include <boost/interprocess/ipc/message_queue.hpp>

using namespace boost::interprocess;

#ifdef __MINGW32__
#define _WIN32_WINNT 0x0500
#endif

#ifdef __WXMSW__
	#include <windows.h>
	#include <tlhelp32.h>
	#include <process.h>
	#include <wx/msw/winundef.h>
#endif

#include "path.h"
#include "fileops.h"
#include "toucan.h"
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
	EVT_COMMAND(ID_PROCESS, wxEVT_COMMAND_BUTTON_CLICKED, Toucan::OnProcess)
	EVT_COMMAND(ID_BACKUPPROCESS, wxEVT_COMMAND_BUTTON_CLICKED, Toucan::OnBackupProcess)
	EVT_COMMAND(ID_SECUREPROCESS, wxEVT_COMMAND_BUTTON_CLICKED, Toucan::OnSecureProcess)
	EVT_COMMAND(ID_GETPASSWORD, wxEVT_COMMAND_BUTTON_CLICKED, Toucan::OnGetPassword)
	EVT_COMMAND(ID_PROGRESSSETUP, wxEVT_COMMAND_BUTTON_CLICKED, Toucan::OnProgressSetup)
    EVT_TIMER(wxID_ANY, Toucan::OnTimer)
END_EVENT_TABLE()

int main(int argc, char *argv[]){
	wxEntry(argc, argv);
}

//Toucan startup
bool Toucan::OnInit(){
	static const wxCmdLineEntryDesc desc[] =
	{
		{wxCMD_LINE_SWITCH, "h", "disablesplash", "Disables the splashscreen"},
		{wxCMD_LINE_OPTION, "d", "datadirectory", "Location of the Data folder", wxCMD_LINE_VAL_STRING},
		{wxCMD_LINE_OPTION, "s", "script", "Script to run", wxCMD_LINE_VAL_STRING},
		{wxCMD_LINE_OPTION, "l", "logfile", "Path to save log", wxCMD_LINE_VAL_STRING},
		{wxCMD_LINE_OPTION, "j", "job", "Job to run", wxCMD_LINE_VAL_STRING},
        {wxCMD_LINE_OPTION, "p", "password", "Password for jobs and scripts", wxCMD_LINE_VAL_STRING},
		{wxCMD_LINE_NONE}
	};
	wxCmdLineParser parser(desc, argc, argv);

	wxMessageOutput *old = wxMessageOutput::Set(new wxMessageOutputStderr);
	if(parser.Parse() > 0){
		return false;
	}
	delete wxMessageOutput::Set(old);

	//If no script is found then we are in gui mode
	if(!parser.Found("script") && !parser.Found("job")){
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
		File::Delete(wxFileName::FileName(exedir + wxT("writetest")), false, false);
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
    //Set the global settings path
    Locations::SetSettingsPath(m_SettingsPath);
	//Make sure the data directories are there
	if(!wxDirExists(GetSettingsPath()))
		Path::CreateDirectoryPath(wxFileName::DirName(GetSettingsPath()));
    if(!wxDirExists(GetSettingsPath() + "rules"))
        Path::CreateDirectoryPath(wxFileName::DirName(GetSettingsPath() + "rules"));
    if(!wxDirExists(GetSettingsPath() + "scripts"))
        Path::CreateDirectoryPath(wxFileName::DirName(GetSettingsPath() + "scripts"));

	//Create the config stuff and set it up
 	m_Jobs_Config = new wxFileConfig("", "", m_SettingsPath + "Jobs.ini");
	m_Scripts_Config = new wxFileConfig("", "", m_SettingsPath + "Scripts.ini");

	m_Jobs_Config->SetExpandEnvVars(false);
	m_Scripts_Config->SetExpandEnvVars(false);

	//Set the language and init the maps
 	wxFileConfig *settings = new wxFileConfig("", "", m_SettingsPath + "Settings.ini");
	SetLanguage(settings->Read("General/LanguageCode", "en"));
	delete settings;
	SetupLanguageMap();

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

    //Remove any messgae queues that might be left from a crash 
    boost::interprocess::message_queue::remove("progress");

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
        m_Timer = new wxTimer(this, wxID_ANY);
        m_Timer->Start(5);
        if(parser.Found("password")){
            wxString pass;
            parser.Found("password", &pass);
            m_Password = pass;
        }
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
                OutputProgress(_("The job does not exist"), FinishingLine);
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
    if(!IsGui()){
        m_Timer->Stop();
        delete m_Timer;
    }
    boost::interprocess::message_queue::remove("progress");
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
		delete m_Scripts_Config;
	}
	return wxApp::OnExit();
}

void Toucan::RebuildForm(){
	MainWindow->Destroy();
	SetLanguage(m_Settings->GetLanguageCode());
	SetupLanguageMap();
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

void Toucan::OnProcess(wxCommandEvent &event){
	m_StatusMap[event.GetInt()] = false;
	SignalProcess *process = new SignalProcess(event.GetInt());
	long pid = wxExecute(event.GetString(), wxEXEC_ASYNC|wxEXEC_NODISABLE, process);
	if(pid == 0){
		//We have an error
		OutputProgress(_("Could not run ") + event.GetString(), Error);
		m_StatusMap[event.GetInt()] = true;
	}
}

void Toucan::OnBackupProcess(wxCommandEvent &event){
	m_StatusMap[event.GetInt()] = false;
	long pid = wxExecute(event.GetString(), wxEXEC_ASYNC|wxEXEC_NODISABLE, static_cast<BackupProcess*>(event.GetEventObject()));
	static_cast<BackupProcess*>(event.GetEventObject())->SetRealPid(pid);
	if(pid == 0){
		//We have an error
		OutputProgress(_("Could not run ") + event.GetString(), Error);
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
		OutputProgress(_("Could not run ") + event.GetString(), Error);
		m_StatusMap[event.GetInt()] = true;
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

void Toucan::OnTimer(wxTimerEvent &WXUNUSED(event)){
    try{
        message_queue mq(open_or_create, "progress", 100, 10000);

        std::string message;
        message.resize(10000);
        size_t size;
        unsigned int priority;

        if(mq.try_receive(&message[0], message.size(), size, priority)){
            message.resize(size);
            wxString wxmessage(message.c_str(), wxConvUTF8);

            std::cout << wxmessage;

			if(priority == Error || priority == StartingLine || priority == FinishingLine){
                std::cout << "\t" << wxDateTime::Now().FormatISOTime();
			}

            if(priority == FinishingLine){
                OnExit();
                exit(EXIT_SUCCESS);
            }

            std::cout << std::endl;
        }
    }
    catch(std::exception &ex){
        wxLogError("%s", ex.what());
    }
}
