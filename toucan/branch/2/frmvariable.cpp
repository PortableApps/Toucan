/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "frmvariable.h"
#include "basicfunctions.h"
#include "variables.h"

/*!
 * frmVariable type definition
 */

IMPLEMENT_DYNAMIC_CLASS( frmVariable, wxDialog )


/*!
 * frmVariable event table definition
 */

BEGIN_EVENT_TABLE(frmVariable, wxDialog)

    EVT_BUTTON(ID_PATH, frmVariable::OnPathClick)

    EVT_BUTTON(ID_VARS_ADD, frmVariable::OnVarsAddClick)

    EVT_BUTTON(wxID_OK, frmVariable::OnOkClick)

    EVT_BUTTON(wxID_CANCEL, frmVariable::OnCancelClick)
	
	EVT_TEXT(ID_TEXT, frmVariable::OnTextChange)
	
END_EVENT_TABLE()


/*!
 * frmVariable constructors
 */

frmVariable::frmVariable()
{
    Init();
}

frmVariable::frmVariable( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * frmVariable creator
 */

bool frmVariable::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
    return true;
}


/*!
 * frmVariable destructor
 */

frmVariable::~frmVariable()
{
}


/*!
 * Member initialisation
 */

void frmVariable::Init()
{
    m_Text = NULL;
    m_Variables = NULL;
    m_Preview = NULL;
}


/*!
 * Control creation for frmVariable
 */

void frmVariable::CreateControls()
{    
    frmVariable* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxGROW|wxALL, 5);

    m_Text = new wxTextCtrl( itemDialog1, ID_TEXT, _T(""), wxDefaultPosition, wxSize(-1, -1), 0 );
	m_Text->SetMinSize(wxSize(300, -1));
    itemBoxSizer3->Add(m_Text, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton5 = new wxButton( itemDialog1, ID_PATH, wxT("..."), wxDefaultPosition, wxSize(25, -1), 0 );
    itemBoxSizer3->Add(itemButton5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer6, 0, wxALIGN_LEFT|wxALL, 5);

    wxArrayString m_VariablesStrings;
    m_Variables = new wxComboBox( itemDialog1, ID_VARS, wxEmptyString, wxDefaultPosition, wxDefaultSize, m_VariablesStrings, 0 );
    itemBoxSizer6->Add(m_Variables, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBitmapButton* itemBitmapButton8 = new wxBitmapButton( itemDialog1, ID_VARS_ADD, this->GetBitmapResource(wxT("add.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
    itemBoxSizer6->Add(itemBitmapButton8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer9, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText10 = new wxStaticText( itemDialog1, wxID_STATIC, _("Preview"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer9->Add(itemStaticText10, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_Preview = new wxTextCtrl( itemDialog1, ID_PREVIEW, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
    itemBoxSizer9->Add(m_Preview, 1, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer12 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer12, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxButton* itemButton13 = new wxButton( itemDialog1, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer12->Add(itemButton13, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton14 = new wxButton( itemDialog1, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer12->Add(itemButton14, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
	this->SetIcon(wxIcon(wxT("Toucan.ico"), wxBITMAP_TYPE_ICO));
	
	SetVariablesBox(m_Variables);
	m_Variables->Append(wxT("date"));
	m_Variables->Append(wxT("time"));
	m_Variables->Append(wxT("docs"));
	m_Variables->Append(wxT("drive"));
}


/*!
 * Should we show tooltips?
 */

bool frmVariable::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap frmVariable::GetBitmapResource(const wxString& name)
{
	wxUnusedVar(name);
	if (name == _T("add.png")) {
		wxBitmap bitmap(_T("add.png"), wxBITMAP_TYPE_PNG);
		return bitmap;
	} 
	return wxNullBitmap;
}

/*!
 * Get icon resources
 */

wxIcon frmVariable::GetIconResource(const wxString& name)
{
    wxUnusedVar(name);
    return wxNullIcon;
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_PATH
 */

void frmVariable::OnPathClick(wxCommandEvent& event)
{
	wxDirDialog dialog(this,_("Please select a folder."));
	if (dialog.ShowModal() == wxID_OK) {
		m_Text->SetValue(dialog.GetPath());
	}
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_VARS_ADD
 */

void frmVariable::OnVarsAddClick(wxCommandEvent& event)
{
    m_Text->WriteText(wxT("@") + m_Variables->GetStringSelection() + wxT("@"));	
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void frmVariable::OnOkClick(wxCommandEvent& event)
{
    event.Skip();
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
 */

void frmVariable::OnCancelClick(wxCommandEvent& event)
{
    event.Skip();
}

void frmVariable::OnTextChange(wxCommandEvent& event){
	//wxMessageBox(_("New text!"));
	wxString strPath = m_Text->GetValue();
	strPath = Normalise(strPath);
	strPath = Normalise(strPath);
	m_Preview->SetValue(strPath);
}
