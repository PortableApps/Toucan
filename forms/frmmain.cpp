/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006 - 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include <wx/stdpaths.h>
#include <wx/aboutdlg.h>
#include <wx/fileconf.h>
#include <wx/html/helpctrl.h>
#include <wx/listctrl.h>
#include <wx/fontdlg.h>
#include <wx/dir.h>
#include <wx/gbsizer.h>
#include <wx/stc/stc.h>
#include <wx/grid.h>
#include <wx/wx.h>

#include "frmmain.h"
#include "frmvariable.h"
#include "../toucan.h"
#include "../dragndrop.h"
#include "../rules.h"
#include "../luamanager.h"
#include "../basicfunctions.h"
#include "../path.h"
#include "../settings.h"
#include "../data/securedata.h"
#include "../data/backupdata.h"
#include "../data/syncdata.h"
#include "../controls/syncctrl.h"
#include "../controls/previewctrl.h"
#include "../controls/localdirctrl.h"
#include "../controls/loglistctrl.h"
#include "../controls/rulesgrid.h"

#if defined(__WXMSW__) && !defined(__MINGW32__)
	#include <shobjidl.h>
	#undef Yield
#endif

//frmMain event table
BEGIN_EVENT_TABLE(frmMain, wxFrame)
	//Sync
	EVT_BUTTON(ID_SYNC_RUN, frmMain::OnSyncRunClick)
	EVT_BUTTON(ID_SYNC_PREVIEW, frmMain::OnSyncPreviewClick)	
	EVT_BUTTON(ID_SYNC_SOURCE_EXPAND, frmMain::OnSyncSourceExpandClick)
	EVT_BUTTON(ID_SYNC_DEST_EXPAND, frmMain::OnSyncDestExpandClick)
	EVT_BUTTON(ID_SYNC_JOB_ADD, frmMain::OnSyncJobAddClick)
	EVT_BUTTON(ID_SYNC_JOB_REMOVE, frmMain::OnSyncJobRemoveClick)
	EVT_BUTTON(ID_SYNC_JOB_SAVE, frmMain::OnSyncJobSaveClick)
	EVT_COMBOBOX(ID_SYNC_JOB_SELECT, frmMain::OnSyncJobSelectSelected)
	EVT_BUTTON(ID_SYNC_SOURCE_BTN, frmMain::OnSyncSourceBtnClick)
	EVT_BUTTON(ID_SYNC_DEST_BTN, frmMain::OnSyncDestBtnClick)
	EVT_TREE_ITEM_RIGHT_CLICK(ID_SYNC_SOURCE_TREE, frmMain::OnSyncSourceTreeRightClick)
	EVT_TREE_ITEM_RIGHT_CLICK(ID_SYNC_DEST_TREE, frmMain::OnSyncDestTreeRightClick)
	EVT_TREE_ITEM_GETTOOLTIP(ID_SYNC_SOURCE_TREE, frmMain::OnSyncTreeCtrlTooltip)
	EVT_TREE_ITEM_GETTOOLTIP(ID_SYNC_DEST_TREE, frmMain::OnSyncTreeCtrlTooltip)
	EVT_BUTTON(ID_SYNC_SOURCE_INSERT, frmMain::OnSyncSourceInsertClick)
	EVT_BUTTON(ID_SYNC_DEST_INSERT, frmMain::OnSyncDestInsertClick)
	EVT_TEXT_ENTER(ID_SYNC_SOURCE_TXT, frmMain::OnSyncSourceRefresh)
	EVT_TEXT_ENTER(ID_SYNC_DEST_TXT, frmMain::OnSyncDestRefresh)
	EVT_BUTTON(ID_SYNC_REFRESH, frmMain::OnSyncRefresh)

	//Backup
	EVT_BUTTON(ID_BACKUP_RUN, frmMain::OnBackupRunClick)
	EVT_COMBOBOX(ID_BACKUP_RULES, frmMain::OnBackupRulesSelected)
	EVT_BUTTON(ID_BACKUP_LOCATION, frmMain::OnBackupLocationClick)
	EVT_BUTTON(ID_BACKUP_ADD, frmMain::OnBackupAddClick)
	EVT_BUTTON(ID_BACKUP_REMOVE, frmMain::OnBackupRemoveClick)
	EVT_BUTTON(ID_BACKUP_EXPAND, frmMain::OnBackupExpandClick)	
	EVT_BUTTON(ID_BACKUP_JOB_SAVE, frmMain::OnBackupJobSaveClick)
	EVT_BUTTON(ID_BACKUP_JOB_ADD, frmMain::OnBackupJobAddClick)
	EVT_BUTTON(ID_BACKUP_JOB_REMOVE, frmMain::OnBackupJobRemoveClick)
	EVT_BUTTON(ID_BACKUP_ADDVAR, frmMain::OnBackupAddVarClick)
	EVT_COMBOBOX(ID_BACKUP_JOB_SELECT, frmMain::OnBackupJobSelectSelected)
	EVT_COMMAND_SCROLL_CHANGED(ID_BACKUP_RATIO, frmMain::OnBackupRatioChanged)
	EVT_TREE_ITEM_GETTOOLTIP(ID_BACKUP_TREECTRL, frmMain::OnBackupTreeCtrlTooltip)
	EVT_TREE_ITEM_RIGHT_CLICK(ID_BACKUP_TREECTRL, frmMain::OnBackupTreeRightClick)
	EVT_RADIOBOX(ID_BACKUP_FUNCTION, frmMain::OnBackupFunctionSelected)
	EVT_BUTTON(ID_BACKUP_REFRESH, frmMain::OnBackupRefresh)

	//Secure
	EVT_BUTTON(ID_SECURE_RUN, frmMain::OnSecureRunClick)
	EVT_COMBOBOX(ID_SECURE_RULES, frmMain::OnSecureRulesSelected)
	EVT_COMBOBOX(ID_SECURE_JOB_SELECT, frmMain::OnSecureJobSelectSelected)
	EVT_BUTTON(ID_SECURE_JOB_SAVE, frmMain::OnSecureJobSaveClick)
	EVT_BUTTON(ID_SECURE_JOB_ADD, frmMain::OnSecureJobAddClick)
	EVT_BUTTON(ID_SECURE_JOB_REMOVE, frmMain::OnSecureJobRemoveClick)
	EVT_BUTTON(ID_SECURE_ADD, frmMain::OnSecureAddClick)
	EVT_BUTTON(ID_SECURE_REMOVE, frmMain::OnSecureRemoveClick)
	EVT_BUTTON(ID_SECURE_EXPAND, frmMain::OnSecureExpandClick)	
	EVT_BUTTON(ID_SECURE_ADDVAR, frmMain::OnSecureAddVarClick)	
	EVT_TREE_ITEM_GETTOOLTIP(ID_SECURE_TREECTRL, frmMain::OnSecureTreeCtrlTooltip)
	EVT_TREE_ITEM_RIGHT_CLICK(ID_SECURE_TREECTRL, frmMain::OnSecureTreeRightClick)
	EVT_RADIOBOX(ID_SECURE_FUNCTION, frmMain::OnSecureFunctionSelected)
	EVT_BUTTON(ID_SECURE_REFRESH, frmMain::OnSecureRefresh)

	//Rules
	EVT_COMBOBOX(ID_RULES_COMBO, frmMain::OnRulesComboSelected)
	EVT_BUTTON(ID_RULES_SAVE, frmMain::OnRulesSaveClick)
	EVT_BUTTON(ID_RULES_ADD, frmMain::OnRulesAddClick)
	EVT_BUTTON(ID_RULES_REMOVE, frmMain::OnRulesRemoveClick)
	EVT_BUTTON(ID_RULES_ADDITEM, frmMain::OnRulesAddItemClick)
	EVT_BUTTON(ID_RULES_REMOVEITEM, frmMain::OnRulesRemoveItemClick)
	
	//Variables
	EVT_BUTTON(ID_VARIABLES_SAVE, frmMain::OnVariablesSaveClick)
	EVT_BUTTON(ID_VARIABLES_ADD, frmMain::OnVariablesAddClick)
	EVT_BUTTON(ID_VARIABLES_REMOVE, frmMain::OnVariablesRemoveClick)
	EVT_BUTTON(ID_VARIABLES_ADDITEM, frmMain::OnVariablesAddItemClick)
	EVT_BUTTON(ID_VARIABLES_REMOVEITEM, frmMain::OnVariablesRemoveItemClick)
	EVT_COMBOBOX(ID_VARIABLES_NAME, frmMain::OnVariablesNameSelected)
	EVT_LIST_ITEM_ACTIVATED(ID_VARIABLES_LIST, frmMain::OnVariablesListActivated)
	
	//Script
	EVT_BUTTON(ID_SCRIPT_RUN, frmMain::OnScriptExecute)
	EVT_BUTTON(ID_SCRIPT_SAVE, frmMain::OnScriptSaveClick)
	EVT_BUTTON(ID_SCRIPT_ADD, frmMain::OnScriptAddClick)
	EVT_BUTTON (ID_SCRIPT_REMOVE, frmMain::OnScriptRemoveClick)
	EVT_COMBOBOX(ID_SCRIPT_NAME, frmMain::OnScriptSelected)
	
	//Other
	EVT_CLOSE(frmMain::OnCloseWindow)
	EVT_BUTTON(wxID_ABOUT, frmMain::OnAboutClick)
	EVT_BUTTON(wxID_HELP, frmMain::OnHelpClick)
	EVT_BUTTON(wxID_APPLY, frmMain::OnSettingsApplyClick)
	EVT_BUTTON(ID_SETTINGS_FONT, frmMain::OnSettingsFontClick)
	EVT_AUINOTEBOOK_PAGE_CHANGED(ID_AUINOTEBOOK, frmMain::OnTabChanged)
	
	//Menu
	EVT_MENU(ID_MENU_FILEEXCLUDE_EXTENSION, frmMain::OnMenuFileExcludeExtensionClick)
	EVT_MENU(ID_MENU_FILEEXCLUDE_NAME, frmMain::OnMenuFileExcludeNameClick)
	EVT_MENU(ID_MENU_LOCATIONINCLUDE_EXTENSION, frmMain::OnMenuLocationIncludeExtensionClick)
	EVT_MENU(ID_MENU_LOCATIONINCLUDE_NAME, frmMain::OnMenuLocationIncludeNameClick)
	EVT_MENU(ID_MENU_FOLDEREXCLUDE_NAME, frmMain::OnMenuFolderExcludeNameClick)
END_EVENT_TABLE()

//Constructor
frmMain::frmMain(){
	//Set up the sizes and so forth
	int height, width;
	wxClientDisplayRect(NULL, NULL, &width, &height);

	wxPoint position((int)(wxGetApp().m_Settings->GetX() * width), (int)(wxGetApp().m_Settings->GetY() * height));
	if(position.x < 0 || position.y < 0 || position.x > width || position.y > height){
		position = wxDefaultPosition;
	}
	wxSize size((int)(wxGetApp().m_Settings->GetWidth() * width), (int)(wxGetApp().m_Settings->GetHeight() * height));
	long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxMAXIMIZE|wxMINIMIZE_BOX|wxMAXIMIZE_BOX|wxCLOSE_BOX;

	Init();
	wxFrame::Create(NULL, ID_AUIFRAME, wxT("Toucan"), position, size, style);
#if defined(__WXMSW__) && !defined(__MINGW32__)
	m_TaskBarId = RegisterWindowMessage(wxT("TaskbarButtonCreated"));
#endif
	CreateControls();
}

//Destructor
frmMain::~frmMain(){
	delete m_Font;
	delete m_BackupLocations;
	delete m_SecureLocations;
	m_auiManager.UnInit();
}

//frmMain initialisation
void frmMain::Init(){
	m_Notebook = NULL;
	SyncTopSizer = NULL;
	m_Sync_Job_Select = NULL;
	m_Sync_Rules = NULL;
	m_Sync_Source_Txt = NULL;
	m_Sync_Source_Tree = NULL;
	m_Sync_Dest_Txt = NULL;
	m_Sync_Dest_Tree = NULL;
	m_Sync_Function = NULL;
	m_SyncCheckSize = NULL;
	m_SyncCheckTime = NULL;
	m_SyncCheckShort = NULL;
	m_SyncCheckFull = NULL;
	m_Sync_Timestamp = NULL;
	m_Sync_Attributes = NULL;
	m_Sync_Ignore_Readonly = NULL;
	m_Sync_Recycle = NULL;
	m_SyncPreviewChanges = NULL;
	BackupTopSizer = NULL;
	m_Backup_Job_Select = NULL;
	m_Backup_Rules = NULL;
	m_Backup_Location = NULL;
	m_Backup_Location_Txt = NULL;
	m_Backup_DirCtrl = NULL;
	m_Backup_TreeCtrl = NULL;
	m_Backup_Function = NULL;
	m_Backup_Format = NULL;
	m_Backup_Ratio = NULL;
	m_Backup_IsPass = NULL;
	m_Backup_Test = NULL;
	m_BackupSolid = NULL;
	SecureTopSizer = NULL;
	m_Secure_Rules = NULL;
	m_Secure_Job_Select = NULL;
	m_Secure_DirCtrl = NULL;
	m_Secure_TreeCtrl = NULL;
	m_Secure_Function = NULL;
	m_Rules_Name = NULL;
	m_RulesGrid = NULL;
	m_Script_Styled = NULL;

	menuRules = NULL;

#if defined(__WXMSW__) && !defined(__MINGW32__)
	m_Taskbar = NULL;
#endif

	m_Font = new wxFont();
	m_BackupLocations = new wxArrayString();
	m_SecureLocations = new wxArrayString();

    m_RulesChoices.Add(_("File Include"));
    m_RulesChoices.Add(_("File Exclude"));
    m_RulesChoices.Add(_("Folder Include"));
    m_RulesChoices.Add(_("Folder Exclude"));
    m_RulesChoices.Add(_("Absolute Folder Exclude"));

    m_RulesTypeChoices.Add(_("Simple"));
    m_RulesTypeChoices.Add(_("Regex"));
    m_RulesTypeChoices.Add(_("Size"));
    m_RulesTypeChoices.Add(_("Date"));
}

