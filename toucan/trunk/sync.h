#include "normalise.h"
#include "removedir.h"
#include "frmprogress.h"
#include "sync-file.h"
#include "sync-mirror.h"
#include <wx/dir.h>

bool SyncLoop(wxString strFrom, wxString strTo,  wxString strFunction, wxArrayString arrExclusions, bool blAttributes, frmProgress* window, int length1, int length2, bool blPreview)
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
	// because who knows what people will pass strTo the function.
	if (strTo[strTo.length()-1] != SLASH) {
		strTo += SLASH;       
	}
	// for both dirs
	if (strFrom[strFrom.length()-1] != SLASH) {
		strFrom += SLASH;       
	}
	if(!blPreview){
		if (!wxDirExists(strTo))
		wxMkdir(strTo);
	}
	wxDir dir(strFrom);
	wxString strFilename;
	bool blDir = dir.GetFirst(&strFilename);

	if(blDir){
		do {
			if(wxGetApp().GetStrAbort() == wxT("ABORT")){
				return false;	
			}
			if(wxDirExists(strFrom + strFilename))
			{

				// Loop to check if the folder name is one of the exclusions, if it is then blEqual is set to true.
				bool blEqual = false;
				unsigned int i;
				for(i = 0; i <arrExclusions.GetCount(); i++)
				{
					if(arrExclusions.Item(i) == strFilename){
						blEqual = true;
					}
				}
				//If blEqual is false, i.e. it is not on the list of items to be excluded then either create a folder in the destination if it doesn't exist and call itself again in the new folder, or just call itself in the new folder.
				if(blEqual == false)
				{ 
					if (wxDirExists(strTo + strFilename)){
						SyncLoop(strFrom + strFilename, strTo + strFilename, strFunction, arrExclusions, blAttributes, window, length1, length2, blPreview);
					}
					else{
						if(!blPreview){
							wxMkdir(strTo + strFilename);
						}
						SyncLoop(strFrom + strFilename, strTo + strFilename, strFunction, arrExclusions, blAttributes, window, length1, length2, blPreview);
					}
				}
			}
			else
			{
				if(wxGetApp().GetStrAbort() == wxT("ABORT")){
					return false;
				}
				SyncFile(strFrom + strFilename, strTo + strFilename, strFunction, arrExclusions, blAttributes, window, length1, length2, blPreview);
			}
		}
		while (dir.GetNext(&strFilename) );
	}  
	return true;
	
}

bool Sync(wxString strSource, wxString strDest, wxString strFunction, wxArrayString arrExclusion, bool blBox, bool blAttributes, bool blPreview)
{	
	wxGetApp().SetStrTemp(wxEmptyString);
	wxGetApp().SetStrAbort(wxEmptyString);
	
	frmProgress* window = new frmProgress(NULL, ID_FRMPROGRESS, _("Progress"));
	window->m_OK->Enable(false);
	window->m_Save->Enable(false);
	window->m_Progress_Text->SetValue(_("Starting..."));
	if(blBox){
		window->Show();
	}
	window->Update();
	
	strSource = Normalise(strSource);
	strSource = Normalise(strSource);
	strDest = Normalise(strDest);
	strDest = Normalise(strDest);
	int length1 = strSource.Length();
	int length2 = strDest.Length();   

	if(strFunction == _("Copy") || strFunction == _("Update"))
	{
		if(blPreview){
			window->SetLabel(_("Preview"));
			
			SyncLoop(strSource, strDest, strFunction, arrExclusion,  blAttributes, window,length1, length2, blPreview);
			window->m_Progress_Text->AppendText(_("\nPreview Finished..."));
			window->m_Progress_Text->SaveFile(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("PreviewLog.txt"));
			wxMessageDialog* dialog = new wxMessageDialog(NULL, _("Would you like to carry out the full operation? If you are not\n sure you can check the preview log stored in the Toucan directory"), _("Continue?"), wxYES_NO, wxDefaultPosition);
			if(dialog->ShowModal() == wxID_YES){
				window->SetLabel(_("Progress"));
				SyncLoop(strSource, strDest, strFunction, arrExclusion,  blAttributes, window,length1, length2, false);
			}
		}
		else{
			SyncLoop(strSource, strDest, strFunction, arrExclusion,  blAttributes, window,length1, length2, blPreview);
		}
	}
	else if(strFunction == _("Mirror"))
	{
		if(blPreview){
			window->SetLabel(_("Preview"));
			SyncLoop(strSource, strDest, _("Copy"), arrExclusion,  blAttributes, window,length1, length2, blPreview);
			MirrorDir(strSource,strDest, window,length1, length2, blPreview);
			window->m_Progress_Text->AppendText(_("\nPreview Finished..."));
			window->m_Progress_Text->SaveFile(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("PreviewLog.txt"));
			wxMessageDialog* dialog = new wxMessageDialog(NULL, _("Would you like to carry out the full operation? If you are not\n sure you can check the preview log stored in the Toucan directory"), _("Continue?"), wxYES_NO, wxDefaultPosition);
			if(dialog->ShowModal() == wxID_YES){
				window->SetLabel(_("Progress"));
				SyncLoop(strSource, strDest, _("Copy"), arrExclusion,  blAttributes, window,length1, length2, false);
				MirrorDir(strSource,strDest, window,length1, length2, false);
			}
		}
		//wxMessageBox(_("Finished Copy"));
		//SyncLoop( strDest, strSource, strFunction, arrExclusion,  blAttributes, window,length1, length2, blPreview);

		//wxMessageBox(_("Finished Mirror"));
	}
	else if(strFunction == _("Equalise"))
	{
        window->SetLabel(_("Preview"));
		SyncLoop(strSource, strDest, _("Update"), arrExclusion,  blAttributes, window,length1, length2, blPreview);
		//wxMessageBox(_("Finished Copy"));
		SyncLoop(strDest, strSource, _("Update"), arrExclusion,  blAttributes, window,length1, length2, blPreview);
			window->m_Progress_Text->AppendText(_("\nPreview Finished..."));
			window->m_Progress_Text->SaveFile(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("PreviewLog.txt"));
			wxMessageDialog* dialog = new wxMessageDialog(NULL, _("Would you like to carry out the full operation? If you are not\n sure you can check the preview log stored in the Toucan directory"), _("Continue?"), wxYES_NO, wxDefaultPosition);
			if(dialog->ShowModal() == wxID_YES){
				window->SetLabel(_("Progress"));
      		SyncLoop(strSource, strDest, _("Update"), arrExclusion,  blAttributes, window,length1, length2, false);
		//wxMessageBox(_("Finished Copy"));
		SyncLoop(strDest, strSource, _("Update"), arrExclusion,  blAttributes, window,length1, length2, false);
        }
        
	}
	
	window->m_Progress_Text->AppendText(_("\nFinished..."));
	window->m_OK->Enable();
	window->m_Save->Enable();
	window->m_Abort->Enable(false);

	
	return true;
}