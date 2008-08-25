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

////Main controls 10000-10049
#define ID_AUIFRAME 10000
#define ID_AUINOTEBOOK 10001


//Sync 10050-10099
#define ID_PANEL_SYNC 10050
#define ID_SYNC_OK 10051
#define ID_SYNC_PREVIEW 10052
#define ID_SYNC_JOB_SELECT 110053
#define ID_SYNC_JOB_SAVE 10054
#define ID_SYNC_JOB_ADD 10055
#define ID_SYNC_JOB_REMOVE 10056
#define ID_SYNC_RULES 10057
#define ID_SYNC_SOURCE_TXT 10058
#define ID_SYNC_SOURCE_BTN 10059
#define ID_SYNC_SOURCE_TREE 10060
#define ID_SYNC_SOURCE_EXPAND 10061
#define ID_sYNC_DEST_EXPAND 10062
#define ID_SYNC_DEST_TXT 10063
#define ID_SYNC_DEST_BTN 10064
#define ID_SYNC_DEST_TREE 10065
#define ID_SYNC_DEST_EXPAND 10066
#define ID_SYNC_FUNCTION 10067
#define ID_SYNC_TIMESTAMP 10068
#define ID_SYNC_ATTRIB 10069
#define ID_SYNC_IGNORERO 10070
#define ID_SYNC_IGNOREDS 10071

//Backup 10100-10149

#define ID_PANEL_BACKUP 10100
#define ID_BACKUP_OK 10101
#define ID_BACKUP_PREVIEW 10102
#define ID_BACKUP_RESTORE 10103
#define ID_BACKUP_JOB_SELECT 10104
#define ID_BACKUP_JOB_SAVE 10105
#define ID_BACKUP_JOB_ADD 10106
#define ID_BACKUP_JOB_REMOVE 10107
#define ID_BACKUP_RULES 10108
#define ID_TEXTCTRL1 10109
#define ID_BACKUP_STATIC 10110
#define ID_BACKUP_LOCATION 10111
#define ID_BACKUP_DIRCTRL 10112
#define ID_BACKUP_ADD 10113
#define ID_BACKUP_REMOVE 10114
#define ID_BACKUP_TREECTRL 10115
#define ID_BACKUP_ADDVAR 10116
#define ID_BACKUP_FUNCTION 10117
#define ID_BACKUP_FORMAT 10118
#define ID_BACKUP_RATIO 10119
#define ID_BACKUP_ISPASS 10120
#define ID_BACKUP_EXPAND 10121

//Secure 10150-10199
#define ID_PANEL_SECURE 10150
#define ID_SECURE_OK 10151
#define ID_SECURE_PREVIEW 10152
#define ID_SECURE_RULES 10153
#define ID_SECURE_JOB_SELECT 10154
#define ID_SECURE_JOB_SAVE 10155
#define ID_SECURE_JOB_ADD 10156
#define ID_SECURE_JOB_REMOVE 10157
#define ID_SECURE_DIRCTRL 10158
#define ID_SECURE_ADD 10159
#define ID_SECURE_REMOVE 10160
#define ID_SECURE_TREECTRL 10161
#define ID_SECURE_ADDVAR 10162
#define ID_SECURE_MAKERELATIVE 10163
#define ID_SECURE_FUNCTION 10164
#define ID_SECURE_PASS 10165
#define ID_SECURE_REPASS 10166
#define ID_SECURE_EXPAND 10167

//Rules 10200-10249
#define ID_PANEL_RULES 10200
#define ID_RULES_COMBO 10201
#define ID_RULES_SAVE 10202
#define ID_RULES_ADD 10203
#define ID_RULES_REMOVE 10204
#define ID_RULE_FILE_EXCLUDE 10205
#define ID_RULES_ADD_FILEEXCLUDE 10206
#define ID_RULES_REMOVE_FILEEXCLUDE 10207
#define ID_RULES_FOLDER_EXCLUDE 10208
#define ID_RULES_ADD_FOLDEREXCLUDE 10209
#define ID_RULES_REMOVE_FOLDEREXCLUDE 10210
#define ID_RULES_LOCATION_INCLUDE 10211
#define ID_RULES_ADD_LOCATIONINCLUDE 10212
#define ID_RULES_REMOVE_LOCATIONINCLUDE 10213
#define ID_RULES_FILE_DELETE 10214
#define ID_RULES_ADD_FILEDELETE 10215
#define ID_RULES_REMOVE_FILEDELETE 10216

//Portable Variables 10250-10299

#define ID_PANEL_PVAR 10250
#define ID_PVAR_NAME 10251
#define ID_PVAR_LIST 10252
#define ID_PVAR_ADD 10253
#define ID_PVAR_REMOVE 10254
#define ID_PVAR_ADDITEM 10255
#define ID_PVAR_REMOVEITEM 10256

//Script 10300-10349

#define ID_PANEL_SCRIPT 10301
#define ID_SCRIPT_NAME 10302
#define ID_SCRIPT_SAVE 10303
#define ID_SCRIPT_ADD 10304
#define ID_SCRIPT_REMOVE 10305
#define ID_SCRIPT_RICH 10306
#define ID_SCRIPT_CHECK 10307
#define ID_SCRIPT_EXECUTE 10308


//Settings 10350-10399
#define ID_PANEL_SETTINGS 10350
#define ID_SETTINGS_TABSTYLE 10351
#define ID_SETTINGS_LANGUAGE 10352
#define ID_SETTINGS_FONT 10353


/*frmMain, the main form for Toucan*/
class frmMain: public wxFrame
{
	DECLARE_CLASS( frmMain )
	DECLARE_EVENT_TABLE()

public:
	/// Constructors
	frmMain();
	frmMain( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style);

	bool Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style);

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
	
	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SYNC_SOURCE_EXPAND
	void OnSyncSourceExpandClick(wxCommandEvent& event);
	
	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SYNC_DEST_EXPAND
	void OnSyncDestExpandClick(wxCommandEvent& event);

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
	
	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BACKUP_EXPAND
	void OnBackupExpandClick(wxCommandEvent& event);

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
	
	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SECURE_ADDVAR
	void OnSecureExpandClick(wxCommandEvent& event);
	
	void OnHelpClick(wxCommandEvent& event);

	void OnAboutClick(wxCommandEvent& event);
	
	void OnBackupTreeCtrlTooltip(wxTreeEvent& event);
	
	void OnSecureTreeCtrlTooltip(wxTreeEvent& event);
	
	void OnBackupTreeRightClick(wxTreeEvent& event);
	
	void OnFontChange(wxFontPickerEvent& event);
	
	//Functions

	/// Returns the AUI manager object
	wxAuiManager& GetAuiManager() {
		return m_auiManager;
	}

	/// Retrieves bitmap resources
	wxBitmap GetBitmapResource( const wxString& name );

	/// Retrieves icon resources
	wxIcon GetIconResource( const wxString& name );

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
