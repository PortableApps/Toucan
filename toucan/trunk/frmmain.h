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
#include <wx/listctrl.h>
#include <wx/fontpicker.h>

#include "virtualdirtreectrl.h"

//Forward declarations
class Settings;

//Main controls 10000-10049
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
#define ID_SYNC_DEST_TXT 10062
#define ID_SYNC_DEST_BTN 10063
#define ID_SYNC_DEST_TREE 10064
#define ID_SYNC_DEST_EXPAND 10065
#define ID_SYNC_FUNCTION 10066
#define ID_SYNC_TIMESTAMP 10067
#define ID_SYNC_ATTRIB 10068
#define ID_SYNC_IGNORERO 10069
#define ID_SYNC_IGNOREDS 10070

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

//Menu 10400-10449
#define ID_MENU_SYNCSOURCEFILEEXCLUDE_EXTENSION 10400
#define ID_MENU_SYNCSOURCEFILEEXCLUDE_NAME 10401
#define ID_MENU_SYNCSOURCEFOLDEREXCLUDE_NAME 10402
#define ID_MENU_SYNCSOURCELOCATIONINCLUDE_NAME 10403
#define ID_MENU_SYNCSOURCELOCATIONINCLUDE_EXTENSION 10404

#define ID_MENU_SYNCDESTFILEEXCLUDE_EXTENSION 10405
#define ID_MENU_SYNCDESTFILEEXCLUDE_NAME 10406
#define ID_MENU_SYNCDESTFOLDEREXCLUDE_NAME 10407
#define ID_MENU_SYNCDESTLOCATIONINCLUDE_NAME 10408
#define ID_MENU_SYNCDESTLOCATIONINCLUDE_EXTENSION 10409

#define ID_MENU_BACKUPFILEEXCLUDE_EXTENSION 10410
#define ID_MENU_BACKUPFILEEXCLUDE_NAME 10411
#define ID_MENU_BACKUPFOLDEREXCLUDE_NAME 10412
#define ID_MENU_BACKUPLOCATIONINCLUDE_NAME 10413
#define ID_MENU_BACKUPLOCATIONINCLUDE_EXTENSION 10414

#define ID_MENU_SECUREFILEEXCLUDE_EXTENSION 10415
#define ID_MENU_SECUREFILEEXCLUDE_NAME 10416
#define ID_MENU_SECUREFOLDEREXCLUDE_NAME 10417
#define ID_MENU_SECURELOCATIONINCLUDE_NAME 10418
#define ID_MENU_SECURELOCATIONINCLUDE_EXTENSION 10419

