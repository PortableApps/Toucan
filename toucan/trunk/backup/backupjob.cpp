/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "backupjob.h"
#include "backupthread.h"
#include "backupprocess.h"
#include "../toucan.h"
#include "../variables.h"
#include "../basicfunctions.h"
#include "../data/backupdata.h"

#include <wx/filename.h>
#include <wx/textfile.h>

BackupJob::BackupJob(BackupData *Data) : Job(Data){
	;
}

bool BackupJob::Execute(){
	BackupData *data = (BackupData*)GetData();
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
		wxString strCommand = data->CreateCommand(i);
		wxSetWorkingDirectory(path);
		
		BackupProcess *process = new BackupProcess();
		long lgPID = wxExecute(strCommand, wxEXEC_ASYNC|wxEXEC_NODISABLE, process);
	
		process->SetRealPid(lgPID);
		BackupThread *thread = new BackupThread(lgPID, process);

		thread->Create();
		thread->Run();
		
		if(!wxGetApp().GetUsesGUI()){
			while(!wxGetApp().GetFinished()){
				//So we dont thrash the processor
				wxMilliSleep(10);
				wxGetApp().Yield();
			}
		}
	}
	return true;
}
