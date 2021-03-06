/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006 - 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#ifndef FRMMAIN_H
#define FRMMAIN_H

class wxFrame;
class wxDirCtrl;
class wxListCtrl;
class wxListEvent;
class wxHtmlHelpWindow;
class wxFontPickerCtrl;
class wxTreeEvent;
class wxComboBox;
class wxTextCtrl;
class wxCheckBox;
class wxStaticText;
class wxListBox;
class wxRadioBox;
class wxSlider;
class wxStyledTextCtrl;
class wxGrid;
class wxChoice;

class DirCtrl;
class LocalDirCtrl;
class PreviewDirCtrl;
class SyncPreviewDirCtrl;
class Settings;
class RulesGrid;

#include <wx/frame.h>
#include <wx/aui/framemanager.h>
#include <wx/aui/auibook.h>

#if defined(__WXMSW__) && !defined(__MINGW32__)
    struct ITaskbarList3;
#endif

enum{
	ID_AUIFRAME = wxID_HIGHEST + 1,
	ID_AUINOTEBOOK,
	//Sync
	ID_PANEL_SYNC,
	ID_SYNC_RUN,
	ID_SYNC_PREVIEW,
	ID_SYNC_REFRESH,
	ID_SYNC_JOB_SELECT,
	ID_SYNC_JOB_SAVE,
	ID_SYNC_JOB_ADD,
	ID_SYNC_JOB_REMOVE,
	ID_SYNC_RULES,
	ID_SYNC_SOURCE_TXT,
	ID_SYNC_SOURCE_BTN,
	ID_SYNC_SOURCE_TREE,
	ID_SYNC_SOURCE_EXPAND,
	ID_SYNC_SOURCE_INSERT,
	ID_SYNC_DEST_TXT,
	ID_SYNC_DEST_BTN,
	ID_SYNC_DEST_TREE,
	ID_SYNC_DEST_EXPAND,
	ID_SYNC_DEST_INSERT,
	ID_SYNC_FUNCTION,
	ID_SYNC_CHECK_SIZE,
	ID_SYNC_CHECK_TIME,
	ID_SYNC_CHECK_SHORT,
	ID_SYNC_CHECK_FULL,
	ID_SYNC_TIMESTAMP,
	ID_SYNC_ATTRIB,
	ID_SYNC_IGNORERO,
	ID_SYNC_RECYCLE,
	ID_SYNC_PREVIEW_CHANGES,
	ID_SYNC_NO_SKIPPED,
	//Backup
	ID_PANEL_BACKUP,
	ID_BACKUP_RUN,
	ID_BACKUP_REFRESH,
	ID_BACKUP_JOB_SELECT,
	ID_BACKUP_JOB_SAVE,
	ID_BACKUP_JOB_ADD,
	ID_BACKUP_JOB_REMOVE,
	ID_BACKUP_RULES,
	ID_BACKUP_LOCATION_TEXT,
	ID_BACKUP_STATIC,
	ID_BACKUP_LOCATION,
	ID_BACKUP_DIRCTRL,
	ID_BACKUP_ADD,
	ID_BACKUP_REMOVE,
	ID_BACKUP_TREECTRL,
	ID_BACKUP_ADDVAR,
	ID_BACKUP_FUNCTION,
	ID_BACKUP_FORMAT,
	ID_BACKUP_RATIO,
	ID_BACKUP_ISPASS,
	ID_BACKUP_TEST,
	ID_BACKUP_SOLID,
	ID_BACKUP_EXPAND,
	ID_BACKUP_LOCATIONSTATIC,
	ID_BACKUP_FILESSTATIC,
	//Secure
	ID_PANEL_SECURE,
	ID_SECURE_RUN,
	ID_SECURE_REFRESH,
	ID_SECURE_RULES,
	ID_SECURE_JOB_SELECT,
	ID_SECURE_JOB_SAVE,
	ID_SECURE_JOB_ADD,
	ID_SECURE_JOB_REMOVE,
	ID_SECURE_DIRCTRL,
	ID_SECURE_ADD,
	ID_SECURE_REMOVE,
	ID_SECURE_TREECTRL,
	ID_SECURE_ADDVAR,
	ID_SECURE_MAKERELATIVE,
	ID_SECURE_FUNCTION,
	ID_SECURE_PASS,
	ID_SECURE_REPASS,
	ID_SECURE_EXPAND,
	ID_SECURE_FILESSATATIC,
	//Rules
	ID_PANEL_RULES,
	ID_RULES_COMBO,
	ID_RULES_SAVE,
	ID_RULES_ADD,
	ID_RULES_REMOVE,
    ID_RULES_GRID,
	ID_RULES_ADDITEM,
	ID_RULES_REMOVEITEM,
	//Variables
	ID_PANEL_VARIABLES,
	ID_VARIABLES_NAME,
	ID_VARIABLES_LIST,
	ID_VARIABLES_ADD,
	ID_VARIABLES_REMOVE,
	ID_VARIABLES_ADDITEM,
	ID_VARIABLES_REMOVEITEM,
	ID_VARIABLES_SAVE,
	//Script
	ID_PANEL_SCRIPT,
	ID_SCRIPT_NAME,
	ID_SCRIPT_SAVE,
	ID_SCRIPT_ADD,
	ID_SCRIPT_REMOVE,
	ID_SCRIPT_STYLED,
	ID_SCRIPT_CHECK,
	ID_SCRIPT_RUN,
    ID_SCRIPT_COMMAND_ADD,
    ID_SCRIPT_COMMAND_TYPE,
    ID_SCRIPT_UNDO,
    ID_SCRIPT_REDO,
	//Settings
	ID_PANEL_SETTINGS,
	ID_SETTINGS_TABSTYLE,
	ID_SETTINGS_LANGUAGE,
	ID_SETTINGS_FONT,
	ID_SETTINGS_REMEMBERSYNC,
	ID_SETTINGS_REMEMBERBACKUP,
	ID_SETTINGS_REMEMBERSECURE,
	ID_SETTINGS_ENABLETOOLTIPS,
	ID_SETTINGS_SMALLBORDERS,
	//Menu
	ID_MENU_FILEEXCLUDE_NAME,
	ID_MENU_FILEEXCLUDE_EXTENSION,
	ID_MENU_FILEINCLUDE_NAME,
	ID_MENU_FILEINCLUDE_EXTENSION,
	ID_MENU_FOLDEREXCLUDE_NAME,
	ID_MENU_FOLDERINCLUDE_NAME
};

