/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef _TOUCAN_H_
#define _TOUCAN_H_

#include <wx/wx.h>
#include <map>

class frmMain;
class frmProgress;
class Settings;
class ScriptManager;
class wxFileConfig;
class wxHtmlHelpController;

/*!
 * Toucan's wxApp, can be accessed by wxGetApp().
 */
class Toucan: public wxApp{    

public:
	/*!
	 * Initialisation code
	 */
	virtual bool OnInit();
	/*!
	 * Cleanup code
	 */
	virtual int OnExit();
    /*!
	 * Called when Toucan fatally crashes, generates a debug report
	 */
    virtual void OnFatalException();

	/*!
	 * Set the language for Toucans wxLocale instance, either useing short or 
	 * long string form (de or de_DE)
	 */
	void SetLanguage(wxString langcode);

	/*!
	 * Rebuild frmMain, used when settings have been changed that need a restart
	 * for example a language change.
	 */
	void RebuildForm();

	/*!
	 * Sets whether Toucan should abort as soon as possible
	 */
	void SetAbort(const bool& Abort) {this->m_Abort = Abort;}
	/*!
	 * Set if the backup job is finished, used only in console mode backups
	 */
	void SetFinished(const bool& Finished) {this->m_Finished = Finished;}

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
	/*!
	 * Get whether or not the backup is finished, console mode only
	 */
	const bool& GetFinished() const {return m_Finished;}

	/*!
	 * A pointer to the current instance of the main Toucan window
	 */
	frmMain* MainWindow;
	/*!
	 * A pointer to the current instance of the Toucan progress window
	 */
	frmProgress* ProgressWindow;
	/*!
	 * A pointer to the settings of the current run of Toucan
	 */
	Settings* m_Settings;
	/*!
	 * A pointer to the script manager
	 */
	ScriptManager* m_Script;

	/*!
	 * A pointer to the jobs config, remember to flush or writes will not occur
	 * until Toucan is closed
	 */
	wxFileConfig* m_Jobs_Config;
	/*!
	 * A pointer to the rules config, remember to flush or writes will not occur
	 * until Toucan is closed
	 */
	wxFileConfig* m_Rules_Config;
	/*!
	 * A pointer to the scripts config, remember to flush or writes will not occur
	 * until Toucan is closed
	 */
	wxFileConfig* m_Scripts_Config;
	/*!
	 * A pointer to the variables config, remember to flush or writes will not occur
	 * until Toucan is closed
	 */
	wxFileConfig* m_Variables_Config;

	/*!
	 * A pointer to the help controller, currently this is only used to display
	 * the help in the help tab
	 */
	wxHtmlHelpController* m_Help;
	
	/*!
	 * The drive label to letter map, they are stored as followed: 
	 * <drive label, drive letter> Used by variables to resolve drive labels
	 */
	std::map<wxString, wxString> m_DriveLabels;
	
	/*!
	 * A translation map to allow english in settings files to be converted 
	 * back to the native language for the UI
	 */
	std::map<wxString, wxString> m_EnToLang;
	/*!
	 * A translation map to allow native ui selections to be converted back 
	 * into english for the settings files
	 */
	std::map<wxString, wxString> m_LangToEn;

private:
	//Clean up the temporary files that might be in the data folder
	void CleanTemp();
	//Remove the conime process if it is still running on windows
	void KillConime();
	//Set up the maps used for language conversion
	void InitLangMaps();

	bool m_Abort;
	wxString m_SettingsPath;
	bool m_UsesGUI;
	bool m_Finished;
	wxLocale* m_Locale;	
};

DECLARE_APP(Toucan)

#endif
