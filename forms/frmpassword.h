/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef FRMPASSWORD_H
#define FRMPASSWORD_H

#include <wx/dialog.h>
class wxTextCtrl;
class wxButton;

enum{
	ID_FRMPASSWORD = wxID_HIGHEST + 1,
	ID_PASSWORD,
	ID_PASSWORDREPEAT
};

class frmPassword: public wxDialog
{    
    DECLARE_EVENT_TABLE()

public:
    frmPassword();
    frmPassword(wxWindow* parent, wxWindowID id = ID_FRMPASSWORD, const wxString& caption = _("Input Password"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(-1, -1), long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL);

    bool Create(wxWindow* parent, wxWindowID id = ID_FRMPASSWORD, const wxString& caption = _("Input Password"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(-1, -1), long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL);

    void Init();
    void CreateControls();
    void TextEntered(wxCommandEvent& event);
	wxString GetPassword();

private:
    wxTextCtrl* m_Password;
    wxTextCtrl* m_PasswordRepeat;
    wxButton* m_OK;
};

#endif
