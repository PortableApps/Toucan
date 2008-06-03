/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef _FRMMAIN_H_
#define _FRMMAIN_H_

#include <wx/aui/framemanager.h>
#include <wx/frame.h>
#include <wx/toolbar.h>
#include <wx/aui/auibook.h>
#include <wx/treectrl.h>
#include <wx/dirctrl.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/listctrl.h>
#include <wx/fontpicker.h>

#include "virtualdirtreectrl.h"

//Forward declarations
class wxAuiNotebook;
class wxTreeCtrl;
class wxGenericDirCtrl;
class Settings;

////Control identifiers
#define ID_AUIFRAME 10000
#define ID_AUINOTEBOOK 10003
#define ID_PANEL_SYNC 10009

//Sync
#define ID_SYNC_OK 20019
#define ID_SYNC_PREVIEW 20020
#define ID_SYNC_JOB_SELECT 10019
#define ID_SYNC_JOB_SAVE 10020
#define ID_SYNC_JOB_ADD 10021
#define ID_SYNC_JOB_REMOVE 10025
#define ID_SYNC_RULES 10022
#define ID_SYNC_SOURCE_TXT 10039
#define ID_SYNC_SOURCE_BTN 10040
#define ID_SYNC_SOURCE_TREE 10041
#define ID_SYNC_DEST_TXT 10043
#define ID_SYNC_DEST_BTN 10044
#define ID_SYNC_DEST_TREE 10045
#define ID_SYNC_FUNCTION 10046
#define ID_SYNC_TIMESTAMP 10047
#define ID_SYNC_ATTRIB 10048
#define ID_SYNC_IGNORERO 10049
#define ID_SYNC_IGNOREDS 10063

//Backup

#define ID_PANEL_BACKUP 10010
#define ID_BACKUP_OK 20022
#define ID_BACKUP_PREVIEW 20023
#define ID_BACKUP_RESTORE 20024
#define ID_BACKUP_JOB_SELECT 10008
#define ID_BACKUP_JOB_SAVE 20011
#define ID_BACKUP_JOB_ADD 20012
#define ID_BACKUP_JOB_REMOVE 20013
#define ID_BACKUP_RULES 10013
#define ID_TEXTCTRL1 10071
#define ID_BACKUP_STATIC 20014
#define ID_BACKUP_LOCATION 10068
#define ID_BACKUP_DIRCTRL 10002
#define ID_BACKUP_ADD 10005
#define ID_BACKUP_REMOVE 10006
#define ID_BACKUP_TREECTRL 10029
#define ID_BACKUP_ADDVAR 10038
#define ID_BACKUP_FUNCTION 10014
#define ID_BACKUP_FORMAT 10015
#define ID_BACKUP_RATIO 10016
#define ID_BACKUP_ISPASS 10017

//Secure
#define ID_PANEL_SECURE 10023
#define ID_SECURE_OK 20050
#define ID_SECURE_PREVIEW 20051
#define ID_SECURE_RULES 10027
#define ID_SECURE_JOB_SELECT 10024
#define ID_SECURE_JOB_SAVE 10300
#define ID_SECURE_JOB_ADD 10026
#define ID_SECURE_JOB_REMOVE 10060
#define ID_SECURE_DIRCTRL 10101
#define ID_SECURE_ADD 10102
#define ID_SECURE_REMOVE 10103
#define ID_SECURE_TREECTRL 10100
#define ID_SECURE_ADDVAR 10038
#define ID_SECURE_MAKERELATIVE 10037
#define ID_SECURE_FUNCTION 10039
#define ID_SECURE_FORMAT 10040
#define ID_SECURE_PASS 10041
#define ID_SECURE_REPASS 10042
//Rules
#define ID_RULES 10050
#define ID_RULES_COMBO 10061
#define ID_RULES_SAVE 10062
#define ID_RULES_ADD 10069
#define ID_RULES_REMOVE 10070
#define ID_RULE_FILE_EXCLUDE 10051
#define ID_RULES_ADD_FILEEXCLUDE 10052
#define ID_RULES_REMOVE_FILEEXCLUDE 10053
#define ID_RULES_FOLDER_EXCLUDE 10054
#define ID_RULES_ADD_FOLDEREXCLUDE 10055
#define ID_RULES_REMOVE_FOLDEREXCLUDE 10056
#define ID_RULES_LOCATION_INCLUDE 10057
#define ID_RULES_ADD_LOCATIONINCLUDE 10058
#define ID_RULES_REMOVE_LOCATIONINCLUDE 10059
#define ID_RULES_FILE_DELETE 10200
#define ID_RULES_ADD_FILEDELETE 10201
#define ID_RULES_REMOVE_FILEDELETE 10202

