/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "frmrule.h"
#include <wx/regex.h>

IMPLEMENT_DYNAMIC_CLASS(frmRule, wxDialog)

BEGIN_EVENT_TABLE(frmRule, wxDialog)

    EVT_TEXT(ID_TEXTCTRL, frmRule::OnTextUpdated)
    EVT_TEXT_ENTER(ID_TEXTCTRL, frmRule::OnTextUpdated)
	EVT_COMBOBOX(ID_COMBOBOX, frmRule::OnTextUpdated)

END_EVENT_TABLE()

frmRule::frmRule(){
    Init();
}

frmRule::frmRule(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style){
    Init();
    Create(parent, id, caption, pos, size, style);
}

bool frmRule::Create(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style){
    wxDialog::Create(parent, id, caption, pos, size, style);
    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
    return true;
}

void frmRule::Init(){
    m_Rule = NULL;
    m_Type = NULL;
    m_OK = NULL;
}

void frmRule::CreateControls(){    
    wxBoxSizer* MainSizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(MainSizer);

    wxBoxSizer* TopSizer = new wxBoxSizer(wxHORIZONTAL);
    MainSizer->Add(TopSizer, 0, wxGROW|wxALL, 5);

    wxStaticText* StaticRule = new wxStaticText(this, wxID_STATIC, _("Rule"));
    TopSizer->Add(StaticRule, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_Rule = new wxTextCtrl(this, ID_TEXTCTRL);
    TopSizer->Add(m_Rule, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* MiddleSizer = new wxBoxSizer(wxHORIZONTAL);
    MainSizer->Add(MiddleSizer, 0, wxGROW|wxALL, 5);

    wxStaticText* StaticType = new wxStaticText(this, wxID_STATIC, _("Type"));
    MiddleSizer->Add(StaticType, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_TypeStrings;
    m_TypeStrings.Add(_("File to Exclude"));
    m_TypeStrings.Add(_("Folder to Exclude"));
    m_TypeStrings.Add(_("Location to Include"));
    m_Type = new wxComboBox(this, ID_COMBOBOX, _("File to Exclude"), wxDefaultPosition, wxDefaultSize, m_TypeStrings, wxCB_READONLY);
    m_Type->SetStringSelection(_("File to Exclude"));
    MiddleSizer->Add(m_Type, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* BottomSizer = new wxBoxSizer(wxHORIZONTAL);
    MainSizer->Add(BottomSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    m_OK = new wxButton(this, wxID_OK, _("OK"));
    m_OK->Enable(false);
    BottomSizer->Add(m_OK, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* CancelButton = new wxButton(this, wxID_CANCEL, _("Cancel"));
    BottomSizer->Add(CancelButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
}

void frmRule::OnTextUpdated(wxCommandEvent& WXUNUSED(event)){
    wxString rule = m_Rule->GetValue();
    //Regex
    if(rule.Left(1) == wxT("*")){
        //Otherwise we get popups every time there is an error
        wxLogNull nulllog;
        wxRegEx regex(rule.Right(rule.Length() - 1), wxRE_ICASE| wxRE_EXTENDED);
        if(regex.IsValid()){
            m_OK->Enable(true);
        }
        else{
            m_OK->Enable(false);        
        }
    }
    //Date or time
    else if(rule.Left(1) == wxT("<") || rule.Left(1) == wxT(">")){
		if(m_Type->GetStringSelection() != _("File to Exclude")){
			m_OK->Enable(false);
			return;
		}
        wxDateTime date;
        wxRegEx regex(wxT("(([0-9]+)(kB|MB|GB))"));
        if(date.ParseDate(rule.Right(rule.Length() - 1))){
            m_OK->Enable(true);
        }
        else if(regex.Matches(rule.Right(rule.Length() - 1))){
			//Check that we are matching the whole length of the string
            if(regex.GetMatch(rule.Right(rule.Length() - 1)).Length() == rule.Length() - 1){
                m_OK->Enable(true);
            }
            else{
                m_OK->Enable(false); 
            }
        }
        else{
            m_OK->Enable(false);        
        }
    }
    //Plain text
    else if(rule != wxEmptyString){
        m_OK->Enable(true);
    }
	else{
        m_OK->Enable(false);
	}
}
