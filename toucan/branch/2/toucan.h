/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef _TOUCAN_H_
#define _TOUCAN_H_

#include <wx\process.h>
#include <wx\image.h>

//#include "frmmain.h"
#include "backupdata.h"
#include "rules.h"

class frmProgress;
class frmMain;
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
			
    bool RegisterProcess(PipedProcess *process);
    bool UnregisterProcess(PipedProcess *process);
	
	bool ShouldAbort() { return blAbort; }
	void SetAbort(bool abort) { blAbort = abort; }
		
	long GetPID() { return lgPID; }
	void SetPID(long pid) { lgPID = pid; }

    void OnIdle(wxIdleEvent& event);
    void OnTimer(wxTimerEvent& event);
	
	unsigned long GetBackup() { return lgBackupNumber; }
	void SetBackup(long backupnumber) { lgBackupNumber = backupnumber; }	
	
	wxString GetSettingsPath() { return strSettingsPath; }
	void SetSettingsPath(wxString settingspath) { strSettingsPath = settingspath; }
	
	frmMain* MainWindow;
	frmProgress* ProgressWindow;
	
protected:
    // Timer to wake up idle processing
    wxTimer m_wakeUpTimer;

	bool blAbort;
	long lgPID;
	unsigned long lgBackupNumber;
	wxString strSettingsPath;
	
};

DECLARE_APP(Toucan)

#endif
