/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "backupjob.h"
#include "backupprocess.h"
#include "../toucan.h"
#include "../variables.h"
#include "../basicfunctions.h"
#include "../data/backupdata.h"
#include "../forms/frmprogress.h"

#include <wx/filename.h>
#include <wx/textfile.h>

BackupJob::BackupJob(BackupData *Data) : Job(Data){
	;
}

void* BackupJob::Entry(){
	BackupData *data = static_cast<BackupData*>(GetData());
	//Expand all of the variables
	for(unsigned int i = 0; i < data->GetLocations().Count(); i++){
		data->SetLocation(i, Normalise(data->GetLocation(i)));
	}
	data->SetFileLocation(Normalise(data->GetFileLocation()));
	for(unsigned int i = 0; i < data->GetLocations().Count(); i++){
		wxString path = data->GetLocation(i);
		bool isDir = false;
		//If we have a directory clean it up
		if(wxDirExists(path)){
			if (path[path.length()-1] != wxFILE_SEP_PATH) {
				path += wxFILE_SEP_PATH;       
			}
			isDir = true;
		}
		//If we are not running a restore job we need to create a list file
		if(data->GetFunction() != _("Restore")){
			//Set up the include file
			wxTextFile *file = new wxTextFile(wxGetApp().GetSettingsPath() + wxT("Includes.txt"));
			if(wxFileExists(wxGetApp().GetSettingsPath() + wxT("Includes.txt"))){
				file->Open();
				file->Clear();
				file->Write();
			}
			else{
				file->Create();
			}
			wxFileName filename(path);
			int length; 
			//If we have a directory then take of the last dir and take off one for the remaining slash
			if(isDir){
				filename.RemoveLastDir();
				length = filename.GetFullPath().Length();
				path = filename.GetFullPath();
			}
			//For files remove the filename and the remaining slash
			else{
				length = filename.GetPath().Length() - 1;
				path = filename.GetPath();
				if(path.Length() != 3){
					length += 2;
				}
			}
			OutputProgress(_("Creating file list, this may take some time."));
			if(!data->CreateList(file, data->GetLocation(i), length)){
				return false;
			}
			file->Write();
			//Set up the progress bar
			EnableGauge(true);
		}
		else{
			EnableGauge(false);
		}
		wxString command = data->CreateCommand(i);
		wxSetWorkingDirectory(path);
		
		int id = wxDateTime::Now().GetTicks();
		BackupProcess *process = new BackupProcess(id);
		wxCommandEvent *event = new wxCommandEvent(wxEVT_COMMAND_BUTTON_CLICKED, ID_BACKUPPROCESS);
		event->SetEventObject(process);
		event->SetInt(id);
		event->SetString(command);
		wxGetApp().QueueEvent(event);
		while(wxGetApp().m_ProcessMap[id] != true){
			if(!process->HasInput()){
				//If there was no input then sleep for a while so we don't thrash the CPU
				wxMilliSleep(100);
				//Also yield for input incase it is backing up a large file
				//wxGetApp().Yield();
			}
		}

		//Grab any remaining output
		while(process->HasInput())
				;
		//Tidy up any temp files
		if(wxFileExists(data->GetFileLocation() + wxT(".tmp"))){
			wxRemoveFile(data->GetFileLocation() + wxT(".tmp"));
		}
	}
	return NULL;
}
