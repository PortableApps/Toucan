/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2007 Steven Lamerton
// Licence:     GNU GPL 2 (See readme for more info
/////////////////////////////////////////////////////////////////////////////////

#include "normalise.h"
#include "frmprogress.h"
#include "sync-file.h"
#include "sync-mirror.h"
#include <wx/dir.h>
#include <wx/log.h>

/*The main SyncLoop is initially called from the Sync function, and calls itself when it reaches a folder and calls SyncFille when a file is reached*/
bool SyncLoop(wxString strFrom, wxString strTo,  wxString strFunction, wxArrayString arrExclusions, bool blAttributes, frmProgress* window, int length1, int length2, bool blPreview)
{
	if(wxGetApp().GetStrAbort() == wxT("ABORT")){
		return false;
	}
	//Append a slash in case of an incorrect pass
	if (strTo[strTo.length()-1] != wxFILE_SEP_PATH) {
		strTo += wxFILE_SEP_PATH;       
	}
	// for both dirs
	if (strFrom[strFrom.length()-1] != wxFILE_SEP_PATH) {
		strFrom += wxFILE_SEP_PATH;       
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

//The main Sync function simply calls SyncLoop with the correct arguments based on the users request
bool Sync(wxString strSource, wxString strDest, wxString strFunction, wxArrayString arrExclusion, bool blBox, bool blAttributes, bool blPreview)
{	
	wxGetApp().SetStrTemp(wxEmptyString);
	wxGetApp().SetStrAbort(wxEmptyString);
	
	//Create the progress form and redirect the log output to it via cout	
	frmProgress* window = new frmProgress(NULL, ID_FRMPROGRESS, _("Progress"));
    /*wxLog *logger=new wxLogStream(&cout);
    wxLog::SetActiveTarget(logger);
    wxStreamToTextRedirector redirect(window->m_Progress_Text);	*/
	window->m_OK->Enable(false);
	window->m_Save->Enable(false);
	window->m_Progress_Text->SetValue(_("Starting..."));
	if(blBox){
		window->Show();
	}
	window->Update();
	
	//Normalise the filenames, twice to allow for double depth
	strSource = Normalise(strSource);
	strSource = Normalise(strSource);
	if(strSource == wxEmptyString){
		wxMessageBox(_("There was an error processing the source Portable Variable."), _("Error"), wxICON_ERROR);
		return false;
	}
	strDest = Normalise(strDest);
	strDest = Normalise(strDest);
	if(strDest == wxEmptyString){
		wxMessageBox(_("There was an error processing the destination Portable Variable."), _("Error"), wxICON_ERROR);
		return false;
	}
	//Set up the length variables for the shortening of filenames 
	int length1 = strSource.Length();
	int length2 = strDest.Length();   

	if(strFunction == _("Copy") || strFunction == _("Update")){
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
	else if(strFunction == _("Mirror")){
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
        		else{
            			SyncLoop(strSource, strDest, _("Copy"), arrExclusion,  blAttributes, window,length1, length2, false);
            			MirrorDir(strSource,strDest, window,length1, length2, false);
            		}
	}
	else if(strFunction == _("Equalise")){
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
	if(!blBox){
        		window->Update();
        		window->m_Progress_Text->Update();
      		// wxMessageBox(_("boo"));
       		wxSleep(5);
       		wxYield();
        		window->m_Progress_Text->SaveFile(wxPathOnly(wxStandardPaths::Get().GetExecutablePath())+ + _T("\\Output.txt"));
   	}
	return true;
}