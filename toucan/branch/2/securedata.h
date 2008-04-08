/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SECUREDATA
#define H_SECUREDATA

#include <wx\string.h>
#include "frmmain.h"

/*Securedata class stores all information needed for sync operations
excluding the rule set. Also includes functions for copying too and from
the gui and ini files*/
class SecureData{

public:

//Functions
	bool TransferToFile(wxString strName);
	bool TransferFromFile(wxString strName);
	void TransferToForm(frmMain *window);
	bool TransferFromForm(frmMain*window, bool blShowError);
	void Output();

	//Inline functions
	void SetLocations(wxArrayString locations) { arrLocations = locations; }
	wxArrayString GetLocations() { return arrLocations; }

	void SetFunction(wxString function) { strFunction = function; }
	wxString GetFunction() { return strFunction; }

	void SetFormat(wxString format) { strFormat = format; }
	wxString GetFormat() { return strFormat; }

	void SetPass(wxString pass) { strPass = pass; }
	wxString GetPass() { return strPass; }


private:
	//List of locations to secure, inlcuding files and folders
	wxArrayString arrLocations;
	//Encrypt or decrypt
	wxString strFunction;
	//Rijndael or blowfish (decrypt only
	wxString strFormat;
	//A single copy of the apssword, checking is done by the gui
	wxString strPass;

};

#endif
