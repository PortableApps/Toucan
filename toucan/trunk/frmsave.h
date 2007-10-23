/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2007 Steven Lamerton
// Licence:     GNU GPL 2 (See readme for more info
/////////////////////////////////////////////////////////////////////////////////

#ifndef _FRMSAVE_H_
#define _FRMSAVE_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "frmsave.h"
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

////@begin control identifiers
#define ID_FRMSAVE 10011
#define ID_LISTBOX3 10012
#define SYMBOL_FRMSAVE_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxDIALOG_MODAL
#define SYMBOL_FRMSAVE_TITLE _("Save")
#define SYMBOL_FRMSAVE_IDNAME ID_FRMSAVE
#define SYMBOL_FRMSAVE_SIZE wxSize(400, 300)
#define SYMBOL_FRMSAVE_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * frmSave class declaration
 */

class frmSave: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( frmSave )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    frmSave();
    frmSave( wxWindow* parent, wxWindowID id = SYMBOL_FRMSAVE_IDNAME, const wxString& caption = SYMBOL_FRMSAVE_TITLE, const wxPoint& pos = SYMBOL_FRMSAVE_POSITION, const wxSize& size = SYMBOL_FRMSAVE_SIZE, long style = SYMBOL_FRMSAVE_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_FRMSAVE_IDNAME, const wxString& caption = SYMBOL_FRMSAVE_TITLE, const wxPoint& pos = SYMBOL_FRMSAVE_POSITION, const wxSize& size = SYMBOL_FRMSAVE_SIZE, long style = SYMBOL_FRMSAVE_STYLE );

    /// Destructor
    ~frmSave();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin frmSave event handler declarations

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_SAVE
    void OnSAVEClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_NEW
    void OnNEWClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
    void OnCANCELClick( wxCommandEvent& event );

////@end frmSave event handler declarations

////@begin frmSave member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end frmSave member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin frmSave member variables
    wxListBox* m_List;
////@end frmSave member variables
};

#endif
    // _FRMSAVE_H_
