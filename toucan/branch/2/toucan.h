/////////////////////////////////////////////////////////////////////////////
// Name:        toucan.h
// Purpose:     
// Author:      Steven Lamerton
// Modified by: 
// Created:     28/09/2007 20:20:52
// RCS-ID:      
// Copyright:   Copyright (c) Steven Lamerton 2006-2007
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _TOUCAN_H_
#define _TOUCAN_H_

/*!
* Includes
*/

////@begin includes
#include "wx/image.h"
#include "frmmain.h"
////@end includes

#include <wx\process.h>

/*!
* Forward declarations
*/

////@begin forward declarations
////@end forward declarations


//WX_DEFINE_LIST(m_processes);

class PipedProcess;
WX_DEFINE_ARRAY_PTR(PipedProcess *, ProcessArray);


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

	void Init();

	/// Initialises the application
	virtual bool OnInit();

	/// Called on exit
	virtual int OnExit();

	////@begin Toucan event handler declarations

	////@end Toucan event handler declarations

	////@begin Toucan member function declarations

	wxArrayString GetSecureLocations() { return SecureLocations ; }
	void SetSecureLocations(wxArrayString value) { SecureLocations = value ; }
	void AppendSecureLocation (wxString string) { SecureLocations.Add(string) ;}
	void RemoveSecureLocation (int pos) { SecureLocations.RemoveAt(pos) ;}



	////@end Toucan member function declarations

	wxArrayString GetBackupLocations() { return BackupLocations ; }
	void SetBackupLocations(wxArrayString value) { BackupLocations = value ; }
	void AppendBackupLocation (wxString string) { BackupLocations.Add(string) ;}
	void RemoveBackupLocation (int pos) { BackupLocations.RemoveAt(pos) ;}
	
	//void SetProcess(wxProcess process) { m_Process = process; }
	
	////@begin Toucan member variables
	wxArrayString SecureLocations;
	////@end Toucan member variables
	wxArrayString BackupLocations;
	
	ProcessArray m_Running;
	
    //bool HandleProcessInput();

    /// Register a process with the application, and start
    /// idle timer if necessary
    bool RegisterProcess(PipedProcess *process);

    /// UnregisterProcess
    bool UnregisterProcess(PipedProcess *process);

    /// Are there processes?
    //bool HasProcesses() { return m_Processes.GetCount() > 0; }

    /// Handle idle events
    void OnIdle(wxIdleEvent& event);
	
	//Handle timer events
    void OnTimer(wxTimerEvent& event);
	

	
protected:
    // List of processes that the app knows about
    //wxList m_Processes;

    // Timer to wake up idle processing
    wxTimer m_wakeUpTimer;
	
};

/*!
* Application instance declaration 
*/

////@begin declare app
DECLARE_APP(Toucan)
////@end declare app

#endif
// _TOUCAN_H_
