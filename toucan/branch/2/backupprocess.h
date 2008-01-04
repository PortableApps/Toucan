class MyPipedProcess : public wxThread
{
public:
    MyPipedProcess(frmProgress *parent, const wxString& cmd)
	{
		Redirect();
	}

    virtual void OnTerminate(int pid, int status);

    virtual bool HasInput();
};


bool MyPipedProcess::HasInput()
{
    bool hasInput = false;
   
    if ( IsInputAvailable() )
    {

        wxTextInputStream tis(*GetInputStream());
        // this assumes that the output is always line buffered
        wxString msg;
        msg = tis.ReadLine();
        //wxMessageBox(_("has input"));
        if(wxGetApp().GetBlVisible()){
            m_parent->m_Progress_Text->AppendText(msg + wxT("\n"));
            wxMilliSleep(50);
        }
        else{
           newcout<<msg + wxT("\n");
        }
        hasInput = true;
    }

    if ( IsErrorAvailable() )
    {
        wxTextInputStream tis(*GetErrorStream());
        // this assumes that the output is always line buffered
        wxString msg;
        msg = tis.ReadLine();
        if(wxGetApp().GetBlVisible()){
            m_parent->m_Progress_Text->AppendText(msg + wxT("\n"));
            wxMilliSleep(50);
        }
        else{
            newcout<<msg + wxT("\n");
        }
        hasInput = true;
    }
    return hasInput;
}

void MyPipedProcess::OnTerminate(int pid, int status)
{
    // show the rest of the output
    while(HasInput())
        ;

    m_parent->OnProcessTerminated(this);

    //MyProcess::OnTerminate(pid, status);
    if(wxGetApp().GetStrAbort() == wxT("ABORT")){
        m_parent->m_Progress_Text->AppendText(_("Aborted...\n"));
    }
    m_parent->m_OK->Enable(true);
    m_parent->m_Save->Enable(true);
    m_parent->m_Abort->Enable(false);
    //wxMessageBox(_("Abort code"));

    if(!wxGetApp().GetBlVisible()){
        m_parent->EndModal(wxID_OK);
    }
}