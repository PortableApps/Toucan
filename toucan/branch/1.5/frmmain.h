/////////////////////////////////////////////////////////////////////////////
// Name:        frmmain.h
// Purpose:     
// Author:      Steven Lamerton
// Modified by: 
// Created:     20/08/2007 16:44:04
// RCS-ID:      
// Copyright:   Copyright (C)  2007 Steven Lamerton
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _FRMMAIN_H_
#define _FRMMAIN_H_

/*!
 * Includes
 */

////@begin includes
#include "wx/aui/framemanager.h"
#include "wx/frame.h"
#include "wx/aui/auibook.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_FRMMAIN 10000
#define ID_AUINOTEBOOK1 10001
#define ID_PANEL1 10002
#define ID_PANEL2 10003
#define ID_PANEL3 10004
#define SYMBOL_FRMMAIN_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxCLIP_CHILDREN
#define SYMBOL_FRMMAIN_TITLE _("Toucan")
#define SYMBOL_FRMMAIN_IDNAME ID_FRMMAIN
#define SYMBOL_FRMMAIN_SIZE wxDefaultSize
#define SYMBOL_FRMMAIN_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * frmMain class declaration
 */

class frmMain: public wxFrame
{    
    DECLARE_CLASS( frmMain )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    frmMain();
    frmMain( wxWindow* parent, wxWindowID id = SYMBOL_FRMMAIN_IDNAME, const wxString& caption = SYMBOL_FRMMAIN_TITLE, const wxPoint& pos = SYMBOL_FRMMAIN_POSITION, const wxSize& size = SYMBOL_FRMMAIN_SIZE, long style = SYMBOL_FRMMAIN_STYLE );

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_FRMMAIN_IDNAME, const wxString& caption = SYMBOL_FRMMAIN_TITLE, const wxPoint& pos = SYMBOL_FRMMAIN_POSITION, const wxSize& size = SYMBOL_FRMMAIN_SIZE, long style = SYMBOL_FRMMAIN_STYLE );

    /// Destructor
    ~frmMain();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin frmMain event handler declarations

////@end frmMain event handler declarations

////@begin frmMain member function declarations

    /// Returns the AUI manager object
    wxAuiManager& GetAuiManager() { return m_auiManager; }

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end frmMain member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin frmMain member variables
    wxAuiManager m_auiManager;
////@end frmMain member variables
};

#endif
    // _FRMMAIN_H_
