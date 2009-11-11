/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "../toucan.h"
#include "../basicfunctions.h"
#include "backupprocess.h"
#include <wx/log.h>
#include <wx/txtstrm.h>

IMPLEMENT_CLASS(BackupProcess, wxProcess)

bool BackupProcess::HasInput()
{
	bool hasInput = false;
	if (IsInputAvailable()){
		if(wxGetApp().GetAbort()){
			wxLogNull null;
			wxProcess::Kill(this->GetRealPid(), wxSIGKILL);
		}
		wxTextInputStream tis(*GetInputStream());
		wxString msg = tis.ReadLine();
		if(msg.Left(7).Lower() == wxT("warning") || msg.Left(5).Lower() == wxT("error")){
			OutputProgress(msg, true, true);
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

void BackupProcess::OnTerminate(int WXUNUSED(pid), int WXUNUSED(status)){
	;
}