class frmMain: public wxFrame
{
	DECLARE_EVENT_TABLE()

public:
	// Constructor
	frmMain();

	//Destructor, for uninitialising AUI
	~frmMain();

	//Sets up all of the member vaiables
	void Init();

	// Creates the controls and sizers
	void CreateControls();
	
	//Set up the title bar test
	void SetTitleBarText();

	//Sync
	void OnSyncRunClick(wxCommandEvent& event);
	void OnSyncPreviewClick(wxCommandEvent& event);
	void OnSyncJobSelectSelected(wxCommandEvent& event);	
	void OnSyncJobSaveClick(wxCommandEvent& event);
	void OnSyncJobAddClick(wxCommandEvent& event);
	void OnSyncJobRemoveClick(wxCommandEvent& event);
	void OnSyncSourceExpandClick(wxCommandEvent& event);
	void OnSyncSourceBtnClick(wxCommandEvent& event);
	void OnSyncDestExpandClick(wxCommandEvent& event);
	void OnSyncDestBtnClick(wxCommandEvent& event);
	void OnSyncSourceTreeRightClick(wxTreeEvent& event);
	void OnSyncDestTreeRightClick(wxTreeEvent& event);
	void OnSyncTreeCtrlTooltip(wxTreeEvent& event);
	void OnSyncSourceInsertClick(wxCommandEvent& event);
	void OnSyncDestInsertClick(wxCommandEvent& event);
	void OnSyncSourceRefresh(wxCommandEvent& event);
	void OnSyncDestRefresh(wxCommandEvent& event);
	void OnSyncRefresh(wxCommandEvent& event);

