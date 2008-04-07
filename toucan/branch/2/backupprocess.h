/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_BACKUPPROCESS
#define H_BACKUPPROCESS

#include <wx/process.h>
#include <wx/txtstrm.h>
#include "frmprogress.h"

//The base process
class BaseProcess : public wxProcess
{
    DECLARE_CLASS(BaseProcess)
public:
    BaseProcess(wxWindow* win): wxProcess(win) {}

};

IMPLEMENT_CLASS(BaseProcess, wxProcess)

//The actual piped process, this could be merged with the base process
class PipedProcess : public BaseProcess
{
public:
    DECLARE_CLASS(PipedProcess)

    PipedProcess(frmProgress* window): BaseProcess(window), m_Window(window)
    {
	//Redirect the process input and output
        Redirect();
    }

    virtual void OnTerminate(int pid, int status);

    virtual bool HasInput();

protected:
    frmProgress*   m_Window;
};

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

#endif
