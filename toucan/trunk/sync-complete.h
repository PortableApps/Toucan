#include <wx/filefn.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/progdlg.h>

#include "frmprogress.h"

bool CopyDir(wxString strFrom, wxString strTo, wxArrayString arrExclusions, bool blAttributes, frmProgress* window, int length1, int length2){
    #ifdef __WXMSW__
            wxString SLASH = wxT("\\");
    #else
            wxString SLASH = wxT("/");
    #endif
   // wxMessageBox(wxGetApp().GetStrAbort());
    if(wxGetApp().GetStrAbort() == wxT("ABORT"))
    {
    
        return false;
    
    }
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
   
    if (!wxDirExists(strTo))
	wxMkdir(strTo);
       
    wxDir dir(strFrom);
    wxString strFilename;
    bool bla = dir.GetFirst(&strFilename);
   
    if (bla){
        do {
               if(wxGetApp().GetStrAbort() == wxT("ABORT"))
                {
    
                return false;
    
                }
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
                        CopyDir(strFrom + strFilename, strTo + strFilename, arrExclusions, blAttributes, window, length1, length2);
                    }
    
                    
                    else
                    {
                    //wxMessageBox(wxT("Creating and continuing with ") + strFrom + strFilename + wxT(" to ") + strTo + strFilename);
                    //CopyDir(strFrom + strFilename, strTo + strFilename, arrExclusions);
                    wxMkdir(strTo + strFilename);
                    CopyDir(strFrom + strFilename, strTo + strFilename, arrExclusions, blAttributes, window, length1, length2);
                    }
                }
            }
            else{
                           if(wxGetApp().GetStrAbort() == wxT("ABORT"))
                {
    
                return false;
    
                }
                //wxMessageBox(wxT("Found file ") + strTo + strFilename);
                wxFileName flName(strTo + strFilename);
                unsigned int i;
                bool blEqual = false;
                for(i = 0; i <arrExclusions.GetCount(); i++)
                {
                    if(flName.GetExt() != wxEmptyString  &&  wxT(".") + flName.GetExt() == arrExclusions.Item(i))
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
                        int filearrtibs = GetFileAttributes(strFrom + strFilename);
                        SetFileAttributes(strTo + strFilename,FILE_ATTRIBUTE_NORMAL);                       
                        wxDateTime access;
                        wxDateTime mod;
                        wxDateTime created;
                        from.GetTimes(&access ,&mod ,&created );
                        to.SetTimes(&access ,&mod , &created); 
                        SetFileAttributes(strTo + strFilename,filearrtibs);
                                // wxMessageBox(_("Entered Attrib updates"));
                    }
                    wxString both1 = strFrom + strFilename;
                    both1 = both1.Right(both1.Length() - length1);
                    
                    window->m_Progress_Text->AppendText(_("\r\nCopied \t") + both1);
                    //wxMilliSleep(50);
                    wxGetApp().Yield(true);
                }
            }
            
        }
        while (dir.GetNext(&strFilename) );
    }  
    return true;
   
}