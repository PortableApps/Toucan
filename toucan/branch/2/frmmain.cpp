/////////////////////////////////////////////////////////////////////////////
// Name:        frmmain.cpp
// Purpose:     
// Author:      Steven Lamerton
// Modified by: 
// Created:     19/10/2007 11:57:09
// RCS-ID:      
// Copyright:   Copyright (c) Steven Lamerton 2006-2007
// Licence:     
/////////////////////////////////////////////////////////////////////////////


// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes



#include "dragndrop.h"
#include "treectrlfunc.h"

#include "frmmain.h"
#include "frmprogress.h"

#include "securedata.h"
#include "securethread.h"
#include "securefunctions.h"


#include "syncdata.h"
#include "sync.h"
#include "syncpreview.h"

#include "backupdata.h"
#include "backupprocess.h"

#include "toucan.h"

#include <wx/srchctrl.h>

////@begin XPM images
////@end XPM images

/*!
* frmMain type definition
*/

IMPLEMENT_CLASS( frmMain, wxFrame )


/*!
* frmMain event table definition
*/

BEGIN_EVENT_TABLE( frmMain, wxFrame )

////@begin frmMain event table entries
 EVT_MENU( ID_TOOL_OK, frmMain::OnToolOkClick )

 EVT_MENU( ID_TOOL_PREVIEW, frmMain::OnToolPreviewClick )

 EVT_COMBOBOX( ID_SYNC_JOB_SELECT, frmMain::OnSyncJobSelectSelected )

 EVT_BUTTON( ID_SYNC_JOB_SAVE, frmMain::OnSyncJobSaveClick )

 EVT_BUTTON( ID_SYNC_JOB_ADD, frmMain::OnSyncJobAddClick )

 EVT_BUTTON( ID_SYNC_JOB_REMOVE, frmMain::OnSyncJobRemoveClick )

 EVT_BUTTON( ID_SYNC_SOURCE_BTN, frmMain::OnSyncSourceBtnClick )

 EVT_BUTTON( ID_SYNC_DEST_BTN, frmMain::OnSyncDestBtnClick )

 EVT_BUTTON( ID_BACKUP_LOCATION, frmMain::OnBackupLocationClick )

 EVT_BUTTON( ID_BACKUP_ADD, frmMain::OnBackupAddClick )

 EVT_BUTTON( ID_BACKUP_REMOVE, frmMain::OnBackupRemoveClick )

 EVT_COMBOBOX( ID_SECURE_JOB_SELECT, frmMain::OnSecureJobSelectSelected )

 EVT_BUTTON( ID_SECURE_JOB_SAVE, frmMain::OnSecureJobSaveClick )

 EVT_BUTTON( ID_SECURE_JOB_ADD, frmMain::OnSecureJobAddClick )

 EVT_BUTTON( ID_SECURE_JOB_REMOVE, frmMain::OnSecureJobRemoveClick )

 EVT_BUTTON( ID_SECURE_ADD, frmMain::OnSecureAddClick )

 EVT_BUTTON( ID_SECURE_REMOVE, frmMain::OnSecureRemoveClick )

 EVT_COMBOBOX( ID_RULES_COMBO, frmMain::OnRulesComboSelected )

 EVT_BUTTON( ID_RULES_SAVE, frmMain::OnRulesSaveClick )

 EVT_BUTTON( ID_RULES_ADD, frmMain::OnRulesAddClick )

 EVT_BUTTON( ID_RULES_REMOVE, frmMain::OnRulesRemoveClick )

 EVT_BUTTON( ID_RULES_ADD_FILEEXCLUDE, frmMain::OnRulesAddFileexcludeClick )

 EVT_BUTTON( ID_RULES_REMOVE_FILEEXCLUDE, frmMain::OnRulesRemoveFileexcludeClick )

 EVT_BUTTON( ID_RULES_ADD_FOLDEREXCLUDE, frmMain::OnRulesAddFolderexcludeClick )

 EVT_BUTTON( ID_RULES_REMOVE_FOLDEREXCLUDE, frmMain::OnRulesRemoveFolderexcludeClick )

 EVT_BUTTON( ID_RULES_ADD_LOCATIONINCLUDE, frmMain::OnRulesAddLocationincludeClick )

 EVT_BUTTON( ID_RULES_REMOVE_LOCATIONINCLUDE, frmMain::OnRulesRemoveLocationincludeClick )

 EVT_BUTTON( ID_RULES_ADD_FILEDELETE, frmMain::OnRulesAddFiledeleteClick )

 EVT_BUTTON( ID_RULES_REMOVE_FILEDELETE, frmMain::OnRulesRemoveFiledeleteClick )

 EVT_TEXT( ID_SCRIPT_RICH, frmMain::OnScriptRichTextUpdated )

////@end frmMain event table entries

END_EVENT_TABLE()


/*!
* frmMain constructors
*/

frmMain::frmMain()
{
	Init();
}

frmMain::frmMain( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
	Init();
	Create( parent, id, caption, pos, size, style );
}


/*!
* frmMain creator
*/

bool frmMain::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
	////@begin frmMain creation
 wxFrame::Create( parent, id, caption, pos, size, style );

 CreateControls();
 Centre();
	////@end frmMain creation
	return true;
}


/*!
* frmMain destructor
*/

frmMain::~frmMain()
{
	////@begin frmMain destruction
 GetAuiManager().UnInit();
	////@end frmMain destruction
}


/*!
* Member initialisation
*/

void frmMain::Init()
{
	////@begin frmMain member initialisation
 m_Notebook = NULL;
 m_Sync_Job_Select = NULL;
 m_Sync_Rules = NULL;
 m_Sync_Source_Txt = NULL;
 m_Sync_Source_Tree = NULL;
 m_Sync_Dest_Txt = NULL;
 m_Sync_Dest_Tree = NULL;
 m_Sync_Function = NULL;
 m_Sync_Timestamp = NULL;
 m_Sync_Attributes = NULL;
 m_Sync_Ignore_Readonly = NULL;
 m_Sync_Ignore_DaylightS = NULL;
 m_Backup_Job_Select = NULL;
 m_Backup_Rules = NULL;
 m_Backup_Location = NULL;
 m_Backup_DirCtrl = NULL;
 m_Backup_TreeCtrl = NULL;
 m_Backup_Function = NULL;
 m_Backup_Format = NULL;
 m_Backup_Ratio = NULL;
 m_Backup_Pass = NULL;
 m_Backup_Repass = NULL;
 m_Secure_Rules = NULL;
 m_Secure_Job_Select = NULL;
 m_Secure_DirCtrl = NULL;
 m_Secure_TreeCtrl = NULL;
 m_Secure_Function = NULL;
 m_Secure_Format = NULL;
 m_Secure_Pass = NULL;
 m_Secure_Repass = NULL;
 m_Rules_Combo = NULL;
 m_Rules_FileExclude = NULL;
 m_Rules_FolderExclude = NULL;
 m_Rules_LocationInclude = NULL;
 m_Rules_FileDelete = NULL;
	////@end frmMain member initialisation
}


/*!
* Control creation for frmMain
*/

