/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "backupprocess.h"
#include "toucan.h"

IMPLEMENT_CLASS(PipedProcess, BaseProcess)

bool PipedProcess::HasInput()
{
	bool hasInput = false;
	if ( IsInputAvailable() ){
		if(wxGetApp().ShouldAbort()){
			//wxMessageBox(_("Abort"));
			HANDLE hProcess=OpenProcess(PROCESS_ALL_ACCESS,TRUE,wxGetApp().GetPID());
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

void PipedProcess::OnTerminate(int pid, int status)
{
	// show the rest of the output
	while(HasInput())
        ;
	//Set the correct buttons
	m_Window->m_OK->Enable(true);
	m_Window->m_Save->Enable(true);
	m_Window->m_Cancel->Enable(false);
	wxDateTime now = wxDateTime::Now();
	m_Window->m_Text->AppendText(_("Time: ") + now.FormatISOTime() + wxT("\n"));
	m_Window->m_Text->AppendText(_("Finished"));
	wxGetApp().SetAbort(false);
	//Unregister the process as we are now done
	wxGetApp().UnregisterProcess(this);
}
