#ifndef H_BACKUPPROCESS
#define H_BACKUPPROCESS

#include <wx/process.h>
#include <wx/txtstrm.h>
#include "frmprogress.h"


class BaseProcess : public wxProcess
{
    DECLARE_CLASS(BaseProcess)
public:
    BaseProcess(wxWindow* win): wxProcess(win) {}

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
};

IMPLEMENT_CLASS(PipedProcess, BaseProcess)


bool PipedProcess::HasInput()
{
    //wxMessageBox(_("Has input"));
	bool hasInput = false;
   
    if ( IsInputAvailable() )
    {
		//wxMessageBox(_("Output"));
        wxTextInputStream tis(*GetInputStream());
        wxString msg;
        msg = tis.ReadLine();
		m_Window->m_Text->AppendText(msg + wxT("\n"));
		m_Window->Refresh();
		m_Window->Update();
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
    wxGetApp().UnregisterProcess(this);
}

#endif
