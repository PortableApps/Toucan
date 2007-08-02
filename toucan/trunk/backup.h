#include "frmprogress.h"
#include <wx/snglinst.h>
bool Backup(wxString strFirst, wxString strSecond, wxString strType, wxString strFormat, wxString strRatio, bool blBox)
{	
    wxGetApp().SetStrTemp(_("Backup"));
    wxGetApp().SetStrType(strType);
    wxGetApp().SetStrFormat(strFormat);
    wxGetApp().SetStrRatio(strRatio);
    wxGetApp().SetStrSecond(strSecond);
    wxGetApp().SetStrFirst(strFirst);
    
    frmProgress* window = new frmProgress(NULL, ID_FRMPROGRESS, _("Progress"));
    window->m_OK->Enable(false);
    window->m_Save->Enable(false);
    window->m_Progress_Text->AppendText(_("Starting..."));
    if(blBox)
    {
    window->ShowModal();
    }
    window->Update();
    if(!blBox)
    {
    DWORD dwExitCode;
    HANDLE hProcess=OpenProcess(PROCESS_ALL_ACCESS,TRUE,wxGetApp().GetlngPID());
    GetExitCodeProcess( hProcess, &dwExitCode );
     while(dwExitCode == STILL_ACTIVE )
     {
     
         GetExitCodeProcess( hProcess, &dwExitCode );
                    //wxMessageBox(_("Still running"));
                    wxSleep(5);
     }
       
    
    }
     //window->Show();
       // window->Show(false);
       if(!blBox)
       {
        window->Update();
        window->m_Progress_Text->Update();
       // wxMessageBox(_("boo"));
       wxSleep(5);
       wxYield();
        window->m_Progress_Text->SaveFile(wxPathOnly(wxStandardPaths::Get().GetExecutablePath())+ + _T("\\Output.txt"));
   }
   return true;

}
