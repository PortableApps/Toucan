/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_RULES
#define H_RULES

#include <wx/arrstr.h>

class Rules{
public:
	//Constructor
	//Rules();

	//Functions
	bool IsEmpty();
	bool TransferToFile(wxString strName);
	bool TransferFromFile(wxString strName);
	bool ShouldExclude(wxString strName, bool blIsDir); 
	
	//Inline Functions
	void SetFilesToExclude(wxArrayString filestoexclude){ arrFilesToExclude = filestoexclude; }
	wxArrayString GetFilesToExclude() { return arrFilesToExclude; }
	
	void SetLocationsToInclude(wxArrayString locationstoinclude){ arrLocationsToInclude = locationstoinclude; }
	wxArrayString GetLocationsToInclude() { return arrLocationsToInclude; }
	
	void SetFoldersToExclude(wxArrayString folderstoexclude){ arrFoldersToExclude = folderstoexclude; }
	wxArrayString GetFoldersToExclude() { return arrFoldersToExclude; }
	
	void Clear(){
		arrFoldersToExclude.Clear();
		arrLocationsToInclude.Clear();
		arrFilesToExclude.Clear();
	}
	
private:

	wxArrayString arrFoldersToExclude;
	wxArrayString arrLocationsToInclude;
	wxArrayString arrFilesToExclude;

};	

#endif
