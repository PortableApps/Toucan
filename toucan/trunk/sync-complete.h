#include <wx/filefn.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/progdlg.h>

bool CopyDir(wxString strFrom, wxString strTo, wxArrayString arrExclusions, bool blBox, bool blAttributes){
    #ifdef __WXMSW__
            wxString SLASH = wxT("\\");
    #else
            wxString SLASH = wxT("/");
    #endif



    // append a slash if there is not one (for easier parsing)
    // because who knows what people will pass strTo the function.
    if (strTo[strTo.length()-1] != SLASH) {
            strTo += SLASH;       
    }
    // for both dirs
    if (strFrom[strFrom.length()-1] != SLASH) {
            strFrom += SLASH;       
    }
    
    int intNumber = 0;
    wxArrayString arrFileList;
    wxDir::GetAllFiles(strFrom, & arrFileList);
    wxProgressDialog dialog( _("Progress"), _("Working"), arrFileList.GetCount(), NULL , wxPD_AUTO_HIDE);
    if(blBox == false)
    {
        //wxMessageBox(wxT("Boo"));
        dialog.Update(arrFileList.GetCount(), _("Working"));
    }
    if (!wxDirExists(strTo))
	wxMkdir(strTo);
       
    wxDir dir(strFrom);
    wxString strFilename;
    bool bla = dir.GetFirst(&strFilename);
   
    if (bla){
        do {
           
            if (wxDirExists(strFrom + strFilename) )
            {
               // wxMessageBox(strFrom + strFilename);
                bool blEqual = false;
                unsigned int i;
                for(i = 0; i <arrExclusions.GetCount(); i++)
                {
                    if(arrExclusions.Item(i) == strFilename)
                    {
                       // wxMessageBox(arrExclusions.Item(i) + wxT(" Equals ") + strFilename);
                        blEqual = true;
                    }
                    
                
                }
                if(blEqual == false)
                { 
                    if (wxDirExists(strTo + strFilename) )
                    {
                   
                       //wxMessageBox(wxT("Continuing with ") + strFrom + strFilename + wxT(" to ") + strTo + strFilename);
                        CopyDir(strFrom + strFilename, strTo + strFilename, arrExclusions, blBox, blAttributes);
                    }
    
                    
                    else
                    {
                    //wxMessageBox(wxT("Creating and continuing with ") + strFrom + strFilename + wxT(" to ") + strTo + strFilename);
                    //CopyDir(strFrom + strFilename, strTo + strFilename, arrExclusions);
                    wxMkdir(strTo + strFilename);
                    CopyDir(strFrom + strFilename, strTo + strFilename, arrExclusions, blBox, blAttributes);
                    }
                }
            }
            else{
                //wxMessageBox(wxT("Found file ") + strTo + strFilename);
                wxFileName flName(strTo + strFilename);
                unsigned int i;
                bool blEqual = false;
                for(i = 0; i <arrExclusions.GetCount(); i++)
                {
                    if(flName.GetExt() != wxEmptyString  &&  flName.GetExt() == arrExclusions.Item(i))
					{
						//wxMessageBox(flName.GetExt() + wxT(" equals ") + arrExclusions.Item(i));
						blEqual = true;
					}
                }
                if(blEqual == false)
                {
                    wxRemoveFile(strTo + strFilename);
                    wxCopyFile(strFrom + strFilename, strTo + strFilename, true);
                    if(blAttributes == true)
                    {
                        wxFileName from(strFrom + strFilename);
                        wxFileName to(strTo + strFilename);
                        wxDateTime access;
                        wxDateTime mod;
                        wxDateTime created;
                        from.GetTimes(&access ,&mod ,&created );
                        to.SetTimes(&access ,&mod , &created); 
                    wxMessageBox(_("Entered Attrib updates"));
                    }
                    dialog.Update(intNumber, _("Working"));
                    intNumber++;
                }
            }
        }
        while (dir.GetNext(&strFilename) );
    }  
    dialog.Update(arrFileList.GetCount(), _("Working"));
    return true;
   
}