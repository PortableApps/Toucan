#include "removedir.h"

#include <wx/filefn.h>

bool MirrorDir(wxString strOriginal, wxString strNew, frmProgress* window, int length1, int length2)
{
    #ifdef __WXMSW__
            wxString SLASH = wxT("\\");
    #else
            wxString SLASH = wxT("/");
    #endif

    // append a slash if there is not one (for easier parsing)
    // because who knows what people will pass strOriginal the function.
    if (strOriginal[strOriginal.length()-1] != SLASH) {
            strOriginal += SLASH;       
    }
    // for both dirs
    if (strNew[strNew.length()-1] != SLASH) {
            strNew += SLASH;       
    }

    wxDir dir(strNew);
    wxString strFilename;
    bool bla = dir.GetFirst(&strFilename);
   
    if (bla){
        do {
           
            if (wxDirExists(strNew + strFilename) )
            {
                wxMessageBox(strNew + strFilename);
                if(!wxDirExists(strOriginal + strFilename))
                {
                    RemDir(strNew + strFilename);
                }
                else
                {
                    MirrorDir(strOriginal+ strFilename, strNew+ strFilename,window, length1, length2);
                }
               
            }
            else
            {
                wxMessageBox(strOriginal + strFilename);
                if(!wxFileExists(strOriginal + strFilename))
                {
                    wxRemoveFile(strNew + strFilename);
                    wxString both1 = strNew + strFilename;
                    both1 = both1.Right(both1.Length() - length1);
                    window->m_Progress_Text->AppendText(_("\r\nRemoved \t") + both1);
                }
            }
        } while (dir.GetNext(&strFilename) );
    }  
    return true;
}