/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_BACKUPDATA
#define H_BACKUPDATA

#include <wx/textfile.h>
#include <wx/string.h>
#include "../forms/frmmain.h"
#include "rootdata.h"

/*The backupdata class contains all of the information needed for backups, except the ruels. Includes
functiosn for writing the data to and from the gui and an ini file*/
class BackupData : public RootData{

public:
	//Functions
	bool TransferToFile();
	bool TransferFromFile();
	bool TransferToForm();
	bool TransferFromForm();
	
	bool NeededFieldsFilled();

	void Output();
	
	bool Execute(Rules rules);
	
	bool NeedsPassword() { return IsPassword; }
	
	//Used to create the command to exectue
	wxString CreateCommand(int i);
	
	//Used to create the exclusions list
	bool CreateList(wxTextFile *file, Rules rules, wxString strPath, int iRootLength);

	//Inline functions
	void SetBackupLocation(wxString location) {strBackupLocation = location; }
	wxString GetBackupLocation() { return strBackupLocation; }
	
	void SetLocations(wxArrayString locations) { arrLocations = locations; }
	wxArrayString GetLocations() { return arrLocations; }
	
	void SetLocation(int i, wxString location) { arrLocations.Item(i) = location; }
	wxString GetLocation(int i) { return arrLocations.Item(i); }

	void SetFunction(wxString function) { strFunction = function; }
	wxString GetFunction() { return strFunction; }

	void SetFormat(wxString format) { strFormat = format; }
	wxString GetFormat() { return strFormat; }

	void SetRatio(int ratio) { strRatio = ratio; }
	int GetRatio() { return strRatio; }
	
	//Is a password needed?
	bool IsPassword;
	
private:
	//Contains the filepath to the backup file
	wxString strBackupLocation;
	//A list of paths to be used for backup
	wxArrayString arrLocations;
	//Complete, update, Differential, restore
	wxString strFunction;
	//7zip, zip
	wxString strFormat;
	//Need to change this to a sliding scale from 1 to 5
	int strRatio;
};

#endif
