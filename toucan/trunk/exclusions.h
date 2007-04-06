#include <wx/filefn.h>
#include <wx/dir.h>
#include <wx/filename.h>

bool GenerateExclusions(wxString strFrom, wxArrayString arrExclusions, bool blBox){
    #ifdef __WXMSW__
            wxString SLASH = wxT("\\");
    #else
            wxString SLASH = wxT("/");
    #endif

    // for both dirs
    if (strFrom[strFrom.length()-1] != SLASH) {
            strFrom += SLASH;       
    }
    
    int intNumber = 0;
    wxArrayString arrFileList;
    wxDir::GetAllFiles(strFrom, & arrFileList);
    wxProgressDialog dialog( _("Progress"), _("Working - Generating Exceptions"), arrFileList.GetCount(), NULL , wxPD_AUTO_HIDE);
    if(blBox == false)
    {
        //wxMessageBox(wxT("Boo"));
        dialog.Update(arrFileList.GetCount(), _("Working"));
    }
    wxDir dir(strFrom);
    wxString strFilename;
    bool bla = dir.GetFirst(&strFilename);
   
    if (bla){
        do {
           
            if (wxDirExists(strFrom + strFilename) )
            {
                unsigned int i;
                bool blEqual = false;
                for(i = 0; i <arrExclusions.GetCount(); i++)
                {
                    //wxMessageBox(strFilename);
                    if(strFilename == arrExclusions.Item(i))
                    {
                        //wxMessageBox(flName.GetExt() + wxT(" equals ") + arrExclusions.Item(i));
                        blEqual = true;
                    }
                }
                if(blEqual == false)
                {
                    GenerateExclusions(strFrom + strFilename, arrExclusions, blBox);
                }
                else
                {
                    wxString strPath = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("App") + wxFILE_SEP_PATH + wxT("temp-exclusions.txt");
                    wxTextFile file;
                    file.Open(strPath);
                    file.AddLine(strFrom + strFilename);
                    file.Write();
                }
            }
            else{
                //wxMessageBox(wxT("Found file ") + strTo + strFilename);
                wxFileName flName(strFrom + strFilename);
                unsigned int i;
                bool blEqual = false;
                for(i = 0; i <arrExclusions.GetCount(); i++)
                {
                    //wxMessageBox(flName.GetExt());
                    if(flName.GetExt() == arrExclusions.Item(i))
                    {
                        //wxMessageBox(flName.GetExt() + wxT(" equals ") + arrExclusions.Item(i));
                        blEqual = true;
                    }
                }
             
                if(blEqual == true)
                {
                    wxString strPath = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("App") + wxFILE_SEP_PATH + wxT("temp-exclusions.txt");
                    wxTextFile file;
                    file.Open(strPath);
                    file.AddLine(strFrom + strFilename);
                    file.Write();
                    dialog.Update(intNumber, _("Working - Generating Exceptions"));
                    intNumber++;

                    
                }
            }
        }
        while (dir.GetNext(&strFilename) );
    }  
    
    dialog.Update(arrFileList.GetCount(), _("Working - Generating Exceptions"));
    return true;
   
}