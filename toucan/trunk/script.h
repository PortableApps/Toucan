/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SCRIPT
#define H_SCRIPT

#include "forms/frmprogress.h"

class ScriptManager{
public:

	//Ensure the script is valid
	bool Validate();
	//Start the script
	bool Execute();
	//Runs a specific command in the script
	bool ParseCommand(int i);
	//Set up the progress bar
	bool ProgressBarSetup();

	bool CleanUp();
	bool StartUp();

	//Inline functions
	void SetCommand(int i){ m_Command = i; }
	int GetCommand(){ return m_Command; }

	int GetCount(){ return m_Script.GetCount(); }

	void SetScript(wxArrayString script){ m_Script = script; }
	wxArrayString GetScript(){ return m_Script; }

	wxDateTime GetTime(){ return startTime; }

private:
	wxArrayString m_Script;
	int m_Command;
	wxString m_Password;
	frmProgress* m_ProgressWindow;
	wxDateTime startTime;
};
		
#endif
