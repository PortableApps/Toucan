/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_BACKUPDATA
#define H_BACKUPDATA

#include <wx\string.h>
#include "frmmain.h"

/*The backupdata class contains all of the information needed for backups, except the ruels. Includes
functiosn for writing the data to and from the gui and an ini file*/
class BackupData{

public:
	//Functions
	bool TransferToFile(wxString strName);
	bool TransferFromFile(wxString strName);
	void TransferToForm(frmMain *window);
	bool TransferFromForm(frmMain*window, bool blShowErrors);
	wxString CreateCommand(int i);
	void Output();

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

	void SetPass(wxString pass) { strPass = pass; }
	wxString GetPass() { return strPass; }
	
	//Is a password needed?
	bool IsPassword;
	
private:
	//Contains the filepath to the backup file
	wxString strBackupLocation;
	//A list of paths to be used for backup
	wxArrayString arrLocations;
	//Complete, update, incremental, restore
	wxString strFunction;
	//7zip, zip
	wxString strFormat;
	//Need to change this to a sliding scale from 1 to 5
	int strRatio;
	//A password if one is used	
	wxString strPass;

};

#endif
