/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "backupfunctions.h"
#include "backupprocess.h"
#include "toucan.h"
#include "backupdata.h"
#include <wx/textfile.h>

bool PipedProcess::HasInput()
{
	bool hasInput = false;
	if (IsInputAvailable()){
		if(wxGetApp().ShouldAbort()){
			HANDLE hProcess=OpenProcess(PROCESS_ALL_ACCESS,TRUE,this->GetRealPid());
			TerminateProcess(hProcess,0);
		}
		wxTextInputStream tis(*GetInputStream());
		wxString msg = tis.ReadLine();
		//Need to change this to OutputProgress
		m_Window->m_Text->AppendText(msg + wxT("\n"));
		//Need a window update or refresh in here
		wxMilliSleep(50);
		hasInput = true;
    	}
	return hasInput;
}

void PipedProcess::SetRealPid(long pid){
	m_PID = pid;
}