//Portable Variables

#define ID_PVAR_NAME 30000
#define ID_PVAR_LIST 30001
#define ID_PVAR_ADD 30002
#define ID_PVAR_REMOVE 30003
#define ID_PVAR_ADDITEM 30002
#define ID_PVAR_REMOVEITEM 30003

//Script
#define ID_PANEL4 10064
#define ID_PANEL6 10066
#define ID_SCRIPT_NAME 10033
#define ID_SCRIPT_SAVE 10035
#define ID_SCRIPT_ADD 10034
#define ID_SCRIPT_REMOVE 10036
#define ID_SCRIPT_RICH 10030
#define ID_SCRIPT_CHECK 10031
#define ID_SCRIPT_EXECUTE 10032
#define ID_PANEL5 10065

//Settings
#define ID_SETTINGS_TABSTYLE 20030
#define ID_SETTINGS_LANGUAGE 20031
#define ID_SETTINGS_FONT 20032

//The form
#define SYMBOL_FRMMAIN_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxMAXIMIZE|wxMINIMIZE_BOX|wxMAXIMIZE_BOX|wxCLOSE_BOX
#define SYMBOL_FRMMAIN_TITLE _("Toucan")
#define SYMBOL_FRMMAIN_IDNAME ID_AUIFRAME
#define SYMBOL_FRMMAIN_SIZE wxSize(-1, -1)
#define SYMBOL_FRMMAIN_POSITION wxDefaultPosition


/*frmMain, the main form for Toucan*/
class frmMain: public wxFrame
{
	DECLARE_CLASS( frmMain )
	DECLARE_EVENT_TABLE()

public:
	/// Constructors
	frmMain();
	frmMain( wxWindow* parent, wxWindowID id = SYMBOL_FRMMAIN_IDNAME, const wxString& caption = SYMBOL_FRMMAIN_TITLE, const wxPoint& pos = SYMBOL_FRMMAIN_POSITION, const wxSize& size = SYMBOL_FRMMAIN_SIZE, long style = SYMBOL_FRMMAIN_STYLE );

	bool Create( wxWindow* parent, wxWindowID id = SYMBOL_FRMMAIN_IDNAME, const wxString& caption = SYMBOL_FRMMAIN_TITLE, const wxPoint& pos = SYMBOL_FRMMAIN_POSITION, const wxSize& size = SYMBOL_FRMMAIN_SIZE, long style = SYMBOL_FRMMAIN_STYLE );

	/// Destructor
	~frmMain();

	/// Initialises member variables
	void Init();

	/// Creates the controls and sizers
	void CreateControls();

	/// wxEVT_CLOSE_WINDOW event handler for ID_AUIFRAME
	void OnCloseWindow( wxCloseEvent& event );

