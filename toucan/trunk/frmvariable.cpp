/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include <wx/wx.h>
#include <wx/stdpaths.h>

#include "frmvariable.h"
#include "basicfunctions.h"
#include "variables.h"


BEGIN_EVENT_TABLE(frmVariable, wxDialog)
    EVT_BUTTON(ID_LOCATION, frmVariable::OnLocationClick)
    EVT_BUTTON(ID_VARIABLES, frmVariable::OnVariablesClick)
	EVT_TEXT(ID_LOCATION_TEXT, frmVariable::OnTextChange)
END_EVENT_TABLE()


frmVariable::frmVariable( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    wxDialog::Create( parent, id, caption, pos, size, style );
    CreateControls();
	
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
}

void frmVariable::Init()
{
    m_Location_Text = NULL;
    m_Preview_Text = NULL;
    m_Variables_Combo = NULL;
}

void frmVariable::CreateControls()
{    
    frmVariable* Dialog = this;

    wxBoxSizer* TopSizer = new wxBoxSizer(wxVERTICAL);
    Dialog->SetSizer(TopSizer);

		wxBoxSizer* LocationSizer = new wxBoxSizer(wxHORIZONTAL);
		TopSizer->Add(LocationSizer, 0, wxGROW|wxALL, 5);
		
			wxStaticText* LocationStaticText = new wxStaticText(Dialog, wxID_STATIC, _("Location"));
			LocationSizer->Add(LocationStaticText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
			
			m_Location_Text = new wxTextCtrl(Dialog, ID_LOCATION_TEXT, _T(""), wxDefaultPosition, wxSize(-1, -1));
			m_Location_Text->SetMinSize(wxSize(300, -1));
			LocationSizer->Add(m_Location_Text, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

			wxButton* LocationButton = new wxButton(Dialog, ID_LOCATION, wxT("..."), wxDefaultPosition, wxSize(25, -1), 0);
			LocationSizer->Add(LocationButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

		wxBoxSizer* VariablesSizer = new wxBoxSizer(wxHORIZONTAL);
		TopSizer->Add(VariablesSizer, 0, wxALIGN_LEFT|wxALL, 5);
		
			wxStaticText* VariablesStaticText = new wxStaticText(Dialog, wxID_STATIC, _("Portable Variables"), wxDefaultPosition, wxDefaultSize, 0);
			VariablesSizer->Add(VariablesStaticText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

			wxArrayString m_VariablesStrings;
			m_Variables_Combo = new wxComboBox(Dialog, ID_VARIABLES_COMBO, wxEmptyString, wxDefaultPosition, wxDefaultSize, m_VariablesStrings, 0);
			VariablesSizer->Add(m_Variables_Combo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

			wxBitmapButton* VariablesButton = new wxBitmapButton(Dialog, ID_VARIABLES, this->GetBitmapResource(wxT("add.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
			VariablesSizer->Add(VariablesButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

		wxBoxSizer* PreviewSizer = new wxBoxSizer(wxHORIZONTAL);
		TopSizer->Add(PreviewSizer, 0, wxGROW|wxALL, 5);

			wxStaticText* PreviewStaticText = new wxStaticText(Dialog, wxID_STATIC, _("Preview"));
			PreviewSizer->Add(PreviewStaticText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

			m_Preview_Text = new wxTextCtrl(Dialog, ID_PREVIEW_TEXT, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
			PreviewSizer->Add(m_Preview_Text, 1, wxGROW|wxALL, 5);

		wxBoxSizer* ButtonSizer = new wxBoxSizer(wxHORIZONTAL);
		TopSizer->Add(ButtonSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

			wxButton* OKButton = new wxButton(Dialog, wxID_OK, _("OK"));
			ButtonSizer->Add(OKButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

			wxButton* CancelButton = new wxButton(Dialog, wxID_CANCEL, _("Cancel"));
			ButtonSizer->Add(CancelButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
		
	wxString strPath = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH;
	this->SetIcon(wxIcon(strPath + wxT("Toucan.ico"), wxBITMAP_TYPE_ICO));
	
	SetVariablesBox(m_Variables_Combo);
	m_Variables_Combo->Append(wxT("date"));
	m_Variables_Combo->Append(wxT("time"));
	m_Variables_Combo->Append(wxT("docs"));
	m_Variables_Combo->Append(wxT("drive"));
	m_Variables_Combo->Append(wxT("YYYY"));
	m_Variables_Combo->Append(wxT("MM"));
	m_Variables_Combo->Append(wxT("DD"));
	m_Variables_Combo->Append(wxT("hh"));
	m_Variables_Combo->Append(wxT("mm"));
}

wxBitmap frmVariable::GetBitmapResource(const wxString& name)
{
	wxString strPath = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH;
	if (name == _T("add.png")) {
		wxBitmap bitmap(strPath + _T("add.png"), wxBITMAP_TYPE_PNG);
		return bitmap;
	} 
	return wxNullBitmap;
}

void frmVariable::OnLocationClick(wxCommandEvent& event)
{
	wxDirDialog dialog(this,_("Please select a folder."));
	if (dialog.ShowModal() == wxID_OK) {
		m_Location_Text->SetValue(dialog.GetPath());
	}
}

void frmVariable::OnVariablesClick(wxCommandEvent& event){
	m_Location_Text->WriteText(wxT("@") + m_Variables_Combo->GetStringSelection() + wxT("@"));	
}

void frmVariable::OnTextChange(wxCommandEvent& event){
	wxString strPath = m_Location_Text->GetValue();
	strPath = Normalise(Normalise(strPath));
	m_Preview_Text->SetValue(strPath);
}
