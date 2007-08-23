/////////////////////////////////////////////////////////////////////////////
// Name:        toucan.h
// Purpose:     
// Author:      Steven Lamerton
// Modified by: 
// Created:     20/08/2007 16:41:25
// RCS-ID:      
// Copyright:   Copyright (C)  2007 Steven Lamerton
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _TOUCAN_H_
#define _TOUCAN_H_

/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
#include "frmmain.h"
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
////@end control identifiers

/*!
 * Toucan class declaration
 */

class Toucan: public wxApp
{    
    DECLARE_CLASS( Toucan )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    Toucan();

    void Init();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

////@begin Toucan event handler declarations

////@end Toucan event handler declarations

////@begin Toucan member function declarations

////@end Toucan member function declarations

////@begin Toucan member variables
////@end Toucan member variables
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(Toucan)
////@end declare app

#endif
    // _TOUCAN_H_
