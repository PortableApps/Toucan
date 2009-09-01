/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "frmpassword.h"

IMPLEMENT_DYNAMIC_CLASS(frmPassword, wxDialog)

BEGIN_EVENT_TABLE(frmPassword, wxDialog)

    EVT_TEXT(ID_PASSWORD, frmPassword::TextEntered)
    EVT_TEXT(ID_PASSWORDREPEAT, frmPassword::TextEntered)

END_EVENT_TABLE()

frmPassword::frmPassword(){
    Init();
}

frmPassword::frmPassword(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style){
    Init();
    Create(parent, id, caption, pos, size, style);
}

bool frmPassword::Create(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style){
    wxDialog::Create(parent, id, caption, pos, size, style);
    CreateControls();
    Centre();
    return true;
}

void frmPassword::Init(){
    m_Password = NULL;
    m_PasswordRepeat = NULL;
    m_OK = NULL;
}

void frmPassword::CreateControls(){    
	wxGridBagSizer* MainSizer = new wxGridBagSizer(0, 0);
    MainSizer->AddGrowableCol(1);
    MainSizer->SetEmptyCellSize(wxSize(10, 20));
    SetSizer(MainSizer);

    wxStaticText* StaticPassword = new wxStaticText(this, wxID_STATIC, _("Password"));
    MainSizer->Add(StaticPassword, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* StaticPasswordRepeat = new wxStaticText(this, wxID_STATIC, _("Repeat Password"));
    MainSizer->Add(StaticPasswordRepeat, wxGBPosition(1, 0), wxGBSpan(1, 1), wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_Password = new wxTextCtrl(this, ID_PASSWORD, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
    MainSizer->Add(m_Password, wxGBPosition(0, 1), wxGBSpan(1, 1), wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_PasswordRepeat = new wxTextCtrl(this, ID_PASSWORDREPEAT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
    MainSizer->Add(m_PasswordRepeat, wxGBPosition(1, 1), wxGBSpan(1, 1), wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* ButtonSizer = new wxBoxSizer(wxHORIZONTAL);
    MainSizer->Add(ButtonSizer, wxGBPosition(2, 0), wxGBSpan(1, 2), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_OK = new wxButton(this, wxID_OK, _("OK"));
    m_OK->Enable(false);
    ButtonSizer->Add(m_OK, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* CancelButton = new wxButton(this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0);
    ButtonSizer->Add(CancelButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
}

void frmPassword::TextEntered(wxCommandEvent& WXUNUSED(event)){
	//We only need to check if one is empty as we know both are equal
    if(m_Password->GetValue() == m_PasswordRepeat->GetValue() && m_Password->GetValue() != wxEmptyString){
        m_OK->Enable(true);
    }
    else{
        m_OK->Enable(false);
    }
}

wxString frmPassword::GetPassword(){
	return m_Password->GetValue();
}