	//Backup
	void OnBackupRunClick(wxCommandEvent& event);
	void OnBackupRulesSelected(wxCommandEvent& event);
	void OnBackupLocationClick(wxCommandEvent& event);
	void OnBackupAddClick(wxCommandEvent& event);
	void OnBackupRemoveClick(wxCommandEvent& event);
	void OnBackupJobAddClick(wxCommandEvent& event);
	void OnBackupJobRemoveClick(wxCommandEvent& event);
	void OnBackupJobSelectSelected(wxCommandEvent& event);	
	void OnBackupJobSaveClick(wxCommandEvent& event);
	void OnBackupFunctionSelected(wxCommandEvent& event);
	void OnBackupExpandClick(wxCommandEvent& event);
	void OnBackupRatioChanged(wxScrollEvent& event);	
	void OnBackupAddVarClick(wxCommandEvent& event);
	void OnBackupTreeCtrlTooltip(wxTreeEvent& event);
	void OnBackupTreeRightClick(wxTreeEvent& event);
	void OnBackupRefresh(wxCommandEvent& event);

	//Secure
	void OnSecureRunClick(wxCommandEvent& event);
	void OnSecureRulesSelected(wxCommandEvent& event);
	void OnSecureJobSelectSelected(wxCommandEvent& event);
	void OnSecureJobSaveClick(wxCommandEvent& event);
	void OnSecureJobAddClick(wxCommandEvent& event);
	void OnSecureJobRemoveClick(wxCommandEvent& event);
	void OnSecureAddClick(wxCommandEvent& event);
	void OnSecureRemoveClick(wxCommandEvent& event);
	void OnSecureAddVarClick(wxCommandEvent& event);
	void OnSecureExpandClick(wxCommandEvent& event);
	void OnSecureTreeCtrlTooltip(wxTreeEvent& event);
	void OnSecureTreeRightClick(wxTreeEvent& event);
	void OnSecureRefresh(wxCommandEvent& event);
	void OnSecureFunctionSelected(wxCommandEvent& event);

	//Rules
	void OnRulesComboSelected(wxCommandEvent& event);
	void OnRulesSaveClick(wxCommandEvent& event);
	void OnRulesAddClick(wxCommandEvent& event);
	void OnRulesRemoveClick(wxCommandEvent& event);
	void OnRulesAddItemClick(wxCommandEvent& event);
	void OnRulesRemoveItemClick(wxCommandEvent& event);
	
	//Variables
	void OnVariablesSaveClick(wxCommandEvent& event);
	void OnVariablesAddClick(wxCommandEvent& event);
	void OnVariablesRemoveClick(wxCommandEvent& event);	
	void OnVariablesAddItemClick(wxCommandEvent& event);
	void OnVariablesRemoveItemClick(wxCommandEvent& event);
	void OnVariablesNameSelected(wxCommandEvent& event);
	void OnVariablesListActivated(wxListEvent& event);
	
	//Script
	void OnScriptExecute(wxCommandEvent& event);	
	void OnScriptSelected(wxCommandEvent& event);
	void OnScriptSaveClick(wxCommandEvent& event);	
	void OnScriptAddClick(wxCommandEvent& event);
	void OnScriptRemoveClick(wxCommandEvent& event);
    void OnScriptCommandTypeChange(wxCommandEvent &event);
    void OnScriptCommandAddClick(wxCommandEvent &event);
    void OnScriptUndo(wxCommandEvent &event);
    void OnScriptRedo(wxCommandEvent &event);
		
	//Other
	void OnCloseWindow(wxCloseEvent& event);
	void OnAboutClick(wxCommandEvent& event);
	void OnHelpClick(wxCommandEvent& event);
	void OnSettingsFontClick(wxCommandEvent& event);
	void OnTabChanged(wxAuiNotebookEvent& event); 
	void OnSettingsApplyClick(wxCommandEvent& event);
	
	//Menu
	void OnRulesMenuClick(wxCommandEvent& event);
	
	//Job functions
	void JobAdd(wxComboBox* box);
	void JobRemove(wxComboBox* box);
	void JobSave(const wxString &name, const wxString &type);
	void JobLoad(const wxString &name, const wxString &type);
	
	//Reset the form to its empty state
	void ClearToDefault();
	
	//Create the popup menu for rules
	void CreateMenu(wxTreeEvent& event);
	
	//Update the backup compression slider
	void SetSliderText();
	
