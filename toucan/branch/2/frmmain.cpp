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
 m_Backup_List = NULL;
 m_Backup_Function = NULL;
 m_Backup_Format = NULL;
 m_Backup_Ratio = NULL;
 m_Backup_Pass = NULL;
 m_Backup_Repass = NULL;
 m_Secure_Job_Select = NULL;
 m_Secure_Rules = NULL;
 m_Secure_DirCtrl = NULL;
 m_Secure_List = NULL;
 m_Secure_Function = NULL;
 m_Secure_Format = NULL;
 m_Secure_Pass = NULL;
 m_Secure_Repass = NULL;
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
 itemToolBar2->AddTool(ID_TOOL1, _("OK"), itemtool3Bitmap, itemtool3BitmapDisabled, wxITEM_NORMAL, _T(""), wxEmptyString);
 wxBitmap itemtool4Bitmap(wxNullBitmap);
 wxBitmap itemtool4BitmapDisabled;
 itemToolBar2->AddTool(ID_TOOL2, _("Preview"), itemtool4Bitmap, itemtool4BitmapDisabled, wxITEM_NORMAL, _T(""), wxEmptyString);
 itemToolBar2->Realize();
 itemFrame1->GetAuiManager().AddPane(itemToolBar2, wxAuiPaneInfo()
  .ToolbarPane().Name(_T("Pane5")).Top().Layer(10).LeftDockable(false).RightDockable(false).BottomDockable(false).CaptionVisible(false).CloseButton(false).DestroyOnClose(false).Resizable(false).Gripper(true).PaneBorder(false));

 wxAuiNotebook* itemAuiNotebook5 = new wxAuiNotebook( itemFrame1, ID_AUINOTEBOOK, wxDefaultPosition, wxDefaultSize, wxAUI_NB_SCROLL_BUTTONS|wxNO_BORDER );

 wxPanel* itemPanel6 = new wxPanel( itemAuiNotebook5, ID_PANEL_SYNC, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
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

 m_Sync_Source_Tree = new wxTreeCtrl( itemPanel6, ID_SYNC_SOURCE_TREE, wxDefaultPosition, wxDefaultSize, wxTR_SINGLE );
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

 itemAuiNotebook5->AddPage(itemPanel6, _("Sync"), false);

 wxPanel* itemPanel34 = new wxPanel( itemAuiNotebook5, ID_PANEL_BACKUP, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
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
 wxBitmapButton* itemBitmapButton46 = new wxBitmapButton( itemPanel34, ID_BACKUP_ADD, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer45->Add(itemBitmapButton46, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton47 = new wxBitmapButton( itemPanel34, ID_BACKUP_REMOVE, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer45->Add(itemBitmapButton47, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 wxArrayString m_Backup_ListStrings;
 m_Backup_List = new wxListBox( itemPanel34, ID_BACKUP_LISTBOX, wxDefaultPosition, wxDefaultSize, m_Backup_ListStrings, wxLB_SINGLE|wxSUNKEN_BORDER );
 itemBoxSizer43->Add(m_Backup_List, 1, wxGROW|wxALL, 5);

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

 itemAuiNotebook5->AddPage(itemPanel34, _("Backup"), false);

 wxPanel* itemPanel58 = new wxPanel( itemAuiNotebook5, ID_PANEL_SECURE, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
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

 wxBitmapButton* itemBitmapButton63 = new wxBitmapButton( itemPanel58, ID_SECURE_JOB_OPEN, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemStaticBoxSizer61->Add(itemBitmapButton63, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton64 = new wxBitmapButton( itemPanel58, ID_SECURE_JOB_SAVE, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemStaticBoxSizer61->Add(itemBitmapButton64, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxStaticBox* itemStaticBoxSizer65Static = new wxStaticBox(itemPanel58, wxID_ANY, _("Rules"));
 wxStaticBoxSizer* itemStaticBoxSizer65 = new wxStaticBoxSizer(itemStaticBoxSizer65Static, wxHORIZONTAL);
 itemBoxSizer60->Add(itemStaticBoxSizer65, 0, wxALIGN_TOP|wxALL, 5);
 wxArrayString m_Secure_RulesStrings;
 m_Secure_Rules = new wxComboBox( itemPanel58, ID_SECURE_RULES, _T(""), wxDefaultPosition, wxDefaultSize, m_Secure_RulesStrings, wxCB_DROPDOWN );
 itemStaticBoxSizer65->Add(m_Secure_Rules, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

 wxBoxSizer* itemBoxSizer67 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer59->Add(itemBoxSizer67, 1, wxGROW|wxALL, 5);
 m_Secure_DirCtrl = new wxGenericDirCtrl( itemPanel58, ID_SECURE_DIRCTRL, _T(""), wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER, _T("All files (*.*)|*.*"), 0 );
 itemBoxSizer67->Add(m_Secure_DirCtrl, 1, wxGROW|wxALL, 5);

 wxBoxSizer* itemBoxSizer69 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer67->Add(itemBoxSizer69, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
 wxBitmapButton* itemBitmapButton70 = new wxBitmapButton( itemPanel58, ID_SECURE_ADD, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer69->Add(itemBitmapButton70, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton71 = new wxBitmapButton( itemPanel58, ID_SECURE_REMOVE, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer69->Add(itemBitmapButton71, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 wxArrayString m_Secure_ListStrings;
 m_Secure_List = new wxListBox( itemPanel58, ID_SECURE_LISTBOX, wxDefaultPosition, wxDefaultSize, m_Secure_ListStrings, wxLB_SINGLE|wxSUNKEN_BORDER );
 itemBoxSizer67->Add(m_Secure_List, 1, wxGROW|wxALL, 5);

 wxBoxSizer* itemBoxSizer73 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer67->Add(itemBoxSizer73, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
 wxBitmapButton* itemBitmapButton74 = new wxBitmapButton( itemPanel58, ID_SECURE_ADDVAR, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer73->Add(itemBitmapButton74, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton75 = new wxBitmapButton( itemPanel58, ID_SECURE_MAKERELATIVE, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer73->Add(itemBitmapButton75, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 wxBoxSizer* itemBoxSizer76 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer59->Add(itemBoxSizer76, 0, wxALIGN_LEFT|wxALL, 5);
 wxArrayString m_Secure_FunctionStrings;
 m_Secure_FunctionStrings.Add(_("Encrypt"));
 m_Secure_FunctionStrings.Add(_("Decrypt"));
 m_Secure_Function = new wxRadioBox( itemPanel58, ID_SECURE_FUNCTION, _("Function"), wxDefaultPosition, wxDefaultSize, m_Secure_FunctionStrings, 1, wxRA_SPECIFY_COLS );
 m_Secure_Function->SetSelection(0);
 itemBoxSizer76->Add(m_Secure_Function, 0, wxALIGN_TOP|wxALL, 5);

 wxArrayString m_Secure_FormatStrings;
 m_Secure_FormatStrings.Add(_("Rijndeal"));
 m_Secure_FormatStrings.Add(_("Blowfish (Decrypt Only)"));
 m_Secure_Format = new wxRadioBox( itemPanel58, ID_SECURE_FORMAT, _("Format"), wxDefaultPosition, wxDefaultSize, m_Secure_FormatStrings, 1, wxRA_SPECIFY_COLS );
 m_Secure_Format->SetSelection(0);
 itemBoxSizer76->Add(m_Secure_Format, 0, wxALIGN_TOP|wxALL, 5);

 wxStaticBox* itemStaticBoxSizer79Static = new wxStaticBox(itemPanel58, wxID_ANY, _("Password (Repeated)"));
 wxStaticBoxSizer* itemStaticBoxSizer79 = new wxStaticBoxSizer(itemStaticBoxSizer79Static, wxVERTICAL);
 itemBoxSizer76->Add(itemStaticBoxSizer79, 0, wxALIGN_TOP|wxALL, 5);
 m_Secure_Pass = new wxTextCtrl( itemPanel58, ID_SECURE_PASS, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD );
 itemStaticBoxSizer79->Add(m_Secure_Pass, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 m_Secure_Repass = new wxTextCtrl( itemPanel58, ID_SECURE_REPASS, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD );
 itemStaticBoxSizer79->Add(m_Secure_Repass, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 itemAuiNotebook5->AddPage(itemPanel58, _("Secure"), false);

 wxPanel* itemPanel82 = new wxPanel( itemAuiNotebook5, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
 wxBoxSizer* itemBoxSizer83 = new wxBoxSizer(wxVERTICAL);
 itemPanel82->SetSizer(itemBoxSizer83);

 wxBoxSizer* itemBoxSizer84 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer83->Add(itemBoxSizer84, 1, wxGROW|wxALL, 5);
 wxStaticText* itemStaticText85 = new wxStaticText( itemPanel82, wxID_STATIC, _("Files to exclude (file name, path or extension)"), wxDefaultPosition, wxDefaultSize, 0 );
 itemBoxSizer84->Add(itemStaticText85, 0, wxALIGN_LEFT|wxALL, 5);

 wxBoxSizer* itemBoxSizer86 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer84->Add(itemBoxSizer86, 1, wxGROW|wxALL, 5);
 wxArrayString itemListBox87Strings;
 wxListBox* itemListBox87 = new wxListBox( itemPanel82, ID_LISTBOX2, wxDefaultPosition, wxDefaultSize, itemListBox87Strings, wxLB_SINGLE );
 itemBoxSizer86->Add(itemListBox87, 1, wxGROW|wxALL, 5);

 wxBoxSizer* itemBoxSizer88 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer86->Add(itemBoxSizer88, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
 wxBitmapButton* itemBitmapButton89 = new wxBitmapButton( itemPanel82, ID_BITMAPBUTTON13, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer88->Add(itemBitmapButton89, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton90 = new wxBitmapButton( itemPanel82, ID_BITMAPBUTTON14, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer88->Add(itemBitmapButton90, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 wxBoxSizer* itemBoxSizer91 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer83->Add(itemBoxSizer91, 1, wxGROW|wxALL, 5);
 wxStaticText* itemStaticText92 = new wxStaticText( itemPanel82, wxID_STATIC, _("Folders to exclude (folder name or path)"), wxDefaultPosition, wxDefaultSize, 0 );
 itemBoxSizer91->Add(itemStaticText92, 0, wxALIGN_LEFT|wxALL, 5);

 wxBoxSizer* itemBoxSizer93 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer91->Add(itemBoxSizer93, 1, wxGROW|wxALL, 5);
 wxArrayString itemListBox94Strings;
 wxListBox* itemListBox94 = new wxListBox( itemPanel82, ID_LISTBOX3, wxDefaultPosition, wxDefaultSize, itemListBox94Strings, wxLB_SINGLE );
 itemBoxSizer93->Add(itemListBox94, 1, wxGROW|wxALL, 5);

 wxBoxSizer* itemBoxSizer95 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer93->Add(itemBoxSizer95, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
 wxBitmapButton* itemBitmapButton96 = new wxBitmapButton( itemPanel82, ID_BITMAPBUTTON15, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer95->Add(itemBitmapButton96, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton97 = new wxBitmapButton( itemPanel82, ID_BITMAPBUTTON16, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer95->Add(itemBitmapButton97, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 wxBoxSizer* itemBoxSizer98 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer83->Add(itemBoxSizer98, 1, wxGROW|wxALL, 5);
 wxStaticText* itemStaticText99 = new wxStaticText( itemPanel82, wxID_STATIC, _("Files to always include (file name, path or extension)"), wxDefaultPosition, wxDefaultSize, 0 );
 itemBoxSizer98->Add(itemStaticText99, 0, wxALIGN_LEFT|wxALL, 5);

 wxBoxSizer* itemBoxSizer100 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer98->Add(itemBoxSizer100, 1, wxGROW|wxALL, 5);
 wxArrayString itemListBox101Strings;
 wxListBox* itemListBox101 = new wxListBox( itemPanel82, ID_LISTBOX4, wxDefaultPosition, wxDefaultSize, itemListBox101Strings, wxLB_SINGLE );
 itemBoxSizer100->Add(itemListBox101, 1, wxGROW|wxALL, 5);

 wxBoxSizer* itemBoxSizer102 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer100->Add(itemBoxSizer102, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
 wxBitmapButton* itemBitmapButton103 = new wxBitmapButton( itemPanel82, ID_BITMAPBUTTON17, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer102->Add(itemBitmapButton103, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton104 = new wxBitmapButton( itemPanel82, ID_BITMAPBUTTON18, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer102->Add(itemBitmapButton104, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 wxBoxSizer* itemBoxSizer105 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer83->Add(itemBoxSizer105, 1, wxGROW|wxALL, 5);
 wxStaticText* itemStaticText106 = new wxStaticText( itemPanel82, wxID_STATIC, _("RegEx to match (matches entire file path)"), wxDefaultPosition, wxDefaultSize, 0 );
 itemBoxSizer105->Add(itemStaticText106, 0, wxALIGN_LEFT|wxALL, 5);

 wxBoxSizer* itemBoxSizer107 = new wxBoxSizer(wxHORIZONTAL);
 itemBoxSizer105->Add(itemBoxSizer107, 1, wxGROW|wxALL, 5);
 wxArrayString itemListBox108Strings;
 wxListBox* itemListBox108 = new wxListBox( itemPanel82, ID_LISTBOX5, wxDefaultPosition, wxDefaultSize, itemListBox108Strings, wxLB_SINGLE );
 itemBoxSizer107->Add(itemListBox108, 1, wxGROW|wxALL, 5);

 wxBoxSizer* itemBoxSizer109 = new wxBoxSizer(wxVERTICAL);
 itemBoxSizer107->Add(itemBoxSizer109, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
 wxBitmapButton* itemBitmapButton110 = new wxBitmapButton( itemPanel82, ID_BITMAPBUTTON19, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer109->Add(itemBitmapButton110, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 wxBitmapButton* itemBitmapButton111 = new wxBitmapButton( itemPanel82, ID_BITMAPBUTTON20, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
 itemBoxSizer109->Add(itemBitmapButton111, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

 itemAuiNotebook5->AddPage(itemPanel82, _("Rules"), false);

 wxPanel* itemPanel112 = new wxPanel( itemAuiNotebook5, ID_PANEL4, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );

 itemAuiNotebook5->AddPage(itemPanel112, _("Portable Variables"), false);

 wxPanel* itemPanel113 = new wxPanel( itemAuiNotebook5, ID_PANEL6, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );

 itemAuiNotebook5->AddPage(itemPanel113, _("Scripting"), false);

 wxPanel* itemPanel114 = new wxPanel( itemAuiNotebook5, ID_PANEL5, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );

 itemAuiNotebook5->AddPage(itemPanel114, _("Settings"), false);

 itemFrame1->GetAuiManager().AddPane(itemAuiNotebook5, wxAuiPaneInfo()
  .Name(_T("Pane3")).Centre().CaptionVisible(false).CloseButton(false).DestroyOnClose(false).Resizable(true).Floatable(false).PaneBorder(false));

 GetAuiManager().Update();

	////@end frmMain content construction
    m_Backup_List->SetDropTarget(new DnDFileList(m_Backup_List));
    m_Secure_List->SetDropTarget(new DnDFileList(m_Secure_List));
    m_Sync_Source_Txt->SetDropTarget(new DnDFileText(m_Sync_Source_Txt));
    m_Sync_Dest_Txt->SetDropTarget(new DnDFileText(m_Sync_Dest_Txt));
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
