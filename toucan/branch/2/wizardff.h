/////////////////////////////////////////////////////////////////////////////
// Name:        wizardff.h
// Purpose:     
// Author:      Steven Lamerton
// Modified by: 
// Created:     19/10/2007 14:56:42
// RCS-ID:      
// Copyright:   Copyright (c) Steven Lamerton 2006-2007
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _WIZARDFF_H_
#define _WIZARDFF_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/wizard.h"
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
#define ID_WIZARDFF 10029
#define SYMBOL_WIZARDFF_IDNAME ID_WIZARDFF
////@end control identifiers


/*!
 * WizardFF class declaration
 */

class WizardFF: public wxWizard
{    
 DECLARE_DYNAMIC_CLASS( WizardFF )
 DECLARE_EVENT_TABLE()

public:
 /// Constructors
 WizardFF();
 WizardFF( wxWindow* parent, wxWindowID id = SYMBOL_WIZARDFF_IDNAME, const wxPoint& pos = wxDefaultPosition );

 /// Creation
 bool Create( wxWindow* parent, wxWindowID id = SYMBOL_WIZARDFF_IDNAME, const wxPoint& pos = wxDefaultPosition );

 /// Destructor
 ~WizardFF();

 /// Initialises member variables
 void Init();

 /// Creates the controls and sizers
 void CreateControls();

////@begin WizardFF event handler declarations

////@end WizardFF event handler declarations

////@begin WizardFF member function declarations

 /// Runs the wizard
 bool Run();

 /// Retrieves bitmap resources
 wxBitmap GetBitmapResource( const wxString& name );

 /// Retrieves icon resources
 wxIcon GetIconResource( const wxString& name );
////@end WizardFF member function declarations

 /// Should we show tooltips?
 static bool ShowToolTips();

////@begin WizardFF member variables
////@end WizardFF member variables
};

#endif
 // _WIZARDFF_H_
