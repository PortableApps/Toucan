#include <wx/filefn.h>
#include <wx/dir.h>
#include <wx/datetime.h>
#include <wx/progdlg.h>

bool UpdateDir(wxString strFrom, wxString strTo, wxArrayString arrExclusions, bool blBox){

	//wxLogNull logNo;
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
	//wxMessageBox(_("Updating"));
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
	
	if (bla)
	{
		do 
		{
			
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
						UpdateDir(strFrom + strFilename, strTo + strFilename, arrExclusions, blBox);
					}
					
					
					else
					{
						//wxMessageBox(wxT("Creating and continuing with ") + strFrom + strFilename + wxT(" to ") + strTo + strFilename);
						//CopyDir(strFrom + strFilename, strTo + strFilename, arrExclusions);
						wxMkdir(strTo + strFilename);
						UpdateDir(strFrom + strFilename, strTo + strFilename, arrExclusions, blBox);
					}
				}
			}
			else
			{
				wxFileName flName(strTo + strFilename);
				unsigned int i;
				bool blEqual = false;
				for(i = 0; i <arrExclusions.GetCount(); i++)
				{
					//wxMessageBox(flName.GetExt());
					if(flName.GetExt() != wxEmptyString  &&  flName.GetExt() == arrExclusions.Item(i))
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
							dialog.Update(intNumber, _("Working"));
							intNumber++;
						}
					}
					else
					{
						//wxMessageBox(strTo + strFilename, _("Doesn't Exist"));
						wxCopyFile(strFrom + strFilename, strTo + strFilename, true);
						dialog.Update(intNumber, _("Working"));
						intNumber++;
					}
				}
			}
		}
		while (dir.GetNext(&strFilename) );
	}  
	dialog.Update(arrFileList.GetCount(), _("Working"));
	return true;
	
}