void frmMain::CreateControls()
{    
	////@begin frmMain content construction
 frmMain* itemFrame1 = this;

 GetAuiManager().SetManagedWindow(this);

 wxToolBar* itemToolBar2 = new wxToolBar( itemFrame1, ID_TOOLBAR, wxDefaultPosition, wxSize(120, -1), wxTB_FLAT|wxTB_HORIZONTAL|wxTB_TEXT|wxTB_NODIVIDER|wxTB_NOALIGN|wxNO_BORDER );
 itemToolBar2->SetToolBitmapSize(wxSize(32, 32));
 wxBitmap itemtool3Bitmap(wxNullBitmap);
 wxBitmap itemtool3BitmapDisabled;
 itemToolBar2->AddTool(ID_TOOL_OK, _("OK"), itemtool3Bitmap, itemtool3BitmapDisabled, wxITEM_NORMAL, _T(""), wxEmptyString);
 wxBitmap itemtool4Bitmap(wxNullBitmap);
 wxBitmap itemtool4BitmapDisabled;
 itemToolBar2->AddTool(ID_TOOL_PREVIEW, _("Preview"), itemtool4Bitmap, itemtool4BitmapDisabled, wxITEM_NORMAL, _T(""), wxEmptyString);
 itemToolBar2->Realize();
 itemFrame1->GetAuiManager().AddPane(itemToolBar2, wxAuiPaneInfo()
  .ToolbarPane().Name(_T("Pane5")).Top().Layer(10).LeftDockable(false).RightDockable(false).BottomDockable(false).CaptionVisible(false).CloseButton(false).DestroyOnClose(false).Resizable(false).Gripper(true).PaneBorder(false));

 m_Notebook = new wxAuiNotebook( itemFrame1, ID_AUINOTEBOOK, wxDefaultPosition, wxDefaultSize, wxAUI_NB_TAB_MOVE|wxAUI_NB_SCROLL_BUTTONS|wxNO_BORDER );

 wxPanel* itemPanel6 = new wxPanel( m_Notebook, ID_PANEL_SYNC, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
 wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxVERTICAL);
 itemPanel6->SetSizer(itemBoxSizer7);

 wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer7->Add(itemBoxSizer8, 1, wxGROW|wxALL, 0);
 wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer8->Add(itemBoxSizer9, 0, wxGROW|wxALL, 5);
 wxStaticBox* itemStaticBoxSizer10Static = new wxStaticBox(itemPanel6, wxID_ANY, _("Job Name"));
 wxStaticBoxSizer* itemStaticBoxSizer10 = new wxStaticBoxSizer(itemStaticBoxSizer10Static, wxHORIZONTAL);
 itemBoxSizer9->Add(itemStaticBoxSizer10, 0, wxALIGN_TOP|wxALL, 5);
 wxArrayString m_Sync_Job_SelectStrings;
 m_Sync_Job_Select = new wxComboBox( itemPanel6, ID_SYNC_JOB_SELECT, _T(""), wxDefaultPosition, wxDefaultSize, m_Sync_Job_SelectStrings, wxCB_DROPDOWN );
 itemStaticBoxSizer10->Add(m_Sync_Job_Select, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton12 = new wxBitmapButton( itemPanel6, ID_SYNC_JOB_SAVE, itemFrame1->GetBitmapResource(wxT("save.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemStaticBoxSizer10->Add(itemBitmapButton12, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton13 = new wxBitmapButton( itemPanel6, ID_SYNC_JOB_ADD, itemFrame1->GetBitmapResource(wxT("list-add.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemStaticBoxSizer10->Add(itemBitmapButton13, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton14 = new wxBitmapButton( itemPanel6, ID_SYNC_JOB_REMOVE, itemFrame1->GetBitmapResource(wxT("list-remove.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemStaticBoxSizer10->Add(itemBitmapButton14, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxStaticBox* itemStaticBoxSizer15Static = new wxStaticBox(itemPanel6, wxID_ANY, _("Rules"));
 wxStaticBoxSizer* itemStaticBoxSizer15 = new wxStaticBoxSizer(itemStaticBoxSizer15Static, wxHORIZONTAL);
 itemBoxSizer9->Add(itemStaticBoxSizer15, 0, wxALIGN_TOP|wxALL, 5);
 wxArrayString m_Sync_RulesStrings;
 m_Sync_Rules = new wxComboBox( itemPanel6, ID_SYNC_RULES, _T(""), wxDefaultPosition, wxDefaultSize, m_Sync_RulesStrings, wxCB_DROPDOWN );
 itemStaticBoxSizer15->Add(m_Sync_Rules, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBoxSizer* itemBoxSizer17 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer8->Add(itemBoxSizer17, 1, wxGROW|wxALL, 5);
 wxBoxSizer* itemBoxSizer18 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer17->Add(itemBoxSizer18, 1, wxGROW|wxALL, 5);
 wxBoxSizer* itemBoxSizer19 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer18->Add(itemBoxSizer19, 0, wxGROW|wxALL, 5);
 m_Sync_Source_Txt = new wxTextCtrl( itemPanel6, ID_SYNC_SOURCE_TXT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
 itemBoxSizer19->Add(m_Sync_Source_Txt, 1, wxALIGN_TOP|wxALL, 5);

 wxButton* itemButton21 = new wxButton( itemPanel6, ID_SYNC_SOURCE_BTN, _("..."), wxDefaultPosition, wxSize(25, 25), 0 );
 itemBoxSizer19->Add(itemButton21, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 m_Sync_Source_Tree = new wxTreeCtrl( itemPanel6, ID_SYNC_SOURCE_TREE, wxDefaultPosition, wxDefaultSize, wxTR_HAS_BUTTONS |wxTR_LINES_AT_ROOT|wxTR_SINGLE );
 itemBoxSizer18->Add(m_Sync_Source_Tree, 1, wxGROW|wxALL, 5);

 wxBoxSizer* itemBoxSizer23 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer17->Add(itemBoxSizer23, 1, wxGROW|wxALL, 5);
 wxBoxSizer* itemBoxSizer24 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer23->Add(itemBoxSizer24, 0, wxGROW|wxALL, 5);
 m_Sync_Dest_Txt = new wxTextCtrl( itemPanel6, ID_SYNC_DEST_TXT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
 itemBoxSizer24->Add(m_Sync_Dest_Txt, 1, wxALIGN_TOP|wxALL, 5);

 wxButton* itemButton26 = new wxButton( itemPanel6, ID_SYNC_DEST_BTN, _("..."), wxDefaultPosition, wxSize(25, 25), 0 );
 itemBoxSizer24->Add(itemButton26, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 m_Sync_Dest_Tree = new wxTreeCtrl( itemPanel6, ID_SYNC_DEST_TREE, wxDefaultPosition, wxDefaultSize, wxTR_HAS_BUTTONS |wxTR_LINES_AT_ROOT|wxTR_SINGLE );
 itemBoxSizer23->Add(m_Sync_Dest_Tree, 1, wxGROW|wxALL, 5);

 wxBoxSizer* itemBoxSizer28 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer8->Add(itemBoxSizer28, 0, wxGROW|wxALL, 5);
 wxArrayString m_Sync_FunctionStrings;
 m_Sync_FunctionStrings.Add(_("Copy"));
 m_Sync_FunctionStrings.Add(_("Update"));
 m_Sync_FunctionStrings.Add(_("Mirror (Copy)"));
 m_Sync_FunctionStrings.Add(_("Mirror (Update)"));
 m_Sync_FunctionStrings.Add(_("Equalise"));
 m_Sync_Function = new wxRadioBox( itemPanel6, ID_SYNC_FUNCTION, _("Function"), wxDefaultPosition, wxDefaultSize, m_Sync_FunctionStrings, 1, wxRA_SPECIFY_COLS );
 m_Sync_Function->SetSelection(0);
 itemBoxSizer28->Add(m_Sync_Function, 0, wxALIGN_TOP|wxALL, 5);

 wxStaticBox* itemStaticBoxSizer30Static = new wxStaticBox(itemPanel6, wxID_ANY, _("Other"));
 wxStaticBoxSizer* itemStaticBoxSizer30 = new wxStaticBoxSizer(itemStaticBoxSizer30Static, wxVERTICAL);
 itemBoxSizer28->Add(itemStaticBoxSizer30, 0, wxALIGN_TOP|wxALL, 5);
 m_Sync_Timestamp = new wxCheckBox( itemPanel6, ID_SYNC_TIMESTAMP, _("Retain Timestamps"), wxDefaultPosition, wxDefaultSize, 0 );
 m_Sync_Timestamp->SetValue(false);
 itemStaticBoxSizer30->Add(m_Sync_Timestamp, 0, wxALIGN_LEFT|wxALL, 5);

 m_Sync_Attributes = new wxCheckBox( itemPanel6, ID_SYNC_ATTRIB, _("Retain Attributes"), wxDefaultPosition, wxDefaultSize, 0 );
 m_Sync_Attributes->SetValue(false);
 itemStaticBoxSizer30->Add(m_Sync_Attributes, 0, wxALIGN_LEFT|wxALL, 5);

 m_Sync_Ignore_Readonly = new wxCheckBox( itemPanel6, ID_SYNC_IGNORERO, _("Ignore Read-Only"), wxDefaultPosition, wxDefaultSize, 0 );
 m_Sync_Ignore_Readonly->SetValue(false);
 itemStaticBoxSizer30->Add(m_Sync_Ignore_Readonly, 0, wxALIGN_LEFT|wxALL, 5);

 m_Sync_Ignore_DaylightS = new wxCheckBox( itemPanel6, ID_SYNC_IGNOREDS, _("Ignore Daylight Savings"), wxDefaultPosition, wxDefaultSize, 0 );
 m_Sync_Ignore_DaylightS->SetValue(false);
 itemStaticBoxSizer30->Add(m_Sync_Ignore_DaylightS, 0, wxALIGN_LEFT|wxALL, 5);

 m_Notebook->AddPage(itemPanel6, _("Sync"), false);

 wxPanel* itemPanel35 = new wxPanel( m_Notebook, ID_PANEL_BACKUP, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
 wxBoxSizer* itemBoxSizer36 = new wxBoxSizer(wxVERTICAL);
 itemPanel35->SetSizer(itemBoxSizer36);

 wxBoxSizer* itemBoxSizer37 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer36->Add(itemBoxSizer37, 0, wxGROW|wxALL, 5);
 wxStaticBox* itemStaticBoxSizer38Static = new wxStaticBox(itemPanel35, wxID_ANY, _("Job Name"));
 wxStaticBoxSizer* itemStaticBoxSizer38 = new wxStaticBoxSizer(itemStaticBoxSizer38Static, wxHORIZONTAL);
 itemBoxSizer37->Add(itemStaticBoxSizer38, 0, wxALIGN_TOP|wxALL, 5);
 wxArrayString m_Backup_Job_SelectStrings;
 m_Backup_Job_Select = new wxComboBox( itemPanel35, ID_BACKUP_JOB_SELECT, _T(""), wxDefaultPosition, wxDefaultSize, m_Backup_Job_SelectStrings, wxCB_DROPDOWN );
 itemStaticBoxSizer38->Add(m_Backup_Job_Select, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton40 = new wxBitmapButton( itemPanel35, ID_BACKUP_JOB_OPEN, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemStaticBoxSizer38->Add(itemBitmapButton40, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton41 = new wxBitmapButton( itemPanel35, ID_BACKUP_JOB_SAVE, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemStaticBoxSizer38->Add(itemBitmapButton41, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxStaticBox* itemStaticBoxSizer42Static = new wxStaticBox(itemPanel35, wxID_ANY, _("Rules"));
 wxStaticBoxSizer* itemStaticBoxSizer42 = new wxStaticBoxSizer(itemStaticBoxSizer42Static, wxHORIZONTAL);
 itemBoxSizer37->Add(itemStaticBoxSizer42, 0, wxALIGN_TOP|wxALL, 5);
 wxArrayString m_Backup_RulesStrings;
 m_Backup_Rules = new wxComboBox( itemPanel35, ID_BACKUP_RULES, _T(""), wxDefaultPosition, wxDefaultSize, m_Backup_RulesStrings, wxCB_DROPDOWN );
 itemStaticBoxSizer42->Add(m_Backup_Rules, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBoxSizer* itemBoxSizer44 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer36->Add(itemBoxSizer44, 0, wxGROW|wxALL, 5);
 wxBoxSizer* itemBoxSizer45 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer44->Add(itemBoxSizer45, 1, wxGROW|wxALL, 5);
 wxStaticText* itemStaticText46 = new wxStaticText( itemPanel35, wxID_STATIC, _("Backup Location"), wxDefaultPosition, wxDefaultSize, 0 );
 itemBoxSizer45->Add(itemStaticText46, 0, wxALIGN_LEFT|wxALL, 5);

 wxBoxSizer* itemBoxSizer47 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer45->Add(itemBoxSizer47, 1, wxGROW|wxALL, 0);
 m_Backup_Location = new wxTextCtrl( itemPanel35, ID_TEXTCTRL1, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
 itemBoxSizer47->Add(m_Backup_Location, 1, wxGROW|wxALL, 5);

 wxButton* itemButton49 = new wxButton( itemPanel35, ID_BACKUP_LOCATION, _("..."), wxDefaultPosition, wxSize(25, -1), 0 );
 itemBoxSizer47->Add(itemButton49, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBoxSizer* itemBoxSizer50 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer36->Add(itemBoxSizer50, 0, wxALIGN_LEFT|wxALL, 5);
 itemBoxSizer50->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

 wxStaticText* itemStaticText52 = new wxStaticText( itemPanel35, wxID_STATIC, _("Files to Backup"), wxDefaultPosition, wxDefaultSize, 0 );
 itemBoxSizer50->Add(itemStaticText52, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBoxSizer* itemBoxSizer53 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer36->Add(itemBoxSizer53, 1, wxGROW|wxALL, 5);
 m_Backup_DirCtrl = new wxGenericDirCtrl( itemPanel35, ID_BACKUP_DIRCTRL, _T(""), wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER, _T("All files (*.*)|*.*"), 0 );
 itemBoxSizer53->Add(m_Backup_DirCtrl, 1, wxGROW|wxALL, 5);

 wxBoxSizer* itemBoxSizer55 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer53->Add(itemBoxSizer55, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
 wxBitmapButton* itemBitmapButton56 = new wxBitmapButton( itemPanel35, ID_BACKUP_ADD, itemFrame1->GetBitmapResource(wxT("list-add.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer55->Add(itemBitmapButton56, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton57 = new wxBitmapButton( itemPanel35, ID_BACKUP_REMOVE, itemFrame1->GetBitmapResource(wxT("list-remove.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer55->Add(itemBitmapButton57, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 m_Backup_TreeCtrl = new wxTreeCtrl( itemPanel35, ID_BACKUP_TREECTRL, wxDefaultPosition, wxSize(100, 100), wxTR_HAS_BUTTONS |wxTR_LINES_AT_ROOT|wxTR_HIDE_ROOT|wxTR_SINGLE );
 itemBoxSizer53->Add(m_Backup_TreeCtrl, 1, wxGROW|wxALL, 5);

 wxBoxSizer* itemBoxSizer59 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer53->Add(itemBoxSizer59, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
 wxBitmapButton* itemBitmapButton60 = new wxBitmapButton( itemPanel35, ID_BACKUP_ADDVAR, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer59->Add(itemBitmapButton60, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 wxBoxSizer* itemBoxSizer61 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer36->Add(itemBoxSizer61, 0, wxALIGN_LEFT|wxALL, 5);
 wxArrayString m_Backup_FunctionStrings;
 m_Backup_FunctionStrings.Add(_("Complete"));
 m_Backup_FunctionStrings.Add(_("Update"));
 m_Backup_FunctionStrings.Add(_("Incremental"));
 m_Backup_FunctionStrings.Add(_("Resotre"));
 m_Backup_Function = new wxRadioBox( itemPanel35, ID_BACKUP_FUNCTION, _("Type"), wxDefaultPosition, wxDefaultSize, m_Backup_FunctionStrings, 1, wxRA_SPECIFY_COLS );
 m_Backup_Function->SetSelection(0);
 itemBoxSizer61->Add(m_Backup_Function, 0, wxALIGN_TOP|wxALL, 5);

 wxArrayString m_Backup_FormatStrings;
 m_Backup_FormatStrings.Add(_("Zip"));
 m_Backup_FormatStrings.Add(_("7-Zip"));
 m_Backup_Format = new wxRadioBox( itemPanel35, ID_BACKUP_FORMAT, _("Format"), wxDefaultPosition, wxDefaultSize, m_Backup_FormatStrings, 1, wxRA_SPECIFY_COLS );
 m_Backup_Format->SetSelection(0);
 itemBoxSizer61->Add(m_Backup_Format, 0, wxALIGN_TOP|wxALL, 5);

 wxArrayString m_Backup_RatioStrings;
 m_Backup_RatioStrings.Add(_("Low"));
 m_Backup_RatioStrings.Add(_("Normal"));
 m_Backup_RatioStrings.Add(_("Maximum"));
 m_Backup_Ratio = new wxRadioBox( itemPanel35, ID_BACKUP_RATIO, _("Compression Ratio"), wxDefaultPosition, wxDefaultSize, m_Backup_RatioStrings, 1, wxRA_SPECIFY_COLS );
 m_Backup_Ratio->SetSelection(1);
 itemBoxSizer61->Add(m_Backup_Ratio, 0, wxALIGN_TOP|wxALL, 5);

 wxStaticBox* itemStaticBoxSizer65Static = new wxStaticBox(itemPanel35, wxID_ANY, _("Password (If Required)"));
 wxStaticBoxSizer* itemStaticBoxSizer65 = new wxStaticBoxSizer(itemStaticBoxSizer65Static, wxVERTICAL);
 itemBoxSizer61->Add(itemStaticBoxSizer65, 0, wxALIGN_TOP|wxALL, 5);
 m_Backup_Pass = new wxTextCtrl( itemPanel35, ID_BACKUP_PASS, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
 itemStaticBoxSizer65->Add(m_Backup_Pass, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 m_Backup_Repass = new wxTextCtrl( itemPanel35, ID_BACKUP_REPASS, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
 itemStaticBoxSizer65->Add(m_Backup_Repass, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 m_Notebook->AddPage(itemPanel35, _("Backup"), false);

 wxPanel* itemPanel68 = new wxPanel( m_Notebook, ID_PANEL_SECURE, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
 wxBoxSizer* itemBoxSizer69 = new wxBoxSizer(wxVERTICAL);
 itemPanel68->SetSizer(itemBoxSizer69);

 wxBoxSizer* itemBoxSizer70 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer69->Add(itemBoxSizer70, 0, wxALIGN_LEFT|wxALL, 5);
 wxStaticBox* itemStaticBoxSizer71Static = new wxStaticBox(itemPanel68, wxID_ANY, _("Rules"));
 wxStaticBoxSizer* itemStaticBoxSizer71 = new wxStaticBoxSizer(itemStaticBoxSizer71Static, wxHORIZONTAL);
 itemBoxSizer70->Add(itemStaticBoxSizer71, 0, wxALIGN_TOP|wxALL, 5);
 wxArrayString m_Secure_RulesStrings;
 m_Secure_Rules = new wxComboBox( itemPanel68, ID_SECURE_RULES, _T(""), wxDefaultPosition, wxDefaultSize, m_Secure_RulesStrings, wxCB_DROPDOWN );
 itemStaticBoxSizer71->Add(m_Secure_Rules, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxStaticBox* itemStaticBoxSizer73Static = new wxStaticBox(itemPanel68, wxID_ANY, _("Job Name"));
 wxStaticBoxSizer* itemStaticBoxSizer73 = new wxStaticBoxSizer(itemStaticBoxSizer73Static, wxHORIZONTAL);
 itemBoxSizer70->Add(itemStaticBoxSizer73, 0, wxALIGN_TOP|wxALL, 5);
 wxArrayString m_Secure_Job_SelectStrings;
 m_Secure_Job_Select = new wxComboBox( itemPanel68, ID_SECURE_JOB_SELECT, _T(""), wxDefaultPosition, wxDefaultSize, m_Secure_Job_SelectStrings, wxCB_DROPDOWN );
 itemStaticBoxSizer73->Add(m_Secure_Job_Select, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton75 = new wxBitmapButton( itemPanel68, ID_SECURE_JOB_SAVE, itemFrame1->GetBitmapResource(wxT("save.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemStaticBoxSizer73->Add(itemBitmapButton75, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton76 = new wxBitmapButton( itemPanel68, ID_SECURE_JOB_ADD, itemFrame1->GetBitmapResource(wxT("list-add.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemStaticBoxSizer73->Add(itemBitmapButton76, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton77 = new wxBitmapButton( itemPanel68, ID_SECURE_JOB_REMOVE, itemFrame1->GetBitmapResource(wxT("list-remove.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemStaticBoxSizer73->Add(itemBitmapButton77, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBoxSizer* itemBoxSizer78 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer69->Add(itemBoxSizer78, 1, wxGROW|wxALL, 5);
 m_Secure_DirCtrl = new wxGenericDirCtrl( itemPanel68, ID_SECURE_DIRCTRL, _T(""), wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER, _T("All files (*.*)|*.*"), 0 );
 itemBoxSizer78->Add(m_Secure_DirCtrl, 1, wxGROW|wxALL, 5);

 wxBoxSizer* itemBoxSizer80 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer78->Add(itemBoxSizer80, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
 wxBitmapButton* itemBitmapButton81 = new wxBitmapButton( itemPanel68, ID_SECURE_ADD, itemFrame1->GetBitmapResource(wxT("list-add.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer80->Add(itemBitmapButton81, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton82 = new wxBitmapButton( itemPanel68, ID_SECURE_REMOVE, itemFrame1->GetBitmapResource(wxT("list-remove.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer80->Add(itemBitmapButton82, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 m_Secure_TreeCtrl = new wxTreeCtrl( itemPanel68, ID_SECURE_TREECTRL, wxDefaultPosition, wxSize(100, 100), wxTR_HAS_BUTTONS |wxTR_LINES_AT_ROOT|wxTR_HIDE_ROOT|wxTR_SINGLE );
 itemBoxSizer78->Add(m_Secure_TreeCtrl, 1, wxGROW|wxALL, 5);

 wxBoxSizer* itemBoxSizer84 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer78->Add(itemBoxSizer84, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
 wxBitmapButton* itemBitmapButton85 = new wxBitmapButton( itemPanel68, ID_SECURE_ADDVAR, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer84->Add(itemBitmapButton85, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton86 = new wxBitmapButton( itemPanel68, ID_SECURE_MAKERELATIVE, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer84->Add(itemBitmapButton86, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 wxBoxSizer* itemBoxSizer87 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer69->Add(itemBoxSizer87, 0, wxALIGN_LEFT|wxALL, 5);
 wxArrayString m_Secure_FunctionStrings;
 m_Secure_FunctionStrings.Add(_("Encrypt"));
 m_Secure_FunctionStrings.Add(_("Decrypt"));
 m_Secure_Function = new wxRadioBox( itemPanel68, ID_SECURE_FUNCTION, _("Function"), wxDefaultPosition, wxDefaultSize, m_Secure_FunctionStrings, 1, wxRA_SPECIFY_COLS );
 m_Secure_Function->SetSelection(0);
 itemBoxSizer87->Add(m_Secure_Function, 0, wxALIGN_TOP|wxALL, 5);

 wxArrayString m_Secure_FormatStrings;
 m_Secure_FormatStrings.Add(_("Rijndael"));
 m_Secure_FormatStrings.Add(_("Blowfish (Decrypt Only)"));
 m_Secure_Format = new wxRadioBox( itemPanel68, ID_SECURE_FORMAT, _("Format"), wxDefaultPosition, wxDefaultSize, m_Secure_FormatStrings, 1, wxRA_SPECIFY_COLS );
 m_Secure_Format->SetSelection(0);
 itemBoxSizer87->Add(m_Secure_Format, 0, wxALIGN_TOP|wxALL, 5);

 wxStaticBox* itemStaticBoxSizer90Static = new wxStaticBox(itemPanel68, wxID_ANY, _("Password (Repeated)"));
 wxStaticBoxSizer* itemStaticBoxSizer90 = new wxStaticBoxSizer(itemStaticBoxSizer90Static, wxVERTICAL);
 itemBoxSizer87->Add(itemStaticBoxSizer90, 0, wxALIGN_TOP|wxALL, 5);
 m_Secure_Pass = new wxTextCtrl( itemPanel68, ID_SECURE_PASS, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD );
 itemStaticBoxSizer90->Add(m_Secure_Pass, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 m_Secure_Repass = new wxTextCtrl( itemPanel68, ID_SECURE_REPASS, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD );
 itemStaticBoxSizer90->Add(m_Secure_Repass, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 m_Notebook->AddPage(itemPanel68, _("Secure"), false);

 wxPanel* itemPanel93 = new wxPanel( m_Notebook, ID_RULES, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
 wxBoxSizer* itemBoxSizer94 = new wxBoxSizer(wxVERTICAL);
 itemPanel93->SetSizer(itemBoxSizer94);

 wxBoxSizer* itemBoxSizer95 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer94->Add(itemBoxSizer95, 0, wxALIGN_LEFT|wxALL, 5);
 wxStaticText* itemStaticText96 = new wxStaticText( itemPanel93, wxID_STATIC, _("Rule set name"), wxDefaultPosition, wxDefaultSize, 0 );
 itemBoxSizer95->Add(itemStaticText96, 0, wxALIGN_LEFT|wxALL, 5);

 wxBoxSizer* itemBoxSizer97 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer95->Add(itemBoxSizer97, 0, wxALIGN_LEFT|wxALL, 5);
 wxArrayString m_Rules_ComboStrings;
 m_Rules_Combo = new wxComboBox( itemPanel93, ID_RULES_COMBO, _T(""), wxDefaultPosition, wxDefaultSize, m_Rules_ComboStrings, wxCB_READONLY );
 itemBoxSizer97->Add(m_Rules_Combo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton99 = new wxBitmapButton( itemPanel93, ID_RULES_SAVE, itemFrame1->GetBitmapResource(wxT("save.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer97->Add(itemBitmapButton99, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton100 = new wxBitmapButton( itemPanel93, ID_RULES_ADD, itemFrame1->GetBitmapResource(wxT("list-add.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer97->Add(itemBitmapButton100, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton101 = new wxBitmapButton( itemPanel93, ID_RULES_REMOVE, itemFrame1->GetBitmapResource(wxT("list-remove.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer97->Add(itemBitmapButton101, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBoxSizer* itemBoxSizer102 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer94->Add(itemBoxSizer102, 1, wxGROW|wxALL, 5);
 wxStaticText* itemStaticText103 = new wxStaticText( itemPanel93, wxID_STATIC, _("Files to exclude (file name, path or extension)"), wxDefaultPosition, wxDefaultSize, 0 );
 itemBoxSizer102->Add(itemStaticText103, 0, wxALIGN_LEFT|wxALL, 5);

 wxBoxSizer* itemBoxSizer104 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer102->Add(itemBoxSizer104, 1, wxGROW|wxALL, 5);
 wxArrayString m_Rules_FileExcludeStrings;
 m_Rules_FileExclude = new wxListBox( itemPanel93, ID_RULE_FILE_EXCLUDE, wxDefaultPosition, wxDefaultSize, m_Rules_FileExcludeStrings, wxLB_SINGLE );
 itemBoxSizer104->Add(m_Rules_FileExclude, 1, wxGROW|wxALL, 5);

 wxBoxSizer* itemBoxSizer106 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer104->Add(itemBoxSizer106, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
 wxBitmapButton* itemBitmapButton107 = new wxBitmapButton( itemPanel93, ID_RULES_ADD_FILEEXCLUDE, itemFrame1->GetBitmapResource(wxT("list-add.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer106->Add(itemBitmapButton107, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton108 = new wxBitmapButton( itemPanel93, ID_RULES_REMOVE_FILEEXCLUDE, itemFrame1->GetBitmapResource(wxT("list-remove.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer106->Add(itemBitmapButton108, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 wxBoxSizer* itemBoxSizer109 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer94->Add(itemBoxSizer109, 1, wxGROW|wxALL, 5);
 wxStaticText* itemStaticText110 = new wxStaticText( itemPanel93, wxID_STATIC, _("Folders to exclude (folder name or path)"), wxDefaultPosition, wxDefaultSize, 0 );
 itemBoxSizer109->Add(itemStaticText110, 0, wxALIGN_LEFT|wxALL, 5);

 wxBoxSizer* itemBoxSizer111 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer109->Add(itemBoxSizer111, 1, wxGROW|wxALL, 5);
 wxArrayString m_Rules_FolderExcludeStrings;
 m_Rules_FolderExclude = new wxListBox( itemPanel93, ID_RULES_FOLDER_EXCLUDE, wxDefaultPosition, wxDefaultSize, m_Rules_FolderExcludeStrings, wxLB_SINGLE );
 itemBoxSizer111->Add(m_Rules_FolderExclude, 1, wxGROW|wxALL, 5);

 wxBoxSizer* itemBoxSizer113 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer111->Add(itemBoxSizer113, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
 wxBitmapButton* itemBitmapButton114 = new wxBitmapButton( itemPanel93, ID_RULES_ADD_FOLDEREXCLUDE, itemFrame1->GetBitmapResource(wxT("list-add.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer113->Add(itemBitmapButton114, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton115 = new wxBitmapButton( itemPanel93, ID_RULES_REMOVE_FOLDEREXCLUDE, itemFrame1->GetBitmapResource(wxT("list-remove.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer113->Add(itemBitmapButton115, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 wxBoxSizer* itemBoxSizer116 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer94->Add(itemBoxSizer116, 1, wxGROW|wxALL, 5);
 wxStaticText* itemStaticText117 = new wxStaticText( itemPanel93, wxID_STATIC, _("Locations to always include (file/folder name, path or  file extension)"), wxDefaultPosition, wxDefaultSize, 0 );
 itemBoxSizer116->Add(itemStaticText117, 0, wxALIGN_LEFT|wxALL, 5);

 wxBoxSizer* itemBoxSizer118 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer116->Add(itemBoxSizer118, 1, wxGROW|wxALL, 5);
 wxArrayString m_Rules_LocationIncludeStrings;
 m_Rules_LocationInclude = new wxListBox( itemPanel93, ID_RULES_LOCATION_INCLUDE, wxDefaultPosition, wxDefaultSize, m_Rules_LocationIncludeStrings, wxLB_SINGLE );
 itemBoxSizer118->Add(m_Rules_LocationInclude, 1, wxGROW|wxALL, 5);

 wxBoxSizer* itemBoxSizer120 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer118->Add(itemBoxSizer120, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
 wxBitmapButton* itemBitmapButton121 = new wxBitmapButton( itemPanel93, ID_RULES_ADD_LOCATIONINCLUDE, itemFrame1->GetBitmapResource(wxT("list-add.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer120->Add(itemBitmapButton121, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton122 = new wxBitmapButton( itemPanel93, ID_RULES_REMOVE_LOCATIONINCLUDE, itemFrame1->GetBitmapResource(wxT("list-remove.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer120->Add(itemBitmapButton122, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 wxBoxSizer* itemBoxSizer123 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer94->Add(itemBoxSizer123, 1, wxGROW|wxALL, 5);
 wxStaticText* itemStaticText124 = new wxStaticText( itemPanel93, wxID_STATIC, _("Files to delete"), wxDefaultPosition, wxDefaultSize, 0 );
 itemBoxSizer123->Add(itemStaticText124, 0, wxALIGN_LEFT|wxALL, 5);

 wxBoxSizer* itemBoxSizer125 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer123->Add(itemBoxSizer125, 1, wxGROW|wxALL, 5);
 wxArrayString m_Rules_FileDeleteStrings;
 m_Rules_FileDelete = new wxListBox( itemPanel93, ID_RULES_FILE_DELETE, wxDefaultPosition, wxDefaultSize, m_Rules_FileDeleteStrings, wxLB_SINGLE );
 itemBoxSizer125->Add(m_Rules_FileDelete, 1, wxGROW|wxALL, 5);

 wxBoxSizer* itemBoxSizer127 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer125->Add(itemBoxSizer127, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
 wxBitmapButton* itemBitmapButton128 = new wxBitmapButton( itemPanel93, ID_RULES_ADD_FILEDELETE, itemFrame1->GetBitmapResource(wxT("list-add.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer127->Add(itemBitmapButton128, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton129 = new wxBitmapButton( itemPanel93, ID_RULES_REMOVE_FILEDELETE, itemFrame1->GetBitmapResource(wxT("list-remove.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer127->Add(itemBitmapButton129, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 m_Notebook->AddPage(itemPanel93, _("Rules"), false);

 wxPanel* itemPanel130 = new wxPanel( m_Notebook, ID_PANEL4, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );

 m_Notebook->AddPage(itemPanel130, _("Portable Variables"), false);

 wxPanel* itemPanel131 = new wxPanel( m_Notebook, ID_PANEL6, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
 wxBoxSizer* itemBoxSizer132 = new wxBoxSizer(wxVERTICAL);
 itemPanel131->SetSizer(itemBoxSizer132);

 wxBoxSizer* itemBoxSizer133 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer132->Add(itemBoxSizer133, 0, wxALIGN_LEFT|wxALL, 5);
 wxArrayString itemComboBox134Strings;
 wxComboBox* itemComboBox134 = new wxComboBox( itemPanel131, ID_SCRIPT_COMBO, _T(""), wxDefaultPosition, wxDefaultSize, itemComboBox134Strings, wxCB_DROPDOWN );
 itemBoxSizer133->Add(itemComboBox134, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton135 = new wxBitmapButton( itemPanel131, ID_SCRIPT_SAVE, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer133->Add(itemBitmapButton135, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton136 = new wxBitmapButton( itemPanel131, ID_SCRIPT_ADD, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer133->Add(itemBitmapButton136, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton137 = new wxBitmapButton( itemPanel131, ID_SCRIPT_REMOVE, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer133->Add(itemBitmapButton137, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBoxSizer* itemBoxSizer138 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer132->Add(itemBoxSizer138, 1, wxGROW|wxALL, 5);
 wxRichTextCtrl* itemRichTextCtrl139 = new wxRichTextCtrl( itemPanel131, ID_SCRIPT_RICH, _T(""), wxDefaultPosition, wxSize(100, 100), wxWANTS_CHARS );
 itemBoxSizer138->Add(itemRichTextCtrl139, 1, wxGROW|wxALL, 5);

 wxBoxSizer* itemBoxSizer140 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer132->Add(itemBoxSizer140, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
 wxButton* itemButton141 = new wxButton( itemPanel131, ID_SCRIPT_CHECK, _("Check Script"), wxDefaultPosition, wxDefaultSize, 0 );
 itemBoxSizer140->Add(itemButton141, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxButton* itemButton142 = new wxButton( itemPanel131, ID_SCRIPT_EXECUTE, _("Execute"), wxDefaultPosition, wxDefaultSize, 0 );
 itemBoxSizer140->Add(itemButton142, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 m_Notebook->AddPage(itemPanel131, _("Scripting"), false);

 wxPanel* itemPanel143 = new wxPanel( m_Notebook, ID_PANEL5, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );

 m_Notebook->AddPage(itemPanel143, _("Settings"), false);

 itemFrame1->GetAuiManager().AddPane(m_Notebook, wxAuiPaneInfo()
  .Name(_T("Pane3")).Centre().CaptionVisible(false).CloseButton(false).DestroyOnClose(false).Resizable(true).Floatable(false).PaneBorder(false));

 GetAuiManager().Update();

	////@end frmMain content construction
	//m_Backup_List->SetDropTarget(new DnDFileList(m_Backup_List));

	//Set the drag and drop targets
	m_Sync_Source_Txt->SetDropTarget(new DnDFileText(m_Sync_Source_Txt));
	m_Sync_Dest_Txt->SetDropTarget(new DnDFileText(m_Sync_Dest_Txt));

	//Add the hidden roots to the wxTreeCtrls
	m_Backup_TreeCtrl->AddRoot(wxT("Hidden Root"));
	m_Secure_TreeCtrl->AddRoot(wxT("Hidden Root"));
	
	//Set up the rules boxes
	SetRulesBox(m_Sync_Rules);
	SetRulesBox(m_Backup_Rules);
	SetRulesBox(m_Secure_Rules);
	SetRulesBox(m_Rules_Combo);
	
	//Set up the jobs boxes
	SetJobsBox(m_Sync_Job_Select, _("Sync"));
	SetJobsBox(m_Backup_Job_Select, _("Backup"));
	SetJobsBox(m_Secure_Job_Select, _("Secure"));

}


/*!
* Should we show tooltips?
*/

bool frmMain::ShowToolTips()
{
	return true;
}

/*!
* Get bitmap resources
*/

wxBitmap frmMain::GetBitmapResource( const wxString& name )
{
	// Bitmap retrieval
	////@begin frmMain bitmap retrieval
 wxUnusedVar(name);
 if (name == _T("save.png"))
 {
  wxBitmap bitmap(_T("save.png"), wxBITMAP_TYPE_PNG);
  return bitmap;
 }
 else if (name == _T("list-add.png"))
 {
  wxBitmap bitmap(_T("list-add.png"), wxBITMAP_TYPE_PNG);
  return bitmap;
 }
 else if (name == _T("list-remove.png"))
 {
  wxBitmap bitmap(_T("list-remove.png"), wxBITMAP_TYPE_PNG);
  return bitmap;
 }
 return wxNullBitmap;
	////@end frmMain bitmap retrieval
}

/*!
* Get icon resources
*/

wxIcon frmMain::GetIconResource( const wxString& name )
{
	// Icon retrieval
	////@begin frmMain icon retrieval
 wxUnusedVar(name);
 return wxNullIcon;
	////@end frmMain icon retrieval

}


/*!
* wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BACKUP_ADD
*/

void frmMain::OnBackupAddClick( wxCommandEvent& event )
{
	wxGetApp().AppendBackupLocation(m_Backup_DirCtrl->GetPath());
	AddDirectoryThread *thread = new AddDirectoryThread(m_Backup_DirCtrl->GetTreeCtrl()->GetItemText(m_Backup_DirCtrl->GetTreeCtrl()->GetSelection()),m_Backup_TreeCtrl);
	thread->Create();
	thread->Run();
}


/*!
* wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BACKUP_REMOVE
*/

void frmMain::OnBackupRemoveClick( wxCommandEvent& event )
{
	m_Backup_TreeCtrl->Delete(m_Backup_TreeCtrl->GetSelection());
	//wxGetApp().RemoveSecureLocation(m_Backup_TreeCtrl->GetItemText(m_Backup_TreeCtrl->GetSelection()));
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SECURE_ADD
 */

void frmMain::OnSecureAddClick( wxCommandEvent& event )
{
	wxGetApp().AppendSecureLocation(m_Secure_DirCtrl->GetPath());
	AddDirectoryThread *thread = new AddDirectoryThread(m_Secure_DirCtrl->GetTreeCtrl()->GetItemText(m_Secure_DirCtrl->GetTreeCtrl()->GetSelection()),m_Secure_TreeCtrl);
	thread->Create();
	thread->Run();
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SECURE_REMOVE
 */

void frmMain::OnSecureRemoveClick( wxCommandEvent& event )
{
	
	for(unsigned int i = 0; i < wxGetApp().GetSecureLocations().Count(); i++){;
		if(wxGetApp().GetSecureLocations().Item(i) == m_Secure_TreeCtrl->GetItemText(m_Secure_TreeCtrl->GetSelection())){
			wxGetApp().RemoveSecureLocation(i);
		}
		if(wxGetApp().GetSecureLocations().Item(i).AfterLast(wxFILE_SEP_PATH) == m_Secure_TreeCtrl->GetItemText(m_Secure_TreeCtrl->GetSelection())){
			wxGetApp().RemoveSecureLocation(i);
		}
	}
	m_Secure_TreeCtrl->Delete(m_Secure_TreeCtrl->GetSelection()); 
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SYNC_SOURCE_BTN
 */

void frmMain::OnSyncSourceBtnClick( wxCommandEvent& event )
{
		wxDirDialog dialog(this,_("Please select the source folder."), wxEmptyString);
		if (dialog.ShowModal() == wxID_OK){
			AddDirectoryThread *thread = new AddDirectoryThread(dialog.GetPath(),m_Sync_Source_Tree);
			thread->Create();
			thread->Run();
			m_Sync_Source_Txt->SetValue(dialog.GetPath());
		}
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SYNC_DEST_BTN
 */

void frmMain::OnSyncDestBtnClick( wxCommandEvent& event )
{
		wxDirDialog dialog(this,_("Please select the desination folder."), wxEmptyString);
		if (dialog.ShowModal() == wxID_OK){
			AddDirectoryThread *thread = new AddDirectoryThread(dialog.GetPath(),m_Sync_Dest_Tree);
			thread->Create();
			thread->Run();
			m_Sync_Dest_Txt->SetValue(dialog.GetPath());
		}
}

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_OK
 */

void frmMain::OnToolOkClick( wxCommandEvent& event )
{
	frmProgress *window = new frmProgress(NULL, ID_FRMPROGRESS, _("Progress"));
	window->Show();
	if(m_Notebook->GetPageText(m_Notebook->GetSelection()) == _("Sync")){
		SyncData data;
		data.TransferFromForm(this);
		Rules rules;
		if(m_Sync_Rules->GetStringSelection() != wxEmptyString){
			rules.TransferFromFile(m_Sync_Rules->GetStringSelection());
		}
		SyncThread *thread = new SyncThread(data, rules, window, this);
		thread->Create();
		thread->Run();
	}
	//Secure
	if(m_Notebook->GetPageText(m_Notebook->GetSelection()) == _("Secure")){
		SecureData data;
		data.TransferFromForm(this, true);
		Rules rules;
		if(m_Secure_Rules->GetStringSelection() != wxEmptyString){
			rules.TransferFromFile(m_Secure_Rules->GetStringSelection());
		}
		SecureThread *thread = new SecureThread(data, rules, window);
		thread->Create();
		thread->Run();
	}
	if(m_Notebook->GetPageText(m_Notebook->GetSelection()) == _("Backup")){
		window->m_OK->Enable(false);
		window->m_Save->Enable(false);
		BackupData data;
		data.TransferFromForm(this);
		Rules rules;
		if(m_Backup_Rules->GetStringSelection() != wxEmptyString){
			rules.TransferFromFile(m_Backup_Rules->GetStringSelection());
		}
		wxString strCommand = data.CreateCommand();
		PipedProcess *process = new PipedProcess(window);
        long lngPID  = wxExecute(strCommand, wxEXEC_ASYNC|wxEXEC_NODISABLE, process);
		wxGetApp().RegisterProcess(process);
	}

}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_SCRIPT_RICH
 */

void frmMain::OnScriptRichTextUpdated( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_TEXT_UPDATED event handler for ID_SCRIPT_RICH in frmMain.
 // Before editing this code, remove the block markers.
 event.Skip();
////@end wxEVT_COMMAND_TEXT_UPDATED event handler for ID_SCRIPT_RICH in frmMain. 
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RULES_ADD_FILEEXCLUDE
 */

void frmMain::OnRulesAddFileexcludeClick( wxCommandEvent& event )
{
	wxTextEntryDialog *dialog = new wxTextEntryDialog(this, _("File to exclude"), _("This dialog needs to be replaced"));
	if(dialog->ShowModal() == wxID_OK){
		m_Rules_FileExclude->Append(dialog->GetValue());
	}
	delete dialog;
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RULES_REMOVE_FILEEXCLUDE
 */

void frmMain::OnRulesRemoveFileexcludeClick( wxCommandEvent& event )
{
	m_Rules_FileExclude->Delete(m_Rules_FileExclude->GetSelection());
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RULES_ADD_FOLDEREXCLUDE
 */

void frmMain::OnRulesAddFolderexcludeClick( wxCommandEvent& event )
{
	wxTextEntryDialog *dialog = new wxTextEntryDialog(this, _("Folder to exclude"), _("This dialog needs to be replaced"));
	if(dialog->ShowModal() == wxID_OK){
		m_Rules_FolderExclude->Append(dialog->GetValue());
	}
	delete dialog;
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RULES_REMOVE_FOLDEREXCLUDE
 */

void frmMain::OnRulesRemoveFolderexcludeClick( wxCommandEvent& event )
{
	m_Rules_FolderExclude->Delete(m_Rules_FolderExclude->GetSelection());
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RULES_ADD_LocationInclude
 */

void frmMain::OnRulesAddFiledeleteClick( wxCommandEvent& event )
{
	wxTextEntryDialog *dialog = new wxTextEntryDialog(this, _("File to Delete"), _("This dialog needs to be replaced"));
	if(dialog->ShowModal() == wxID_OK){
		m_Rules_FileDelete->Append(dialog->GetValue());
	}
	delete dialog; 
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RULES_REMOVE_LocationInclude
 */

void frmMain::OnRulesRemoveFiledeleteClick( wxCommandEvent& event )
{
	m_Rules_LocationInclude->Delete(m_Rules_FileDelete->GetSelection());
}



/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RULES_SAVE
 */

void frmMain::OnRulesSaveClick( wxCommandEvent& event )
{
	//Get all of the form data into arraystrings
	wxArrayString arrLocationsInclude, arrFolderExclude, arrFileExclude, arrFileDelete;
	for(unsigned int i = 0; i < m_Rules_LocationInclude->GetCount(); i++){
		arrLocationsInclude.Add(m_Rules_LocationInclude->GetString(i));
	}
	for(unsigned int j = 0; j < m_Rules_FolderExclude->GetCount(); j++){
		arrFolderExclude.Add(m_Rules_FolderExclude->GetString(j));
	}
	for(unsigned int k = 0; k < m_Rules_FileExclude->GetCount(); k++){
		arrFileExclude.Add(m_Rules_FileExclude->GetString(k));
	}
	for(unsigned int l = 0; l < m_Rules_FileDelete->GetCount(); l++){
		arrFileDelete.Add(m_Rules_FileDelete->GetString(l));
	}
	
	Rules rules;
	rules.SetLocationsToInclude(arrLocationsInclude);
	rules.SetFilesToExclude(arrFileExclude);
	rules.SetFoldersToExclude(arrFolderExclude);
	rules.SetFilesToDelete(arrFileDelete);
	if(m_Rules_Combo->GetStringSelection() != wxEmptyString){
		rules.TransferToFile(m_Rules_Combo->GetStringSelection());
	}
	else{
		ErrorBox(_("You must select a name for this set of Rules"));
	}
	//Set up the rules boxes
	SetRulesBox(m_Sync_Rules);
	SetRulesBox(m_Backup_Rules);
	SetRulesBox(m_Secure_Rules);

}



/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RULES_ADD
 */

void frmMain::OnRulesAddClick( wxCommandEvent& event )
{
	wxTextEntryDialog *dialog = new wxTextEntryDialog(this, _("File to include"), _("This dialog needs to be replaced"));
	if(dialog->ShowModal() == wxID_OK){
		m_Rules_Combo->AppendString(dialog->GetValue());
		m_Rules_Combo->SetStringSelection(dialog->GetValue());
	}
	delete dialog; 
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RULES_REMOVE
 */

void frmMain::OnRulesRemoveClick( wxCommandEvent& event )
{
	m_Rules_Combo->Delete(m_Rules_Combo->GetSelection());
}


/*!
 * wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_RULES_COMBO
 */

void frmMain::OnRulesComboSelected( wxCommandEvent& event )
{
	Rules rules;
	if(rules.TransferFromFile(m_Rules_Combo->GetStringSelection())){
	for(unsigned int i = 0; i < rules.GetLocationsToIncude().GetCount(); i++){
		m_Rules_LocationInclude->Append(rules.GetLocationsToIncude().Item(i));
	}
	for(unsigned int j = 0; j < rules.GetFilesToExclude().GetCount(); j++){
		m_Rules_FileExclude->Append(rules.GetFilesToExclude().Item(j));		
	}
	for(unsigned int k = 0; k < rules.GetFoldersToExclude().GetCount(); k++){
		m_Rules_FolderExclude->Append(rules.GetFoldersToExclude().Item(k));
	}
	for(unsigned int l = 0; l < rules.GetFilesToDelete().GetCount(); l++){
		m_Rules_FileDelete->Append(rules.GetFilesToDelete().Item(l));
	}
	}
	
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_PREVIEW
 */

void frmMain::OnToolPreviewClick( wxCommandEvent& event )
{
	if(m_Notebook->GetPageText(m_Notebook->GetSelection()) == _("Secure")){
		Rules rules;
		if(m_Secure_Rules->GetStringSelection() != wxEmptyString){
			rules.TransferFromFile(m_Secure_Rules->GetStringSelection());
		}
		PreviewSecure(rules, m_Secure_TreeCtrl);

	}
	if(m_Notebook->GetPageText(m_Notebook->GetSelection()) == _("Sync")){
		SyncData data;
		data.TransferFromForm(this);
		Rules rules;
		if(m_Sync_Rules->GetStringSelection() != wxEmptyString){
			rules.TransferFromFile(m_Sync_Rules->GetStringSelection());
		}
		PreviewSyncThread *thread = new PreviewSyncThread(data, rules, this);
		thread->Create();
		thread->Run();
	}
}



/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SECURE_JOB_SAVE
 */

void frmMain::OnSecureJobSaveClick( wxCommandEvent& event )
{
	SecureData data;
	if(data.TransferFromForm(this, false)){
		if(m_Secure_Job_Select->GetStringSelection() != wxEmptyString){
			data.TransferToFile(m_Secure_Job_Select->GetStringSelection());
		}
		else{
			ErrorBox(_("Please chose a job to save to"));
		}
	}
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("Jobs.ini") );
	config->Write(m_Secure_Job_Select->GetStringSelection() + wxT("/Rules"),  m_Secure_Rules->GetStringSelection());	
	config->Write(m_Secure_Job_Select->GetStringSelection() + wxT("/Type"),  _("Secure"));	
	delete config;

}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SECURE_JOB_ADD
 */

void frmMain::OnSecureJobAddClick( wxCommandEvent& event )
{
	wxTextEntryDialog *dialog = new wxTextEntryDialog(this, _("Job name"));
	if(dialog->ShowModal() == wxID_OK){
		m_Secure_Job_Select->Append(dialog->GetValue());
		m_Secure_Job_Select->SetStringSelection(dialog->GetValue());
	}
	delete dialog;
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SECURE_JOB_REMOVE
 */

void frmMain::OnSecureJobRemoveClick( wxCommandEvent& event )
{
	if(m_Secure_Job_Select->GetStringSelection() != wxEmptyString){
		wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("Jobs.ini"));
		config->DeleteGroup(m_Secure_Job_Select->GetStringSelection());
		m_Secure_Job_Select->Delete(m_Secure_Job_Select->GetSelection());
	}
}


/*!
 * wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_SECURE_JOB_SELECT
 */

void frmMain::OnSecureJobSelectSelected( wxCommandEvent& event )
{
	SecureData data;
	if(data.TransferFromFile(m_Secure_Job_Select->GetStringSelection(), true)){
		//data.Output();
		data.TransferToForm(this);	
		wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("Jobs.ini") );
		m_Secure_Rules->SetStringSelection(config->Read(m_Secure_Job_Select->GetStringSelection() + wxT("/Rules")));	
		delete config;
	}
		
}




/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SYNC_JOB_SAVE
 */

void frmMain::OnSyncJobSaveClick( wxCommandEvent& event )
{
	SyncData data;
	if(data.TransferFromForm(this)){
		if(m_Sync_Job_Select->GetStringSelection() != wxEmptyString){
			data.TransferToFile(m_Sync_Job_Select->GetStringSelection());
		}
		else{
			ErrorBox(_("Please chose a job to save to"));
		}
	}
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("Jobs.ini") );
	config->Write(m_Sync_Job_Select->GetStringSelection() + wxT("/Rules"),  m_Sync_Job_Select->GetStringSelection());	
	config->Write(m_Sync_Job_Select->GetStringSelection() + wxT("/Type"),  _("Sync"));	
	delete config;
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SYNC_JOB_ADD
 */

void frmMain::OnSyncJobAddClick( wxCommandEvent& event )
{
	wxTextEntryDialog *dialog = new wxTextEntryDialog(this, _("Job name"));
	if(dialog->ShowModal() == wxID_OK){
		m_Sync_Job_Select->Append(dialog->GetValue());
		m_Sync_Job_Select->SetStringSelection(dialog->GetValue());
	}
	delete dialog;
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SYNC_JOB_REMOVE
 */

void frmMain::OnSyncJobRemoveClick( wxCommandEvent& event )
{
	if(m_Sync_Job_Select->GetStringSelection() != wxEmptyString){
		wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("Jobs.ini"));
		config->DeleteGroup(m_Sync_Job_Select->GetStringSelection());
		m_Sync_Job_Select->Delete(m_Sync_Job_Select->GetSelection());
	}
}


/*!
 * wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_SYNC_JOB_SELECT
 */

void frmMain::OnSyncJobSelectSelected( wxCommandEvent& event )
{
	SyncData data;
	if(data.TransferFromFile(m_Sync_Job_Select->GetStringSelection())){
		//data.Output();
		data.TransferToForm(this);	
		wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("Jobs.ini") );
		m_Sync_Rules->SetStringSelection(config->Read(m_Sync_Job_Select->GetStringSelection() + wxT("/Rules")));	
		delete config;
	}
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RULES_ADD_FILEINCLUDE
 */

void frmMain::OnRulesAddLocationincludeClick( wxCommandEvent& event )
{
	wxTextEntryDialog *dialog = new wxTextEntryDialog(this, _("Location to include"), _("This dialog needs to be replaced"));
	if(dialog->ShowModal() == wxID_OK){
		m_Rules_LocationInclude->Append(dialog->GetValue());
	}
	delete dialog; 
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RULES_REMOVE_FILEINCLUDE
 */

void frmMain::OnRulesRemoveLocationincludeClick( wxCommandEvent& event )
{
	m_Rules_FileDelete->Delete(m_Rules_LocationInclude->GetSelection());
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BACKUP_LOCATION
 */

void frmMain::OnBackupLocationClick( wxCommandEvent& event )
{
		wxFileDialog dialog(this,_("Need to change this dialog."), wxEmptyString);
		if (dialog.ShowModal() == wxID_OK){
			m_Backup_Location->SetValue(dialog.GetPath());
		}
}

