/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "frmrestore.h"

/*!
 * frmRestore type definition
 */

IMPLEMENT_DYNAMIC_CLASS( frmRestore, wxDialog )


/*!
 * frmRestore event table definition
 */

BEGIN_EVENT_TABLE( frmRestore, wxDialog )

	EVT_BUTTON( wxID_OK, frmRestore::OnOkClick )

    EVT_BUTTON( wxID_CANCEL, frmRestore::OnCancelClick )

END_EVENT_TABLE()


/*!
 * frmRestore constructors
 */

frmRestore::frmRestore()
{
    Init();
}

frmRestore::frmRestore( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * frmRestore creator
 */

bool frmRestore::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
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
 * frmRestore destructor
 */

frmRestore::~frmRestore()
{
}


/*!
 * Member initialisation
 */

void frmRestore::Init()
{
    m_File = NULL;
    m_Location = NULL;
}


/*!
 * Control creation for frmRestore
 */

void frmRestore::CreateControls()
{    
    frmRestore* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("Backup file:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemStaticText4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl5 = new wxTextCtrl( itemDialog1, ID_FILE, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemTextCtrl5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton6 = new wxButton( itemDialog1, ID_BUTTON_FILE, _("..."), wxDefaultPosition, wxSize(25, -1), 0 );
    itemBoxSizer3->Add(itemButton6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer7, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText8 = new wxStaticText( itemDialog1, wxID_STATIC, _("Restore into:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer7->Add(itemStaticText8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl9 = new wxTextCtrl( itemDialog1, ID_LOCATION, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer7->Add(itemTextCtrl9, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton10 = new wxButton( itemDialog1, ID_BUTTON_LOCATION, _("..."), wxDefaultPosition, wxSize(25, -1), 0 );
    itemBoxSizer7->Add(itemButton10, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer11 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer11, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxButton* itemButton12 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer11->AddButton(itemButton12);

    wxButton* itemButton13 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer11->AddButton(itemButton13);

    itemStdDialogButtonSizer11->Realize();
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void frmRestore::OnOkClick( wxCommandEvent& event )
{
    event.Skip();
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
 */

void frmRestore::OnCancelClick( wxCommandEvent& event )
{
    event.Skip();
}

