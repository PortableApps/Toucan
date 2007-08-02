#include "removedir.h"
#include <wx/filefn.h>

bool MirrorDir(wxString strOriginal, wxString strNew, frmProgress* window, int length1, int length2, bool blPreview)
{
    #ifdef __WXMSW__
            wxString SLASH = wxT("\\");
    #else
            wxString SLASH = wxT("/");
    #endif
    if(wxGetApp().GetStrAbort() == wxT("ABORT")){
        return false;
    }
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
            if(wxGetApp().GetStrAbort() == wxT("ABORT")){
            return false;
            }
            if (wxDirExists(strNew + strFilename)){
                if(!wxDirExists(strOriginal + strFilename)){
                    if(!blPreview){
                    RemDir(strNew + strFilename);
                    }
                }
                else{
                    MirrorDir(strOriginal+ strFilename, strNew+ strFilename,window, length1, length2, blPreview);
                }  
            }
            else{
                if(wxGetApp().GetStrAbort() == wxT("ABORT")){
                return false;
                }
                if(!wxFileExists(strOriginal + strFilename)){
                    if(!blPreview){
                    wxRemoveFile(strNew + strFilename);
                    }
                    wxString both1 = strNew + strFilename;
                    both1 = both1.Right(both1.Length() - length1);
                    window->m_Progress_Text->AppendText(_("\nRemoved \t") + both1);
                }
            }
        } while (dir.GetNext(&strFilename) );
    }  
    return true;
}