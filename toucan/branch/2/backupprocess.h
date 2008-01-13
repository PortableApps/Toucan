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

    /// Feed it some input
    void SendInput(const wxString& text);

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
		wxMessageBox(msg);
        hasInput = true;
    }

    if ( IsErrorAvailable() )
    {
        wxTextInputStream tis(*GetErrorStream());
        // this assumes that the output is always line buffered
        wxString msg;
        msg = tis.ReadLine();
        hasInput = true;
    }
    return hasInput;
}

void PipedProcess::OnTerminate(int pid, int status)
{
    // show the rest of the output
    while(HasInput())
        ;

//    wxGetApp().RemoveProcess(this);
}

#endif
