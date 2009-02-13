/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_RESTOREDATA
#define H_RESTOREDATA

#include <wx/textfile.h>
#include <wx/string.h>
#include "frmmain.h"
#include "rootdata.h"

class RestoreData : public RootData{

public:
	//Functions
	bool TransferToFile();
	bool TransferFromFile();
	bool NeededFieldsFilled();
	void Output();
	bool Execute(Rules rules);
	bool NeedsPassword() { return IsPassword; }

	//These functions are not fully implemented
	bool TransferToForm();
	bool TransferFromForm();

	//Inline functions
	void SetBackupFile(wxString file) {strBackupFile = file; }
	wxString GetBackupFile() { return strBackupFile; }

	void SetBackupFolder(wxString folder) {strBackupFolder = folder; }
	wxString GetBackupFolder() { return strBackupFolder; }

	//Is a password needed?
	bool IsPassword;

private:
	wxString strBackupFile;
	wxString strBackupFolder;
};

#endif
