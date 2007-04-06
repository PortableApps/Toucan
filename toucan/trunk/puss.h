/////////////////////////////////////////////////////////////////////////////
// Name:        puss.h
// Purpose:     
// Author:      Steven Lamerton
// Modified by: 
// Created:     08/02/2007 19:08:06
// RCS-ID:      
// Copyright:   Copyright (C)  2006 Steven Lamerton
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _PUSS_H_
#define _PUSS_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "puss.h"
#endif

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
* PUSS class declaration
*/

class PUSS: public wxApp
{    
	DECLARE_CLASS( PUSS )
	DECLARE_EVENT_TABLE()

public:
	/// Constructor
	PUSS();

	/// Initialises the application
	virtual bool OnInit();

	/// Called on exit
	virtual int OnExit();
		void SelectLanguage(int lang) ;

	////@begin PUSS event handler declarations

	////@end PUSS event handler declarations

	////@begin PUSS member function declarations

	////@end PUSS member function declarations

	////@begin PUSS member variables
	////@end PUSS member variables
private:
	wxLocale* m_locale; // 'our' locale
};

/*!
* Application instance declaration 
*/

////@begin declare app
DECLARE_APP(PUSS)
////@end declare app

#endif
// _PUSS_H_