//Other 10450-10499
#define ID_SCRIPTFINISH 10450 




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

	//Deconstructor, for uninitialising AUI
	~frmMain();

	//To set up all of the member vaiables
	void Init();

	/// Creates the controls and sizers
	void CreateControls();
	
	//Sync
	void OnSyncOKClick(wxCommandEvent& event);
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
	
	//Backup
	void OnBackupOKClick(wxCommandEvent& event);
	void OnBackupPreviewClick(wxCommandEvent& event);
	void OnBackupRestoreClick(wxCommandEvent& event);
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

	//Secure
	void OnSecureOKClick(wxCommandEvent& event);
	void OnSecurePreviewClick(wxCommandEvent& event);
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
	
	//Rules
	void OnRulesComboSelected(wxCommandEvent& event);
	void OnRulesSaveClick(wxCommandEvent& event);
	void OnRulesAddClick(wxCommandEvent& event);
	void OnRulesRemoveClick(wxCommandEvent& event);
	void OnRulesAddFileexcludeClick(wxCommandEvent& event);
	void OnRulesRemoveFileexcludeClick(wxCommandEvent& event);
	void OnRulesAddFolderexcludeClick(wxCommandEvent& event);
	void OnRulesRemoveFolderexcludeClick(wxCommandEvent& event);
	void OnRulesAddLocationincludeClick(wxCommandEvent& event);
	void OnRulesRemoveLocationincludeClick(wxCommandEvent& event);	
	
	//Portable Variables
	void OnPvarAddClick(wxCommandEvent& event);
	void OnPvarRemoveClick(wxCommandEvent& event);	
	void OnPvarAddItemClick(wxCommandEvent& event);
	void OnPvarRemoveItemClick(wxCommandEvent& event);
	void OnPvarNameSelected(wxCommandEvent& event);
	void OnPvarListActivated(wxListEvent& event);
	
	//Script
	void OnScriptExecute(wxCommandEvent& event);	
	void OnScriptSelected(wxCommandEvent& event);
	void OnScriptSaveClick(wxCommandEvent& event);	
	void OnScriptAddClick(wxCommandEvent& event);
	void OnScriptRemoveClick(wxCommandEvent& event);
	
	//Settings
	void OnFontChange(wxFontPickerEvent& event);
	
	//Other
	void OnCloseWindow(wxCloseEvent& event);
	void OnHelpClick(wxCommandEvent& event);
	void OnAboutClick(wxCommandEvent& event);
	void OnScriptFinish(wxCommandEvent& event);
	
	//Menu
	void OnMenuSyncSourceFileExcludeExtensionClick(wxCommandEvent& event);
	void OnMenuSyncSourceFileExcludeNameClick(wxCommandEvent& event);
	void OnMenuSyncSourceLocationIncludeExtensionClick(wxCommandEvent& event);
	void OnMenuSyncSourceLocationIncludeNameClick(wxCommandEvent& event);
	void OnMenuSyncSourceFolderExcludeNameClick(wxCommandEvent& event);
	
	void OnMenuSyncDestFileExcludeExtensionClick(wxCommandEvent& event);
	void OnMenuSyncDestFileExcludeNameClick(wxCommandEvent& event);
	void OnMenuSyncDestLocationIncludeExtensionClick(wxCommandEvent& event);
	void OnMenuSyncDestLocationIncludeNameClick(wxCommandEvent& event);
	void OnMenuSyncDestFolderExcludeNameClick(wxCommandEvent& event);
	
	void OnMenuBackupFileExcludeExtensionClick(wxCommandEvent& event);
	void OnMenuBackupFileExcludeNameClick(wxCommandEvent& event);
	void OnMenuBackupLocationIncludeExtensionClick(wxCommandEvent& event);
	void OnMenuBackupLocationIncludeNameClick(wxCommandEvent& event);
	void OnMenuBackupFolderExcludeNameClick(wxCommandEvent& event);
	
	void OnMenuSecureFileExcludeExtensionClick(wxCommandEvent& event);
	void OnMenuSecureFileExcludeNameClick(wxCommandEvent& event);
	void OnMenuSecureLocationIncludeExtensionClick(wxCommandEvent& event);
	void OnMenuSecureLocationIncludeNameClick(wxCommandEvent& event);
	void OnMenuSecureFolderExcludeNameClick(wxCommandEvent& event);

	////Functions

	// Returns the AUI manager object
	wxAuiManager& GetAuiManager() {
		return m_auiManager;
	}
	
	// Retrieves bitmap resources
	wxBitmap GetBitmapResource( const wxString& name );

	// Retrieves icon resources
	wxIcon GetIconResource( const wxString& name );

	////Member variables
	
	//General
	wxAuiManager m_auiManager;
	wxAuiNotebook* m_Notebook;
	
	//Sync
	wxComboBox* m_Sync_Job_Select;
	wxComboBox* m_Sync_Rules;
	wxTextCtrl* m_Sync_Source_Txt;
	wxTextCtrl* m_Sync_Dest_Txt;
	wxVirtualDirTreeCtrl* m_Sync_Source_Tree;
	wxVirtualDirTreeCtrl* m_Sync_Dest_Tree;
	wxRadioBox* m_Sync_Function;
	wxCheckBox* m_Sync_Timestamp;
	wxCheckBox* m_Sync_Attributes;
	wxCheckBox* m_Sync_Ignore_Readonly;
	wxCheckBox* m_Sync_Ignore_DaylightS;
	
	//Backup
	wxComboBox* m_Backup_Job_Select;
	wxComboBox* m_Backup_Rules;
	wxStaticText* m_Backup_Location_Txt;
	wxStaticText* m_Backup_Ratio_Text;
	wxTextCtrl* m_Backup_Location;
	wxTextCtrl* m_Backup_Pass;
	wxTextCtrl* m_Backup_Repass;
	wxGenericDirCtrl* m_Backup_DirCtrl;
	wxVirtualDirTreeCtrl* m_Backup_TreeCtrl;
	wxRadioBox* m_Backup_Function;
	wxRadioBox* m_Backup_Format;
	wxSlider* m_Backup_Ratio;
	wxCheckBox* m_Backup_IsPass;
	
	//Secure
	wxComboBox* m_Secure_Rules;
	wxComboBox* m_Secure_Job_Select;
	wxGenericDirCtrl* m_Secure_DirCtrl;
	wxVirtualDirTreeCtrl* m_Secure_TreeCtrl;
	wxRadioBox* m_Secure_Function;
	wxTextCtrl* m_Secure_Pass;
	wxTextCtrl* m_Secure_Repass;
	
	//Rules
	wxComboBox* m_Rules_Combo;
	wxListBox* m_Rules_FileExclude;
	wxListBox* m_Rules_FolderExclude;
	wxListBox* m_Rules_LocationInclude;
	wxListBox* m_Rules_FileDelete;
	
	//Portable Variables
	wxComboBox* m_Pvar_Name;
    wxListCtrl* m_Pvar_List;
	
	//Script
	wxTextCtrl* m_Script_Rich;
	wxComboBox* m_Script_Name;
	
	//Settings
	wxRadioBox* m_Settings_TabStyle;
	wxComboBox* m_Settings_Language;
 	wxFontPickerCtrl* m_Settings_Font;

	//Other
	wxArrayString* m_SecureLocations;
	wxArrayString* m_BackupLocations;

};

#endif
