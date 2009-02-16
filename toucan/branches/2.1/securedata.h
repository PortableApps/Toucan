/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SECUREDATA
#define H_SECUREDATA

#include <wx/string.h>
#include "frmmain.h"
#include "rootdata.h"

/*Securedata class stores all information needed for sync operations
excluding the rule set. Also includes functions for copying too and from
the gui and ini files*/
class SecureData : public RootData{

public:

//Functions
	bool TransferToFile();
	bool TransferFromFile();
	bool TransferToForm();
	bool TransferFromForm();
	void Output();
	
	bool NeededFieldsFilled();

	//Inline functions
	void SetLocations(wxArrayString locations) { arrLocations = locations; }
	wxArrayString GetLocations() { return arrLocations; }
	
	void SetLocation(int i, wxString location) { arrLocations.Item(i) = location; }
	wxString GetLocation(int i) { return arrLocations.Item(i); }

	void SetFunction(wxString function) { strFunction = function; }
	wxString GetFunction() { return strFunction; }
	
	bool Execute(Rules rules);
	bool NeedsPassword() { return true; }

private:
	//List of locations to secure, inlcuding files and folders
	wxArrayString arrLocations;
	//Encrypt or decrypt
	wxString strFunction;
};

#endif
