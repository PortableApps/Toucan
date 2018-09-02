/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006 - 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#ifndef TOUCAN_H
#define TOUCAN_H

#include <wx/app.h>
#include <wx/timer.h>
#include <map>

class frmMain;
class frmProgress;
class Settings;
class ScriptManager;
class LuaManager;
class wxTextFile;
class wxFileConfig;
class wxTimerEvent;
class wxSingleInstanceChecker;

#define TOUCAN_VERSION "3.1.6"

enum{
	ID_OUTPUT = wxID_HIGHEST + 1,
	ID_PROCESS,
	ID_BACKUPPROCESS,
	ID_SECUREPROCESS,
	ID_GETPASSWORD,
	ID_PROGRESS,
	ID_PROGRESSSETUP
};

class Toucan: public wxApp{    

public:
    Toucan();

	bool OnInit();
	int OnExit();
    void OnTimer(wxTimerEvent &event);


	void SetLanguage(const wxString &lang);
	void RebuildForm();
	void SetAbort(const bool& Abort) {this->m_Abort = Abort;}

	const bool& GetAbort() const {return m_Abort;}
	const wxString& GetSettingsPath() const {return m_SettingsPath;}
	const bool& IsGui() const {return m_IsGui;}
	const bool& IsReadOnly() const {return m_IsReadOnly;}

	frmMain* MainWindow;
	Settings* m_Settings;
	LuaManager *m_LuaManager;

	wxFileConfig* m_Jobs_Config;
	wxFileConfig* m_Scripts_Config;

	//Maps a unique id used for process generation and whether the process
	//has finished
	std::map<int, bool> m_StatusMap;
	//Used to see if the process finished successfully
	std::map<int, int> m_ProcessStatusMap;

	wxString m_Password;
    wxLogChain* m_LogChain;
	wxTextFile* m_LogFile;

private:
	//Clean up the temporary files that might be in the data folder
	void CleanTemp();
	//Remove the conime process if it is still running on windows
	void KillConime();
	//Set up the maps used for language conversion
	void InitLangMaps();
	//Used for outputing progress
	void OnProcess(wxCommandEvent &event);
	void OnBackupProcess(wxCommandEvent &event);
	void OnSecureProcess(wxCommandEvent &event);
	void OnGetPassword(wxCommandEvent &event);
	void OnProgressSetup(wxCommandEvent &event);

	bool m_Abort;
	wxString m_SettingsPath;
	bool m_IsGui;
	bool m_IsReadOnly;
	bool m_Finished;
	wxLocale* m_Locale;
    wxTimer *m_Timer;
    wxSingleInstanceChecker *m_Checker;

    DECLARE_CLASS(Toucan)
    DECLARE_EVENT_TABLE()
};

DECLARE_APP(Toucan)

#endif
