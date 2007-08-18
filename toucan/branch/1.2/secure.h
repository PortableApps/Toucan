/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2007 Steven Lamerton
// Licence:     GNU GPL 2 (See readme for more info
/////////////////////////////////////////////////////////////////////////////////

#include <wx/file.h>
#include <wx/filefn.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include "normalise.h"
#include "frmprogress.h"
#include "secure-file.h"

/*The main loop for the Secure process. It is called by Secure initially and then either calls itself when it reaches a
folder or CryptFile when it reaches a file.*/
bool CryptDir(wxString strPath, wxString strPass, wxString strFunction, frmProgress* window, wxString strRoutine)
{   
    if(wxGetApp().GetStrAbort() == wxT("ABORT")){
        return false;
    }
    
    wxDir dir(strPath);
    wxString filename;
    bool blDir = dir.GetFirst(&filename);
    if (blDir)
	{
        do {
            if (wxDirExists(strPath + wxT("\\") + filename) )
            {
                CryptDir(strPath + wxT("\\") + filename, strPass, strFunction, window, strRoutine);
            }
            else
			{
                if(wxGetApp().GetStrAbort() == wxT("ABORT")){
                return false;
                }
                CryptFile(strPath + wxT("\\") + filename, strPass, strFunction, window, strRoutine);
            }

        }

        while (dir.GetNext(&filename) );
    }   
    return true;
   
}



/*The main Secure function is very simple in that it iterates through the names pass to it and fires off the correct
function, eithrr CryptDir or CryptFile*/
bool Secure(wxArrayString arrLocation, wxString strFunction, wxString strPass, bool blBox, wxString strRoutine)
{  
    wxGetApp().SetStrTemp(wxEmptyString);
    wxGetApp().SetStrAbort(wxEmptyString);
    frmProgress* window = new frmProgress(NULL, ID_FRMPROGRESS, _("Progress"));
	//Redirect log messages to the text control by way of cout
    /*wxLog *logger=new wxLogStream(&cout);
    wxLog::SetActiveTarget(logger);
    wxStreamToTextRedirector redirect(window->m_Progress_Text);  */ 
	//Set the correct buttons on the form 
    window->m_OK->Enable(false);
    window->m_Save->Enable(false);
    window->m_Progress_Text->SetValue(_("Starting..."));
    if(blBox){
    window->Show();
    }
    window->Update();
    wxGetApp().SetStrAbort(wxEmptyString);
    unsigned int i;
	//Iterate through the entries in the array
    for(i = 0; i < arrLocation.Count(); i++)
    {
    if(wxGetApp().GetStrAbort() == wxT("ABORT")){
        return false;
    }
	//Normalise the entry name, making sure that the function passes back a value.
    arrLocation.Item(i) = Normalise(arrLocation.Item(i));
    arrLocation.Item(i) = Normalise(arrLocation.Item(i));
	if(arrLocation.Item(i) != wxEmptyString){

      //wxMessageBox(arrLocation.Item(i));
       if(wxDirExists(arrLocation.Item(i)))
        {

            //wxMessageBox(wxT("Crypt Dir ") + arrLocation.Item(i));
            CryptDir(arrLocation.Item(i), strPass, strFunction, window, strRoutine);
        }
        else
        {


            //wxMessageBox(wxT("Crypt File ") + arrLocation.Item(i));
            CryptFile(arrLocation.Item(i), strPass, strFunction, window, strRoutine);
        }
}
    }
    window->m_Progress_Text->AppendText(_("\nFinished..."));
    window->m_OK->Enable(true);
    window->m_Save->Enable(true);
    window->m_Abort->Enable(false);
	//Write a log file when in command line mode
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

