/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2007 Steven Lamerton
// Licence:     GNU GPL 2 (See readme for more info
/////////////////////////////////////////////////////////////////////////////////

#include "normalise.h"
#include "frmprogress.h"
#include "sync-file.h"
#include "sync-mirror.h"
#include "basicops.h"
#include <wx/dir.h>
#include <wx/log.h>

/*The main SyncLoop is initially called from the Sync function, and calls itself when it reaches a folder and calls SyncFille when a file is reached*/
bool SyncLoop(SyncData data, Rules rules)
{
	if(wxGetApp().GetStrAbort() == wxT("ABORT")){
		return false;
	}
	wxString strTo = data.GetDest();
	wxString strFrom = data.GetSource();
	//This section need to be updated for the new data stuff
	//Append a slash in case of an incorrect pass
	if (strTo[strTo.length()-1] != wxFILE_SEP_PATH) {
		strTo += wxFILE_SEP_PATH;       
	}
	// for both dirs
	if (strFrom[strFrom.length()-1] != wxFILE_SEP_PATH) {
		strFrom += wxFILE_SEP_PATH;       
	}
	if (!wxDirExists(strTo)){
            		if(!rules.Matches(strTo, true)){
                		wxMkdir(strTo);
		}
		else{
			return false;
		}
	}
	wxDir dir(strFrom);
	wxString strFilename;
	bool blDir = dir.GetFirst(&strFilename);
	if(blDir){
		do {
			if(wxDirExists(strFrom + strFilename))
			{
				if(!rules.Matches(strTo + strFilename, true))
				{ 
					if (wxDirExists(strTo + strFilename)){
						data.SetSource(strFrom + strFilename);
						data.SetDest(strTo + strFilename);
						SyncLoop(data, rules);
					}
					else{
						if(data.GetFunction == _("Mirror")){
							RemoveDirectory(strTo + strFilename);
						}
						else{
							wxMkdir(strTo + strFilename);
							data.SetSource(strFrom + strFilename);
							data.SetDest(strTo + strFilename);
							SyncLoop(data, rules);
						}
					}
				}
			}
			else{
				data.SetSource(strFrom + strFilename);
				data.SetDest(strTo + strFilename);
				SyncFile(data, rules);
			}
		}
		while (dir.GetNext(&strFilename) );
	}  
	return true;
	
}

//The main Sync function simply calls SyncLoop with the correct arguments based on the users request
bool Sync(SyncData data, Rules rules)
{	
	wxGetApp().SetStrTemp(wxEmptyString);
	wxGetApp().SetStrAbort(wxEmptyString);
	
	//Create the progress form and redirect the log output to it	
	frmProgress* window = new frmProgress(NULL, ID_FRMPROGRESS, _("Progress"));
    	wxLogTextCtrl* logTxt = new wxLogTextCtrl(window->m_Progress_Text);
    	delete wxLog::SetActiveTarget(logTxt);
    
	//Set the form up before the loop is run
	window->m_OK->Enable(false);
	window->m_Save->Enable(false);
	OutputProgress(_("Starting...\n"));
	
	//Need to update this for the new command line stuff
	if(blVisible){
		window->Show();
	}
	window->Update();
	
	//Launch the correct set of loops
	if(data.GetFunction()== _("Copy") || data.GetFunction() == _("Update")){
		SyncLoop(data, rules);
	}
	else if(data.GetFunction() ==  _("Mirror (Copy)")){
		data.SetFunction(_("Copy"));
		SyncLoop(data, rules);
		//Swap the source and dest around for the mirror routine
		wxString strTemp = data.GetSource();
		data.SetSource(data.GetDest());
		data.SetDest(strTemp);
		//Run the mirror loop
		data.SetFunction(_("Mirror"));
		SyncLoop(data, rules);
	}
	else if(data.GetFunction() ==  _("Mirror (Copy)")){
		data.SetFunction(_("Update"));
		SyncLoop(data, rules);
		//Swap the source and dest around for the mirror routine
		wxString strTemp = data.GetSource();
		data.SetSource(data.GetDest());
		data.SetDest(strTemp);
		//Run the mirror loop
		data.SetFunction(_("Mirror"));
		SyncLoop(data, rules);
	}
	else if(data.GetFunction() ==  _("Equalise")){
		data.SetFunction(_("Update"));
		wxString strTemp = data.GetSource();
		data.SetSource(data.GetDest());
		data.SetDest(strTemp);
		data.SetFunction(_("Update"));
		SyncLoop(data, rules);
	}

	//Set up the progress window once the loop has finished
	OutputProgress(_("Finished...\n"));
	window->m_OK->Enable();
	window->m_Save->Enable();
	window->m_Abort->Enable(false);
	
	return true;
}
