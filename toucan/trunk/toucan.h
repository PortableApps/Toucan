/////////////////////////////////////////////////////////////////////////////
// Name:        toucan.h
// Purpose:     
// Author:      Steven Lamerton
// Modified by: 
// Created:     08/02/2007 19:08:06
// RCS-ID:      
// Copyright:   Copyright (C)  2006 Steven Lamerton
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _Toucan_H_
#define _Toucan_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "toucan.h"
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
* Toucan class declaration
*/

class Toucan: public wxApp
{    
	DECLARE_CLASS( Toucan )
	DECLARE_EVENT_TABLE()

public:
	/// Constructor
	Toucan();

	/// Initialises the application
	virtual bool OnInit();

	/// Called on exit
	virtual int OnExit();
		
        void SelectLanguage(int lang) ;
       

	////@begin Toucan event handler declarations

	////@end Toucan event handler declarations

	////\@begin Toucan member function declarations

	wxString GetStrTemp() const { return strTemp ; }
	void SetStrTemp(wxString value) { strTemp = value ; }

	wxString GetStrFirst() const { return strFirst ; }
	void SetStrFirst(wxString value) { strFirst = value ; }

	wxString GetStrSecond() const { return strSecond ; }
	void SetStrSecond(wxString value) { strSecond = value ; }

	wxString GetStrFunction() const { return strFunction ; }
	void SetStrFunction(wxString value) { strFunction = value ; }

	wxString GetStrFormat() const { return strFormat ; }
	void SetStrFormat(wxString value) { strFormat = value ; }

	wxString GetStrRatio() const { return strRatio ; }
	void SetStrRatio(wxString value) { strRatio = value ; }

	wxString GetStrExclusions() const { return strExclusions ; }
	void SetStrExclusions(wxString value) { strExclusions = value ; }

	wxString GetStrAbort() const { return strAbort ; }
	void SetStrAbort(wxString value) { strAbort = value ; }

	wxString GetStrType() const { return strType ; }
	void SetStrType(wxString value) { strType = value ; }

	long GetlngPID() const { return lngPID ; }
	void SetlngPID(long value) { lngPID = value ; }

	////\@end Toucan member function declarations

	////@begin Toucan member variables
	wxString strFirst;
	wxString strSecond;
	wxString strFunction;
	wxString strFormat;
	wxString strRatio;
	wxString strExclusions;
	wxString strAbort;
	wxString strType;
	long lngPID;
public:
	wxString strTemp;
	////@end Toucan member variables
private:
	wxLocale* m_locale; // 'our' locale
};

/*!
* Application instance declaration 
*/

///@begin declare app
DECLARE_APP(Toucan)
///@end declare app

#endif
