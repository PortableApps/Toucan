#include <wx/filefn.h>
#include <wx/dir.h>
#include <wx/datetime.h>
#include <wx/progdlg.h>

bool UpdateDir(wxString strFrom, wxString strTo, wxArrayString arrExclusions, bool blAttributes, frmProgress* window, int length1, int length2){

	//wxLogNull logNo;
	#ifdef __WXMSW__
	wxString SLASH = wxT("\\");
	#else
	wxString SLASH = wxT("/");
	#endif
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

	if (!wxDirExists(strTo))
	wxMkdir(strTo);
	
	wxDir dir(strFrom);
	wxString strFilename;
	bool bla = dir.GetFirst(&strFilename);
	
	if (bla)
	{
		do 
		{
			               if(wxGetApp().GetStrAbort() == wxT("ABORT"))
                {
    
                return false;
    
                }
			if (wxDirExists(strFrom + strFilename) )
			{
				//wxMessageBox(strFrom + strFilename);
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
						UpdateDir(strFrom + strFilename, strTo + strFilename, arrExclusions, blAttributes, window, length1, length2);
					}
					
					
					else
					{
						//wxMessageBox(wxT("Creating and continuing with ") + strFrom + strFilename + wxT(" to ") + strTo + strFilename);
						//CopyDir(strFrom + strFilename, strTo + strFilename, arrExclusions);
						wxMkdir(strTo + strFilename);
						UpdateDir(strFrom + strFilename, strTo + strFilename, arrExclusions, blAttributes, window, length1, length2);
					}
				}
			}
			else
			{
                           if(wxGetApp().GetStrAbort() == wxT("ABORT"))
                {
    
                return false;
    
                }
				wxFileName flName(strTo + strFilename);
				unsigned int i;
				bool blEqual = false;
				for(i = 0; i <arrExclusions.GetCount(); i++)
				{
					//wxMessageBox(flName.GetExt());
					if(flName.GetExt() != wxEmptyString  &&  wxT(".") + flName.GetExt() == arrExclusions.Item(i))
					{
						//wxMessageBox(flName.GetExt() + wxT(" equals ") + arrExclusions.Item(i));
						blEqual = true;
					}
				}
				if(blEqual == false)
				{         
					if(wxFileExists(strTo + strFilename))
					{
						// wxMessageBox(strTo + strFilename, _("Already Exists"));
						wxDateTime tmTo = wxFileModificationTime(strTo + strFilename);
						wxDateTime tmFrom = wxFileModificationTime(strFrom + strFilename);
						if(tmFrom > tmTo)
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
							//dialog.Update(intNumber, _("Working"));
							//intNumber++;
                            wxString both1 = strFrom + strFilename;
                            both1 = both1.Right(both1.Length() - length1);
                            window->m_Progress_Text->AppendText(_("\r\nOverwrote \t") + both1);
						}
					}
					else
					{
						//wxMessageBox(strTo + strFilename, _("Doesn't Exist"));
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
						//intNumber++;
					}
				}
			}
		}
		while (dir.GetNext(&strFilename) );
	}  
	return true;	
}