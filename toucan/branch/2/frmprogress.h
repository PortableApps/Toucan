/////////////////////////////////////////////////////////////////////////////
// Name:        frmprogress.h
// Purpose:     
// Author:      Steven Lamerton
// Modified by: 
// Created:     21/12/2007 12:12:17
// RCS-ID:      
// Copyright:   Copyright (c) Steven Lamerton 2006-2007
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _FRMPROGRESS_H_
#define _FRMPROGRESS_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/frame.h"
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
#define ID_FRMPROGRESS 10007
#define ID_PANEL 10028
#define ID_TEXTCTRL 10067
#define SYMBOL_FRMPROGRESS_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_FRMPROGRESS_TITLE _("Progress")
#define SYMBOL_FRMPROGRESS_IDNAME ID_FRMPROGRESS
#define SYMBOL_FRMPROGRESS_SIZE wxSize(400, 300)
#define SYMBOL_FRMPROGRESS_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * frmProgress class declaration
 */

class frmProgress: public wxFrame
{    
 DECLARE_CLASS( frmProgress )
 DECLARE_EVENT_TABLE()

public:
 /// Constructors
 frmProgress();
 frmProgress( wxWindow* parent, wxWindowID id = SYMBOL_FRMPROGRESS_IDNAME, const wxString& caption = SYMBOL_FRMPROGRESS_TITLE, const wxPoint& pos = SYMBOL_FRMPROGRESS_POSITION, const wxSize& size = SYMBOL_FRMPROGRESS_SIZE, long style = SYMBOL_FRMPROGRESS_STYLE );

 bool Create( wxWindow* parent, wxWindowID id = SYMBOL_FRMPROGRESS_IDNAME, const wxString& caption = SYMBOL_FRMPROGRESS_TITLE, const wxPoint& pos = SYMBOL_FRMPROGRESS_POSITION, const wxSize& size = SYMBOL_FRMPROGRESS_SIZE, long style = SYMBOL_FRMPROGRESS_STYLE );

 /// Destructor
 ~frmProgress();

 /// Initialises member variables
 void Init();

 /// Creates the controls and sizers
 void CreateControls();

////@begin frmProgress event handler declarations

 /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 void OnOkClick( wxCommandEvent& event );

////@end frmProgress event handler declarations

////@begin frmProgress member function declarations

 /// Retrieves bitmap resources
 wxBitmap GetBitmapResource( const wxString& name );

 /// Retrieves icon resources
 wxIcon GetIconResource( const wxString& name );
////@end frmProgress member function declarations

 /// Should we show tooltips?
 static bool ShowToolTips();

////@begin frmProgress member variables
 wxTextCtrl* m_Text;
////@end frmProgress member variables
};

#endif
 // _FRMPROGRESS_H_
