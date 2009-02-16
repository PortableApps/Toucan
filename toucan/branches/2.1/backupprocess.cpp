/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "backupprocess.h"
#include "basicfunctions.h"

IMPLEMENT_CLASS(PipedProcess, wxProcess)

bool PipedProcess::HasInput()
{
	bool hasInput = false;
	if (IsInputAvailable()){
		if(wxGetApp().ShouldAbort()){
			wxLogNull null;
			wxProcess::Kill(this->GetRealPid(), wxSIGKILL);
		}
		wxTextInputStream tis(*GetInputStream());
		wxString msg = tis.ReadLine();
		if(msg.Left(7) == wxT("WARNING")){
			OutputProgress(wxDateTime::Now().FormatTime(), msg);
		}
		else{
			OutputProgress(msg);
		}
		//Need a window update or refresh in here
		wxMilliSleep(50);
		hasInput = true;
    	}
	return hasInput;
}

void PipedProcess::SetRealPid(long pid){
	m_PID = pid;
}

void PipedProcess::OnTerminate(int pid, int status){
	;
}
