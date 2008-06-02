/////////////////////////////////////////////////////////////////////////////
// Name:        frmvariable.cpp
// Purpose:     
// Author:      Steven Lamerton
// Modified by: 
// Created:     02/06/2008 20:08:01
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

#include "frmvariable.h"

////@begin XPM images
////@end XPM images


/*!
 * frmVariable type definition
 */

IMPLEMENT_DYNAMIC_CLASS( frmVariable, wxDialog )


/*!
 * frmVariable event table definition
 */

BEGIN_EVENT_TABLE( frmVariable, wxDialog )

////@begin frmVariable event table entries
    EVT_BUTTON( ID_PATH, frmVariable::OnPathClick )

    EVT_BUTTON( ID_VARS_ADD, frmVariable::OnVarsAddClick )

    EVT_BUTTON( wxID_OK, frmVariable::OnOkClick )

    EVT_BUTTON( wxID_CANCEL, frmVariable::OnCancelClick )

////@end frmVariable event table entries

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
////@begin frmVariable creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end frmVariable creation
    return true;
}


/*!
 * frmVariable destructor
 */

frmVariable::~frmVariable()
{
////@begin frmVariable destruction
////@end frmVariable destruction
}


/*!
 * Member initialisation
 */

void frmVariable::Init()
{
////@begin frmVariable member initialisation
    m_Text = NULL;
    m_Variables = NULL;
    m_Preview = NULL;
////@end frmVariable member initialisation
}


/*!
 * Control creation for frmVariable
 */

void frmVariable::CreateControls()
{    
////@begin frmVariable content construction
    frmVariable* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxGROW|wxALL, 5);

    m_Text = new wxTextCtrl( itemDialog1, ID_TEXT, _T(""), wxDefaultPosition, wxSize(300, -1), 0 );
    itemBoxSizer3->Add(m_Text, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton5 = new wxButton( itemDialog1, ID_PATH, _("..."), wxDefaultPosition, wxSize(25, -1), 0 );
    itemBoxSizer3->Add(itemButton5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer6, 0, wxALIGN_LEFT|wxALL, 5);

    wxArrayString m_VariablesStrings;
    m_Variables = new wxChoice( itemDialog1, ID_VARS, wxDefaultPosition, wxDefaultSize, m_VariablesStrings, 0 );
    itemBoxSizer6->Add(m_Variables, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBitmapButton* itemBitmapButton8 = new wxBitmapButton( itemDialog1, ID_VARS_ADD, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
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

////@end frmVariable content construction
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

wxBitmap frmVariable::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin frmVariable bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end frmVariable bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon frmVariable::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin frmVariable icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end frmVariable icon retrieval
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_PATH
 */

void frmVariable::OnPathClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_PATH in frmVariable.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_PATH in frmVariable. 
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_VARS_ADD
 */

void frmVariable::OnVarsAddClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_VARS_ADD in frmVariable.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_VARS_ADD in frmVariable. 
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void frmVariable::OnOkClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK in frmVariable.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK in frmVariable. 
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
 */

void frmVariable::OnCancelClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL in frmVariable.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL in frmVariable. 
}

