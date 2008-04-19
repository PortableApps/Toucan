/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef _TOUCAN_H_
#define _TOUCAN_H_

#include <wx\process.h>
#include <wx\image.h>

#include "frmmain.h"
#include "backupdata.h"
#include "rules.h"

class PipedProcess;
WX_DEFINE_ARRAY_PTR(PipedProcess *, ProcessArray);


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

	//The process storage array
	ProcessArray m_Running;
	
	wxArrayString GetSecureLocations() { return SecureLocations ; }
	void SetSecureLocations(wxArrayString value) { SecureLocations = value ; }
	void AppendSecureLocation (wxString string) { SecureLocations.Add(string) ;}
	void RemoveSecureLocation (int pos) { SecureLocations.RemoveAt(pos) ;}

	wxArrayString GetBackupLocations() { return BackupLocations ; }
	void SetBackupLocations(wxArrayString value) { BackupLocations = value ; }
	void AppendBackupLocation (wxString string) { BackupLocations.Add(string) ;}
	void RemoveBackupLocation (int pos) { BackupLocations.RemoveAt(pos) ;}
	
		
    bool RegisterProcess(PipedProcess *process);
    bool UnregisterProcess(PipedProcess *process);
	
	bool ShouldAbort() { return blAbort; }
	void SetAbort(bool abort) { blAbort = abort; }
		
	long GetPID() { return lgPID; }
	void SetPID(long pid) { lgPID = pid; }


    void OnIdle(wxIdleEvent& event);
    void OnTimer(wxTimerEvent& event);
	
	long GetBackup() { return lgBackupNumber; }
	void SetBackup(long backupnumber) { lgBackupNumber = backupnumber; }	
	
	wxString GetSettingsPath() { return strSettingsPath; }
	void SetSettingsPath(wxString settingspath) { strSettingsPath = settingspath; }
	
	BackupData GetBackupData() { return data; }
	void SetBackupData(BackupData newdata) { data = newdata; }
	
	Rules GetRules() { return rules; }
	void SetRules(Rules newrules) { rules = newrules; }	
	
protected:
    // Timer to wake up idle processing
    wxTimer m_wakeUpTimer;
	wxArrayString SecureLocations;
	wxArrayString BackupLocations;
	bool blAbort;
	long lgPID;
	long lgBackupNumber;
	wxString strSettingsPath;
	
	BackupData data;
	Rules rules;
};

DECLARE_APP(Toucan)

#endif
