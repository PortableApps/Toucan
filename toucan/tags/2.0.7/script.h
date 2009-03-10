/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SCRIPT
#define H_SCRIPT

#include "frmprogress.h"

class ScriptManager{
	public:
	
	//Get and set the internal script
	void SetScript(wxArrayString script);
	wxArrayString GetScript();
	
	//The number of commands in the script
	int GetCount();
	
	//Get or set the current command
	void SetCommand(int i);
	int GetCommand();
	
	//Get the start time
	wxDateTime GetTime();
	
	//Ensure the script is valid
	bool Validate();
	
	//Start the script
	bool Execute();
	
	//Runs a speciic command in the script
	bool ParseCommand(int i);
	
	//Clean up when finished
	bool CleanUp();
	bool StartUp();
	
	private:
	
	wxArrayString m_Script;
	int m_Command;
	wxString m_Password;
	frmProgress* m_ProgressWindow;
	wxDateTime startTime;
	
};
		
#endif
