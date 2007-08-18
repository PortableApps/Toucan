/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2007 Steven Lamerton
// Licence:     GNU GPL 2 (See readme for more info
/////////////////////////////////////////////////////////////////////////////////

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
		
    void SelectLanguage() ;
       

	////@begin Toucan event handler declarations

	////@end Toucan event handler declarations

	////\@begin Toucan member function declarations

	wxString GetStrCommand() const { return strCommand ; }
	void SetStrCommand(wxString value) { strCommand = value ; }
    
	wxString GetStrAbort() const { return strAbort ; }
	void SetStrAbort(wxString value) { strAbort = value ; }

	wxString GetStrTemp() const { return strTemp ; }
	void SetStrTemp(wxString value) { strTemp = value ; }

	long GetlngPID() const { return lngPID ; }
	void SetlngPID(long value) { lngPID = value ; }
    
	bool GetBlFinished() const { return blFinished ; }
	void SetBlFinished(bool value) { blFinished = value ; }

	////\@end Toucan member function declarations

	////@begin Toucan member variables
	wxString strAbort;
	long lngPID;
	bool blFinished;
	wxString strCommand;
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
