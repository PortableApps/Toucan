/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef FRMPASSWORD_H
#define FRMPASSWORD_H

#include <wx/gbsizer.h>
#include <wx/wx.h>

#define ID_FRMPASSWORD 10000
#define ID_PASSWORD 10001
#define ID_PASSWORDREPEAT 10002

class frmPassword: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS(frmPassword)
    DECLARE_EVENT_TABLE()

public:
    frmPassword();
    frmPassword(wxWindow* parent, wxWindowID id = ID_FRMPASSWORD, const wxString& caption = _("Input Password"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(-1, 140), long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL);

    bool Create(wxWindow* parent, wxWindowID id = ID_FRMPASSWORD, const wxString& caption = _("Input Password"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(-1, 140), long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL);

    void Init();
    void CreateControls();
    void TextEntered(wxCommandEvent& event);
	wxString GetPassword();

    wxTextCtrl* m_Password;
    wxTextCtrl* m_PasswordRepeat;
    wxButton* m_OK;
};

#endif
