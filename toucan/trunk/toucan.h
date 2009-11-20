/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef TOUCAN_H
#define TOUCAN_H

#include <wx/app.h>
#include <map>

class frmMain;
class frmProgress;
class Settings;
class ScriptManager;
class LuaManager;
class wxFileConfig;
class wxHtmlHelpController;

enum{
	ID_PROGRESS = wxID_HIGHEST + 1,
	ID_FINISH,
	ID_PROCESS,
	ID_BACKUPPROCESS,
	ID_SECUREPROCESS,
	ID_GETPASSWORD
};

class Toucan: public wxApp{    

public:
	virtual bool OnInit();
	virtual int OnExit();


	void SetLanguage(wxString langcode);
	void RebuildForm();
	void SetAbort(const bool& Abort) {this->m_Abort = Abort;}


	/*!
	 * Gets whether Toucan should abort
	 */
	const bool& GetAbort() const {return m_Abort;}
	/*!
	 * Gets the path to save all settings into, it ends with the appropriate 
	 * file path seperator
	 */
	const wxString& GetSettingsPath() const {return m_SettingsPath;}
	/*!
	 * Returns whether or not Toucan is operation in GUI mode
	 */
	const bool& GetUsesGUI() const {return m_UsesGUI;}

	frmMain* MainWindow;
	Settings* m_Settings;
	LuaManager *m_LuaManager;

	wxFileConfig* m_Jobs_Config;
	wxFileConfig* m_Rules_Config;
	wxFileConfig* m_Scripts_Config;
	wxFileConfig* m_Variables_Config;


	wxHtmlHelpController* m_Help;
	
	//<drive label, drive letter> Used by variables to resolve drive labels
	std::map<wxString, wxString> m_DriveLabels;
	std::map<wxString, wxString> m_EnToLang;
	std::map<wxString, wxString> m_LangToEn;
	//Maps a unique id used for process generation and whether the process
	//has finished
	std::map<int, bool> m_StatusMap;
	//Used by secure to see if the process finished successfully
	std::map<int, int> m_ProcessStatusMap;

	wxString m_Password;

private:
	//Clean up the temporary files that might be in the data folder
	void CleanTemp();
	//Remove the conime process if it is still running on windows
	void KillConime();
	//Set up the maps used for language conversion
	void InitLangMaps();
	//Used for outputing progress
	void OnProgress(wxCommandEvent &event);
	void OnProcess(wxCommandEvent &event);
	void OnBackupProcess(wxCommandEvent &event);
	void OnSecureProcess(wxCommandEvent &event);
	void OnFinish(wxCommandEvent &event);
	void OnGetPassword(wxCommandEvent &event);

	bool m_Abort;
	wxString m_SettingsPath;
	bool m_UsesGUI;
	bool m_Finished;
	wxLocale* m_Locale;

    DECLARE_CLASS(Toucan)
    DECLARE_EVENT_TABLE()
};

DECLARE_APP(Toucan)

#endif
