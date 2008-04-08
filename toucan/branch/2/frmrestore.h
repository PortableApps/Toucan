/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////


#ifndef _FRMRESTORE_H_
#define _FRMRESTORE_H_

#include <wx\wx.h>
#include <wx\dialog.h>

/*!
 * Control identifiers
 */

#define ID_FRMRESTORE 10001
#define wxID_STATIC_FILE 10006
#define ID_FILE 10002
#define ID_BUTTON_FILE 10003
#define wxID_STATIC_LOCATION 10007
#define ID_LOCATION 10004
#define ID_BUTTON_LOCATION 10005
#define SYMBOL_FRMRESTORE_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_FRMRESTORE_TITLE _("Restore")
#define SYMBOL_FRMRESTORE_IDNAME ID_FRMRESTORE
#define SYMBOL_FRMRESTORE_SIZE wxSize(400, 300)
#define SYMBOL_FRMRESTORE_POSITION wxDefaultPosition

/*!
 * frmRestore class declaration
 */

class frmRestore: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( frmRestore )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    frmRestore();
    frmRestore( wxWindow* parent, wxWindowID id = SYMBOL_FRMRESTORE_IDNAME, const wxString& caption = SYMBOL_FRMRESTORE_TITLE, const wxPoint& pos = SYMBOL_FRMRESTORE_POSITION, const wxSize& size = SYMBOL_FRMRESTORE_SIZE, long style = SYMBOL_FRMRESTORE_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_FRMRESTORE_IDNAME, const wxString& caption = SYMBOL_FRMRESTORE_TITLE, const wxPoint& pos = SYMBOL_FRMRESTORE_POSITION, const wxSize& size = SYMBOL_FRMRESTORE_SIZE, long style = SYMBOL_FRMRESTORE_STYLE );

    /// Destructor
    ~frmRestore();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
    void OnOkClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
    void OnCancelClick( wxCommandEvent& event );

    wxTextCtrl* m_File;
    wxTextCtrl* m_Location;

};

#endif
