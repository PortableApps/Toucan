#include "removedir.h"

#include <wx/filefn.h>

bool MirrorDir(wxString strOriginal, wxString strNew, bool blBox)
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
    
    int intNumber = 0;
    wxArrayString arrFileList;
    wxDir::GetAllFiles(strNew, & arrFileList);
    wxProgressDialog dialog( _("Progress"), _("Working"), arrFileList.GetCount(), NULL , wxPD_AUTO_HIDE);
    if(blBox == false)
    {
        dialog.Update(arrFileList.GetCount(), _("Working"));
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
                    MirrorDir(strOriginal+ strFilename, strNew+ strFilename, blBox);
                }
               
            }
            else
            {
                wxMessageBox(strOriginal + strFilename);
                if(!wxFileExists(strOriginal + strFilename))
                {
                    wxRemoveFile(strNew + strFilename);
                }
            }
        } while (dir.GetNext(&strFilename) );
    }  
    dialog.Update(arrFileList.GetCount(), _("Working"));
    return true;
   




}