void frmMain::CreateControls(){	
	//Set up the border size based on the users preference and the size of the screen
	int height, width, border;
	wxClientDisplayRect(NULL, NULL, &width, &height);
	if(width < 850 || height < 500 || wxGetApp().m_Settings->GetSmallBorders()){
		border = 2;
		SetMinSize(wxSize(750, 450));
	}
	else{
		border = 5;
		SetMinSize(wxSize(820, 540));
	}

	//Set the font from the settings
	m_Font->SetNativeFontInfo(wxGetApp().m_Settings->GetFont());
	this->SetFont(*m_Font);

	//Set the form up as managed by AUI
	m_auiManager.SetManagedWindow(this);

	//Create the root AUI notebook
	m_Notebook = new wxAuiNotebook(this, ID_AUINOTEBOOK, wxDefaultPosition, wxDefaultSize, wxAUI_NB_SCROLL_BUTTONS|wxNO_BORDER);

	//Sync
	wxPanel* SyncPanel = new wxPanel(m_Notebook, ID_PANEL_SYNC, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL);
	wxBoxSizer* SyncSizer = new wxBoxSizer(wxVERTICAL);
	SyncPanel->SetSizer(SyncSizer);

	SyncTopSizer = new wxBoxSizer(wxHORIZONTAL);
	SyncSizer->Add(SyncTopSizer, 0, wxEXPAND|wxALL, border);

	//Sync - Jobs
	wxBoxSizer* SyncJobsRulesSizer = new wxBoxSizer(wxVERTICAL);
	SyncTopSizer->Add(SyncJobsRulesSizer, 0, wxGROW|wxALL, 0);	

	wxStaticBox* SyncJobs = new wxStaticBox(SyncPanel, wxID_ANY, _("Job Name"));
	wxStaticBoxSizer* SyncJobsSizer = new wxStaticBoxSizer(SyncJobs, wxHORIZONTAL);
	SyncJobsRulesSizer->Add(SyncJobsSizer, 0, wxALIGN_TOP|wxALL, border);

	wxArrayString syncjobs = GetJobs(wxT("Sync"));
	m_Sync_Job_Select = new wxComboBox(SyncPanel, ID_SYNC_JOB_SELECT, _T(""), wxDefaultPosition, wxDefaultSize, syncjobs, wxCB_DROPDOWN|wxCB_READONLY);
	SyncJobsSizer->Add(m_Sync_Job_Select, 0, wxALIGN_CENTER_VERTICAL|wxALL, border);

	wxBitmapButton* SyncJobSave = new wxBitmapButton(SyncPanel, ID_SYNC_JOB_SAVE, GetBitmapResource(wxT("save.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	SyncJobsSizer->Add(SyncJobSave, 0, wxALIGN_CENTER_VERTICAL|wxALL, border);

	wxBitmapButton* SyncJobAdd = new wxBitmapButton(SyncPanel, ID_SYNC_JOB_ADD, GetBitmapResource(wxT("add.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	SyncJobsSizer->Add(SyncJobAdd, 0, wxALIGN_CENTER_VERTICAL|wxALL, border);

	wxBitmapButton* SyncJobRemove = new wxBitmapButton(SyncPanel, ID_SYNC_JOB_REMOVE, GetBitmapResource(wxT("remove.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	SyncJobsSizer->Add(SyncJobRemove, 0, wxALIGN_CENTER_VERTICAL|wxALL, border);

	//Sync - Rules
	wxStaticBox* SyncRules = new wxStaticBox(SyncPanel, wxID_ANY, _("Rules"));
	wxStaticBoxSizer* SyncRulesSizer = new wxStaticBoxSizer(SyncRules, wxHORIZONTAL);
	SyncJobsRulesSizer->Add(SyncRulesSizer, 0, wxALIGN_TOP|wxALL|wxGROW, border);

	m_Sync_Rules = new wxComboBox(SyncPanel, ID_SYNC_RULES, _T(""), wxDefaultPosition, wxDefaultSize, GetRules(), wxCB_DROPDOWN|wxCB_READONLY);
	SyncRulesSizer->Add(m_Sync_Rules, 1, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, border);

	//Sync - Options
	wxArrayString m_Sync_FunctionStrings;
	m_Sync_FunctionStrings.Add(_("Copy"));
	m_Sync_FunctionStrings.Add(_("Mirror"));
	m_Sync_FunctionStrings.Add(_("Equalise"));
	m_Sync_FunctionStrings.Add(_("Move"));
	m_Sync_FunctionStrings.Add(_("Clean"));
	m_Sync_Function = new wxRadioBox(SyncPanel, ID_SYNC_FUNCTION, _("Function"), wxDefaultPosition, wxDefaultSize, m_Sync_FunctionStrings, 5, wxRA_SPECIFY_ROWS);
	m_Sync_Function->SetSelection(0);
	SyncTopSizer->Add(m_Sync_Function, 0, wxALIGN_TOP|wxALL, border);

	wxStaticBox* SyncChecks = new wxStaticBox(SyncPanel, wxID_ANY, _("File Checks"));
	wxStaticBoxSizer* SyncChecksSizer = new wxStaticBoxSizer(SyncChecks, wxVERTICAL);
	SyncTopSizer->Add(SyncChecksSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, border);

	m_SyncCheckSize = new wxCheckBox(SyncPanel, ID_SYNC_CHECK_SIZE, _("File size"));
	m_SyncCheckSize->SetValue(false);
	SyncChecksSizer->Add(m_SyncCheckSize, 0, wxALIGN_LEFT|wxALL, border);

	m_SyncCheckTime = new wxCheckBox(SyncPanel, ID_SYNC_CHECK_TIME, _("Modified time"));
	m_SyncCheckTime->SetValue(false);
	SyncChecksSizer->Add(m_SyncCheckTime, 0, wxALIGN_LEFT|wxALL, border);

	m_SyncCheckShort = new wxCheckBox(SyncPanel, ID_SYNC_CHECK_SHORT, _("Short Comparison"));
	m_SyncCheckShort->SetValue(false);
	SyncChecksSizer->Add(m_SyncCheckShort, 0, wxALIGN_LEFT|wxALL, border);

	m_SyncCheckFull = new wxCheckBox(SyncPanel, ID_SYNC_CHECK_FULL, _("Full Comparison"));
	m_SyncCheckFull->SetValue(false);
	SyncChecksSizer->Add(m_SyncCheckFull, 0, wxALIGN_LEFT|wxALL, border);

	wxStaticBox* SyncOther = new wxStaticBox(SyncPanel, wxID_ANY, _("Other"));
	wxStaticBoxSizer* SyncOtherSizer = new wxStaticBoxSizer(SyncOther, wxVERTICAL);
	SyncTopSizer->Add(SyncOtherSizer, 0, wxALIGN_TOP|wxALL, border);

	m_Sync_Timestamp = new wxCheckBox(SyncPanel, ID_SYNC_TIMESTAMP, _("Retain Timestamps"));
	m_Sync_Timestamp->SetValue(false);
	SyncOtherSizer->Add(m_Sync_Timestamp, 0, wxALIGN_LEFT|wxALL, border);

	m_Sync_Attributes = new wxCheckBox(SyncPanel, ID_SYNC_ATTRIB, _("Retain Attributes"));
	m_Sync_Attributes->SetValue(false);
	SyncOtherSizer->Add(m_Sync_Attributes, 0, wxALIGN_LEFT|wxALL, border);

	m_Sync_Ignore_Readonly = new wxCheckBox(SyncPanel, ID_SYNC_IGNORERO, _("Ignore Read-Only"));
	m_Sync_Ignore_Readonly->SetValue(false);
	SyncOtherSizer->Add(m_Sync_Ignore_Readonly, 0, wxALIGN_LEFT|wxALL, border);

	m_Sync_Recycle = new wxCheckBox(SyncPanel, ID_SYNC_RECYCLE, _("Use Recycle Bin"));
	m_Sync_Recycle->SetValue(false);
	SyncOtherSizer->Add(m_Sync_Recycle, 0, wxALIGN_LEFT|wxALL, border);

	m_SyncPreviewChanges = new wxCheckBox(SyncPanel, ID_SYNC_PREVIEW_CHANGES, _("Preview Only Changes"));
	m_SyncPreviewChanges->SetValue(false);
	SyncOtherSizer->Add(m_SyncPreviewChanges, 0, wxALIGN_LEFT|wxALL, border);

	wxBoxSizer* SyncButtonsSizer = new wxBoxSizer(wxVERTICAL);
	SyncTopSizer->Add(SyncButtonsSizer, 1, wxGROW|wxALL|wxALIGN_CENTER_VERTICAL, border);	

	wxButton* SyncRunButton = new wxButton(SyncPanel, ID_SYNC_RUN, _("Run"));
	SyncButtonsSizer->Add(SyncRunButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, border);

	wxButton* SyncPreviewButton = new wxButton(SyncPanel, ID_SYNC_PREVIEW , _("Preview"));
	SyncButtonsSizer->Add(SyncPreviewButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, border);

	wxButton* SyncRefreshButton = new wxButton(SyncPanel, ID_SYNC_REFRESH , _("Refresh"));
	SyncButtonsSizer->Add(SyncRefreshButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, border);

	//Sync - Main
	wxGridBagSizer* SyncMainSizer = new wxGridBagSizer(0, 0);
	SyncMainSizer->SetCols(4);
	SyncMainSizer->SetRows(3);
	SyncMainSizer->AddGrowableCol(0);
	SyncMainSizer->AddGrowableCol(2);
	SyncMainSizer->AddGrowableRow(2);
	SyncSizer->Add(SyncMainSizer, 1, wxEXPAND|wxALL, border);

	wxStaticText* SyncSourceText = new wxStaticText(SyncPanel, wxID_STATIC, _("Source"));
	SyncMainSizer->Add(SyncSourceText, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALL, border);

	m_Sync_Source_Txt = new wxTextCtrl(SyncPanel, ID_SYNC_SOURCE_TXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	SyncMainSizer->Add(m_Sync_Source_Txt, wxGBPosition(1, 0), wxGBSpan(1, 1), wxEXPAND|wxALL, border);

	wxButton* SyncSourceButton = new wxButton(SyncPanel, ID_SYNC_SOURCE_BTN, wxT("..."), wxDefaultPosition, wxSize(25, -1));
	SyncMainSizer->Add(SyncSourceButton, wxGBPosition(1, 1), wxGBSpan(1, 1), wxALL, border);

	m_Sync_Source_Tree = new SyncPreviewDirCtrl(SyncPanel, ID_SYNC_SOURCE_TREE, SYNC_SOURCE);
	SyncMainSizer->Add(m_Sync_Source_Tree, wxGBPosition(2, 0), wxGBSpan(1, 1), wxEXPAND|wxALL, border);

	wxBoxSizer* SyncSourceButtonSizer = new wxBoxSizer(wxVERTICAL);
	SyncMainSizer->Add(SyncSourceButtonSizer, wxGBPosition(2, 1), wxGBSpan(1, 1), wxALIGN_CENTRE_VERTICAL|wxALL, border);

	wxBitmapButton* SyncSourceExpand = new wxBitmapButton(SyncPanel, ID_SYNC_SOURCE_EXPAND, GetBitmapResource(wxT("expandall.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	SyncSourceButtonSizer->Add(SyncSourceExpand, 0, wxALIGN_CENTRE_VERTICAL|wxALL, 0);

	wxBitmapButton* SyncSourceInsert = new wxBitmapButton(SyncPanel, ID_SYNC_SOURCE_INSERT, GetBitmapResource(wxT("addvar.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	SyncSourceButtonSizer->Add(SyncSourceInsert, 0, wxALIGN_CENTRE_VERTICAL|wxTOP, border);

	wxStaticText* SyncDestText = new wxStaticText(SyncPanel, wxID_STATIC, _("Destination"));
	SyncMainSizer->Add(SyncDestText, wxGBPosition(0, 2), wxGBSpan(1, 1), wxALL, border);

	m_Sync_Dest_Txt = new wxTextCtrl(SyncPanel, ID_SYNC_DEST_TXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	SyncMainSizer->Add(m_Sync_Dest_Txt, wxGBPosition(1, 2), wxGBSpan(1, 1), wxEXPAND|wxALL, border);

	wxButton* SyncDestButton = new wxButton(SyncPanel, ID_SYNC_DEST_BTN, wxT("..."), wxDefaultPosition, wxSize(25, -1));
	SyncMainSizer->Add(SyncDestButton, wxGBPosition(1, 3), wxGBSpan(1, 1), wxALL, border);

	m_Sync_Dest_Tree = new SyncPreviewDirCtrl(SyncPanel, ID_SYNC_DEST_TREE, SYNC_DEST);
	SyncMainSizer->Add(m_Sync_Dest_Tree, wxGBPosition(2, 2), wxGBSpan(1, 1), wxEXPAND|wxALL, border);

	wxBoxSizer* SyncDestButtonSizer = new wxBoxSizer(wxVERTICAL);
	SyncMainSizer->Add(SyncDestButtonSizer, wxGBPosition(2, 3), wxGBSpan(1, 1), wxALIGN_CENTRE_VERTICAL|wxALL, border);

	wxBitmapButton* SyncDestExpand = new wxBitmapButton(SyncPanel, ID_SYNC_DEST_EXPAND, GetBitmapResource(wxT("expandall.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	SyncDestButtonSizer->Add(SyncDestExpand, 0, wxALIGN_CENTRE_VERTICAL|wxALL, 0);

	wxBitmapButton* SyncDestInsert = new wxBitmapButton(SyncPanel, ID_SYNC_DEST_INSERT, GetBitmapResource(wxT("addvar.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	SyncDestButtonSizer->Add(SyncDestInsert, 0, wxALIGN_CENTRE_VERTICAL|wxTOP, border);

	//Backup
	wxPanel* BackupPanel = new wxPanel(m_Notebook, ID_PANEL_BACKUP, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL);
	wxBoxSizer* BackupSizer = new wxBoxSizer(wxVERTICAL);
	BackupPanel->SetSizer(BackupSizer);

	BackupTopSizer = new wxBoxSizer(wxHORIZONTAL);
	BackupSizer->Add(BackupTopSizer, 0, wxALIGN_LEFT|wxALL, border);

	//Backup - Jobs
	wxBoxSizer* BackupJobsRulesSizer = new wxBoxSizer(wxVERTICAL);
	BackupTopSizer->Add(BackupJobsRulesSizer, 0, wxALIGN_LEFT|wxALL, 0);

	wxStaticBox* BackupJobs= new wxStaticBox(BackupPanel, wxID_ANY, _("Job Name"));
	wxStaticBoxSizer* BackupJobsSizer = new wxStaticBoxSizer(BackupJobs, wxHORIZONTAL);
	BackupJobsRulesSizer->Add(BackupJobsSizer, 0, wxALIGN_TOP|wxALL, border);

	wxArrayString backupjobs = GetJobs(wxT("Backup"));
	m_Backup_Job_Select = new wxComboBox(BackupPanel, ID_BACKUP_JOB_SELECT, _T(""), wxDefaultPosition, wxDefaultSize, backupjobs, wxCB_DROPDOWN|wxCB_READONLY);
	BackupJobsSizer->Add(m_Backup_Job_Select, 0, wxALIGN_CENTER_VERTICAL|wxALL, border);

	wxBitmapButton* BackupJobSave = new wxBitmapButton(BackupPanel, ID_BACKUP_JOB_SAVE, GetBitmapResource(wxT("save.png")));
	BackupJobsSizer->Add(BackupJobSave, 0, wxALIGN_CENTER_VERTICAL|wxALL, border);

	wxBitmapButton* BackupJobAdd = new wxBitmapButton(BackupPanel, ID_BACKUP_JOB_ADD, GetBitmapResource(wxT("add.png")));
	BackupJobsSizer->Add(BackupJobAdd, 0, wxALIGN_CENTER_VERTICAL|wxALL, border);

	wxBitmapButton* BackupJobRemove = new wxBitmapButton(BackupPanel, ID_BACKUP_JOB_REMOVE, GetBitmapResource(wxT("remove.png")));
	BackupJobsSizer->Add(BackupJobRemove, 0, wxALIGN_CENTER_VERTICAL|wxALL, border);

	//Backup - Rules
	wxStaticBox* BackupRules = new wxStaticBox(BackupPanel, wxID_ANY, _("Rules"));
	wxStaticBoxSizer* BackupRulesSizer = new wxStaticBoxSizer(BackupRules, wxHORIZONTAL);
	BackupJobsRulesSizer->Add(BackupRulesSizer, 0, wxALL|wxEXPAND, border);

	m_Backup_Rules = new wxComboBox(BackupPanel, ID_BACKUP_RULES, _T(""), wxDefaultPosition, wxDefaultSize, GetRules(), wxCB_DROPDOWN|wxCB_READONLY);
	BackupRulesSizer->Add(m_Backup_Rules, 1, wxALL, border);

	//Backup - Options
	wxArrayString m_Backup_FunctionStrings;
	m_Backup_FunctionStrings.Add(_("Complete"));
	m_Backup_FunctionStrings.Add(_("Update"));
	m_Backup_FunctionStrings.Add(_("Differential"));
	m_Backup_FunctionStrings.Add(_("Restore"));
	m_Backup_Function = new wxRadioBox(BackupPanel, ID_BACKUP_FUNCTION, _("Type"), wxDefaultPosition, wxDefaultSize, m_Backup_FunctionStrings, 1, wxRA_SPECIFY_COLS);
	m_Backup_Function->SetSelection(0);
	BackupTopSizer->Add(m_Backup_Function, 0, wxALIGN_TOP|wxALL, border);

	wxArrayString m_Backup_FormatStrings;
	m_Backup_FormatStrings.Add(wxT("Zip"));
	m_Backup_FormatStrings.Add(wxT("7-Zip"));
	m_Backup_FormatStrings.Add(wxT("GZip"));
	m_Backup_Format = new wxRadioBox(BackupPanel, ID_BACKUP_FORMAT, _("Format"), wxDefaultPosition, wxDefaultSize, m_Backup_FormatStrings, 1, wxRA_SPECIFY_COLS);
	m_Backup_Format->SetSelection(0);
	BackupTopSizer->Add(m_Backup_Format, 0, wxALIGN_TOP|wxALL, border);

	wxStaticBox* BackupRatio = new wxStaticBox(BackupPanel, wxID_ANY, _("Compression Level"));
	wxStaticBoxSizer* BackupRatioSizer = new wxStaticBoxSizer(BackupRatio, wxVERTICAL);
	BackupTopSizer->Add(BackupRatioSizer, 0, wxALIGN_TOP|wxALL, border);

	m_Backup_Ratio_Text = new wxStaticText(BackupPanel, wxID_STATIC, _("Default"));
	BackupRatioSizer->Add(m_Backup_Ratio_Text);

	m_Backup_Ratio = new wxSlider(BackupPanel, ID_BACKUP_RATIO, 3, 0, 5);
	int ratiox;
	m_Backup_Ratio->GetTextExtent(_("Compression Level"), &ratiox, NULL);
	m_Backup_Ratio->SetMinSize(wxSize(ratiox + 10, -1));
	BackupRatioSizer->Add(m_Backup_Ratio);

	wxStaticBox* BackupOther = new wxStaticBox(BackupPanel, wxID_ANY, _("Other"));
	wxStaticBoxSizer* BackupOtherSizer = new wxStaticBoxSizer(BackupOther, wxVERTICAL);
	BackupTopSizer->Add(BackupOtherSizer, 0, wxALIGN_TOP|wxALL, border);

	m_Backup_IsPass = new wxCheckBox(BackupPanel, ID_BACKUP_ISPASS, _("Password"));
	BackupOtherSizer->Add(m_Backup_IsPass, 0, wxALIGN_TOP|wxALL, border);

	m_Backup_Test = new wxCheckBox(BackupPanel, ID_BACKUP_TEST, _("Test After Creation"));
	BackupOtherSizer->Add(m_Backup_Test, 0, wxALIGN_TOP|wxALL, border);

	m_BackupSolid = new wxCheckBox(BackupPanel, ID_BACKUP_SOLID, _("Solid Mode"));
	BackupOtherSizer->Add(m_BackupSolid, 0, wxALIGN_TOP|wxALL, border);

	wxBoxSizer* BackupButtonsSizer = new wxBoxSizer(wxVERTICAL);
	BackupTopSizer->Add(BackupButtonsSizer, 1, wxGROW|wxALL|wxALIGN_CENTER_VERTICAL, border);	

	wxButton* BackupRunButton = new wxButton(BackupPanel, ID_BACKUP_RUN, _("Run"));
	BackupButtonsSizer->Add(BackupRunButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, border);

	wxButton* BackupRefreshButton = new wxButton(BackupPanel, ID_BACKUP_REFRESH, _("Refresh"));
	BackupButtonsSizer->Add(BackupRefreshButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, border);

	//Backup - Main
	wxGridBagSizer* BackupMainSizer = new wxGridBagSizer(0, 0);
	BackupMainSizer->SetCols(4);
	BackupMainSizer->SetRows(4);
	BackupMainSizer->AddGrowableCol(0);
	BackupMainSizer->AddGrowableCol(2);
	BackupMainSizer->AddGrowableRow(3);
	BackupSizer->Add(BackupMainSizer, 1, wxEXPAND|wxALL, border);

	wxStaticText* BackupComputerStatic = new wxStaticText(BackupPanel, wxID_ANY, _("Computer"));
	BackupMainSizer->Add(BackupComputerStatic, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALL, border);

	m_Backup_DirCtrl = new LocalDirCtrl(BackupPanel, ID_BACKUP_DIRCTRL);
	BackupMainSizer->Add(m_Backup_DirCtrl, wxGBPosition(1, 0), wxGBSpan(3, 1), wxEXPAND|wxALL, border);

	wxBoxSizer* BackupAddRemoveSizer = new wxBoxSizer(wxVERTICAL);
	BackupMainSizer->Add(BackupAddRemoveSizer, wxGBPosition(1, 1), wxGBSpan(3, 1), wxALIGN_CENTRE_VERTICAL|wxALL, 0);

	wxBitmapButton*BackupAdd = new wxBitmapButton(BackupPanel, ID_BACKUP_ADD, GetBitmapResource(wxT("add.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	BackupAddRemoveSizer->Add(BackupAdd, 0, wxALL, border);

	wxBitmapButton* BackupRemove = new wxBitmapButton(BackupPanel, ID_BACKUP_REMOVE, GetBitmapResource(wxT("remove.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	BackupAddRemoveSizer->Add(BackupRemove, 0, wxALL, border);

	wxStaticText* BackupLocationStatic = new wxStaticText(BackupPanel, ID_BACKUP_LOCATIONSTATIC, _("Backup Location"));
	BackupMainSizer->Add(BackupLocationStatic, wxGBPosition(0, 2), wxGBSpan(1, 1), wxALL, border);

	m_Backup_Location = new wxTextCtrl(BackupPanel, ID_BACKUP_LOCATION_TEXT);
	BackupMainSizer->Add(m_Backup_Location, wxGBPosition(1, 2), wxGBSpan(1, 1), wxEXPAND|wxALL, border);

	wxButton* BackupLocationButton = new wxButton(BackupPanel, ID_BACKUP_LOCATION, wxT("..."), wxDefaultPosition, wxSize(25, -1), 0);
	BackupMainSizer->Add(BackupLocationButton, wxGBPosition(1, 3), wxGBSpan(1, 1), wxALL, border);

	wxStaticText* BackupFilesStatic = new wxStaticText(BackupPanel, ID_BACKUP_FILESSTATIC, _("Files to Backup"));
	BackupMainSizer->Add(BackupFilesStatic, wxGBPosition(2, 2), wxGBSpan(1, 1), wxALL, border);

	m_Backup_TreeCtrl = new PreviewDirCtrl(BackupPanel, ID_BACKUP_TREECTRL);
	BackupMainSizer->Add(m_Backup_TreeCtrl, wxGBPosition(3, 2), wxGBSpan(1, 1), wxEXPAND|wxALL, border);

	wxBoxSizer* BackupAddExpandSizer = new wxBoxSizer(wxVERTICAL);
	BackupMainSizer->Add(BackupAddExpandSizer, wxGBPosition(3, 3), wxGBSpan(1, 1), wxALIGN_CENTRE_VERTICAL|wxALL, border);

	wxBitmapButton* BackupAddVar = new wxBitmapButton(BackupPanel, ID_BACKUP_ADDVAR, GetBitmapResource(wxT("addvar.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	BackupAddExpandSizer->Add(BackupAddVar, 0, wxBOTTOM, border);

	wxBitmapButton* BackupExpand = new wxBitmapButton(BackupPanel, ID_BACKUP_EXPAND, GetBitmapResource(wxT("expandall.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	BackupAddExpandSizer->Add(BackupExpand, 0, wxTOP|wxBOTTOM, border);

	//Secure
	wxPanel* SecurePanel = new wxPanel(m_Notebook, ID_PANEL_SECURE, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL);
	wxBoxSizer* SecureSizer = new wxBoxSizer(wxVERTICAL);
	SecurePanel->SetSizer(SecureSizer);

	SecureTopSizer = new wxBoxSizer(wxHORIZONTAL);
	SecureSizer->Add(SecureTopSizer, 0, wxALIGN_LEFT|wxALL, border);

	//Secure - Jobs
	wxBoxSizer* SecureJobsRulesSizer = new wxBoxSizer(wxVERTICAL);
	SecureTopSizer->Add(SecureJobsRulesSizer, 0, wxGROW|wxALL, 0);	

	wxStaticBox* SecureJobs = new wxStaticBox(SecurePanel, wxID_ANY, _("Job Name"));
	wxStaticBoxSizer* SecureJobsSizer = new wxStaticBoxSizer(SecureJobs, wxHORIZONTAL);
	SecureJobsRulesSizer->Add(SecureJobsSizer, 0, wxALIGN_TOP|wxALL, border);

	wxArrayString securejobs = GetJobs(wxT("Secure"));
	m_Secure_Job_Select = new wxComboBox(SecurePanel, ID_SECURE_JOB_SELECT, _T(""), wxDefaultPosition, wxDefaultSize, securejobs, wxCB_DROPDOWN|wxCB_READONLY);
	SecureJobsSizer->Add(m_Secure_Job_Select, 0, wxALIGN_CENTER_VERTICAL|wxALL, border);

	wxBitmapButton* SecureJobSave = new wxBitmapButton(SecurePanel, ID_SECURE_JOB_SAVE, GetBitmapResource(wxT("save.png")));
	SecureJobsSizer->Add(SecureJobSave, 0, wxALIGN_CENTER_VERTICAL|wxALL, border);

	wxBitmapButton* SecureJobAdd = new wxBitmapButton(SecurePanel, ID_SECURE_JOB_ADD, GetBitmapResource(wxT("add.png")));
	SecureJobsSizer->Add(SecureJobAdd, 0, wxALIGN_CENTER_VERTICAL|wxALL, border);

	wxBitmapButton* SecureJobRemove = new wxBitmapButton(SecurePanel, ID_SECURE_JOB_REMOVE, GetBitmapResource(wxT("remove.png")));
	SecureJobsSizer->Add(SecureJobRemove, 0, wxALIGN_CENTER_VERTICAL|wxALL, border);

	//Secure - Rules
	wxStaticBox* SecureRules = new wxStaticBox(SecurePanel, wxID_ANY, _("Rules"));
	wxStaticBoxSizer* SecureRulesSizer = new wxStaticBoxSizer(SecureRules, wxHORIZONTAL);
	SecureJobsRulesSizer->Add(SecureRulesSizer, 1, wxALIGN_TOP|wxALL|wxEXPAND, border);

	m_Secure_Rules = new wxComboBox(SecurePanel, ID_SECURE_RULES, _T(""), wxDefaultPosition, wxDefaultSize, GetRules(), wxCB_DROPDOWN|wxCB_READONLY);
	SecureRulesSizer->Add(m_Secure_Rules, 1, wxALIGN_CENTER_VERTICAL|wxALL, border);

	//Secure - Options
	wxArrayString m_Secure_FunctionStrings;
	m_Secure_FunctionStrings.Add(_("Encrypt"));
	m_Secure_FunctionStrings.Add(_("Decrypt"));
	m_Secure_Function = new wxRadioBox(SecurePanel, ID_SECURE_FUNCTION, _("Function"), wxDefaultPosition, wxDefaultSize, m_Secure_FunctionStrings, 1, wxRA_SPECIFY_COLS);
	m_Secure_Function->SetSelection(0);
	SecureTopSizer->Add(m_Secure_Function, 0, wxALIGN_TOP|wxALL, border);

	wxBoxSizer* SecureButtonsSizer = new wxBoxSizer(wxVERTICAL);
	SecureTopSizer->Add(SecureButtonsSizer, 1, wxGROW|wxALL|wxALIGN_CENTER_VERTICAL, border);	

	wxButton* SecureRunButton = new wxButton(SecurePanel, ID_SECURE_RUN, _("Run"));
	SecureButtonsSizer->Add(SecureRunButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, border);

	wxButton* SecureRefreshButton = new wxButton(SecurePanel, ID_SECURE_REFRESH, _("Refresh"));
	SecureButtonsSizer->Add(SecureRefreshButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, border);

	//Secure - Main
	wxGridBagSizer* SecureMainSizer = new wxGridBagSizer(0, 0);
	SecureMainSizer->SetCols(4);
	SecureMainSizer->SetRows(2);
	SecureMainSizer->AddGrowableCol(0);
	SecureMainSizer->AddGrowableCol(2);
	SecureMainSizer->AddGrowableRow(1);
	SecureSizer->Add(SecureMainSizer, 1, wxEXPAND|wxALL, border);
	
	wxStaticText* SecureComputerStatic = new wxStaticText(SecurePanel, wxID_ANY, _("Computer"));
	SecureMainSizer->Add(SecureComputerStatic, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALL, border);

	m_Secure_DirCtrl = new LocalDirCtrl(SecurePanel, ID_SECURE_DIRCTRL);
	SecureMainSizer->Add(m_Secure_DirCtrl, wxGBPosition(1, 0), wxGBSpan(1, 1), wxEXPAND|wxALL, border);

	wxBoxSizer* SecureAddRemoveSizer = new wxBoxSizer(wxVERTICAL);
	SecureMainSizer->Add(SecureAddRemoveSizer, wxGBPosition(1, 1), wxGBSpan(1, 1), wxALIGN_CENTRE_VERTICAL|wxALL, 0);

	wxBitmapButton* SecureAdd = new wxBitmapButton(SecurePanel, ID_SECURE_ADD, GetBitmapResource(wxT("add.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	SecureAddRemoveSizer->Add(SecureAdd, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, border);

	wxBitmapButton* SecureRemove = new wxBitmapButton(SecurePanel, ID_SECURE_REMOVE, GetBitmapResource(wxT("remove.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	SecureAddRemoveSizer->Add(SecureRemove, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, border);
	
	wxStaticText* SecureFilesStatic = new wxStaticText(SecurePanel, ID_SECURE_FILESSATATIC, _("Files to Encrypt"));
	SecureMainSizer->Add(SecureFilesStatic, wxGBPosition(0, 2), wxGBSpan(1, 1), wxALL, border);

	m_Secure_TreeCtrl = new PreviewDirCtrl(SecurePanel, ID_SECURE_TREECTRL);
	SecureMainSizer->Add(m_Secure_TreeCtrl, wxGBPosition(1, 2), wxGBSpan(1, 1), wxEXPAND|wxALL, border);

	wxBoxSizer* SecureAddExpandSizer = new wxBoxSizer(wxVERTICAL);
	SecureMainSizer->Add(SecureAddExpandSizer, wxGBPosition(1, 3), wxGBSpan(1, 1), wxALIGN_CENTRE_VERTICAL|wxALL, border);

	wxBitmapButton* SecureAddVar = new wxBitmapButton(SecurePanel, ID_SECURE_ADDVAR, GetBitmapResource(wxT("addvar.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	SecureAddExpandSizer->Add(SecureAddVar, 0, wxBOTTOM, border);

	wxBitmapButton* SecureExpand = new wxBitmapButton(SecurePanel, ID_SECURE_EXPAND, GetBitmapResource(wxT("expandall.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	SecureAddExpandSizer->Add(SecureExpand, 0, wxTOP|wxBOTTOM, border);

	//Rules
	wxPanel* RulesPanel = new wxPanel(m_Notebook, ID_PANEL_RULES, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL);
	wxBoxSizer* RulesSizer = new wxBoxSizer(wxVERTICAL);
	RulesPanel->SetSizer(RulesSizer);

	//Rules - Name
	wxStaticBox* RulesName = new wxStaticBox(RulesPanel, wxID_ANY, _("Name"));
	RulesNameSizer = new wxStaticBoxSizer(RulesName, wxHORIZONTAL);
	RulesSizer->Add(RulesNameSizer, 0, wxALIGN_TOP|wxTOP|wxLEFT, 2 * border);

	m_Rules_Name = new wxComboBox(RulesPanel, ID_RULES_COMBO, _T(""), wxDefaultPosition, wxDefaultSize, GetRules(), wxCB_READONLY);
	RulesNameSizer->Add(m_Rules_Name, 0, wxALIGN_CENTER_VERTICAL|wxALL, border);

	wxBitmapButton* RulesNameSave = new wxBitmapButton(RulesPanel, ID_RULES_SAVE, GetBitmapResource(wxT("save.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	RulesNameSizer->Add(RulesNameSave, 0, wxALIGN_CENTER_VERTICAL|wxALL, border);

	wxBitmapButton* RulesNameAdd = new wxBitmapButton(RulesPanel, ID_RULES_ADD, GetBitmapResource(wxT("add.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	RulesNameSizer->Add(RulesNameAdd, 0, wxALIGN_CENTER_VERTICAL|wxALL, border);

	wxBitmapButton* RulesNameRemove = new wxBitmapButton(RulesPanel, ID_RULES_REMOVE, GetBitmapResource(wxT("remove.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	RulesNameSizer->Add(RulesNameRemove, 0, wxALIGN_CENTER_VERTICAL|wxALL, border);

	//Rules - Main
	wxBoxSizer* RulesMainSizer = new wxBoxSizer(wxHORIZONTAL);
	RulesSizer->Add(RulesMainSizer, 1, wxGROW|wxALL, border);

    m_RulesGrid = new RulesGrid(RulesPanel, wxID_ANY);
    RulesMainSizer->Add(m_RulesGrid, 1, wxGROW|wxALL, border);

	wxBoxSizer* RulesRightSizer = new wxBoxSizer(wxVERTICAL);
	RulesMainSizer->Add(RulesRightSizer, 0, wxALIGN_CENTER_VERTICAL|wxALL, border);

	wxBitmapButton* RulesAddItem = new wxBitmapButton(RulesPanel, ID_RULES_ADDITEM, GetBitmapResource(wxT("add.png")));
	RulesRightSizer->Add(RulesAddItem, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, border);

	wxBitmapButton* RulesRemoveItem = new wxBitmapButton(RulesPanel, ID_RULES_REMOVEITEM, GetBitmapResource(wxT("remove.png")));
	RulesRightSizer->Add(RulesRemoveItem, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, border);

	//Variables
	wxPanel* VariablesPanel = new wxPanel(m_Notebook, ID_PANEL_VARIABLES, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL);
	wxBoxSizer* VariablesSizer = new wxBoxSizer(wxVERTICAL);
	VariablesPanel->SetSizer(VariablesSizer);

	VariablesTopSizer = new wxBoxSizer(wxHORIZONTAL);
	VariablesSizer->Add(VariablesTopSizer, 0, wxALIGN_LEFT|wxALL, border);
	
	wxStaticBox* VariablesName = new wxStaticBox(VariablesPanel, wxID_ANY, _("Name"));
	wxStaticBoxSizer* VariablesNameSizer = new wxStaticBoxSizer(VariablesName, wxHORIZONTAL);
	VariablesSizer->Add(VariablesNameSizer, 0, wxALIGN_TOP|wxLEFT, 2 * border);

	m_Variables_Name = new wxComboBox(VariablesPanel, ID_VARIABLES_NAME, _T(""), wxDefaultPosition, wxDefaultSize, GetVariables(false), wxCB_DROPDOWN|wxCB_READONLY);
	VariablesNameSizer->Add(m_Variables_Name, 0, wxALIGN_CENTER_VERTICAL|wxALL, border);

	wxBitmapButton* VariablesNameSave = new wxBitmapButton(VariablesPanel, ID_VARIABLES_SAVE, GetBitmapResource(wxT("save.png")));
	VariablesNameSizer->Add(VariablesNameSave, 0, wxALIGN_CENTER_VERTICAL|wxALL, border);

	wxBitmapButton* VariablesNameAdd = new wxBitmapButton(VariablesPanel, ID_VARIABLES_ADD, GetBitmapResource(wxT("add.png")));
	VariablesNameSizer->Add(VariablesNameAdd, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, border);

	wxBitmapButton* VariablesNameRemove = new wxBitmapButton(VariablesPanel, ID_VARIABLES_REMOVE, GetBitmapResource(wxT("remove.png")));
	VariablesNameSizer->Add(VariablesNameRemove, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, border);

	wxBoxSizer* VariablesMainSizer = new wxBoxSizer(wxHORIZONTAL);
	VariablesSizer->Add(VariablesMainSizer, 1, wxGROW|wxALL, border);

	m_Variables_List = new wxListCtrl(VariablesPanel, ID_VARIABLES_LIST, wxDefaultPosition, wxSize(100, 100), wxLC_REPORT|wxBORDER_THEME);
	VariablesMainSizer->Add(m_Variables_List, 1, wxGROW|wxALL, border);

	wxBoxSizer* VariablesRightSizer = new wxBoxSizer(wxVERTICAL);
	VariablesMainSizer->Add(VariablesRightSizer, 0, wxALIGN_CENTER_VERTICAL|wxALL, border);

	wxBitmapButton* VariablesAddItem = new wxBitmapButton(VariablesPanel, ID_VARIABLES_ADDITEM, GetBitmapResource(wxT("add.png")));
	VariablesRightSizer->Add(VariablesAddItem, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, border);

	wxBitmapButton* VariablesRemoveItem = new wxBitmapButton(VariablesPanel, ID_VARIABLES_REMOVEITEM, GetBitmapResource(wxT("remove.png")));
	VariablesRightSizer->Add(VariablesRemoveItem, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, border);

	//Script
	wxPanel* ScriptPanel = new wxPanel(m_Notebook, ID_PANEL_SCRIPT, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL);
	wxBoxSizer* ScriptSizer = new wxBoxSizer(wxVERTICAL);
	ScriptPanel->SetSizer(ScriptSizer);

	wxStaticBox* ScriptName = new wxStaticBox(ScriptPanel, wxID_ANY, _("Name"));
	ScriptNameSizer = new wxStaticBoxSizer(ScriptName, wxHORIZONTAL);
	ScriptSizer->Add(ScriptNameSizer, 0, wxALIGN_TOP|wxALL, 2 * border);

	m_Script_Name = new wxComboBox(ScriptPanel, ID_SCRIPT_NAME, _T(""), wxDefaultPosition, wxDefaultSize, GetScripts(), wxCB_DROPDOWN|wxCB_READONLY);
	ScriptNameSizer->Add(m_Script_Name, 0, wxALIGN_CENTER_VERTICAL|wxALL, border);
	
	wxBitmapButton* ScriptNameSave = new wxBitmapButton(ScriptPanel, ID_SCRIPT_SAVE, GetBitmapResource(wxT("save.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	ScriptNameSizer->Add(ScriptNameSave, 0, wxALIGN_CENTER_VERTICAL|wxALL, border);

	wxBitmapButton* ScriptNameAdd = new wxBitmapButton(ScriptPanel, ID_SCRIPT_ADD, GetBitmapResource(wxT("add.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	ScriptNameSizer->Add(ScriptNameAdd, 0, wxALIGN_CENTER_VERTICAL|wxALL, border);

	wxBitmapButton* ScriptNameRemove = new wxBitmapButton(ScriptPanel, ID_SCRIPT_REMOVE, GetBitmapResource(wxT("remove.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	ScriptNameSizer->Add(ScriptNameRemove, 0, wxALIGN_CENTER_VERTICAL|wxALL, border);

	wxButton* ScriptRun = new wxButton(ScriptPanel, ID_SCRIPT_RUN, _("Run"), wxDefaultPosition, wxDefaultSize, 0);
	ScriptNameSizer->Add(ScriptRun, 0, wxALIGN_CENTER_VERTICAL|wxALL, border);

	m_Script_Styled = new wxStyledTextCtrl(ScriptPanel, ID_SCRIPT_STYLED, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME);
	m_Script_Styled->StyleSetForeground(wxSTC_LUA_COMMENT, wxColour(wxT("rgb(0, 128, 0)")));
	m_Script_Styled->StyleSetForeground(wxSTC_LUA_COMMENTLINE, wxColour(wxT("rgb(0, 128, 0)")));
	m_Script_Styled->StyleSetForeground(wxSTC_LUA_COMMENTDOC, wxColour(wxT("rgb(0, 128, 0)")));
	m_Script_Styled->StyleSetForeground(wxSTC_LUA_STRING, wxColour(wxT("rgb(163, 21, 21)")));
	m_Script_Styled->StyleSetForeground(wxSTC_LUA_LITERALSTRING, wxColour(wxT("rgb(163, 21, 21)")));
	m_Script_Styled->StyleSetForeground(wxSTC_LUA_WORD, wxColour(wxT("rgb(0, 0, 255)")));
	m_Script_Styled->StyleSetForeground(wxSTC_LUA_WORD2, wxColour(wxT("rgb(0, 0, 255)")));
	m_Script_Styled->StyleSetForeground(wxSTC_LUA_WORD3, wxColour(wxT("rgb(0, 0, 255)")));
	m_Script_Styled->StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColour(wxT("rgb(43, 145, 175)")));
	wxFont monofont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("consolas"));
	m_Script_Styled->StyleSetFont(wxSTC_STYLE_DEFAULT, monofont);
	m_Script_Styled->SetMarginType(1,wxSTC_MARGIN_NUMBER);
	m_Script_Styled->SetTabWidth(4);

	//Lua keywords in 0
	m_Script_Styled->SetKeyWords(0, wxT("and break do else elseif end false for function if in local nil not or repeat return then true until while"));
	//Toucan keywords in 1
	m_Script_Styled->SetKeyWords(1, wxT("sync backup secure print expand copy rename move delete execute getscript inputpassword shutdown"));
	//Option keywords in 2
	m_Script_Styled->SetKeyWords(2, wxT("size time short full ignorero ignoredls attributes timestamps recycle previewchanges password test solid"));
	//Turn word wrap on
	m_Script_Styled->SetWrapMode(1);
	m_Script_Styled->SetLexer(wxSTC_LEX_LUA);
	ScriptSizer->Add(m_Script_Styled, 1, wxEXPAND|wxLEFT|wxRIGHT|wxBOTTOM, 2 * border);

	//Settings
	wxPanel* SettingsPanel = new wxPanel(m_Notebook, ID_PANEL_SETTINGS, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL);
	wxGridBagSizer* SettingsSizer = new wxGridBagSizer(0, 0);
	SettingsPanel->SetSizer(SettingsSizer);

	wxStaticBox* FieldsStaticBox = new wxStaticBox(SettingsPanel, wxID_ANY, _("Remember Entered Information"));
	wxStaticBoxSizer* FieldsStaticBoxSizer = new wxStaticBoxSizer(FieldsStaticBox, wxVERTICAL);
	int fieldsx;
	FieldsStaticBox->GetTextExtent(_("Remember Entered Information"), &fieldsx, NULL);
	FieldsStaticBoxSizer->SetMinSize(wxSize(fieldsx + 10, -1));
	SettingsSizer->Add(FieldsStaticBoxSizer, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALIGN_LEFT|wxALIGN_TOP|wxALL, border);

	m_Settings_RememberSync = new wxCheckBox(SettingsPanel, ID_SETTINGS_REMEMBERSYNC, _("Sync"));
	FieldsStaticBoxSizer->Add(m_Settings_RememberSync, 0, wxALIGN_TOP|wxALL, border);

	m_Settings_RememberBackup = new wxCheckBox(SettingsPanel, ID_SETTINGS_REMEMBERBACKUP, _("Backup"));
	FieldsStaticBoxSizer->Add(m_Settings_RememberBackup, 0, wxALIGN_TOP|wxALL, border);

	m_Settings_RememberSecure = new wxCheckBox(SettingsPanel, ID_SETTINGS_REMEMBERSECURE, _("Secure"));
	FieldsStaticBoxSizer->Add(m_Settings_RememberSecure, 0, wxALIGN_TOP|wxALL, border);

	wxStaticBox* LanguageStaticBox = new wxStaticBox(SettingsPanel, wxID_ANY, _("Language"));
	wxStaticBoxSizer* LanguageStaticBoxSizer = new wxStaticBoxSizer(LanguageStaticBox, wxHORIZONTAL);
	SettingsSizer->Add(LanguageStaticBoxSizer, wxGBPosition(1, 0), wxGBSpan(1, 1), wxEXPAND|wxALL, border);

	wxArrayString m_Settings_LanguageStrings = GetLanguages();
	m_Settings_LanguageStrings.Sort();
	m_Settings_Language = new wxComboBox(SettingsPanel, ID_SETTINGS_LANGUAGE, _T(""), wxDefaultPosition, wxDefaultSize, m_Settings_LanguageStrings, wxCB_DROPDOWN|wxCB_READONLY);
	m_Settings_Language->SetMinSize(wxSize(125, -1));
	m_Settings_Language->SetStringSelection(wxLocale::FindLanguageInfo(wxGetApp().m_Settings->GetLanguageCode())->Description);
	LanguageStaticBoxSizer->Add(m_Settings_Language, 1, wxALL|wxEXPAND|wxALIGN_CENTRE_VERTICAL, border);

	wxArrayString arrTabStyles;
	arrTabStyles.Add(_("Icons and Text"));
	arrTabStyles.Add(_("Text"));
	m_Settings_TabStyle = new wxRadioBox(SettingsPanel, ID_SETTINGS_TABSTYLE, _("Tab style"), wxDefaultPosition, wxDefaultSize, arrTabStyles, 2, wxRA_SPECIFY_ROWS);
	m_Settings_TabStyle->SetStringSelection(wxGetApp().m_Settings->GetTabStyle());
	SettingsSizer->Add(m_Settings_TabStyle, wxGBPosition(2, 0), wxGBSpan(1, 1), wxEXPAND|wxALL, border);
	
	wxButton* ApplySettings = new wxButton(SettingsPanel, wxID_APPLY, _("Apply"));
	SettingsSizer->Add(ApplySettings, wxGBPosition(3, 0), wxGBSpan(1, 1), wxALL, border);

	wxStaticBox* HelpStaticBox = new wxStaticBox(SettingsPanel, wxID_ANY, _("Help"));
	wxStaticBoxSizer* HelpStaticBoxSizer = new wxStaticBoxSizer(HelpStaticBox, wxVERTICAL);
	SettingsSizer->Add(HelpStaticBoxSizer, wxGBPosition(0, 1), wxGBSpan(1, 1), wxALL, border);

	wxButton* Help = new wxButton(SettingsPanel, wxID_HELP, _("Help"), wxDefaultPosition, wxDefaultSize, 0);
	HelpStaticBoxSizer->Add(Help, 0, wxALIGN_CENTER_VERTICAL|wxALL, border);

	wxButton* About = new wxButton(SettingsPanel, wxID_ABOUT, _("About"), wxDefaultPosition, wxDefaultSize, 0);
	HelpStaticBoxSizer->Add(About, 0, wxALIGN_CENTER_VERTICAL|wxALL, border);

	wxStaticBox* OtherBox = new wxStaticBox(SettingsPanel, wxID_ANY, _("Other"));
	wxStaticBoxSizer* OtherStaticBoxSizer = new wxStaticBoxSizer(OtherBox, wxVERTICAL);
	SettingsSizer->Add(OtherStaticBoxSizer, wxGBPosition(1, 1), wxGBSpan(1, 1), wxALL, border);
	
	m_Settings_EnableTooltips = new wxCheckBox(SettingsPanel, ID_SETTINGS_ENABLETOOLTIPS, _("Enable Tooltips"));
	m_Settings_EnableTooltips->SetValue(false);
	OtherStaticBoxSizer->Add(m_Settings_EnableTooltips, 0, wxALIGN_TOP|wxALL, border);
	
	m_Settings_SmallBorders = new wxCheckBox(SettingsPanel, ID_SETTINGS_SMALLBORDERS, _("Small Borders"));
	m_Settings_SmallBorders->SetValue(false);
	OtherStaticBoxSizer->Add(m_Settings_SmallBorders, 0, wxALIGN_TOP|wxALL, border);

	wxStaticBox* FontStaticBox = new wxStaticBox(SettingsPanel, wxID_ANY, _("Font"));
	wxStaticBoxSizer* FontStaticBoxSizer = new wxStaticBoxSizer(FontStaticBox, wxHORIZONTAL);
	SettingsSizer->Add(FontStaticBoxSizer, wxGBPosition(2, 1), wxGBSpan(1, 1), wxALL, border);

	m_Settings_Font = new wxButton(SettingsPanel, ID_SETTINGS_FONT, _("Choose font"), wxDefaultPosition, wxDefaultSize, 0);
	FontStaticBoxSizer->Add(m_Settings_Font, 0, wxALIGN_CENTER_VERTICAL|wxALL, border);

	//Add the panels
	wxBitmap syncbitmap, backupbitmap, securebitmap, settingsbitmap, scriptbitmap, rulesbitmap, variablesbitmap, helpbitmap;
	
	if(m_Settings_TabStyle->GetStringSelection() == _("Icons and Text")){
		syncbitmap = GetBitmapResource(wxT("sync.png"));
		backupbitmap = GetBitmapResource(wxT("backup.png"));
		securebitmap = GetBitmapResource(wxT("secure.png"));
		settingsbitmap = GetBitmapResource(wxT("settings.png"));
		scriptbitmap = GetBitmapResource(wxT("script.png"));
		rulesbitmap = GetBitmapResource(wxT("rules.png"));
		variablesbitmap = GetBitmapResource(wxT("variables.png"));
		helpbitmap = GetBitmapResource(wxT("help.png"));
	}

	m_Notebook->AddPage(SyncPanel, _("Sync"), false, syncbitmap);
	m_Notebook->AddPage(BackupPanel, _("Backup"), false, backupbitmap);
	m_Notebook->AddPage(SecurePanel, _("Secure"), false, securebitmap);
	m_Notebook->AddPage(RulesPanel, _("Rules"), false, rulesbitmap);
	m_Notebook->AddPage(VariablesPanel, _("Variables"), false, variablesbitmap);
	m_Notebook->AddPage(ScriptPanel, _("Script"), false, scriptbitmap);
	m_Notebook->AddPage(SettingsPanel, _("Settings"), false, settingsbitmap);

	this->m_auiManager.AddPane(m_Notebook, wxAuiPaneInfo()
	                                    .Name(_T("Pane3")).Centre().CaptionVisible(false).CloseButton(false).DestroyOnClose(false).Resizable(true).Floatable(false).PaneBorder(false));

	m_auiManager.Update();

	//Set the settings
	unsigned int i = 0;
	unsigned int count = m_Notebook->GetPageCount();
	while(m_Notebook->GetPageText(m_Notebook->GetSelection()) != wxGetApp().m_Settings->GetPosition()){
		
		i ++;
		if(i < count){
			m_Notebook->AdvanceSelection();
		}
		else{
			m_Notebook->SetSelection(0);
			break;
		}
	}
	
	m_Settings_RememberSync->SetValue(wxGetApp().m_Settings->GetRememberSync());
	m_Settings_RememberBackup->SetValue(wxGetApp().m_Settings->GetRememberBackup());
	m_Settings_RememberSecure->SetValue(wxGetApp().m_Settings->GetRememberSecure());
	m_Settings_EnableTooltips->SetValue(wxGetApp().m_Settings->GetEnableTooltips());
	m_Settings_SmallBorders->SetValue(wxGetApp().m_Settings->GetSmallBorders());

    //Set up the sync controls
    m_Sync_Source_Tree->SetOppCtrl(m_Sync_Dest_Tree);
    m_Sync_Dest_Tree->SetOppCtrl(m_Sync_Source_Tree);

	//Set the drag and drop targets
	m_Sync_Source_Txt->SetDropTarget(new DnDFileTreeText(m_Sync_Source_Txt, m_Sync_Source_Tree));
	m_Sync_Dest_Txt->SetDropTarget(new DnDFileTreeText(m_Sync_Dest_Txt, m_Sync_Dest_Tree));
	
	m_Sync_Source_Tree->SetDropTarget(new DnDFileTreeText(m_Sync_Source_Txt, m_Sync_Source_Tree));
	m_Sync_Dest_Tree->SetDropTarget(new DnDFileTreeText(m_Sync_Dest_Txt, m_Sync_Dest_Tree));
	
	m_Backup_TreeCtrl->SetDropTarget(new DnDFileTree(m_Backup_TreeCtrl));
	m_Secure_TreeCtrl->SetDropTarget(new DnDFileTree(m_Secure_TreeCtrl));

	m_Variables_List->InsertColumn(0, _("Computer"));
	m_Variables_List->InsertColumn(1, _("Expansion"));
	
	//Add some blank items to the combo box to allow deselection
	m_Sync_Rules->Append(wxEmptyString);
	m_Backup_Rules->Append(wxEmptyString);
	m_Secure_Rules->Append(wxEmptyString);

	//Set the tooltips if required
	if(wxGetApp().m_Settings->GetEnableTooltips()){
		//Sync
		SyncJobSave->SetToolTip(_("Save"));
		SyncJobAdd->SetToolTip(_("Add"));
		SyncJobRemove->SetToolTip(_("Remove"));
		SyncSourceExpand->SetToolTip(_("Expand all"));
		SyncDestExpand->SetToolTip(_("Expand all"));
		SyncSourceInsert->SetToolTip(_("Insert Variable"));
		SyncDestInsert->SetToolTip(_("Insert Variable"));
		SyncRefreshButton->SetToolTip(_("Refresh"));
		//Backup
		BackupJobSave->SetToolTip(_("Save"));
		BackupJobAdd->SetToolTip(_("Add"));
		BackupJobRemove->SetToolTip(_("Remove"));
		BackupAdd->SetToolTip(_("Add"));
		BackupRemove->SetToolTip(_("Remove"));
		BackupAddVar->SetToolTip(_("Insert Variable"));
		BackupExpand->SetToolTip(_("Expand all"));
		BackupRefreshButton->SetToolTip(_("Refresh"));
		//Secure
		SecureJobSave->SetToolTip(_("Save"));
		SecureJobAdd->SetToolTip(_("Add"));
		SecureJobRemove->SetToolTip(_("Remove"));
		SecureAdd->SetToolTip(_("Add"));
		SecureRemove->SetToolTip(_("Remove"));
		SecureAddVar->SetToolTip(_("Insert Variable"));
		SecureExpand->SetToolTip(_("Expand all"));
		SecureRefreshButton->SetToolTip(_("Refresh"));
		//Rules
		RulesNameSave->SetToolTip(_("Save"));
		RulesNameAdd->SetToolTip(_("Add"));
		RulesNameRemove->SetToolTip(_("Remove"));
		RulesAddItem->SetToolTip(_("Add"));
		RulesRemoveItem->SetToolTip(_("Remove"));
		//Variables
		VariablesNameAdd->SetToolTip(_("Add"));
		VariablesNameRemove->SetToolTip(_("Remove"));
		VariablesAddItem->SetToolTip(_("Add"));
		VariablesRemoveItem->SetToolTip(_("Remove"));
		//Scripting
		ScriptNameSave->SetToolTip(_("Save"));
		ScriptNameAdd->SetToolTip(_("Add"));
		ScriptNameRemove->SetToolTip(_("Remove"));
	}

	//Disable certain buttons for read-only operation
	if(wxGetApp().IsReadOnly()){
		SyncJobSave->Disable();
		SyncJobAdd->Disable();
		SyncJobRemove->Disable();
		BackupJobSave->Disable();
		BackupJobAdd->Disable();
		BackupJobRemove->Disable();
		SecureJobSave->Disable();
		SecureJobAdd->Disable();
		SecureJobRemove->Disable();
		RulesNameSave->Disable();
		RulesNameAdd->Disable();
		RulesNameRemove->Disable();
		VariablesNameSave->Disable();
		VariablesNameAdd->Disable();
		VariablesNameRemove->Disable();
		ScriptNameSave->Disable();
		ScriptNameAdd->Disable();
		ScriptNameRemove->Disable();
		ApplySettings->Disable();
	}

	this->SetIcon(wxIcon(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("Toucan.ico"), wxBITMAP_TYPE_ICO));	

	if(wxGetApp().m_Jobs_Config->Exists(wxT("SyncRemember")) && wxGetApp().m_Settings->GetRememberSync()){
		SyncData data(wxT("SyncRemember"));
		data.TransferFromFile();
		data.TransferToForm(this);
		m_Sync_Job_Select->SetStringSelection(wxGetApp().m_Jobs_Config->Read(wxT("SyncRemember/Name")));
	}
	if(wxGetApp().m_Jobs_Config->Exists(wxT("BackupRemember")) && wxGetApp().m_Settings->GetRememberBackup()){
		BackupData data(wxT("BackupRemember"));
		data.TransferFromFile();		
		data.TransferToForm(this);
		m_Backup_Job_Select->SetStringSelection(wxGetApp().m_Jobs_Config->Read(wxT("BackupRemember/Name")));
	}
	if(wxGetApp().m_Jobs_Config->Exists(wxT("SecureRemember")) && wxGetApp().m_Settings->GetRememberSecure()){
		SecureData data(wxT("SecureRemember"));
		data.TransferFromFile();
		data.TransferToForm(this);
		m_Secure_Job_Select->SetStringSelection(wxGetApp().m_Jobs_Config->Read(wxT("SecureRemember/Name")));
	}
}

//Get bitmap resources
wxBitmap frmMain::GetBitmapResource(const wxString& name){
	wxString path = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("bitmaps") + wxFILE_SEP_PATH;
	wxBitmap bitmap(path + name, wxBITMAP_TYPE_PNG);
	if(bitmap.IsOk()){
		return bitmap;
	}
	return wxNullBitmap;
}

//ID_BACKUP_ADD
void frmMain::OnBackupAddClick(wxCommandEvent& WXUNUSED(event)){
	wxBusyCursor cursor;
	wxArrayString arrPaths = m_Backup_DirCtrl->GetSelectedPaths();
	int added = arrPaths.GetCount();
	int existing = m_BackupLocations->GetCount();
	if(m_Backup_Function->GetStringSelection() == _("Differential") && added + existing > 1){
		wxMessageBox(_("Differential only supports one folder"), _("Error"), wxICON_ERROR);
		return;
	}
	for(unsigned int i = 0; i < arrPaths.Count(); i++){
		m_BackupLocations->Add(arrPaths.Item(i));
		m_Backup_TreeCtrl->AddItem(arrPaths.Item(i));		
	}
}

//ID_SECURE_ADD
void frmMain::OnSecureAddClick(wxCommandEvent& WXUNUSED(event)){
	wxBusyCursor cursor;
	wxArrayString arrPaths = m_Secure_DirCtrl->GetSelectedPaths();
	for(unsigned int i = 0; i < arrPaths.Count(); i++){
		m_SecureLocations->Add(arrPaths.Item(i));
		m_Secure_TreeCtrl->AddItem(arrPaths.Item(i));		
	}
}

//ID_BACKUP_REMOVE
void frmMain::OnBackupRemoveClick(wxCommandEvent& WXUNUSED(event)){
	//Checks to see if it is a top level item that is being removed
	if (m_Backup_TreeCtrl->GetItemParent(m_Backup_TreeCtrl->GetSelection()) == m_Backup_TreeCtrl->GetRootItem()){
		//A loop to compare to normalised and non-normalised files
		for(unsigned int i = 0; i < m_BackupLocations->GetCount(); i++){
			if(Path::Normalise(m_BackupLocations->Item(i)) == m_Backup_TreeCtrl->GetPath(m_Backup_TreeCtrl->GetSelection())){
				m_BackupLocations->RemoveAt(i);
			}
		}
		m_Backup_TreeCtrl->Delete(m_Backup_TreeCtrl->GetSelection());
	}
}

//ID_SECURE_REMOVE
void frmMain::OnSecureRemoveClick(wxCommandEvent& WXUNUSED(event)){
	//Checks to see if it is a top level item that is being removed
	if (m_Secure_TreeCtrl->GetItemParent(m_Secure_TreeCtrl->GetSelection()) == m_Secure_TreeCtrl->GetRootItem()){
		//A loop to compare to normalised and non-normalised files
		for(unsigned int i = 0; i < m_SecureLocations->GetCount(); i++){
            if(Path::Normalise(m_SecureLocations->Item(i)) == m_Secure_TreeCtrl->GetPath(m_Secure_TreeCtrl->GetSelection())){
				m_SecureLocations->RemoveAt(i);
			}
		}
		m_Secure_TreeCtrl->Delete(m_Secure_TreeCtrl->GetSelection());
	}
}

//ID_SYNC_SOURCE_BTN
void frmMain::OnSyncSourceBtnClick(wxCommandEvent& WXUNUSED(event)){
	//Need to replace this with a better browser
	wxDirDialog dialog(this, _("Please select the source folder"), Path::Normalise(m_Sync_Source_Txt->GetValue()));
	if (dialog.ShowModal() == wxID_OK) {
		wxBusyCursor cursor;
		m_Sync_Source_Tree->AddItem(dialog.GetPath());
		m_Sync_Source_Txt->SetValue(dialog.GetPath());
	}
}

//ID_SYNC_DEST_BTN
void frmMain::OnSyncDestBtnClick(wxCommandEvent& WXUNUSED(event)){
	//Need to replace this with a better browser	
	wxDirDialog dialog(this, _("Please select the destination folder"), Path::Normalise(m_Sync_Dest_Txt->GetValue()));
	if (dialog.ShowModal() == wxID_OK) {
		wxBusyCursor cursor;
		m_Sync_Dest_Tree->AddItem(dialog.GetPath());
		m_Sync_Dest_Txt->SetValue(dialog.GetPath());
	}
}

//ID_RULES_ADDITEM
void frmMain::OnRulesAddItemClick(wxCommandEvent& WXUNUSED(event)){
    //Append a new item
    m_RulesGrid->AppendRows();

    //Set the cell editor for the type column
    int newrow = m_RulesGrid->GetNumberRows() - 1;
    m_RulesGrid->SetCellEditor(newrow, 0, new wxGridCellChoiceEditor(m_RulesChoices));
    m_RulesGrid->SetCellValue(newrow, 0, m_RulesChoices.Item(0));
    m_RulesGrid->SetCellEditor(newrow, 1, new wxGridCellChoiceEditor(m_RulesTypeChoices));
    m_RulesGrid->SetCellValue(newrow, 1, m_RulesTypeChoices.Item(0));
}

//ID_RULES_ADD_REMOVEITEM
void frmMain::OnRulesRemoveItemClick(wxCommandEvent& WXUNUSED(event)){
    m_RulesGrid->DeleteSelected();
}

//ID_RULES_SAVE
void frmMain::OnRulesSaveClick(wxCommandEvent& WXUNUSED(event)){
    //If there is no existing name prompt for one
	if(m_Rules_Name->GetStringSelection() == wxEmptyString){
		wxTextEntryDialog dialog(this,  _("Please enter the name for the new rules"), _("New Rules"));
		if(dialog.ShowModal() == wxID_OK && dialog.GetValue() != wxEmptyString){
			m_Rules_Name->Append(dialog.GetValue());
			m_Rules_Name->SetStringSelection(dialog.GetValue());
		}
		else{
			return;
		}
	}
    //Actually save the rules
    const wxString name = m_Rules_Name->GetStringSelection();
	RuleSet rules(name);
	if(!rules.TransferFromForm(this) || !rules.TransferToFile())
		return;
	
    //If the name isn't in the rules boxes then add it to all as the are identical
    if(m_Sync_Rules->FindString(name) == wxNOT_FOUND){
        m_Sync_Rules->Append(name);
        m_Backup_Rules->Append(name);
        m_Secure_Rules->Append(name);
    }
	//Then refresh if needed
    wxCommandEvent event;
	if(m_Sync_Rules->GetStringSelection() == m_Rules_Name->GetStringSelection())
		OnSyncRefresh(event);

	if(m_Backup_Rules->GetStringSelection() == m_Rules_Name->GetStringSelection())
		OnBackupRefresh(event);

	if(m_Secure_Rules->GetStringSelection() == m_Rules_Name->GetStringSelection())
		OnSecureRefresh(event);
}

//ID_RULES_ADD
void frmMain::OnRulesAddClick(wxCommandEvent& WXUNUSED(event)){
    //Check to save existing rules
	if(m_RulesGrid->GetNumberRows() > 0){
		wxMessageDialog dialog(this, _("Do you wish to save the current rules?"), _("Rules Save"), wxYES_NO|wxCANCEL);
		int ret = dialog.ShowModal();
		if(ret == wxID_YES){
			wxCommandEvent newevent;
			OnRulesSaveClick(newevent);
		}
		else if(ret == wxID_CANCEL){
			return;
		}
	}
    //Get name for new rules
	wxArrayString existing = GetRules();
	wxTextEntryDialog entrydialog(this, _("Please enter the name for the new rules"), _("New Rules"));
	if(entrydialog.ShowModal() == wxID_OK && entrydialog.GetValue() != wxEmptyString){
		for(unsigned int i = 0; i < existing.Count(); i++){
			if(existing.Item(i).Lower() == entrydialog.GetValue().Lower()){
				wxMessageBox(_("There is already a set of rules with this name"), _("Error"), wxICON_ERROR);
				return;
			}
		}
		m_Rules_Name->AppendString(entrydialog.GetValue());
		m_Rules_Name->SetStringSelection(entrydialog.GetValue());
		m_RulesGrid->Clear();
		UpdateSizer(RulesNameSizer);
	}
}

//ID_RULES_REMOVE
void frmMain::OnRulesRemoveClick(wxCommandEvent& WXUNUSED(event)){
	m_RulesGrid->Clear();
	wxGetApp().m_Rules_Config->DeleteGroup(m_Rules_Name->GetStringSelection());
	wxGetApp().m_Rules_Config->Flush();
	m_Rules_Name->Delete(m_Rules_Name->GetSelection());
	UpdateSizer(RulesNameSizer);
}

//ID_RULES_COMBO
void frmMain::OnRulesComboSelected(wxCommandEvent& WXUNUSED(event)){
	RuleSet rules(m_Rules_Name->GetStringSelection());
	if (rules.TransferFromFile()) {
		rules.TransferToForm(this);
	}
	SetTitleBarText();
}

//ID_SYNC_JOB_SAVE
void frmMain::OnSyncJobSaveClick(wxCommandEvent& WXUNUSED(event)){
	JobSave(m_Sync_Job_Select->GetStringSelection(), wxT("Sync"));
}

//ID_BACKUP_JOB_SAVE
void frmMain::OnBackupJobSaveClick(wxCommandEvent& WXUNUSED(event)){
	JobSave(m_Backup_Job_Select->GetStringSelection(), wxT("Backup"));
}

//ID_SECURE_JOB_SAVE
void frmMain::OnSecureJobSaveClick(wxCommandEvent& WXUNUSED(event)){
	JobSave(m_Secure_Job_Select->GetStringSelection(), wxT("Secure"));
}

//ID_SYNC_JOB_ADD
void frmMain::OnSyncJobAddClick(wxCommandEvent& WXUNUSED(event)){
	if(m_Sync_Job_Select->GetStringSelection() != wxEmptyString || m_Sync_Source_Txt->GetValue() != wxEmptyString || m_Sync_Dest_Txt->GetValue() != wxEmptyString){
		wxMessageDialog dialog(this, _("Do you wish to save the current job?"), _("Job Save"), wxYES_NO|wxCANCEL);
		int ret = dialog.ShowModal();
		if(ret == wxID_YES){
			wxCommandEvent newevent;
			OnSyncJobSaveClick(newevent);
		}
		else if(ret == wxID_CANCEL){
			return;
		}
	}
	JobAdd(m_Sync_Job_Select);
	UpdateSizer(SyncTopSizer);
}

//ID_BACKUP_JOB_ADD
void frmMain::OnBackupJobAddClick(wxCommandEvent& WXUNUSED(event)){
	if(m_Backup_Job_Select->GetStringSelection() != wxEmptyString || m_BackupLocations->GetCount() > 0){
		wxMessageDialog dialog(this, _("Do you wish to save the current job?"), _("Job Save"), wxYES_NO|wxCANCEL);
		int ret = dialog.ShowModal();
		if(ret == wxID_YES){
			wxCommandEvent newevent;
			OnBackupJobSaveClick(newevent);
		}
		else if(ret == wxID_CANCEL){
			return;
		}
	}
	JobAdd(m_Backup_Job_Select);
	UpdateSizer(BackupTopSizer);
}

//ID_SECURE_JOB_ADD
void frmMain::OnSecureJobAddClick(wxCommandEvent& WXUNUSED(event)){
	if(m_Secure_Job_Select->GetStringSelection() != wxEmptyString || m_SecureLocations->GetCount() > 0){
		wxMessageDialog dialog(this, _("Do you wish to save the current job?"), _("Job Save"), wxYES_NO|wxCANCEL);
		int ret = dialog.ShowModal();
		if(ret == wxID_YES){
			wxCommandEvent newevent;
			OnSecureJobSaveClick(newevent);
		}
		else if(ret == wxID_CANCEL){
			return;
		}
	}
	JobAdd(m_Secure_Job_Select);
	UpdateSizer(SecureTopSizer);
}

//ID_SYNC_JOB_REMOVE
void frmMain::OnSyncJobRemoveClick(wxCommandEvent& WXUNUSED(event)){
	JobRemove(m_Sync_Job_Select);
	UpdateSizer(SyncTopSizer);
}

//ID_BACKUP_JOB_REMOVE
void frmMain::OnBackupJobRemoveClick(wxCommandEvent& WXUNUSED(event)){
	JobRemove(m_Backup_Job_Select);
	UpdateSizer(BackupTopSizer);
}

//ID_SECURE_JOB_REMOVE
void frmMain::OnSecureJobRemoveClick(wxCommandEvent& WXUNUSED(event)){
	JobRemove(m_Secure_Job_Select);
	UpdateSizer(SecureTopSizer);
}

//ID_BACKUP_JOB_SELECT
void frmMain::OnBackupJobSelectSelected(wxCommandEvent& WXUNUSED(event)){
	JobLoad(m_Backup_Job_Select->GetStringSelection(), wxT("Backup"));
	SetSliderText();
}

//ID_SECURE_JOB_SELECT
void frmMain::OnSecureJobSelectSelected(wxCommandEvent& WXUNUSED(event)){
	JobLoad(m_Secure_Job_Select->GetStringSelection(), wxT("Secure"));
}

//ID_SYNC_JOB_SELECT
void frmMain::OnSyncJobSelectSelected(wxCommandEvent& WXUNUSED(event)){
	JobLoad(m_Sync_Job_Select->GetStringSelection(), wxT("Sync"));
}

//ID_BACKUP_LOCATION
void frmMain::OnBackupLocationClick(wxCommandEvent& WXUNUSED(event)){
	if(m_Backup_Function->GetStringSelection() == _("Differential")){
		wxDirDialog dialog(this,_("Please select the folder to store your backups"),m_Backup_Location->GetValue());
		if (dialog.ShowModal() == wxID_OK){
			m_Backup_Location->SetValue(dialog.GetPath());
		}
	}
	else if(m_Backup_Function->GetStringSelection() == _("Restore")){
		wxDirDialog dialog(this,_("Restore into:"),m_Backup_Location->GetValue());
		if (dialog.ShowModal() == wxID_OK){
			m_Backup_Location->SetValue(dialog.GetPath());
		}
	}
	else{
		wxString strWildcard;
		if(m_Backup_Format->GetStringSelection() == wxT("7-Zip")){
			strWildcard = wxT("7-Zip Files (*.7z)|*.7z|All Files (*.*)|*.*");
		}
		else if(m_Backup_Format->GetStringSelection() == wxT("Zip")){
			strWildcard = wxT("Zip Files (*.zip)|*.zip|All Files (*.*)|*.*");
		}
		else if(m_Backup_Format->GetStringSelection() == wxT("GZip")){
			strWildcard = wxT("GZip Files (*.tar.gz)|*.tar.gz|All Files (*.*)|*.*");
		}
		wxFileDialog dialog(this,_("Please choose a file to backup to"), wxPathOnly(m_Backup_Location->GetValue()), wxEmptyString, strWildcard, wxFD_SAVE);
		if (dialog.ShowModal() == wxID_OK) {
			m_Backup_Location->SetValue(dialog.GetPath());
		}
	}
}

//ID_SYNC_RUN
void frmMain::OnSyncRunClick(wxCommandEvent& WXUNUSED(event)){
	wxString command;
	//source
	command << "sync([=[" <<m_Sync_Source_Txt->GetValue() << "]=], ";
	//dest
	command << "[=[" << m_Sync_Dest_Txt->GetValue() <<  "]=], ";
	//function
	command << "[[" << m_Sync_Function->GetStringSelection() << "]], ";
	//checks
	command << "{size=" << ToString(m_SyncCheckSize->IsChecked()) << ","
			<< "time=" << ToString(m_SyncCheckTime->IsChecked()) << ","
			<< "short=" << ToString(m_SyncCheckShort->IsChecked()) << ","
			<< "full=" << ToString(m_SyncCheckFull->IsChecked()) << "}, ";
	//options
	command << "{timestamps=" << ToString(m_Sync_Timestamp->IsChecked()) << ","
			<< "attributes=" << ToString(m_Sync_Attributes->IsChecked()) << ","
			<< "recycle=" << ToString(m_Sync_Recycle->IsChecked()) << ","
			<< "ignorero=" << ToString(m_Sync_Ignore_Readonly->IsChecked()) << "}, ";
	//rules
	command << "[[" << m_Sync_Rules->GetStringSelection() << "]])";
	wxGetApp().m_LuaManager->Run(command);
}

//ID_BACKUP_RUN
void frmMain::OnBackupRunClick(wxCommandEvent& WXUNUSED(event)){
	if(m_Backup_Function->GetStringSelection() == _("Differential") && m_BackupLocations->GetCount() > 1){
		wxMessageBox(_("Differential only supports one folder"), _("Error"), wxICON_ERROR);
		return;
	}
	//Differential needs a folder to store its files in
	if(m_Backup_Function->GetStringSelection() ==_("Differential") && m_Backup_Location->GetValue() != wxEmptyString){
		if(m_Backup_Location->GetValue().Right(3) == wxT(".7z") || m_Backup_Location->GetValue().Right(4) == wxT(".zip")){
			wxMessageBox(_("Please select the folder to store your backups"), _("Error"), wxICON_ERROR);
			return;
		}
	}
	wxString command;
	//sources
	command << "backup({";
	for(unsigned int i = 0; i < m_BackupLocations->Count(); i++){
		command << "[=[" <<  m_BackupLocations->Item(i) << "]=],";
	}
	command << "}, ";
	//backup file
	command << "[=[" <<  m_Backup_Location->GetValue() << "]=], ";
	//function
	command << "[[" <<   m_Backup_Function->GetStringSelection() << "]], ";
	//format
	command << "[[" <<   m_Backup_Format->GetStringSelection() << "]], ";
	//compression ratio
	command << m_Backup_Ratio->GetValue() << ", ";
	//options
	command << "{password=" << ToString(m_Backup_IsPass->IsChecked()) << ","
			<< "test=" << ToString(m_Backup_Test->IsChecked()) << "}, ";
	//rules
	command << "[[" << m_Backup_Rules->GetStringSelection() << "]])";
	wxGetApp().m_LuaManager->Run(command);
}

//ID_SECURE_RUN
void frmMain::OnSecureRunClick(wxCommandEvent& WXUNUSED(event)){
	wxString command;
	//sources
	command << "secure({";
	for(unsigned int i = 0; i < m_SecureLocations->Count(); i++){
		command << "[=[" <<  m_SecureLocations->Item(i) << "]=],";
	}
	command << "}, ";
	//function
	command << "[[" << m_Secure_Function->GetStringSelection() << "]], ";
	//rules
	command << "[[" << m_Secure_Rules->GetStringSelection() << "]])";
	wxGetApp().m_LuaManager->Run(command);
}

//ID_SYNC_PREVIEW
void frmMain::OnSyncPreviewClick(wxCommandEvent& WXUNUSED(event)){
	wxBusyCursor cursor;
	m_Notebook->Disable();

	m_Sync_Dest_Tree->SetPreview(true);
	m_Sync_Dest_Tree->SetPreviewChanges(m_SyncPreviewChanges->IsChecked());
	m_Sync_Dest_Tree->AddItem(Path::Normalise(m_Sync_Dest_Txt->GetValue()));

	if(m_Sync_Function->GetStringSelection() == _("Equalise") || m_Sync_Function->GetStringSelection() == _("Move")){
		m_Sync_Source_Tree->SetPreview(true);
	    m_Sync_Source_Tree->SetPreviewChanges(m_SyncPreviewChanges->IsChecked());
		m_Sync_Source_Tree->AddItem(Path::Normalise(m_Sync_Source_Txt->GetValue()));
	}
	else{
		m_Sync_Source_Tree->AddItem(Path::Normalise(m_Sync_Source_Txt->GetValue()));
	}

	m_Notebook->Enable();
}

//ID_BACKUP_PREVIEW
void frmMain::OnBackupRulesSelected(wxCommandEvent& WXUNUSED(event)){
	if(m_Backup_Rules->GetStringSelection() != wxEmptyString){
		RuleSet *rules = new RuleSet(m_Backup_Rules->GetStringSelection());
		if(!rules->TransferFromFile())
			return;
		m_Backup_TreeCtrl->SetRules(rules);
	}
	//Delete all items and re-add the root
	m_Backup_TreeCtrl->DeleteAllItems();
	m_Backup_TreeCtrl->AddRoot(wxT("Hidden root"));
	for (unsigned int i = 0; i < m_BackupLocations->GetCount(); i++){
		//Loop through all the the filenames listed in the array and read them to the tree
		m_Backup_TreeCtrl->AddItem(Path::Normalise(m_BackupLocations->Item(i)));
	}
}

//ID_SECURE_PREVIEW
void frmMain::OnSecureRulesSelected(wxCommandEvent& WXUNUSED(event)){
	if(m_Secure_Rules->GetStringSelection() != wxEmptyString){
		RuleSet *rules = new RuleSet(m_Secure_Rules->GetStringSelection());
		if(!rules->TransferFromFile())
			return;
		m_Secure_TreeCtrl->SetRules(rules);
	}
	//Delete all items and re-add the root
	m_Secure_TreeCtrl->DeleteAllItems();
	m_Secure_TreeCtrl->AddRoot(wxT("Hidden root"));
	for (unsigned int i = 0; i < m_SecureLocations->GetCount(); i++) {
		//Loop through all the the filenames listed in the array and read them to the tree
		m_Secure_TreeCtrl->AddItem(Path::Normalise(m_SecureLocations->Item(i)));
	}
}

void frmMain::OnCloseWindow(wxCloseEvent& WXUNUSED(event)){
	if(wxGetApp().IsReadOnly()){
		wxGetApp().MainWindow->Destroy();
		return;
	}

	int height, width, x, y;
	wxClientDisplayRect(&x, &y, &width, &height);
	
	wxGetApp().m_Settings->SetHeight((double)(this->GetSize().GetHeight())/(height));
	wxGetApp().m_Settings->SetWidth((double)(this->GetSize().GetWidth())/(width));
	wxGetApp().m_Settings->SetX((double)(this->GetScreenPosition().x)/(width));
	wxGetApp().m_Settings->SetY((double)(this->GetScreenPosition().y)/(height));
	wxGetApp().m_Settings->SetPosition(m_Notebook->GetPageText(m_Notebook->GetSelection()));

	wxGetApp().m_Settings->TransferToFile();

	try{
		if(wxGetApp().m_Settings->GetRememberSync()){
			SyncData data(wxT("SyncRemember"));
			data.TransferFromForm(this);
			data.TransferToFile();
			wxGetApp().m_Jobs_Config->Write(wxT("SyncRemember/Name"), m_Sync_Job_Select->GetStringSelection());
			wxGetApp().m_Jobs_Config->Flush();
		}
		if(wxGetApp().m_Settings->GetRememberBackup()){	
			BackupData bdata(wxT("BackupRemember"));
			bdata.TransferFromForm(this);
			bdata.TransferToFile();
			wxGetApp().m_Jobs_Config->Write(wxT("BackupRemember/Name"), m_Backup_Job_Select->GetStringSelection());
			wxGetApp().m_Jobs_Config->Flush();
		}
		if(wxGetApp().m_Settings->GetRememberSecure()){
			SecureData sdata(wxT("SecureRemember"));
			sdata.TransferFromForm(this);
			sdata.TransferToFile();
			wxGetApp().m_Jobs_Config->Write(wxT("SecureRemember/Name"), m_Secure_Job_Select->GetStringSelection());
			wxGetApp().m_Jobs_Config->Flush();
		}
	}
	catch(std::exception &arg){
		wxMessageBox(arg.what(), "Error", wxOK|wxICON_ERROR|wxCENTRE);
	}

	wxGetApp().MainWindow->Destroy();
}

//ID_VARIABLES_SAVE
void frmMain::OnVariablesSaveClick(wxCommandEvent& WXUNUSED(event)){
	if(m_Variables_Name->GetValue() == wxEmptyString){
		wxTextEntryDialog dialog(this,  _("Please enter the name for the new variable"), _("New Variable"));
		if(dialog.ShowModal() == wxID_OK && dialog.GetValue() != wxEmptyString){
			m_Variables_Name->Append(dialog.GetValue());
			m_Variables_Name->SetStringSelection(dialog.GetValue());
		}
		else{
			return;
		}
	}
	for(int i = 0; i < m_Variables_List->GetItemCount(); i++){
		wxListItem itemcol1, itemcol2;
		itemcol1.m_itemId = i;
		itemcol1.m_col = 0;
		itemcol1.m_mask = wxLIST_MASK_TEXT;
		m_Variables_List->GetItem(itemcol1);
		itemcol2.m_itemId = i;
		itemcol2.m_col = 1;
		itemcol2.m_mask = wxLIST_MASK_TEXT;
		m_Variables_List->GetItem(itemcol2);
		wxGetApp().m_Variables_Config->Write(m_Variables_Name->GetValue() + wxT("/") + itemcol1.m_text, itemcol2.m_text);
	}
	wxGetApp().m_Variables_Config->Flush();
}

//ID_VARIABLES_ADD
void frmMain::OnVariablesAddClick(wxCommandEvent& WXUNUSED(event)){	
	wxMessageDialog dialog(this, _("Do you wish to save the current variable?"), _("Variable Save"), wxYES_NO|wxCANCEL);
	int ret = dialog.ShowModal();
	if(ret == wxID_YES){
		wxCommandEvent newevent;
		OnVariablesSaveClick(newevent);
	}
	else if(ret == wxID_CANCEL){
		return;
	}
	wxArrayString existing = GetVariables(true);
	wxTextEntryDialog entrydialog(this, _("Please enter the name for the new variable"), _("New Variable"));
	if(entrydialog.ShowModal() == wxID_OK && entrydialog.GetValue() != wxEmptyString){
		for(unsigned int i = 0; i < existing.Count(); i++){
			if(existing.Item(i).Lower() == entrydialog.GetValue().Lower()){
				wxMessageBox(_("There is already a variable with this name"), _("Error"), wxICON_ERROR);
				return;
			}
		}
        m_Variables_Name->Append(entrydialog.GetValue());
        m_Variables_Name->SetStringSelection(entrydialog.GetValue());
        m_Variables_List->ClearAll();
        wxListItem column;
        m_Variables_List->InsertColumn(0, _("Computer"));
        m_Variables_List->InsertColumn(1, _("Expansion"));
        m_Variables_List->InsertItem(0, wxGetFullHostName());
        m_Variables_List->InsertItem(1, _("Other"));
		UpdateSizer(VariablesTopSizer);
	}
}

//ID_VARIABLES_REMOVE
void frmMain::OnVariablesRemoveClick(wxCommandEvent& WXUNUSED(event)){	
	wxGetApp().m_Variables_Config->DeleteGroup(m_Variables_Name->GetValue());
	wxGetApp().m_Variables_Config->Flush();
	m_Variables_Name->Delete(m_Variables_Name->GetSelection());
	m_Variables_Name->SetValue(wxEmptyString);
	m_Variables_List->ClearAll();
	wxListItem column;
	m_Variables_List->InsertColumn(0, _("Computer"));
	m_Variables_List->InsertColumn(1, _("Expansion"));
	UpdateSizer(VariablesTopSizer);
}

//ID_VARIABLES_NAME
void frmMain::OnVariablesNameSelected(wxCommandEvent& WXUNUSED(event)){	
	m_Variables_List->DeleteAllItems();
	
	wxGetApp().m_Variables_Config->SetPath(m_Variables_Name->GetValue());
    
	long dummy;
	wxString str;
	int i = 0;
	
	bool bCont = wxGetApp().m_Variables_Config->GetFirstEntry(str, dummy);
	while(bCont) {
		m_Variables_List->InsertItem(i, wxT("Test"));
		m_Variables_List->SetItem(i, 0, str);
		//Return the config to the top level
		wxGetApp().m_Variables_Config->SetPath(wxT("/"));
		wxString strTest = wxGetApp().m_Variables_Config->Read(m_Variables_Name->GetValue() + wxT("/") + str, wxT("Cannot Find Value"));
		m_Variables_List->SetItem(i, 1, strTest);
		//Put the config location back
		wxGetApp().m_Variables_Config->SetPath(m_Variables_Name->GetValue());    
		bCont = wxGetApp().m_Variables_Config->GetNextEntry(str, dummy);
		i++;
	}
	m_Variables_List->SetColumnWidth(0, -1);
	m_Variables_List->SetColumnWidth(1, -1);
	SetTitleBarText();
	wxGetApp().m_Variables_Config->SetPath(wxT("/"));
}

//ID_VARIABLES_ADDITEM
void frmMain::OnVariablesAddItemClick(wxCommandEvent& WXUNUSED(event)){	
	wxString caption = "Choose a directory";
	wxDirDialog dialog(this, caption);
	if (dialog.ShowModal() == wxID_OK){
		int j = m_Variables_List->GetItemCount();
		m_Variables_List->InsertItem(j, wxGetFullHostName());
		m_Variables_List->SetItem(j, 1, dialog.GetPath());
		wxGetApp().m_Variables_Config->Write(m_Variables_Name->GetValue() + wxT("/") + wxGetFullHostName() , dialog.GetPath());
		wxGetApp().m_Variables_Config->Flush();
	}
}

//ID_VARIABLES_REMOVEITEM
void frmMain::OnVariablesRemoveItemClick(wxCommandEvent& WXUNUSED(event)){	
	wxString selected;
	long item = -1;
	for (;;)
	{
		item = m_Variables_List->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
		if ( item == -1)
		break;
		selected = m_Variables_List->GetItemText(item);
		m_Variables_List->DeleteItem(item);
	}
	wxGetApp().m_Variables_Config->DeleteEntry(m_Variables_Name->GetValue() + wxT("/") + selected);
	wxGetApp().m_Variables_Config->Flush();
}

//ID_VARIABLES_LIST
void frmMain::OnVariablesListActivated(wxListEvent& WXUNUSED(event)){	
	long item = -1;
	for ( ;;)
	{
		item = m_Variables_List->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
		if ( item == -1)
		break;
		
		wxListItem PreviousColumn1;
		PreviousColumn1.m_itemId = item;
		PreviousColumn1.m_col = 1;
		PreviousColumn1.m_mask = wxLIST_MASK_TEXT;
		m_Variables_List->GetItem(PreviousColumn1);
		wxString strPreviousColumn1 = PreviousColumn1.m_text;
		wxTextEntryDialog location(this, _("Please insert the location you want to point to."), _("Location"), strPreviousColumn1, wxOK);
		if(location.ShowModal() == wxID_OK){
			m_Variables_List->SetItem(item , 1, location.GetValue());
		}
		m_Variables_List->SetColumnWidth(0, -1);
		m_Variables_List->SetColumnWidth(1, -1);
	}
}

//ID_SCRIPT_RUN
void frmMain::OnScriptExecute(wxCommandEvent& WXUNUSED(event)){
	wxGetApp().m_LuaManager->Run(m_Script_Styled->GetValue(), false);
}

//ID_SCRIPT_NAME
void frmMain::OnScriptSelected(wxCommandEvent& WXUNUSED(event)){	
	m_Script_Styled->LoadFile(wxGetApp().GetSettingsPath() + "scripts" + wxFILE_SEP_PATH + m_Script_Name->GetStringSelection() + ".lua");
}

//ID_SCRIPT_SAVE
void frmMain::OnScriptSaveClick(wxCommandEvent& WXUNUSED(event)){
	if(m_Script_Name->GetValue() == wxEmptyString){
		wxTextEntryDialog dialog(this,  _("Please enter the name for the new script"), _("New Script"));
		if(dialog.ShowModal() == wxID_OK && dialog.GetValue() != wxEmptyString){
			m_Script_Name->Append(dialog.GetValue());
			m_Script_Name->SetStringSelection(dialog.GetValue());
		}
		else{
			return;
		}
	}
	m_Script_Styled->SaveFile(wxGetApp().GetSettingsPath() + "scripts" + wxFILE_SEP_PATH + m_Script_Name->GetStringSelection() + ".lua");
}

//ID_SCRIPT_REMOVE
void frmMain::OnScriptRemoveClick(wxCommandEvent& WXUNUSED(event)){
	m_Script_Styled->Clear();
	wxRemoveFile(wxGetApp().GetSettingsPath() + "scripts" + wxFILE_SEP_PATH + m_Script_Name->GetStringSelection() + ".lua");
	m_Script_Name->Delete(m_Script_Name->GetSelection());
	UpdateSizer(ScriptNameSizer);
}

//ID_SCRIPT_ADD
void frmMain::OnScriptAddClick(wxCommandEvent& WXUNUSED(event)){
	if(m_Script_Styled->GetValue() != wxEmptyString){
		wxMessageDialog dialog(this, _("Do you wish to save the current script?"), _("Script Save"), wxYES_NO|wxCANCEL);
		int ret = dialog.ShowModal();
		if(ret == wxID_YES){
			wxCommandEvent newevent;
			OnScriptSaveClick(newevent);
		}
		else if(ret == wxID_CANCEL){
			return;
		}
	}
	wxArrayString existing = GetScripts();
	wxTextEntryDialog entrydialog(this, _("Please enter the name for the new script"), _("New Script"));
	if(entrydialog.ShowModal() == wxID_OK && entrydialog.GetValue() != wxEmptyString){
		for(unsigned int i = 0; i < existing.Count(); i++){
			if(existing.Item(i).Lower() == entrydialog.GetValue().Lower()){
				wxMessageBox(_("There is already a script with this name"), _("Error"), wxICON_ERROR);
				return;
			}
		}
		m_Script_Styled->Clear();
		m_Script_Name->Append(entrydialog.GetValue());
		m_Script_Name->SetStringSelection(entrydialog.GetValue());
		UpdateSizer(ScriptNameSizer);
	}
}

//ID_BACKUP_RATIO
void frmMain::OnBackupRatioChanged(wxScrollEvent& WXUNUSED(event)){
	SetSliderText();
}

//ID_BACKUP_ADDVAR
void frmMain::OnBackupAddVarClick(wxCommandEvent& WXUNUSED(event)){
	frmVariable window(this);
	if(window.ShowModal() == wxID_OK){
		m_BackupLocations->Add(window.GetValue());
		m_Backup_TreeCtrl->AddItem(Path::Normalise(window.GetValue()));
	}
}

//ID_SECURE_ADDVAR
void frmMain::OnSecureAddVarClick(wxCommandEvent& WXUNUSED(event)){
	frmVariable window(this);
	if(window.ShowModal() == wxID_OK){
		m_SecureLocations->Add(window.GetValue());
		m_Secure_TreeCtrl->AddItem(Path::Normalise(window.GetValue()));
	}
}

//wxID_ABOUT
void frmMain::OnAboutClick(wxCommandEvent& WXUNUSED(event)){
	wxAboutDialogInfo info;
	info.SetName(wxT("Toucan"));
	info.SetVersion(wxT("3.0.4"));
	info.SetDescription(wxString::Format(wxT("Built on %s at %s"), __TDATE__, __TTIME__));
	info.SetCopyright(wxT("(C) 2006-2010 Steven Lamerton \nName by Danny Mensingh\nMain icons by Neorame\nOther icons by Silvestre Herrera\nExtra thanks to Jorgen Bodde for his awesome wxVirtualDirTreeCtrl\n7Zip and ccrypt are by their respective teams.\nAll items (C) their owners."));
	info.SetWebSite(wxT("http://portableapps.com/toucan"));
	info.SetLicense(wxT("Toucan and its component parts are all licensed under the GNU GPL Version 2 or a compatible license."));
	info.SetTranslators(GetTranslatorNames());
	wxAboutBox(info);
}

//wxID_HELP
void frmMain::OnHelpClick(wxCommandEvent& WXUNUSED(event)){
    const wxString exedir = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH;
    wxLaunchDefaultBrowser("file:///" + exedir + "/help/index.html");
}

//ID_BACKUP_TREECTRL
void frmMain::OnBackupTreeCtrlTooltip(wxTreeEvent& event){
	if(m_Backup_TreeCtrl->GetItemParent(event.GetItem()) == m_Backup_TreeCtrl->GetRootItem()){
		for(unsigned int i = 0; i < m_BackupLocations->GetCount(); i++){
			if(Path::Normalise(m_BackupLocations->Item(i)) == m_Backup_TreeCtrl->GetPath(event.GetItem())){
				event.SetToolTip(m_BackupLocations->Item(i));
			}
		}
	}
}

//ID_SECURE_TREECTRL
void frmMain::OnSecureTreeCtrlTooltip(wxTreeEvent& event){
	if(m_Secure_TreeCtrl->GetItemParent(event.GetItem()) == m_Secure_TreeCtrl->GetRootItem()){
		for(unsigned int i = 0; i < m_SecureLocations->GetCount(); i++){
			if(Path::Normalise(m_SecureLocations->Item(i)) == m_Secure_TreeCtrl->GetPath(event.GetItem())){
				event.SetToolTip(m_SecureLocations->Item(i));
			}
		}
	}	
}

//ID_SYNC_SOURCE_TREE
void frmMain::OnSyncSourceTreeRightClick(wxTreeEvent& event){
	menuTree = m_Sync_Source_Tree;
	menuRules = m_Sync_Rules;
	CreateMenu(event);
}

//ID_SYNC_DEST_TREE
void frmMain::OnSyncDestTreeRightClick(wxTreeEvent& event){
	menuTree = m_Sync_Dest_Tree;
	menuRules = m_Sync_Rules;
	CreateMenu(event);
}

//ID_BACKUP_TREECTRL
void frmMain::OnBackupTreeRightClick(wxTreeEvent& event){
	menuTree = m_Backup_TreeCtrl;
	menuRules = m_Backup_Rules;
	CreateMenu(event);
}

//ID_SECURE_TREECTRL
void frmMain::OnSecureTreeRightClick(wxTreeEvent& event){
	menuTree = m_Secure_TreeCtrl;
	menuRules = m_Secure_Rules;
	CreateMenu(event);
}

//ID_SYNC_SOURCE_EXPAND
void frmMain::OnSyncSourceExpandClick(wxCommandEvent& WXUNUSED(event)){
    if(m_Sync_Source_Tree->GetSelection().IsOk()){
        m_Sync_Source_Tree->ExpandAllChildren(m_Sync_Source_Tree->GetSelection());
    }
    else{
	    m_Sync_Source_Tree->ExpandAll();
    }
}

//ID_SYNC_DEST_EXPAND
void frmMain::OnSyncDestExpandClick(wxCommandEvent& WXUNUSED(event)){
    if(m_Sync_Dest_Tree->GetSelection().IsOk()){
        m_Sync_Dest_Tree->ExpandAllChildren(m_Sync_Dest_Tree->GetSelection());
    }
    else{
	    m_Sync_Dest_Tree->ExpandAll();
    }
}

//ID_BACKUP_EXPAND
void frmMain::OnBackupExpandClick(wxCommandEvent& WXUNUSED(event)){
    if(m_Backup_TreeCtrl->GetSelection().IsOk()){
        m_Backup_TreeCtrl->ExpandAllChildren(m_Backup_TreeCtrl->GetSelection());
    }
    else{
	    m_Backup_TreeCtrl->ExpandAll();
    }
}

//ID_SECURE_EXPAND
void frmMain::OnSecureExpandClick(wxCommandEvent& WXUNUSED(event)){
    if(m_Secure_TreeCtrl->GetSelection().IsOk()){
        m_Secure_TreeCtrl->ExpandAllChildren(m_Secure_TreeCtrl->GetSelection());
    }
    else{
	    m_Secure_TreeCtrl->ExpandAll();
    }
}

//ID_AUINOTEBOOK
void frmMain::OnTabChanged(wxAuiNotebookEvent& WXUNUSED(event)){
	SetTitleBarText();
}

void frmMain::SetTitleBarText(){
	wxString strTitle = wxT("Toucan - ");
	if(m_Notebook->GetPageText(m_Notebook->GetSelection()) == _("Sync")){
		strTitle = strTitle + m_Sync_Job_Select->GetStringSelection();
	}
	else if(m_Notebook->GetPageText(m_Notebook->GetSelection()) == _("Backup")){
		strTitle = strTitle + m_Backup_Job_Select->GetStringSelection();
	}
	else if(m_Notebook->GetPageText(m_Notebook->GetSelection()) == _("Secure")){
		strTitle = strTitle + m_Secure_Job_Select->GetStringSelection();
	}
	else if(m_Notebook->GetPageText(m_Notebook->GetSelection()) == _("Script")){
		strTitle = strTitle + m_Script_Name->GetStringSelection();
	}
	else if(m_Notebook->GetPageText(m_Notebook->GetSelection()) == _("Rules")){
		strTitle = strTitle + m_Rules_Name->GetStringSelection();
	}
	else if(m_Notebook->GetPageText(m_Notebook->GetSelection()) == _("Variables")){
		strTitle = strTitle +m_Variables_Name->GetStringSelection();
	}	
	if(strTitle == wxT("Toucan - ")){
		this->SetTitle(wxT("Toucan"));
	}
	else{
		this->SetTitle(strTitle);
	}
}

void frmMain::JobAdd(wxComboBox* box){
	wxArrayString existing = GetJobs(wxEmptyString);
	wxTextEntryDialog dialog(this,  _("Please enter the name for the new job"), _("New Job"));
	if (dialog.ShowModal() == wxID_OK && dialog.GetValue() != wxEmptyString){
		for(unsigned int i = 0; i < existing.Count(); i++){
			if(existing.Item(i).Lower() == dialog.GetValue().Lower()){
				wxMessageBox(_("There is already a job with this name"), _("Error"), wxOK|wxICON_ERROR);
				return;
			}
		}
		box->Append(dialog.GetValue());
		box->SetStringSelection(dialog.GetValue());
		SetTitleBarText();
		ClearToDefault();
	}
}

void frmMain::JobRemove(wxComboBox* box){
	if (box->GetStringSelection() != wxEmptyString) {
		wxGetApp().m_Jobs_Config->DeleteGroup(box->GetStringSelection());
		wxGetApp().m_Jobs_Config->Flush();
		box->Delete(box->GetSelection());
	}	
	SetTitleBarText();
	ClearToDefault();
}

void frmMain::JobSave(const wxString &name, const wxString &type){
	JobData* data;
	if(type == wxT("Sync")){
		data = new SyncData(name);
	}
	else if(type == wxT("Backup")){
		data = new BackupData(name);
	}
	else if(type == wxT("Secure")){
		data = new SecureData(name);
	}
	else{
		return;
	}
	if (data->TransferFromForm(this)){
		if (data->GetName() != wxEmptyString) {
			try{
				data->TransferToFile();
			}
			catch(std::exception &arg){
				wxMessageBox(arg.what(), "Error", wxOK|wxICON_ERROR|wxCENTRE);
			}
		}
		else {
			wxTextEntryDialog dialog(this,  _("Please enter the name for the new job"), _("New Job"));
			if (dialog.ShowModal() == wxID_OK && dialog.GetValue() != wxEmptyString){
				data->SetName(dialog.GetValue());
				data->TransferToForm(this);
				data->TransferToFile();
			}
		}
	} 
	delete data;
}

void frmMain::JobLoad(const wxString &name, const wxString &type){
	wxBusyCursor cursor;
	ClearToDefault();
	JobData* data;
	if(type == wxT("Sync")){
		data = new SyncData(name);
	}
	else if(type == wxT("Backup")){
		data = new BackupData(name);
	}
	else if(type == wxT("Secure")){
		data = new SecureData(name);
	}
	else{
		return;
	}
	try{
		data->TransferFromFile();
		data->TransferToForm(this);
	}
	catch(std::exception &arg){
		wxMessageBox(arg.what(), "Error", wxOK|wxICON_ERROR|wxCENTRE);
	}
	SetTitleBarText();
	delete data;
}

void frmMain::ClearToDefault(){
	if(m_Notebook->GetPageText(m_Notebook->GetSelection()) == _("Sync")){
		m_Sync_Function->SetStringSelection(_("Copy"));
		m_Sync_Timestamp->SetValue(false);
		m_Sync_Attributes->SetValue(false);
		m_Sync_Ignore_Readonly->SetValue(false);
		m_Sync_Recycle->SetValue(false);
		m_SyncPreviewChanges->SetValue(false);
		m_SyncCheckFull->SetValue(false);
		m_SyncCheckShort->SetValue(false);
		m_SyncCheckSize->SetValue(false);
		m_SyncCheckTime->SetValue(false);
		m_Sync_Rules->SetStringSelection(wxEmptyString);
		m_Sync_Source_Txt->SetValue(wxEmptyString);
		m_Sync_Dest_Txt->SetValue(wxEmptyString);
		m_Sync_Source_Tree->DeleteAllItems();
		m_Sync_Source_Tree->AddRoot(wxT("Hidden root"));
		m_Sync_Dest_Tree->DeleteAllItems();
		m_Sync_Dest_Tree->AddRoot(wxT("Hidden root"));
		m_Sync_Source_Tree->SetPreview(false);
		m_Sync_Dest_Tree->SetPreview(false);
	}
	if(m_Notebook->GetPageText(m_Notebook->GetSelection()) == _("Backup")){
		m_Backup_Function->SetStringSelection(_("Complete"));
		m_Backup_Format->SetStringSelection(wxT("Zip"));
		m_Backup_Ratio->SetValue(3);
		m_Backup_Ratio_Text->SetLabel(_("Default"));
		m_Backup_IsPass->SetValue(false);
		m_Backup_Test->SetValue(false);
		m_BackupSolid->SetValue(false);
		m_Backup_Location->SetValue(wxEmptyString);
		m_Backup_TreeCtrl->DeleteAllItems();
		m_Backup_TreeCtrl->SetRules(new RuleSet(wxT("temp")));
		m_Backup_TreeCtrl->AddRoot(wxT("Hidden root"));
		m_Backup_Rules->SetStringSelection(wxEmptyString);
		m_BackupLocations->Clear();
	}
	if(m_Notebook->GetPageText(m_Notebook->GetSelection()) == _("Secure")){
		m_Secure_Function->SetStringSelection(_("Encrypt"));
		m_Secure_Rules->SetStringSelection(wxEmptyString);
		m_Secure_TreeCtrl->DeleteAllItems();
		m_Secure_TreeCtrl->SetRules(new RuleSet(wxT("temp")));
		m_Secure_TreeCtrl->AddRoot(wxT("Hidden root"));
		m_SecureLocations->Clear();
	}
}

void frmMain::CreateMenu(wxTreeEvent& event){
	menuTree = static_cast<DirCtrl*> (event.GetEventObject());
	DirCtrlItem* item = static_cast<DirCtrlItem*> (menuTree->GetItemData(event.GetItem()));
	menuTree->SelectItem(event.GetItem());

	wxString title = menuRules->GetStringSelection();
	if(title == wxEmptyString){
		return;
	}

	wxMenu menu(title);
	if(wxFileExists(item->GetFullPath())){
		menu.Append(ID_MENU_FILEEXCLUDE_EXTENSION, _("Exclude by extension"));
		menu.Append(ID_MENU_FILEEXCLUDE_NAME, _("Exclude by name"));
		menu.Append(ID_MENU_LOCATIONINCLUDE_EXTENSION, _("Include by extension"));
		menu.Append(ID_MENU_LOCATIONINCLUDE_NAME, _("Include by name"));
	}	
	else{
		menu.Append(ID_MENU_FOLDEREXCLUDE_NAME, _("Exclude by name"));
		menu.Append(ID_MENU_LOCATIONINCLUDE_NAME, _("Include by name"));
	}	
	this->PopupMenu(&menu, event.GetPoint() + menuTree->GetPosition());
}

//ID_MENU_FILEEXCLUDE_EXTENSION
void frmMain::OnMenuFileExcludeExtensionClick(wxCommandEvent& WXUNUSED(event)){
	if(menuRules->GetStringSelection() != wxEmptyString){
		DirCtrlItem* item = static_cast<DirCtrlItem*> (menuTree->GetItemData(menuTree->GetSelection()));
		RuleSet rules(menuRules->GetStringSelection());
		if(!rules.TransferFromFile())
			return;
		rules.Add(Rule("." + wxFileName(item->GetFullPath()).GetExt(), FileExclude, Simple));
		rules.TransferToFile();
		//Refresh the rules display if needed
		if(m_Rules_Name->GetStringSelection() == menuRules->GetStringSelection()){
			wxCommandEvent event;
			OnRulesComboSelected(event);
		}
	}
}

//ID_MENU_FILEEXCLUDE_NAME
void frmMain::OnMenuFileExcludeNameClick(wxCommandEvent& WXUNUSED(event)){
	if(menuRules->GetStringSelection() != wxEmptyString){
		DirCtrlItem* item = static_cast<DirCtrlItem*> (menuTree->GetItemData(menuTree->GetSelection()));
		RuleSet rules(menuRules->GetStringSelection());
		if(!rules.TransferFromFile())
			return;
		rules.Add(Rule(menuTree->GetItemText(item), FileExclude, Simple));
		rules.TransferToFile();
		//Refresh the rules display if needed
		if(m_Rules_Name->GetStringSelection() == menuRules->GetStringSelection()){
			wxCommandEvent event;
			OnRulesComboSelected(event);
		}
	}
}

//ID_MENU_LOCATIONINCLUDE_EXTENSION
void frmMain::OnMenuLocationIncludeExtensionClick(wxCommandEvent& WXUNUSED(event)){
	if(menuRules->GetStringSelection() != wxEmptyString){
		DirCtrlItem* item = static_cast<DirCtrlItem*> (menuTree->GetItemData(menuTree->GetSelection()));
		RuleSet rules(menuRules->GetStringSelection());
		if(!rules.TransferFromFile())
			return;
		rules.Add(Rule("." + wxFileName(item->GetFullPath()).GetExt(), FileInclude, Simple));
		rules.TransferToFile();
		//Refresh the rules display if needed
		if(m_Rules_Name->GetStringSelection() == menuRules->GetStringSelection()){
			wxCommandEvent event;
			OnRulesComboSelected(event);
		}
	}
}

//ID_MENU_LOCATIONINCLUDE_NAME
void frmMain::OnMenuLocationIncludeNameClick(wxCommandEvent& WXUNUSED(event)){
	if(menuRules->GetStringSelection() != wxEmptyString){
		DirCtrlItem* item = static_cast<DirCtrlItem*> (menuTree->GetItemData(menuTree->GetSelection()));
		RuleSet rules(menuRules->GetStringSelection());
		if(!rules.TransferFromFile())
			return;
		rules.Add(Rule(menuTree->GetItemText(item), FileExclude, Simple));
		rules.TransferToFile();
		//Refresh the rules display if needed
		if(m_Rules_Name->GetStringSelection() == menuRules->GetStringSelection()){
			wxCommandEvent event;
			OnRulesComboSelected(event);
		}
	}
}

//ID_MENU_FOLDEREXCLUDE_NAME
void frmMain::OnMenuFolderExcludeNameClick(wxCommandEvent& WXUNUSED(event)){
	if(menuRules->GetStringSelection() != wxEmptyString){
		DirCtrlItem* item = static_cast<DirCtrlItem*> (menuTree->GetItemData(menuTree->GetSelection()));
		RuleSet rules(menuRules->GetStringSelection());
		if(!rules.TransferFromFile())
			return;
		rules.Add(Rule(menuTree->GetItemText(item), FolderExclude, Simple));
		rules.TransferToFile();
		//Refresh the rules display if needed
		if(m_Rules_Name->GetStringSelection() == menuRules->GetStringSelection()){
			wxCommandEvent event;
			OnRulesComboSelected(event);
		}
	}
}

void frmMain::OnSyncTreeCtrlTooltip(wxTreeEvent& event){
	DirCtrl* tree = static_cast<DirCtrl*> (event.GetEventObject());
	DirCtrlItem* item = static_cast<DirCtrlItem*> (tree->GetItemData(event.GetItem()));

	if(item->GetColour() == wxColour(wxT("Blue"))){
		event.SetToolTip(_("Copied "));
	}
	else if(item->GetColour() == wxColour(wxT("Grey"))){
		event.SetToolTip(_("Deleted "));
	}
	else if(item->GetColour() == wxColour(wxT("Green"))){
		event.SetToolTip(_("Overwritten"));
	}
	else if(item->GetColour() == wxColour(wxT("Red"))){
		event.SetToolTip(_("If needed"));
	}
}

void frmMain::OnSecureFunctionSelected(wxCommandEvent& event){
	if(event.GetString() == _("Encrypt")){
		wxWindow::FindWindowById(ID_SECURE_FILESSATATIC)->SetLabel(_("Files to Encrypt"));
	}
	else{
		wxWindow::FindWindowById(ID_SECURE_FILESSATATIC)->SetLabel(_("Files to Decrypt"));
	}
}

void frmMain::OnBackupFunctionSelected(wxCommandEvent& event){
	if(event.GetString() == _("Restore")){
		wxWindow::FindWindowById(ID_BACKUP_LOCATIONSTATIC)->SetLabel(_("Location to Extract into"));
		wxWindow::FindWindowById(ID_BACKUP_FILESSTATIC)->SetLabel(_("Archives to Extract"));
	}
	else{
		wxWindow::FindWindowById(ID_BACKUP_LOCATIONSTATIC)->SetLabel(_("Backup Location"));
		wxWindow::FindWindowById(ID_BACKUP_FILESSTATIC)->SetLabel(_("Files to Backup"));
	}
}


void frmMain::SetSliderText(){
	switch(m_Backup_Ratio->GetValue()){
		case 0:
			m_Backup_Ratio_Text->SetLabel(_("None"));
			break;
		case 1:
			m_Backup_Ratio_Text->SetLabel(_("Fastest"));
			break;
		case 2:
			m_Backup_Ratio_Text->SetLabel(_("Fast"));
			break;
		case 3:
			m_Backup_Ratio_Text->SetLabel(_("Default"));
			break;
		case 4:
			m_Backup_Ratio_Text->SetLabel(_("Maximum"));
			break;
		case 5:
			m_Backup_Ratio_Text->SetLabel(_("Ultra"));
			break;
	}
}

wxArrayString frmMain::GetLanguages(){
	wxArrayString arrLang;
	wxString strPath = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("lang") + wxFILE_SEP_PATH;
	wxDir dir(strPath);
	wxString strFilename;
	bool blDir = dir.GetFirst(&strFilename);
	if(blDir){
		do {
			if(wxDirExists(strPath + strFilename))
			{
				if(wxFileExists(strPath + strFilename + wxFILE_SEP_PATH + wxT("lang.ini"))){
					wxFileConfig config(wxT(""), wxT(""), strPath + strFilename + wxFILE_SEP_PATH + wxT("lang.ini"));
					wxString strLanguage = config.Read(wxT("General/LanguageCode"));
					arrLang.Add(wxLocale::FindLanguageInfo(strLanguage)->Description);
				}
			}
		}
		while (dir.GetNext(&strFilename));
	} 
	return arrLang;
}

wxArrayString frmMain::GetTranslatorNames(){
	wxArrayString arrNames;
	wxString strPath = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("lang") + wxFILE_SEP_PATH;
	wxDir dir(strPath);
	wxString strFilename;
	bool blDir = dir.GetFirst(&strFilename);
	if(blDir){
		do {
			if(wxDirExists(strPath + strFilename))
			{
				if(wxFileExists(strPath + strFilename + wxFILE_SEP_PATH + wxT("lang.ini"))){
					wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), strPath + strFilename + wxFILE_SEP_PATH + wxT("lang.ini"));
					arrNames.Add(config->Read(wxT("General/Translator")));
				}
			}
		}
		while (dir.GetNext(&strFilename));
	} 
	return arrNames;
}

void frmMain::OnSettingsApplyClick(wxCommandEvent& WXUNUSED(event)){
	if(wxGetApp().IsReadOnly()){
		return;
	}
	wxGetApp().m_Settings->SetPosition(m_Notebook->GetPageText(m_Notebook->GetSelection()));
	wxGetApp().m_Settings->SetTabStyle(m_Settings_TabStyle->GetStringSelection());
	wxGetApp().m_Settings->SetLanguageCode(wxLocale::FindLanguageInfo(m_Settings_Language->GetStringSelection())->CanonicalName);
	wxGetApp().m_Settings->SetFont(m_Font->GetNativeFontInfoDesc());
	wxGetApp().m_Settings->SetRememberSync(m_Settings_RememberSync->GetValue());
	wxGetApp().m_Settings->SetRememberBackup(m_Settings_RememberBackup->GetValue());
	wxGetApp().m_Settings->SetRememberSecure(m_Settings_RememberSecure->GetValue());
	wxGetApp().m_Settings->SetEnableTooltips(m_Settings_EnableTooltips->GetValue());
	wxGetApp().m_Settings->SetSmallBorders(m_Settings_SmallBorders->GetValue());

	//Set the height and width to be relative to allow Toucan to fit properly when resolution is changed
	int height, width, x, y;
	wxClientDisplayRect(&x, &y, &width, &height);

	wxGetApp().m_Settings->SetHeight((double)(this->GetSize().GetHeight())/(height));
	wxGetApp().m_Settings->SetWidth((double)(this->GetSize().GetWidth())/(width));
	wxGetApp().m_Settings->SetX((double)(this->GetScreenPosition().x)/(width));
	wxGetApp().m_Settings->SetY((double)(this->GetScreenPosition().y)/(height));

	wxGetApp().m_Settings->TransferToFile();
	wxGetApp().RebuildForm();	
}

void frmMain::UpdateSizer(wxSizer *sizer){
	sizer->Layout();
	Refresh();
	Update();
}

void frmMain::OnSettingsFontClick(wxCommandEvent& WXUNUSED(event)){
	wxFont temp = wxGetFontFromUser(this, *m_Font, _("Choose font"));
	if(temp.IsOk()){
		delete m_Font;
		m_Font = new wxFont(temp);
	}
}

void frmMain::OnSyncSourceInsertClick(wxCommandEvent& WXUNUSED(event)){
	frmVariable dialog(this);
	if(dialog.ShowModal() == wxID_OK){
		wxBusyCursor cursor;
		m_Sync_Source_Tree->AddItem(Path::Normalise(dialog.GetValue()));
		m_Sync_Source_Txt->SetValue(dialog.GetValue());
	}
}

void frmMain::OnSyncDestInsertClick(wxCommandEvent& WXUNUSED(event)){
	frmVariable dialog(this);
	if(dialog.ShowModal() == wxID_OK){
		wxBusyCursor cursor;
		m_Sync_Dest_Tree->AddItem(Path::Normalise(dialog.GetValue()));
		m_Sync_Dest_Txt->SetValue(dialog.GetValue());
	}
}

void frmMain::OnSyncRefresh(wxCommandEvent& WXUNUSED(event)){
	wxCommandEvent event;
	OnSyncSourceRefresh(event);
	OnSyncDestRefresh(event);
}

void frmMain::OnSyncSourceRefresh(wxCommandEvent& WXUNUSED(event)){
	if(m_Sync_Source_Txt->GetValue() != wxEmptyString){
		TreeStateSaver saver(m_Sync_Source_Tree);
		m_Sync_Source_Tree->AddItem(Path::Normalise(m_Sync_Source_Txt->GetValue()));
	}
}

void frmMain::OnSyncDestRefresh(wxCommandEvent& WXUNUSED(event)){
	if(m_Sync_Dest_Txt->GetValue() != wxEmptyString){
		TreeStateSaver saver(m_Sync_Dest_Tree);
		m_Sync_Dest_Tree->AddItem(Path::Normalise(m_Sync_Dest_Txt->GetValue()));
	}
}

void frmMain::OnBackupRefresh(wxCommandEvent& WXUNUSED(event)){
	TreeStateSaver treesaver(m_Backup_TreeCtrl);
	TreeStateSaver dirsaver(m_Backup_DirCtrl);
	m_Backup_DirCtrl->ReCreateTree();
	m_Backup_TreeCtrl->DeleteAllItems();
	m_Backup_TreeCtrl->AddRoot(wxT("Hidden root"));
	for(unsigned int i = 0; i < m_BackupLocations->GetCount(); i++){
		m_Backup_TreeCtrl->AddItem(m_BackupLocations->Item(i));
	}
}

void frmMain::OnSecureRefresh(wxCommandEvent& WXUNUSED(event)){
	TreeStateSaver treesaver(m_Secure_TreeCtrl);
	TreeStateSaver dirsaver(m_Secure_DirCtrl);
	m_Secure_DirCtrl->ReCreateTree();
	m_Secure_TreeCtrl->DeleteAllItems();
	m_Secure_TreeCtrl->AddRoot(wxT("Hidden root"));
	for(unsigned int i = 0; i < m_SecureLocations->GetCount(); i++){
		m_Secure_TreeCtrl->AddItem(m_SecureLocations->Item(i));
	}
}

wxString frmMain::ToString(bool bl){
	return bl ? "true" : "false";
}

#if defined(__WXMSW__) && !defined(__MINGW32__)
WXLRESULT frmMain::MSWWindowProc(WXUINT message, WXWPARAM wparam, WXLPARAM lparam){
	if(message == m_TaskBarId){
		int major = wxPlatformInfo::Get().GetOSMajorVersion();
		int minor = wxPlatformInfo::Get().GetOSMajorVersion();
		//Only supported on windows 7 and greater
		if(major >= 6 && minor >= 1){
			if(m_Taskbar){
				m_Taskbar->Release();
			}
			
			CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_ALL, IID_ITaskbarList3, (void**)&m_Taskbar);
			m_Taskbar->HrInit();
		}
	}
	return wxFrame::MSWWindowProc(message, wparam, lparam);
}
#endif
