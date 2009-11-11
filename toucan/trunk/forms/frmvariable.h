/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef FRMVARIABLE_H
#define FRMVARIABLE_H

#include <wx/dialog.h>
class wxTextCtrl;
class wxComboBox;

enum{
	ID_FRMVARIABLE,
	ID_LOCATION,
	ID_LOCATION_TEXT,
	ID_VARIABLES,
	ID_VARIABLES_COMBO,
	ID_PREVIEW_TEXT
};

class frmVariable: public wxDialog
{
    DECLARE_EVENT_TABLE()

public:
	frmVariable();
    frmVariable(wxWindow* parent, wxWindowID id = ID_FRMVARIABLE, const wxString& caption = _("Insert Variable"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(-1, -1), long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL);

    bool Create(wxWindow* parent, wxWindowID id = ID_FRMVARIABLE, const wxString& caption = _("Insert Variable"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(-1, -1), long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL);

    void Init();
    void CreateControls();

    void OnLocationClick(wxCommandEvent& event);
    void OnVariablesClick(wxCommandEvent& event);
	void OnTextChange(wxCommandEvent& event);

	wxString GetValue();

private:
    wxTextCtrl* m_Location_Text;
    wxTextCtrl* m_Preview_Text;
    wxComboBox* m_Variables_Combo;
};

#endif
