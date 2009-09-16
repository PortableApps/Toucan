/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef FRMRULE_H
#define FRMRULE_H

#include <wx/wx.h>

#define ID_FRMRULES 10003
#define ID_TEXTCTRL 10004
#define ID_COMBOBOX 10005

class frmRule : public wxDialog
{    
    DECLARE_DYNAMIC_CLASS(frmRule)
    DECLARE_EVENT_TABLE()

public:
    frmRule();
    frmRule(wxWindow* parent, wxWindowID id = ID_FRMRULES, const wxString& caption = _("Rules"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL);

    bool Create(wxWindow* parent, wxWindowID id = ID_FRMRULES, const wxString& caption = _("Rules"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL);
    void Init();
    void CreateControls();

    void OnTextUpdated(wxCommandEvent& event);

	void SetRule(const wxString &rule) { m_Rule->SetValue(rule); }
	void SetType(const wxString &type) { m_Type->SetStringSelection(type); }
	wxString GetRule() { return m_Rule->GetValue(); }
	wxString GetType() { return m_Type->GetStringSelection(); }

private:
    wxTextCtrl* m_Rule;
    wxComboBox* m_Type;
    wxButton* m_OK;
};

#endif
