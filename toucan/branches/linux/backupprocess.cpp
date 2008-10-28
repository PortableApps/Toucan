<<<<<<< .working
/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "backupprocess.h"
#include "toucan.h"
#include "backupdata.h"
#include "basicfunctions.h"
#include <wx/textfile.h>

IMPLEMENT_CLASS(BaseProcess, wxProcess)
IMPLEMENT_CLASS(PipedProcess, BaseProcess)

bool PipedProcess::HasInput()
{
	bool hasInput = false;
	if (IsInputAvailable()){
		if(wxGetApp().ShouldAbort()){
			#ifdef __WXMSW__
				HANDLE hProcess=OpenProcess(PROCESS_ALL_ACCESS,TRUE,this->GetRealPid());
				TerminateProcess(hProcess,0);
			#endif
		}
		wxTextInputStream tis(*GetInputStream());
		wxString msg = tis.ReadLine();
		//Need to change this to OutputProgress
		OutputProgress(msg);
		//Need a window update or refresh in here
		wxMilliSleep(50);
		hasInput = true;
    	}
	return hasInput;
}

void PipedProcess::SetRealPid(long pid){
	m_PID = pid;
}
=======
/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "backupprocess.h"
#include "toucan.h"
#include "backupdata.h"
#include "basicfunctions.h"
#include <wx/textfile.h>

IMPLEMENT_CLASS(BaseProcess, wxProcess)
IMPLEMENT_CLASS(PipedProcess, BaseProcess)

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
		OutputProgress(msg);
		//Need a window update or refresh in here
		wxMilliSleep(50);
		hasInput = true;
    	}
	return hasInput;
}

void PipedProcess::SetRealPid(long pid){
	m_PID = pid;
}
>>>>>>> .merge-right.r223