	//Language functions
	wxArrayString GetLanguages();
	wxArrayString GetTranslatorNames();

	// Retrieves bitmap resources
	wxBitmap GetBitmapResource(const wxString& name);
	
	//Force a sizer to update its layout and then refresh the window
	void UpdateSizer(wxSizer *sizer);

	//Convenience function
	wxString ToString(bool bl);

#if defined(__WXMSW__) && !defined(__MINGW32__)
	WXUINT m_TaskBarId;
	ITaskbarList3 *m_Taskbar;
	//We catch windows events so we can support windows 7 taskbar progress
	WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wparam, WXLPARAM lparam);
#endif

	//General
	wxAuiManager m_auiManager;
	wxAuiNotebook* m_Notebook;
	
	//Sync
	wxBoxSizer* SyncTopSizer;
	wxComboBox* m_Sync_Job_Select;
	wxComboBox* m_Sync_Rules;
	wxTextCtrl* m_Sync_Source_Txt;
	wxTextCtrl* m_Sync_Dest_Txt;
	SyncPreviewDirCtrl* m_Sync_Source_Tree;
	SyncPreviewDirCtrl* m_Sync_Dest_Tree;
	wxRadioBox* m_Sync_Function;
	wxCheckBox* m_SyncCheckSize;
	wxCheckBox* m_SyncCheckTime;
	wxCheckBox* m_SyncCheckShort;
	wxCheckBox* m_SyncCheckFull;
	wxCheckBox* m_Sync_Timestamp;
	wxCheckBox* m_Sync_Attributes;
	wxCheckBox* m_Sync_Ignore_Readonly;
	wxCheckBox* m_Sync_Recycle;
	wxCheckBox* m_SyncPreviewChanges;
	wxCheckBox* m_SyncNoSkipped;
	
	//Backup
	wxBoxSizer* BackupTopSizer;
	wxComboBox* m_Backup_Job_Select;
	wxComboBox* m_Backup_Rules;
	wxStaticText* m_Backup_Location_Txt;
	wxStaticText* m_Backup_Ratio_Text;
	wxTextCtrl* m_Backup_Location;
	LocalDirCtrl* m_Backup_DirCtrl;
	PreviewDirCtrl* m_Backup_TreeCtrl;
	wxRadioBox* m_Backup_Function;
	wxRadioBox* m_Backup_Format;
	wxSlider* m_Backup_Ratio;
	wxCheckBox* m_Backup_IsPass;
	wxCheckBox* m_Backup_Test;
	wxCheckBox* m_BackupSolid;
	
	//Secure
	wxBoxSizer* SecureTopSizer;
	wxComboBox* m_Secure_Rules;
	wxComboBox* m_Secure_Job_Select;
	LocalDirCtrl* m_Secure_DirCtrl;
	PreviewDirCtrl* m_Secure_TreeCtrl;
	wxRadioBox* m_Secure_Function;

	//Rules
	wxStaticBoxSizer* RulesNameSizer;
	wxComboBox* m_Rules_Name;
    RulesGrid* m_RulesGrid;
	
	//Variables
	wxBoxSizer* VariablesTopSizer;
	wxComboBox* m_Variables_Name;
	wxListCtrl* m_Variables_List;
	
	//Script
	wxStaticBoxSizer* ScriptNameSizer;
	wxStyledTextCtrl* m_Script_Styled;
	wxComboBox* m_Script_Name;
    wxChoice* m_ScriptCommandType;
    wxChoice* m_ScriptCommandList;
	
	//Settings
	wxRadioBox* m_Settings_TabStyle;
	wxComboBox* m_Settings_Language;
 	wxButton* m_Settings_Font;
	wxCheckBox* m_Settings_RememberSync;
	wxCheckBox* m_Settings_RememberBackup;
	wxCheckBox* m_Settings_RememberSecure;
	wxCheckBox* m_Settings_EnableTooltips;
	wxCheckBox* m_Settings_SmallBorders;

	//Other
	wxArrayString* m_SecureLocations;
	wxArrayString* m_BackupLocations;
	DirCtrl* menuTree;
	wxComboBox* menuRules;
	wxFont* m_Font;
};

#endif
