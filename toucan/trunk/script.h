/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SCRIPT
#define H_SCRIPT

class frmProgress;

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

	void SetCommand(const int& Command) {this->m_Command = Command;}
	void SetScript(const wxArrayString& Script) {this->m_Script = Script;}
	const int& GetCommand() const {return m_Command;}
	const wxArrayString& GetScript() const {return m_Script;}
	const wxDateTime& GetTime() const {return m_Time;}
	const int GetCount() const {return m_Script.GetCount();}

private:
	wxArrayString m_Script;
	int m_Command;
	wxString m_Password;
	frmProgress* m_ProgressWindow;
	wxDateTime m_Time;
};

#endif
