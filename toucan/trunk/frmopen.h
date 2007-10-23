/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2007 Steven Lamerton
// Licence:     GNU GPL 2 (See readme for more info
/////////////////////////////////////////////////////////////////////////////////

#ifndef _FRMOPEN_H_
#define _FRMOPEN_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "frmopen.h"
#endif

/*!
 * Includes
 */

////@begin includes
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////\@begin control identifiers
#define ID_FRMOPEN 10013
#define ID_LISTBOX 10000
#define SYMBOL_FRMOPEN_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxDIALOG_MODAL
#define SYMBOL_FRMOPEN_TITLE _("Open")
#define SYMBOL_FRMOPEN_IDNAME ID_FRMOPEN
#define SYMBOL_FRMOPEN_SIZE wxSize(400, 300)
#define SYMBOL_FRMOPEN_POSITION wxDefaultPosition
////\@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * frmOpen class declaration
 */

class frmOpen: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( frmOpen )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    frmOpen();
    frmOpen( wxWindow* parent, wxWindowID id = SYMBOL_FRMOPEN_IDNAME, const wxString& caption = SYMBOL_FRMOPEN_TITLE, const wxPoint& pos = SYMBOL_FRMOPEN_POSITION, const wxSize& size = SYMBOL_FRMOPEN_SIZE, long style = SYMBOL_FRMOPEN_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_FRMOPEN_IDNAME, const wxString& caption = SYMBOL_FRMOPEN_TITLE, const wxPoint& pos = SYMBOL_FRMOPEN_POSITION, const wxSize& size = SYMBOL_FRMOPEN_SIZE, long style = SYMBOL_FRMOPEN_STYLE );

    /// Destructor
    ~frmOpen();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin frmOpen event handler declarations

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OPEN
    void OnOPENClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
    void OnCANCELClick( wxCommandEvent& event );

////@end frmOpen event handler declarations

////@begin frmOpen member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end frmOpen member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin frmOpen member variables
    wxListBox* m_List;
////@end frmOpen member variables
};

#endif
    // _FRMOPEN_H_
