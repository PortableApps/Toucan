#include <wx/file.h>
#include <wx/filefn.h>
#include <wx/dir.h>
#include "cryptfile.h"
#include "cryptdir.h"
#include "normalise.h"
#include "frmprogress.h"

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
    window->m_Progress_Text->AppendText(_("\nFinished"));
    window->m_OK->Enable(true);
    window->m_Save->Enable(true);
    window->m_Abort->Enable(false);

return true;

}