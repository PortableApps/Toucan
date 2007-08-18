/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2007 Steven Lamerton
// Licence:     GNU GPL 2 (See readme for more info
/////////////////////////////////////////////////////////////////////////////////

#include "frmprogress.h"
#include "backup-func.h"

/* This is new experimental code with the piped process code separated from frmProgress, needs extensive testing*/
bool Backup(wxString strFirst, wxString strSecond, wxString strType, wxString strFormat, wxString strRatio, bool blBox, wxString strPass)
{	
    wxGetApp().SetStrTemp(_("Backup"));
    wxGetApp().SetStrCommand(CreateCommand(strFirst, strSecond, strType, strFormat, strRatio, strPass));

    //Create the progress form
    frmProgress* window = new frmProgress(NULL, ID_FRMPROGRESS, _("Progress"));
	window->m_OK->Enable(false);
    window->m_Save->Enable(false);
    window->m_Progress_Text->AppendText(_("Starting..."));

    if(blBox){
    window->ShowModal();
    }
    window->Update();
	//If the program is being run in command line mode then a log file is created
    if(!blBox){
	//Make sure that the text control is properly updated.        
        window->Update();
        window->m_Progress_Text->Update();
        wxSleep(5);
        wxYield();
        window->m_Progress_Text->SaveFile(wxPathOnly(wxStandardPaths::Get().GetExecutablePath())+ + _T("\\Output.txt"));
    }
   return true;
}
