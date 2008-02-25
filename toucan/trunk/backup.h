/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2007 Steven Lamerton
// Licence:     GNU GPL 2 See readme for more info
/////////////////////////////////////////////////////////////////////////////////

#include "frmprogress.h"
#include "backup-func.h"
#include <wx/variant.h>

bool Backup(wxString strFirst, wxString strSecond, wxString strType, wxString strFormat, wxString strRatio, bool blBox, wxString strPass, bool blVisible)
{	
    //wxMessageBox(_("Called main function"));
    wxGetApp().SetStrTemp(_("Backup"));
    wxGetApp().SetStrCommand(CreateCommand(strFirst, strSecond, strType, strFormat, strRatio, strPass));
    wxGetApp().SetBlVisible(blVisible);

    //Create the progress form
    //wxMEssageBox(_("Window Created"));
    frmProgress* window = new frmProgress(NULL, ID_FRMPROGRESS, _("Progress"));
	window->m_OK->Enable(false);
    window->m_Save->Enable(false);
    window->m_Progress_Text->AppendText(_("Starting..."));
    //if(blVisible){

    window->Update();
    window->Refresh();
    if(!blBox){
        window->SetWindowStyleFlag(window->GetWindowStyleFlag() | WS_EX_TOOLWINDOW & ~wxCAPTION & ~wxRESIZE_BORDER & ~wxSYSTEM_MENU & ~wxMAXIMIZE_BOX & ~wxMINIMIZE_BOX); 
        int x;
        int y;
        wxDisplaySize(&x,&y);
        HWND m_Hndl  = (HWND) window->GetHandle(); 
        SetWindowLong(m_Hndl, GWL_EXSTYLE, (GetWindowLong(m_Hndl, GWL_EXSTYLE) | WS_EX_TOOLWINDOW) & ~WS_EX_APPWINDOW);
        window->SetSize(x + 10,y + 10, 0, 0, wxSIZE_FORCE);
    }
    window->ShowModal();
   // }
   // if(!blBox){
  //      window->Hide();
  //     DWORD dwExitCode;
  //      HANDLE hProcess=OpenProcess(PROCESS_ALL_ACCESS,TRUE,wxGetApp().GetlngPID());
  //      GetExitCodeProcess( hProcess, &dwExitCode );
  //     while(dwExitCode == STILL_ACTIVE ){
   //         GetExitCodeProcess( hProcess, &dwExitCode );
   //         wxSleep(5);
   //     }
  //  }
 //  wxMessageBox(_("Returning"));
    window->Update();
	//If the program is being run in command line mode then a log file is created
   return true;
}
