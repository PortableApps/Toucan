#include <wx/file.h>
#include <wx/filefn.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include "normalise.h"
#include "frmprogress.h"
#include "secure-file.h"

bool CryptDir(wxString strPath, wxString strPass, wxString strFunction, frmProgress* window)
{   
    if(wxGetApp().GetStrAbort() == wxT("ABORT"))
    {
    
        return false;
    
    }
    
    wxDir dir(strPath);
    wxString filename;
    bool bla = dir.GetFirst(&filename);
   
    if (bla)
	{
        do {
            if (wxDirExists(strPath + wxT("\\") + filename) )
            {
                CryptDir(strPath + wxT("\\") + filename, strPass, strFunction, window);
            }
            else
			{
                if(wxGetApp().GetStrAbort() == wxT("ABORT"))
                {
    
                return false;
    
                }
                CryptFile(strPath + wxT("\\") + filename, strPass, strFunction, window);
            }

        }

        while (dir.GetNext(&filename) );
    }   
    return true;
   
}




bool Secure(wxArrayString arrLocation, wxString strFunction, wxString strPass, bool blBox)
{  
    wxGetApp().SetStrTemp(wxEmptyString);
    wxGetApp().SetStrAbort(wxEmptyString);
    frmProgress* window = new frmProgress(NULL, ID_FRMPROGRESS, _("Progress"));
    window->m_OK->Enable(false);
    window->m_Save->Enable(false);
    window->m_Progress_Text->SetValue(_("Starting..."));
    if(blBox)
    {
    window->Show();
    }
    window->Update();
    wxGetApp().SetStrAbort(wxEmptyString);
    unsigned int i;
    for(i = 0; i < arrLocation.Count(); i++)
    {
    if(wxGetApp().GetStrAbort() == wxT("ABORT"))
    {
    
        return false;
    
    }
    arrLocation.Item(i) = Normalise(arrLocation.Item(i));
    arrLocation.Item(i) = Normalise(arrLocation.Item(i));
      //wxMessageBox(arrLocation.Item(i));
       if(wxDirExists(arrLocation.Item(i)))
        {

            //wxMessageBox(wxT("Crypt Dir ") + arrLocation.Item(i));
            CryptDir(arrLocation.Item(i), strPass, strFunction, window);
        }
        else
        {


            //wxMessageBox(wxT("Crypt File ") + arrLocation.Item(i));
            CryptFile(arrLocation.Item(i), strPass, strFunction, window);
        }
    }
    window->m_Progress_Text->AppendText(_("\nFinished..."));
    window->m_OK->Enable(true);
    window->m_Save->Enable(true);
    window->m_Abort->Enable(false);

return true;

}

