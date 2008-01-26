#ifndef H_BACKUPPROCESS
#define H_BACKUPPROCESS

#include <wx/process.h>
#include <wx/txtstrm.h>
#include "frmprogress.h"



class BaseProcess : public wxProcess
{
    DECLARE_CLASS(BaseProcess)
public:
    BaseProcess(wxWindow* win): wxProcess(win, wxPROCESS_REDIRECT) {}

    virtual bool HasInput() = 0;
};

IMPLEMENT_CLASS(BaseProcess, wxProcess)

class PipedProcess : public BaseProcess
{
public:
    DECLARE_CLASS(PipedProcess)

    PipedProcess(frmProgress* window): BaseProcess(window), m_Window(window)
    {
        Redirect();
    }

    virtual void OnTerminate(int pid, int status);

    virtual bool HasInput();

protected:
    frmProgress*   m_Window;
    wxString          m_input; // to send to process
};

//Implement the class and define the list

IMPLEMENT_CLASS(PipedProcess, BaseProcess)
WX_DECLARE_LIST(PipedProcess, m_Processes);


bool PipedProcess::HasInput()
{
    bool hasInput = false;
   
    if ( IsInputAvailable() )
    {

        wxTextInputStream tis(*GetInputStream());
        wxString msg;
        msg = tis.ReadLine();
		m_Window->m_Text->AppendText(msg + wxT("\n"));
        hasInput = true;
    }
    return hasInput;
}

void PipedProcess::OnTerminate(int pid, int status)
{
    // show the rest of the output
    while(HasInput())
        ;
	m_Window->m_OK->Enable(true);
	m_Window->m_Save->Enable(true);
	m_Window->m_Cancel->Enable(false);
//    wxGetApp().RemoveProcess(this);
}

#endif
