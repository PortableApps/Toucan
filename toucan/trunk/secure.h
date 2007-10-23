/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2007 Steven Lamerton
// Licence:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include <wx/file.h>
#include <wx/filefn.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/log.h>
#include "normalise.h"
#include "frmprogress.h"
#include "secure-file.h"

/*The main loop for the Secure process. It is called by Secure initially and then either calls itself when it reaches a
folder or CryptFile when it reaches a file.*/
bool CryptDir(wxString strPath, wxString strPass, wxString strFunction, frmProgress* window, wxString strRoutine, bool blVisible)
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
            if (wxDirExists(strPath + wxT("\\") + filename) ){
                CryptDir(strPath + wxT("\\") + filename, strPass, strFunction, window, strRoutine, blVisible);
            }
            else{
                if(wxGetApp().GetStrAbort() == wxT("ABORT")){
                    return false;
                }
                CryptFile(strPath + wxT("\\") + filename, strPass, strFunction, window, strRoutine, blVisible);
            }

        }

        while (dir.GetNext(&filename) );
    }   
    return true;
   
}



/*The main Secure function is very simple in that it iterates through the names pass to it and fires off the correct
function, eithrr CryptDir or CryptFile*/
bool Secure(wxArrayString arrLocation, wxString strFunction, wxString strPass, bool blVisible, wxString strRoutine)
{  
    
    wxGetApp().SetStrTemp(wxEmptyString);
    wxGetApp().SetStrAbort(wxEmptyString);
    frmProgress* window = new frmProgress(NULL, ID_FRMPROGRESS, _("Progress"));
	//Redirect log messages to the text control
    wxLogTextCtrl* logTxt = new wxLogTextCtrl(window->m_Progress_Text);
    delete wxLog::SetActiveTarget(logTxt);
	//Set the correct buttons on the form 
    window->m_OK->Enable(false);
    window->m_Save->Enable(false);
    OutputProgress(_("Starting..."), window, blVisible);
    //If we are running in GUI mode we show the form
    if(blVisible){
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
            if(wxDirExists(arrLocation.Item(i))){
                CryptDir(arrLocation.Item(i), strPass, strFunction, window, strRoutine, blVisible);
            }
            else{
                CryptFile(arrLocation.Item(i), strPass, strFunction, window, strRoutine, blVisible);
            }
        }
    }
    OutputProgress(_("Finished..."), window, blVisible);
    window->m_OK->Enable(true);
    window->m_Save->Enable(true);
    window->m_Abort->Enable(false);
    
    return true;

}

