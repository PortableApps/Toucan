#include <wx/filefn.h>
#include <wx/dir.h>
#include <wx/datetime.h>
#include <wx/filename.h>

bool SyncFile(wxString strFrom, wxString strTo, wxString strFunction, wxArrayString arrExclusions, bool blAttributes, frmProgress* window, int length1, int length2, bool blPreview)
{
	wxFileName flName(strTo);
	unsigned int i;
	bool blEqual = false;
	for(i = 0; i <arrExclusions.GetCount(); i++)
	{
		if(flName.GetExt() != wxEmptyString  &&  wxT(".") + flName.GetExt() == arrExclusions.Item(i))
		{
			blEqual = true;
		}
	}
	if(blEqual == false)
	{
		if(blEqual == false)
		{
			if(strFunction == _("Copy"))
			{
				if(!blPreview){
                wxRemoveFile(strTo);
				wxCopyFile(strFrom, strTo, true);
                }
				wxString both1 = strFrom ;
				both1 = both1.Right(both1.Length() - length1);
				window->m_Progress_Text->AppendText(_("\nCopied \t") + both1);
			}
			if(strFunction == _("Update"))
			{
				if(wxFileExists(strTo))
				{
					wxDateTime tmTo = wxFileModificationTime(strTo);
					wxDateTime tmFrom = wxFileModificationTime(strFrom);
					if(tmFrom > tmTo)
					{
						if(!blPreview){
                        wxRemoveFile(strTo);
						wxCopyFile(strFrom, strTo, true);
                        }
						wxString both1 = strTo;
						both1 = both1.Right(both1.Length() - length2);
						window->m_Progress_Text->AppendText(_("\nOverwrote \t") + both1);
					}
				}
				else
				{
                    if(!blPreview){
					wxCopyFile(strFrom, strTo, true);
                    }
					wxString both1 = strFrom ;
					both1 = both1.Right(both1.Length() - length1);
					window->m_Progress_Text->AppendText(_("\nCopied \t") + both1);
				}
			}
			if(blAttributes == true)
			{
				if(!blPreview){
                wxFileName from(strFrom);
				wxFileName to(strTo);
				int filearrtibs = GetFileAttributes(strFrom);
				SetFileAttributes(strTo,FILE_ATTRIBUTE_NORMAL);                       
				wxDateTime access;
				wxDateTime mod;
				wxDateTime created;
				from.GetTimes(&access ,&mod ,&created );
				to.SetTimes(&access ,&mod , &created); 
				SetFileAttributes(strTo,filearrtibs);
                }
			}
			wxGetApp().Yield(true);			
		}
    }
    return true;
}