	/// wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_SYNC_JOB_SELECT
	void OnSyncJobSelectSelected(wxCommandEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SYNC_JOB_OK
	void OnSyncOKClick(wxCommandEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SYNC_JOB_PREVIEW
	void OnSyncPreviewClick(wxCommandEvent& event);


	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SYNC_JOB_SAVE
	void OnSyncJobSaveClick(wxCommandEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SYNC_JOB_ADD
	void OnSyncJobAddClick(wxCommandEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SYNC_JOB_REMOVE
	void OnSyncJobRemoveClick(wxCommandEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SYNC_SOURCE_BTN
	void OnSyncSourceBtnClick(wxCommandEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SYNC_DEST_BTN
	void OnSyncDestBtnClick(wxCommandEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BACKUP_JOB_OK
	void OnBackupOKClick(wxCommandEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BACKUP_JOB_PREVIEW
	void OnBackupPreviewClick(wxCommandEvent& event);
	
	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SYNC_JOB_OK
	void OnSecureOKClick(wxCommandEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SYNC_JOB_PREVIEW
	void OnSecurePreviewClick(wxCommandEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SYNC_JOB_RESTORE
	void OnBackupRestoreClick(wxCommandEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BACKUP_LOCATION
	void OnBackupLocationClick(wxCommandEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BACKUP_ADD
	void OnBackupAddClick(wxCommandEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SYNC_JOB_ADD
	void OnBackupJobAddClick(wxCommandEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SYNC_JOB_REMOVE
	void OnBackupJobRemoveClick(wxCommandEvent& event);

	/// wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_BACKUP_JOB_SELECT
	void OnBackupJobSelectSelected(wxCommandEvent& event);

	/// wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_BACKUP_FUNCTION
	void OnBackupFunctionSelected(wxCommandEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BACKUP_JOB_SAVE
	void OnBackupJobSaveClick(wxCommandEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BACKUP_REMOVE
	void OnBackupRemoveClick(wxCommandEvent& event);

	/// wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_SECURE_JOB_SELECT
	void OnSecureJobSelectSelected(wxCommandEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SECURE_JOB_SAVE
	void OnSecureJobSaveClick(wxCommandEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SECURE_JOB_ADD
	void OnSecureJobAddClick(wxCommandEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SECURE_JOB_REMOVE
	void OnSecureJobRemoveClick(wxCommandEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SECURE_ADD
	void OnSecureAddClick(wxCommandEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SECURE_REMOVE
	void OnSecureRemoveClick(wxCommandEvent& event);

	/// wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_RULES_COMBO
	void OnRulesComboSelected(wxCommandEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RULES_SAVE
	void OnRulesSaveClick(wxCommandEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RULES_ADD
	void OnRulesAddClick(wxCommandEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RULES_REMOVE
	void OnRulesRemoveClick(wxCommandEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RULES_ADD_FILEEXCLUDE
	void OnRulesAddFileexcludeClick(wxCommandEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RULES_REMOVE_FILEEXCLUDE
	void OnRulesRemoveFileexcludeClick(wxCommandEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RULES_ADD_FOLDEREXCLUDE
	void OnRulesAddFolderexcludeClick(wxCommandEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RULES_REMOVE_FOLDEREXCLUDE
	void OnRulesRemoveFolderexcludeClick(wxCommandEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RULES_ADD_LOCATIONINCLUDE
	void OnRulesAddLocationincludeClick(wxCommandEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RULES_REMOVE_LOCATIONINCLUDE
	void OnRulesRemoveLocationincludeClick(wxCommandEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RULES_ADD_FILEDELETE
	void OnRulesAddFiledeleteClick(wxCommandEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RULES_REMOVE_FILEDELETE
	void OnRulesRemoveFiledeleteClick(wxCommandEvent& event);
	
	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SCRIPT_EXECUTE
	void OnScriptExecute(wxCommandEvent& event);	
	
	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_PVAR_ADD
	void OnPvarAddClick(wxCommandEvent& event);
	
	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_PVAR_REMOVE
	void OnPvarRemoveClick(wxCommandEvent& event);
	
	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_PVAR_ADDITEM
	void OnPvarAddItemClick(wxCommandEvent& event);
	
	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_PVAR_REMOVEITEM
	void OnPvarRemoveItemClick(wxCommandEvent& event);
	
	/// wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_PVAR_NAME
	void OnPvarNameSelected(wxCommandEvent& event);
	
	/// wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_SCRIPT_NAME
	void OnScriptSelected(wxCommandEvent& event);
	
	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SCRIPT_SAVE
	void OnScriptSaveClick(wxCommandEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SCRIPT_ADD
	void OnScriptAddClick(wxCommandEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SCRIPT_REMOVE
	void OnScriptRemoveClick(wxCommandEvent& event);
	
	/// wxEVT_LIST_ITEM_ACTIVATED event handler for ID_PVAR_LIST	
	void OnPvarListActivated(wxListEvent& event);
	
	/// wxEVT_SCROLL_CHANGED event handler for ID_BACKUP_RATIO
	void OnBackupRatioChanged(wxScrollEvent& event);	
	
	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BACKUP_ADDVAR
	void OnBackupAddVarClick(wxCommandEvent& event);
	
	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SECURE_ADDVAR
	void OnSecureAddVarClick(wxCommandEvent& event);
	
	void OnHelpClick(wxCommandEvent& event);

	void OnAboutClick(wxCommandEvent& event);

	//Functions

	/// Returns the AUI manager object
	wxAuiManager& GetAuiManager() {
		return m_auiManager;
	}

	/// Retrieves bitmap resources
	wxBitmap GetBitmapResource( const wxString& name );

	/// Retrieves icon resources
	wxIcon GetIconResource( const wxString& name );

	/// Should we show tooltips?
	static bool ShowToolTips();

	//Member variables
	wxAuiManager m_auiManager;
	wxAuiNotebook* m_Notebook;
	wxComboBox* m_Sync_Job_Select;
	wxComboBox* m_Sync_Rules;
	wxTextCtrl* m_Sync_Source_Txt;
	wxVirtualDirTreeCtrl* m_Sync_Source_Tree;
	wxTextCtrl* m_Sync_Dest_Txt;
	wxVirtualDirTreeCtrl* m_Sync_Dest_Tree;
	wxRadioBox* m_Sync_Function;
	wxCheckBox* m_Sync_Timestamp;
	wxCheckBox* m_Sync_Attributes;
	wxCheckBox* m_Sync_Ignore_Readonly;
	wxCheckBox* m_Sync_Ignore_DaylightS;
	wxComboBox* m_Backup_Job_Select;
	wxComboBox* m_Backup_Rules;
	wxStaticText* m_Backup_Location_Txt;
	wxTextCtrl* m_Backup_Location;
	wxGenericDirCtrl* m_Backup_DirCtrl;
	wxVirtualDirTreeCtrl* m_Backup_TreeCtrl;
	wxRadioBox* m_Backup_Function;
	wxRadioBox* m_Backup_Format;
	wxStaticText* m_Backup_Ratio_Text;
	wxSlider* m_Backup_Ratio;
	wxTextCtrl* m_Backup_Pass;
	wxTextCtrl* m_Backup_Repass;
	wxComboBox* m_Secure_Rules;
	wxComboBox* m_Secure_Job_Select;
	wxGenericDirCtrl* m_Secure_DirCtrl;
	wxVirtualDirTreeCtrl* m_Secure_TreeCtrl;
	wxRadioBox* m_Secure_Function;
	wxRadioBox* m_Secure_Format;
	wxTextCtrl* m_Secure_Pass;
	wxTextCtrl* m_Secure_Repass;
	wxComboBox* m_Rules_Combo;
	wxListBox* m_Rules_FileExclude;
	wxListBox* m_Rules_FolderExclude;
	wxListBox* m_Rules_LocationInclude;
	wxListBox* m_Rules_FileDelete;
	wxRadioBox* m_Settings_TabStyle;
    wxComboBox* m_Pvar_Name;
    wxListCtrl* m_Pvar_List;
	wxTextCtrl* m_Script_Rich;
	wxComboBox* m_Settings_Language;
	wxComboBox* m_Script_Name;
	wxCheckBox* m_Backup_IsPass;
	wxFontPickerCtrl* m_Settings_Font;
	
	wxArrayString* m_SecureLocations;
	wxArrayString* m_BackupLocations;

};

#endif
