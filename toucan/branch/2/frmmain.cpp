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

#include "frmmain.h"

#include "dragndrop.h"
#include "treectrlfunc.h"
#include "securedata.h"
#include "securethread.h"
#include "frmprogress.h"
#include "securefunctions.h"

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

 EVT_BUTTON( ID_SYNC_SOURCE_BTN, frmMain::OnSyncSourceBtnClick )

 EVT_BUTTON( ID_BACKUP_ADD, frmMain::OnBackupAddClick )

 EVT_BUTTON( ID_BACKUP_REMOVE, frmMain::OnBackupRemoveClick )

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

 EVT_BUTTON( ID_RULES_ADD_FILEINCLUDE, frmMain::OnRulesAddFileincludeClick )

 EVT_BUTTON( ID_RULES_REMOVE_FILEINCLUDE, frmMain::OnRulesRemoveFileincludeClick )

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
 m_Backup_DirCtrl = NULL;
 m_Backup_TreeCtrl = NULL;
 m_Backup_Function = NULL;
 m_Backup_Format = NULL;
 m_Backup_Ratio = NULL;
 m_Backup_Pass = NULL;
 m_Backup_Repass = NULL;
 m_Secure_Job_Select = NULL;
 m_Secure_Rules = NULL;
 m_Secure_DirCtrl = NULL;
 m_Secure_TreeCtrl = NULL;
 m_Secure_Function = NULL;
 m_Secure_Format = NULL;
 m_Secure_Pass = NULL;
 m_Secure_Repass = NULL;
 m_Rules_Combo = NULL;
 m_Rules_FileExclude = NULL;
 m_Rules_FolderExclude = NULL;
 m_Rules_FileInclude = NULL;
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

 m_Notebook = new wxAuiNotebook( itemFrame1, ID_AUINOTEBOOK, wxDefaultPosition, wxDefaultSize, wxAUI_NB_SCROLL_BUTTONS|wxNO_BORDER );

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

 wxBitmapButton* itemBitmapButton12 = new wxBitmapButton( itemPanel6, ID_SYNC_JOB_OPEN, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemStaticBoxSizer10->Add(itemBitmapButton12, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton13 = new wxBitmapButton( itemPanel6, ID_SYNC_JOB_SAVE, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemStaticBoxSizer10->Add(itemBitmapButton13, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxStaticBox* itemStaticBoxSizer14Static = new wxStaticBox(itemPanel6, wxID_ANY, _("Rules"));
 wxStaticBoxSizer* itemStaticBoxSizer14 = new wxStaticBoxSizer(itemStaticBoxSizer14Static, wxHORIZONTAL);
 itemBoxSizer9->Add(itemStaticBoxSizer14, 0, wxALIGN_TOP|wxALL, 5);
 wxArrayString m_Sync_RulesStrings;
 m_Sync_Rules = new wxComboBox( itemPanel6, ID_SYNC_RULES, _T(""), wxDefaultPosition, wxDefaultSize, m_Sync_RulesStrings, wxCB_DROPDOWN );
 itemStaticBoxSizer14->Add(m_Sync_Rules, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBoxSizer* itemBoxSizer16 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer8->Add(itemBoxSizer16, 1, wxGROW|wxALL, 5);
 wxBoxSizer* itemBoxSizer17 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer16->Add(itemBoxSizer17, 1, wxGROW|wxALL, 5);
 wxBoxSizer* itemBoxSizer18 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer17->Add(itemBoxSizer18, 0, wxGROW|wxALL, 5);
 m_Sync_Source_Txt = new wxTextCtrl( itemPanel6, ID_SYNC_SOURCE_TXT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
 itemBoxSizer18->Add(m_Sync_Source_Txt, 1, wxALIGN_TOP|wxALL, 5);

 wxButton* itemButton20 = new wxButton( itemPanel6, ID_SYNC_SOURCE_BTN, _("..."), wxDefaultPosition, wxSize(25, 25), 0 );
 itemBoxSizer18->Add(itemButton20, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 m_Sync_Source_Tree = new wxTreeCtrl( itemPanel6, ID_SYNC_SOURCE_TREE, wxDefaultPosition, wxDefaultSize, wxTR_HAS_BUTTONS |wxTR_LINES_AT_ROOT|wxTR_SINGLE );
 itemBoxSizer17->Add(m_Sync_Source_Tree, 1, wxGROW|wxALL, 5);

 wxBoxSizer* itemBoxSizer22 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer16->Add(itemBoxSizer22, 1, wxGROW|wxALL, 5);
 wxBoxSizer* itemBoxSizer23 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer22->Add(itemBoxSizer23, 0, wxGROW|wxALL, 5);
 m_Sync_Dest_Txt = new wxTextCtrl( itemPanel6, ID_SYNC_DEST_TXT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
 itemBoxSizer23->Add(m_Sync_Dest_Txt, 1, wxALIGN_TOP|wxALL, 5);

 wxButton* itemButton25 = new wxButton( itemPanel6, ID_SYNC_DEST_BTN, _("..."), wxDefaultPosition, wxSize(25, 25), 0 );
 itemBoxSizer23->Add(itemButton25, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 m_Sync_Dest_Tree = new wxTreeCtrl( itemPanel6, ID_SYNC_DEST_TREE, wxDefaultPosition, wxDefaultSize, wxTR_SINGLE );
 itemBoxSizer22->Add(m_Sync_Dest_Tree, 1, wxGROW|wxALL, 5);

 wxBoxSizer* itemBoxSizer27 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer8->Add(itemBoxSizer27, 0, wxGROW|wxALL, 5);
 wxArrayString m_Sync_FunctionStrings;
 m_Sync_FunctionStrings.Add(_("Copy"));
 m_Sync_FunctionStrings.Add(_("Update"));
 m_Sync_FunctionStrings.Add(_("Mirror (Copy)"));
 m_Sync_FunctionStrings.Add(_("Mirror (Update)"));
 m_Sync_FunctionStrings.Add(_("Equalise"));
 m_Sync_Function = new wxRadioBox( itemPanel6, ID_SYNC_FUNCTION, _("Function"), wxDefaultPosition, wxDefaultSize, m_Sync_FunctionStrings, 1, wxRA_SPECIFY_COLS );
 m_Sync_Function->SetSelection(0);
 itemBoxSizer27->Add(m_Sync_Function, 0, wxALIGN_TOP|wxALL, 5);

 wxStaticBox* itemStaticBoxSizer29Static = new wxStaticBox(itemPanel6, wxID_ANY, _("Other"));
 wxStaticBoxSizer* itemStaticBoxSizer29 = new wxStaticBoxSizer(itemStaticBoxSizer29Static, wxVERTICAL);
 itemBoxSizer27->Add(itemStaticBoxSizer29, 0, wxALIGN_TOP|wxALL, 5);
 m_Sync_Timestamp = new wxCheckBox( itemPanel6, ID_SYNC_TIMESTAMP, _("Retain Timestamps"), wxDefaultPosition, wxDefaultSize, 0 );
 m_Sync_Timestamp->SetValue(false);
 itemStaticBoxSizer29->Add(m_Sync_Timestamp, 0, wxALIGN_LEFT|wxALL, 5);

 m_Sync_Attributes = new wxCheckBox( itemPanel6, ID_SYNC_ATTRIB, _("Retain Attributes"), wxDefaultPosition, wxDefaultSize, 0 );
 m_Sync_Attributes->SetValue(false);
 itemStaticBoxSizer29->Add(m_Sync_Attributes, 0, wxALIGN_LEFT|wxALL, 5);

 m_Sync_Ignore_Readonly = new wxCheckBox( itemPanel6, ID_SYNC_IGNORERO, _("Ignore Read-Only"), wxDefaultPosition, wxDefaultSize, 0 );
 m_Sync_Ignore_Readonly->SetValue(false);
 itemStaticBoxSizer29->Add(m_Sync_Ignore_Readonly, 0, wxALIGN_LEFT|wxALL, 5);

 m_Sync_Ignore_DaylightS = new wxCheckBox( itemPanel6, ID_SYNC_IGNOREDS, _("Ignore Daylight Savings"), wxDefaultPosition, wxDefaultSize, 0 );
 m_Sync_Ignore_DaylightS->SetValue(false);
 itemStaticBoxSizer29->Add(m_Sync_Ignore_DaylightS, 0, wxALIGN_LEFT|wxALL, 5);

 m_Notebook->AddPage(itemPanel6, _("Sync"), false);

 wxPanel* itemPanel34 = new wxPanel( m_Notebook, ID_PANEL_BACKUP, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
 wxBoxSizer* itemBoxSizer35 = new wxBoxSizer(wxVERTICAL);
 itemPanel34->SetSizer(itemBoxSizer35);

 wxBoxSizer* itemBoxSizer36 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer35->Add(itemBoxSizer36, 0, wxGROW|wxALL, 5);
 wxStaticBox* itemStaticBoxSizer37Static = new wxStaticBox(itemPanel34, wxID_ANY, _("Job Name"));
 wxStaticBoxSizer* itemStaticBoxSizer37 = new wxStaticBoxSizer(itemStaticBoxSizer37Static, wxHORIZONTAL);
 itemBoxSizer36->Add(itemStaticBoxSizer37, 0, wxALIGN_TOP|wxALL, 5);
 wxArrayString m_Backup_Job_SelectStrings;
 m_Backup_Job_Select = new wxComboBox( itemPanel34, ID_BACKUP_JOB_SELECT, _T(""), wxDefaultPosition, wxDefaultSize, m_Backup_Job_SelectStrings, wxCB_DROPDOWN );
 itemStaticBoxSizer37->Add(m_Backup_Job_Select, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton39 = new wxBitmapButton( itemPanel34, ID_BACKUP_JOB_OPEN, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemStaticBoxSizer37->Add(itemBitmapButton39, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton40 = new wxBitmapButton( itemPanel34, ID_BACKUP_JOB_SAVE, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemStaticBoxSizer37->Add(itemBitmapButton40, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxStaticBox* itemStaticBoxSizer41Static = new wxStaticBox(itemPanel34, wxID_ANY, _("Rules"));
 wxStaticBoxSizer* itemStaticBoxSizer41 = new wxStaticBoxSizer(itemStaticBoxSizer41Static, wxHORIZONTAL);
 itemBoxSizer36->Add(itemStaticBoxSizer41, 0, wxALIGN_TOP|wxALL, 5);
 wxArrayString m_Backup_RulesStrings;
 m_Backup_Rules = new wxComboBox( itemPanel34, ID_BACKUP_RULES, _T(""), wxDefaultPosition, wxDefaultSize, m_Backup_RulesStrings, wxCB_DROPDOWN );
 itemStaticBoxSizer41->Add(m_Backup_Rules, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBoxSizer* itemBoxSizer43 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer35->Add(itemBoxSizer43, 1, wxGROW|wxALL, 5);
 m_Backup_DirCtrl = new wxGenericDirCtrl( itemPanel34, ID_BACKUP_DIRCTRL, _T(""), wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER, _T("All files (*.*)|*.*"), 0 );
 itemBoxSizer43->Add(m_Backup_DirCtrl, 1, wxGROW|wxALL, 5);

 wxBoxSizer* itemBoxSizer45 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer43->Add(itemBoxSizer45, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
 wxBitmapButton* itemBitmapButton46 = new wxBitmapButton( itemPanel34, ID_BACKUP_ADD, itemFrame1->GetBitmapResource(wxT("list-add.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer45->Add(itemBitmapButton46, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton47 = new wxBitmapButton( itemPanel34, ID_BACKUP_REMOVE, itemFrame1->GetBitmapResource(wxT("list-remove.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer45->Add(itemBitmapButton47, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 m_Backup_TreeCtrl = new wxTreeCtrl( itemPanel34, ID_BACKUP_TREECTRL, wxDefaultPosition, wxSize(100, 100), wxTR_HAS_BUTTONS |wxTR_LINES_AT_ROOT|wxTR_HIDE_ROOT|wxTR_SINGLE );
 itemBoxSizer43->Add(m_Backup_TreeCtrl, 1, wxGROW|wxALL, 5);

 wxBoxSizer* itemBoxSizer49 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer43->Add(itemBoxSizer49, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
 wxBitmapButton* itemBitmapButton50 = new wxBitmapButton( itemPanel34, ID_BACKUP_ADDVAR, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer49->Add(itemBitmapButton50, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 wxBoxSizer* itemBoxSizer51 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer35->Add(itemBoxSizer51, 0, wxALIGN_LEFT|wxALL, 5);
 wxArrayString m_Backup_FunctionStrings;
 m_Backup_FunctionStrings.Add(_("Complete"));
 m_Backup_FunctionStrings.Add(_("Update"));
 m_Backup_FunctionStrings.Add(_("Incremental"));
 m_Backup_FunctionStrings.Add(_("Resotre"));
 m_Backup_Function = new wxRadioBox( itemPanel34, ID_BACKUP_FUNCTION, _("Type"), wxDefaultPosition, wxDefaultSize, m_Backup_FunctionStrings, 1, wxRA_SPECIFY_COLS );
 m_Backup_Function->SetSelection(0);
 itemBoxSizer51->Add(m_Backup_Function, 0, wxALIGN_TOP|wxALL, 5);

 wxArrayString m_Backup_FormatStrings;
 m_Backup_FormatStrings.Add(_("Zip"));
 m_Backup_FormatStrings.Add(_("7-Zip"));
 m_Backup_Format = new wxRadioBox( itemPanel34, ID_BACKUP_FORMAT, _("Format"), wxDefaultPosition, wxDefaultSize, m_Backup_FormatStrings, 1, wxRA_SPECIFY_COLS );
 m_Backup_Format->SetSelection(0);
 itemBoxSizer51->Add(m_Backup_Format, 0, wxALIGN_TOP|wxALL, 5);

 wxArrayString m_Backup_RatioStrings;
 m_Backup_RatioStrings.Add(_("Low"));
 m_Backup_RatioStrings.Add(_("Normal"));
 m_Backup_RatioStrings.Add(_("Maximum"));
 m_Backup_Ratio = new wxRadioBox( itemPanel34, ID_BACKUP_RATIO, _("Compression Ratio"), wxDefaultPosition, wxDefaultSize, m_Backup_RatioStrings, 1, wxRA_SPECIFY_COLS );
 m_Backup_Ratio->SetSelection(1);
 itemBoxSizer51->Add(m_Backup_Ratio, 0, wxALIGN_TOP|wxALL, 5);

 wxStaticBox* itemStaticBoxSizer55Static = new wxStaticBox(itemPanel34, wxID_ANY, _("Password (If Required)"));
 wxStaticBoxSizer* itemStaticBoxSizer55 = new wxStaticBoxSizer(itemStaticBoxSizer55Static, wxVERTICAL);
 itemBoxSizer51->Add(itemStaticBoxSizer55, 0, wxALIGN_TOP|wxALL, 5);
 m_Backup_Pass = new wxTextCtrl( itemPanel34, ID_BACKUP_PASS, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
 itemStaticBoxSizer55->Add(m_Backup_Pass, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 m_Backup_Repass = new wxTextCtrl( itemPanel34, ID_BACKUP_REPASS, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
 itemStaticBoxSizer55->Add(m_Backup_Repass, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 m_Notebook->AddPage(itemPanel34, _("Backup"), false);

 wxPanel* itemPanel58 = new wxPanel( m_Notebook, ID_PANEL_SECURE, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
 wxBoxSizer* itemBoxSizer59 = new wxBoxSizer(wxVERTICAL);
 itemPanel58->SetSizer(itemBoxSizer59);

 wxBoxSizer* itemBoxSizer60 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer59->Add(itemBoxSizer60, 0, wxALIGN_LEFT|wxALL, 5);
 wxStaticBox* itemStaticBoxSizer61Static = new wxStaticBox(itemPanel58, wxID_ANY, _("Job Name"));
 wxStaticBoxSizer* itemStaticBoxSizer61 = new wxStaticBoxSizer(itemStaticBoxSizer61Static, wxHORIZONTAL);
 itemBoxSizer60->Add(itemStaticBoxSizer61, 0, wxALIGN_TOP|wxALL, 5);
 wxArrayString m_Secure_Job_SelectStrings;
 m_Secure_Job_Select = new wxComboBox( itemPanel58, ID_SECURE_JOB_SELECT, _T(""), wxDefaultPosition, wxDefaultSize, m_Secure_Job_SelectStrings, wxCB_DROPDOWN );
 itemStaticBoxSizer61->Add(m_Secure_Job_Select, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton63 = new wxBitmapButton( itemPanel58, ID_SECURE_JOB_SAVE, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemStaticBoxSizer61->Add(itemBitmapButton63, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton64 = new wxBitmapButton( itemPanel58, ID_SECURE_JOB_ADD, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemStaticBoxSizer61->Add(itemBitmapButton64, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton65 = new wxBitmapButton( itemPanel58, ID_SECURE_JOB_REMOVE, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemStaticBoxSizer61->Add(itemBitmapButton65, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxStaticBox* itemStaticBoxSizer66Static = new wxStaticBox(itemPanel58, wxID_ANY, _("Rules"));
 wxStaticBoxSizer* itemStaticBoxSizer66 = new wxStaticBoxSizer(itemStaticBoxSizer66Static, wxHORIZONTAL);
 itemBoxSizer60->Add(itemStaticBoxSizer66, 0, wxALIGN_TOP|wxALL, 5);
 wxArrayString m_Secure_RulesStrings;
 m_Secure_Rules = new wxComboBox( itemPanel58, ID_SECURE_RULES, _T(""), wxDefaultPosition, wxDefaultSize, m_Secure_RulesStrings, wxCB_DROPDOWN );
 itemStaticBoxSizer66->Add(m_Secure_Rules, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBoxSizer* itemBoxSizer68 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer59->Add(itemBoxSizer68, 1, wxGROW|wxALL, 5);
 m_Secure_DirCtrl = new wxGenericDirCtrl( itemPanel58, ID_SECURE_DIRCTRL, _T(""), wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER, _T("All files (*.*)|*.*"), 0 );
 itemBoxSizer68->Add(m_Secure_DirCtrl, 1, wxGROW|wxALL, 5);

 wxBoxSizer* itemBoxSizer70 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer68->Add(itemBoxSizer70, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
 wxBitmapButton* itemBitmapButton71 = new wxBitmapButton( itemPanel58, ID_SECURE_ADD, itemFrame1->GetBitmapResource(wxT("list-add.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer70->Add(itemBitmapButton71, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton72 = new wxBitmapButton( itemPanel58, ID_SECURE_REMOVE, itemFrame1->GetBitmapResource(wxT("list-remove.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer70->Add(itemBitmapButton72, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 m_Secure_TreeCtrl = new wxTreeCtrl( itemPanel58, ID_SECURE_TREECTRL, wxDefaultPosition, wxSize(100, 100), wxTR_HAS_BUTTONS |wxTR_LINES_AT_ROOT|wxTR_HIDE_ROOT|wxTR_SINGLE );
 itemBoxSizer68->Add(m_Secure_TreeCtrl, 1, wxGROW|wxALL, 5);

 wxBoxSizer* itemBoxSizer74 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer68->Add(itemBoxSizer74, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
 wxBitmapButton* itemBitmapButton75 = new wxBitmapButton( itemPanel58, ID_SECURE_ADDVAR, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer74->Add(itemBitmapButton75, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton76 = new wxBitmapButton( itemPanel58, ID_SECURE_MAKERELATIVE, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer74->Add(itemBitmapButton76, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 wxBoxSizer* itemBoxSizer77 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer59->Add(itemBoxSizer77, 0, wxALIGN_LEFT|wxALL, 5);
 wxArrayString m_Secure_FunctionStrings;
 m_Secure_FunctionStrings.Add(_("Encrypt"));
 m_Secure_FunctionStrings.Add(_("Decrypt"));
 m_Secure_Function = new wxRadioBox( itemPanel58, ID_SECURE_FUNCTION, _("Function"), wxDefaultPosition, wxDefaultSize, m_Secure_FunctionStrings, 1, wxRA_SPECIFY_COLS );
 m_Secure_Function->SetSelection(0);
 itemBoxSizer77->Add(m_Secure_Function, 0, wxALIGN_TOP|wxALL, 5);

 wxArrayString m_Secure_FormatStrings;
 m_Secure_FormatStrings.Add(_("Rijndael"));
 m_Secure_FormatStrings.Add(_("Blowfish (Decrypt Only)"));
 m_Secure_Format = new wxRadioBox( itemPanel58, ID_SECURE_FORMAT, _("Format"), wxDefaultPosition, wxDefaultSize, m_Secure_FormatStrings, 1, wxRA_SPECIFY_COLS );
 m_Secure_Format->SetSelection(0);
 itemBoxSizer77->Add(m_Secure_Format, 0, wxALIGN_TOP|wxALL, 5);

 wxStaticBox* itemStaticBoxSizer80Static = new wxStaticBox(itemPanel58, wxID_ANY, _("Password (Repeated)"));
 wxStaticBoxSizer* itemStaticBoxSizer80 = new wxStaticBoxSizer(itemStaticBoxSizer80Static, wxVERTICAL);
 itemBoxSizer77->Add(itemStaticBoxSizer80, 0, wxALIGN_TOP|wxALL, 5);
 m_Secure_Pass = new wxTextCtrl( itemPanel58, ID_SECURE_PASS, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD );
 itemStaticBoxSizer80->Add(m_Secure_Pass, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 m_Secure_Repass = new wxTextCtrl( itemPanel58, ID_SECURE_REPASS, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD );
 itemStaticBoxSizer80->Add(m_Secure_Repass, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 m_Notebook->AddPage(itemPanel58, _("Secure"), false);

 wxPanel* itemPanel83 = new wxPanel( m_Notebook, ID_RULES, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
 wxBoxSizer* itemBoxSizer84 = new wxBoxSizer(wxVERTICAL);
 itemPanel83->SetSizer(itemBoxSizer84);

 wxBoxSizer* itemBoxSizer85 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer84->Add(itemBoxSizer85, 0, wxALIGN_LEFT|wxALL, 5);
 wxStaticText* itemStaticText86 = new wxStaticText( itemPanel83, wxID_STATIC, _("Rule set name"), wxDefaultPosition, wxDefaultSize, 0 );
 itemBoxSizer85->Add(itemStaticText86, 0, wxALIGN_LEFT|wxALL, 5);

 wxBoxSizer* itemBoxSizer87 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer85->Add(itemBoxSizer87, 0, wxALIGN_LEFT|wxALL, 5);
 wxArrayString m_Rules_ComboStrings;
 m_Rules_Combo = new wxComboBox( itemPanel83, ID_RULES_COMBO, _T(""), wxDefaultPosition, wxDefaultSize, m_Rules_ComboStrings, wxCB_READONLY );
 itemBoxSizer87->Add(m_Rules_Combo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton89 = new wxBitmapButton( itemPanel83, ID_RULES_SAVE, itemFrame1->GetBitmapResource(wxT("save.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer87->Add(itemBitmapButton89, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton90 = new wxBitmapButton( itemPanel83, ID_RULES_ADD, itemFrame1->GetBitmapResource(wxT("list-add.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer87->Add(itemBitmapButton90, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton91 = new wxBitmapButton( itemPanel83, ID_RULES_REMOVE, itemFrame1->GetBitmapResource(wxT("list-remove.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer87->Add(itemBitmapButton91, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBoxSizer* itemBoxSizer92 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer84->Add(itemBoxSizer92, 1, wxGROW|wxALL, 5);
 wxStaticText* itemStaticText93 = new wxStaticText( itemPanel83, wxID_STATIC, _("Files to exclude (file name, path or extension)"), wxDefaultPosition, wxDefaultSize, 0 );
 itemBoxSizer92->Add(itemStaticText93, 0, wxALIGN_LEFT|wxALL, 5);

 wxBoxSizer* itemBoxSizer94 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer92->Add(itemBoxSizer94, 1, wxGROW|wxALL, 5);
 wxArrayString m_Rules_FileExcludeStrings;
 m_Rules_FileExclude = new wxListBox( itemPanel83, ID_RULES_FILEEXCLUDE, wxDefaultPosition, wxDefaultSize, m_Rules_FileExcludeStrings, wxLB_SINGLE );
 itemBoxSizer94->Add(m_Rules_FileExclude, 1, wxGROW|wxALL, 5);

 wxBoxSizer* itemBoxSizer96 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer94->Add(itemBoxSizer96, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
 wxBitmapButton* itemBitmapButton97 = new wxBitmapButton( itemPanel83, ID_RULES_ADD_FILEEXCLUDE, itemFrame1->GetBitmapResource(wxT("list-add.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer96->Add(itemBitmapButton97, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton98 = new wxBitmapButton( itemPanel83, ID_RULES_REMOVE_FILEEXCLUDE, itemFrame1->GetBitmapResource(wxT("list-remove.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer96->Add(itemBitmapButton98, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 wxBoxSizer* itemBoxSizer99 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer84->Add(itemBoxSizer99, 1, wxGROW|wxALL, 5);
 wxStaticText* itemStaticText100 = new wxStaticText( itemPanel83, wxID_STATIC, _("Folders to exclude (folder name or path)"), wxDefaultPosition, wxDefaultSize, 0 );
 itemBoxSizer99->Add(itemStaticText100, 0, wxALIGN_LEFT|wxALL, 5);

 wxBoxSizer* itemBoxSizer101 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer99->Add(itemBoxSizer101, 1, wxGROW|wxALL, 5);
 wxArrayString m_Rules_FolderExcludeStrings;
 m_Rules_FolderExclude = new wxListBox( itemPanel83, ID_RULES_FOLDEREXCLUDE, wxDefaultPosition, wxDefaultSize, m_Rules_FolderExcludeStrings, wxLB_SINGLE );
 itemBoxSizer101->Add(m_Rules_FolderExclude, 1, wxGROW|wxALL, 5);

 wxBoxSizer* itemBoxSizer103 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer101->Add(itemBoxSizer103, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
 wxBitmapButton* itemBitmapButton104 = new wxBitmapButton( itemPanel83, ID_RULES_ADD_FOLDEREXCLUDE, itemFrame1->GetBitmapResource(wxT("list-add.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer103->Add(itemBitmapButton104, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton105 = new wxBitmapButton( itemPanel83, ID_RULES_REMOVE_FOLDEREXCLUDE, itemFrame1->GetBitmapResource(wxT("list-remove.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer103->Add(itemBitmapButton105, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 wxBoxSizer* itemBoxSizer106 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer84->Add(itemBoxSizer106, 1, wxGROW|wxALL, 5);
 wxStaticText* itemStaticText107 = new wxStaticText( itemPanel83, wxID_STATIC, _("Files to always include (file name, path or extension)"), wxDefaultPosition, wxDefaultSize, 0 );
 itemBoxSizer106->Add(itemStaticText107, 0, wxALIGN_LEFT|wxALL, 5);

 wxBoxSizer* itemBoxSizer108 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer106->Add(itemBoxSizer108, 1, wxGROW|wxALL, 5);
 wxArrayString m_Rules_FileIncludeStrings;
 m_Rules_FileInclude = new wxListBox( itemPanel83, ID_RULES_FILE_INCLUDE, wxDefaultPosition, wxDefaultSize, m_Rules_FileIncludeStrings, wxLB_SINGLE );
 itemBoxSizer108->Add(m_Rules_FileInclude, 1, wxGROW|wxALL, 5);

 wxBoxSizer* itemBoxSizer110 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer108->Add(itemBoxSizer110, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
 wxBitmapButton* itemBitmapButton111 = new wxBitmapButton( itemPanel83, ID_RULES_ADD_FILEINCLUDE, itemFrame1->GetBitmapResource(wxT("list-add.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer110->Add(itemBitmapButton111, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton112 = new wxBitmapButton( itemPanel83, ID_RULES_REMOVE_FILEINCLUDE, itemFrame1->GetBitmapResource(wxT("list-remove.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer110->Add(itemBitmapButton112, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 wxBoxSizer* itemBoxSizer113 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer84->Add(itemBoxSizer113, 1, wxGROW|wxALL, 5);
 wxStaticText* itemStaticText114 = new wxStaticText( itemPanel83, wxID_STATIC, _("Files to delete"), wxDefaultPosition, wxDefaultSize, 0 );
 itemBoxSizer113->Add(itemStaticText114, 0, wxALIGN_LEFT|wxALL, 5);

 wxBoxSizer* itemBoxSizer115 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer113->Add(itemBoxSizer115, 1, wxGROW|wxALL, 5);
 wxArrayString m_Rules_FileDeleteStrings;
 m_Rules_FileDelete = new wxListBox( itemPanel83, ID_RULES_FILE_DELETE, wxDefaultPosition, wxDefaultSize, m_Rules_FileDeleteStrings, wxLB_SINGLE );
 itemBoxSizer115->Add(m_Rules_FileDelete, 1, wxGROW|wxALL, 5);

 wxBoxSizer* itemBoxSizer117 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer115->Add(itemBoxSizer117, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
 wxBitmapButton* itemBitmapButton118 = new wxBitmapButton( itemPanel83, ID_RULES_ADD_FILEDELETE, itemFrame1->GetBitmapResource(wxT("list-add.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer117->Add(itemBitmapButton118, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton119 = new wxBitmapButton( itemPanel83, ID_RULES_REMOVE_FILEDELETE, itemFrame1->GetBitmapResource(wxT("list-remove.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer117->Add(itemBitmapButton119, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 m_Notebook->AddPage(itemPanel83, _("Rules"), false);

 wxPanel* itemPanel120 = new wxPanel( m_Notebook, ID_PANEL4, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );

 m_Notebook->AddPage(itemPanel120, _("Portable Variables"), false);

 wxPanel* itemPanel121 = new wxPanel( m_Notebook, ID_PANEL6, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
 wxBoxSizer* itemBoxSizer122 = new wxBoxSizer(wxVERTICAL);
 itemPanel121->SetSizer(itemBoxSizer122);

 wxBoxSizer* itemBoxSizer123 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer122->Add(itemBoxSizer123, 0, wxALIGN_LEFT|wxALL, 5);
 wxArrayString itemComboBox124Strings;
 wxComboBox* itemComboBox124 = new wxComboBox( itemPanel121, ID_SCRIPT_COMBO, _T(""), wxDefaultPosition, wxDefaultSize, itemComboBox124Strings, wxCB_DROPDOWN );
 itemBoxSizer123->Add(itemComboBox124, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton125 = new wxBitmapButton( itemPanel121, ID_SCRIPT_SAVE, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer123->Add(itemBitmapButton125, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton126 = new wxBitmapButton( itemPanel121, ID_SCRIPT_ADD, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer123->Add(itemBitmapButton126, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton127 = new wxBitmapButton( itemPanel121, ID_SCRIPT_REMOVE, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer123->Add(itemBitmapButton127, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBoxSizer* itemBoxSizer128 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer122->Add(itemBoxSizer128, 1, wxGROW|wxALL, 5);
 wxRichTextCtrl* itemRichTextCtrl129 = new wxRichTextCtrl( itemPanel121, ID_SCRIPT_RICH, _T(""), wxDefaultPosition, wxSize(100, 100), wxWANTS_CHARS );
 itemBoxSizer128->Add(itemRichTextCtrl129, 1, wxGROW|wxALL, 5);

 wxBoxSizer* itemBoxSizer130 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer122->Add(itemBoxSizer130, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
 wxButton* itemButton131 = new wxButton( itemPanel121, ID_SCRIPT_CHECK, _("Check Script"), wxDefaultPosition, wxDefaultSize, 0 );
 itemBoxSizer130->Add(itemButton131, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxButton* itemButton132 = new wxButton( itemPanel121, ID_SCRIPT_EXECUTE, _("Execute"), wxDefaultPosition, wxDefaultSize, 0 );
 itemBoxSizer130->Add(itemButton132, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 m_Notebook->AddPage(itemPanel121, _("Scripting"), false);

 wxPanel* itemPanel133 = new wxPanel( m_Notebook, ID_PANEL5, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );

 m_Notebook->AddPage(itemPanel133, _("Settings"), false);

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
 if (name == _T("list-add.png"))
 {
  wxBitmap bitmap(_T("list-add.png"), wxBITMAP_TYPE_PNG);
  return bitmap;
 }
 else if (name == _T("list-remove.png"))
 {
  wxBitmap bitmap(_T("list-remove.png"), wxBITMAP_TYPE_PNG);
  return bitmap;
 }
 else if (name == _T("save.png"))
 {
  wxBitmap bitmap(_T("save.png"), wxBITMAP_TYPE_PNG);
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
	AddTreeToTree(m_Backup_DirCtrl->GetTreeCtrl(), m_Backup_TreeCtrl, this);
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
	AddTreeToTree(m_Secure_DirCtrl->GetTreeCtrl(), m_Secure_TreeCtrl, this);
	wxGetApp().AppendSecureLocation(m_Secure_DirCtrl->GetPath());
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
			AddDirToTree(dialog.GetPath(), m_Sync_Source_Tree, this);
			m_Sync_Source_Txt->SetValue(dialog.GetPath());
		}
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_OK
 */

void frmMain::OnToolOkClick( wxCommandEvent& event )
{
	//Secure
	if(m_Notebook->GetSelection() == 2){
		frmProgress *window = new frmProgress(NULL, ID_FRMPROGRESS, _("Progress"));
		window->Show();
		SecureData data;
		data.TransferFromForm(this);
		Rules rules;
		if(m_Secure_Rules->GetStringSelection() != wxEmptyString){
			rules.TransferFromFile(m_Secure_Rules->GetStringSelection());
		}
		SecureThread *thread = new SecureThread(data, rules, window);
		thread->Create();
		thread->Run();
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
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RULES_ADD_FILEINCLUDE
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
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RULES_REMOVE_FILEINCLUDE
 */

void frmMain::OnRulesRemoveFiledeleteClick( wxCommandEvent& event )
{
	m_Rules_FileInclude->Delete(m_Rules_FileDelete->GetSelection());
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RULES_ADD_FILEINCLUDE
 */

void frmMain::OnRulesAddFileincludeClick( wxCommandEvent& event )
{
	wxTextEntryDialog *dialog = new wxTextEntryDialog(this, _("File to include"), _("This dialog needs to be replaced"));
	if(dialog->ShowModal() == wxID_OK){
		m_Rules_FileInclude->Append(dialog->GetValue());
	}
	delete dialog; 
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RULES_REMOVE_FILEINCLUDE
 */

void frmMain::OnRulesRemoveFileincludeClick( wxCommandEvent& event )
{
	m_Rules_FileDelete->Delete(m_Rules_FileInclude->GetSelection());
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RULES_SAVE
 */

void frmMain::OnRulesSaveClick( wxCommandEvent& event )
{
	//Get all of the form data into arraystrings
	wxArrayString arrFileInclude, arrFolderExclude, arrFileExclude, arrFileDelete;
	for(unsigned int i = 0; i < m_Rules_FileInclude->GetCount(); i++){
		arrFileInclude.Add(m_Rules_FileInclude->GetString(i));
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
	rules.SetFilesToInclude(arrFileInclude);
	rules.SetFilesToExclude(arrFileExclude);
	rules.SetFoldersToExclude(arrFolderExclude);
	rules.SetDeleteFiles(arrFileDelete);
	if(m_Rules_Combo->GetStringSelection() != wxEmptyString){
		rules.TransferToFile(m_Rules_Combo->GetStringSelection());
	}
	else{
		ErrorBox(_("You must select a name for this set of Rules"));
	}
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
	for(unsigned int i = 0; i < rules.GetFilesToIncude().GetCount(); i++){
		m_Rules_FileInclude->Append(rules.GetFilesToIncude().Item(i));
	}
	for(unsigned int j = 0; j < rules.GetFilesToExclude().GetCount(); j++){
		m_Rules_FileExclude->Append(rules.GetFilesToExclude().Item(j));		
	}
	for(unsigned int k = 0; k < rules.GetFoldersToExclude().GetCount(); k++){
		m_Rules_FolderExclude->Append(rules.GetFoldersToExclude().Item(k));
	}
	for(unsigned int l = 0; l < rules.GetDeleteFiles().GetCount(); l++){
		m_Rules_FileDelete->Append(rules.GetDeleteFiles().Item(l));
	}
	}
	
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_PREVIEW
 */

void frmMain::OnToolPreviewClick( wxCommandEvent& event )
{
	if(m_Notebook->GetSelection() == 2){
		Rules rules;
		if(m_Secure_Rules->GetStringSelection() != wxEmptyString){
			rules.TransferFromFile(m_Secure_Rules->GetStringSelection());
		}
		PreviewSecure(rules, m_Secure_TreeCtrl);

	}
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SECURE_JOB_SAVE
 */

void frmMain::OnSecureJobSaveClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SECURE_JOB_SAVE in frmMain.
 // Before editing this code, remove the block markers.
 event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SECURE_JOB_SAVE in frmMain. 
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SECURE_JOB_ADD
 */

void frmMain::OnSecureJobAddClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SECURE_JOB_ADD in frmMain.
 // Before editing this code, remove the block markers.
 event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SECURE_JOB_ADD in frmMain. 
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SECURE_JOB_REMOVE
 */

void frmMain::OnSecureJobRemoveClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SECURE_JOB_REMOVE in frmMain.
 // Before editing this code, remove the block markers.
 event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SECURE_JOB_REMOVE in frmMain. 
}

