/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include <wx/wx.h>
#include <wx/stdpaths.h>

#include "frmvariable.h"
#include "../basicfunctions.h"
#include "../variables.h"

IMPLEMENT_DYNAMIC_CLASS(frmVariable, wxDialog)

BEGIN_EVENT_TABLE(frmVariable, wxDialog)
    EVT_BUTTON(ID_LOCATION, frmVariable::OnLocationClick)
    EVT_BUTTON(ID_VARIABLES, frmVariable::OnVariablesClick)
	EVT_TEXT(ID_LOCATION_TEXT, frmVariable::OnTextChange)
END_EVENT_TABLE()

frmVariable::frmVariable(){
    Init();
}

frmVariable::frmVariable(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style){
    Init();
    Create(parent, id, caption, pos, size, style);
}

bool frmVariable::Create(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style){
    wxDialog::Create(parent, id, caption, pos, size, style);
    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
    return true;
}

void frmVariable::Init(){
    m_Location_Text = NULL;
    m_Preview_Text = NULL;
    m_Variables_Combo = NULL;
}

void frmVariable::CreateControls(){  
	wxString iconpath = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH;

    wxBoxSizer* TopSizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(TopSizer);

	wxBoxSizer* LocationSizer = new wxBoxSizer(wxHORIZONTAL);
	TopSizer->Add(LocationSizer, 0, wxGROW|wxALL, 5);
		
	wxStaticText* LocationStaticText = new wxStaticText(this, wxID_STATIC, _("Location"));
	LocationSizer->Add(LocationStaticText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
			
	m_Location_Text = new wxTextCtrl(this, ID_LOCATION_TEXT);
	m_Location_Text->SetMinSize(wxSize(300, -1));
	LocationSizer->Add(m_Location_Text, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxButton* LocationButton = new wxButton(this, ID_LOCATION, wxT("..."), wxDefaultPosition, wxSize(25, -1));
	LocationSizer->Add(LocationButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* VariablesSizer = new wxBoxSizer(wxHORIZONTAL);
	TopSizer->Add(VariablesSizer, 0, wxALIGN_LEFT|wxALL, 5);
		
	wxStaticText* VariablesStaticText = new wxStaticText(this, wxID_STATIC, _("Variables"));
	VariablesSizer->Add(VariablesStaticText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxArrayString m_VariablesStrings = GetVariables(true);
	m_Variables_Combo = new wxComboBox(this, ID_VARIABLES_COMBO, wxEmptyString, wxDefaultPosition, wxDefaultSize, m_VariablesStrings);
	VariablesSizer->Add(m_Variables_Combo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBitmapButton* VariablesButton = new wxBitmapButton(this, ID_VARIABLES, wxBitmap(iconpath + _T("add.png"), wxBITMAP_TYPE_PNG), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	VariablesSizer->Add(VariablesButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* PreviewSizer = new wxBoxSizer(wxHORIZONTAL);
	TopSizer->Add(PreviewSizer, 0, wxGROW|wxALL, 5);

	wxStaticText* PreviewStaticText = new wxStaticText(this, wxID_STATIC, _("Preview"));
	PreviewSizer->Add(PreviewStaticText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_Preview_Text = new wxTextCtrl(this, ID_PREVIEW_TEXT, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
	PreviewSizer->Add(m_Preview_Text, 1, wxGROW|wxALL, 5);

	wxBoxSizer* ButtonSizer = new wxBoxSizer(wxHORIZONTAL);
	TopSizer->Add(ButtonSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxButton* OKButton = new wxButton(this, wxID_OK, _("OK"));
	ButtonSizer->Add(OKButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxButton* CancelButton = new wxButton(this, wxID_CANCEL, _("Cancel"));
	ButtonSizer->Add(CancelButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
		
	SetIcon(wxIcon(iconpath + wxT("Toucan.ico"), wxBITMAP_TYPE_ICO));
}

void frmVariable::OnLocationClick(wxCommandEvent& WXUNUSED(event)){
	wxDirDialog dialog(this,_("Please select a folder"));
	if (dialog.ShowModal() == wxID_OK){
		m_Location_Text->SetValue(dialog.GetPath());
	}
}

void frmVariable::OnVariablesClick(wxCommandEvent& WXUNUSED(event)){
	m_Location_Text->WriteText(wxT("@") + m_Variables_Combo->GetStringSelection() + wxT("@"));	
}

void frmVariable::OnTextChange(wxCommandEvent& WXUNUSED(event)){
	m_Preview_Text->SetValue(Normalise(m_Location_Text->GetValue()));
}

wxString frmVariable::GetValue(){
	return m_Location_Text->GetValue();
}
