/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "backupprocess.h"
#include "basicfunctions.h"
#include "toucan.h"

IMPLEMENT_CLASS(PipedProcess, wxProcess)

bool PipedProcess::HasInput()
{
	bool hasInput = false;
	if (IsInputAvailable()){
		if(wxGetApp().GetAbort()){
			wxLogNull null;
			wxProcess::Kill(this->GetRealPid(), wxSIGKILL);
		}
		wxTextInputStream tis(*GetInputStream());
		wxString msg = tis.ReadLine();
		if(msg.Left(7) == wxT("WARNING")){
			OutputProgress(msg, wxDateTime::Now().FormatTime(), true);
		}
		else{
			OutputProgress(msg);
		}
		IncrementGauge();
		//Need a window update or refresh in here
		wxMilliSleep(50);
		hasInput = true;
    	}
	return hasInput;
}

void PipedProcess::OnTerminate(int WXUNUSED(pid), int WXUNUSED(status)){
	;
}
