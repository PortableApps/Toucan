/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007 - 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include "../toucan.h"
#include "../basicfunctions.h"
#include "backupprocess.h"
#include <wx/txtstrm.h>

bool BackupProcess::HasInput()
{
	bool hasInput = false;
	if (IsInputAvailable()){
		if(wxGetApp().GetAbort()){
			wxLogNull null;
			wxProcess::Kill(this->GetRealPid(), wxSIGKILL);
		}
		wxTextInputStream tis(*GetInputStream(), "\x0009", wxConvUTF8);
		wxString msg = tis.ReadLine();
		if(msg.Left(7).Lower() == wxT("warning") || msg.Left(5).Lower() == wxT("error")){
            OutputProgress(msg, Error);
		}
		else{
            OutputProgress(msg, Message);
		}
		hasInput = true;
	}
	return hasInput;
}
