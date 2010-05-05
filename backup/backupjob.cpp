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
#include <wx/stdpaths.h>

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
		}
		wxArrayString commands = data->CreateCommands();
		wxSetWorkingDirectory(path);
		
		for(unsigned int i = 0; i < commands.Count(); ++i){
			int id = wxDateTime::Now().GetTicks();
			BackupProcess *process = new BackupProcess(id);
			wxCommandEvent *event = new wxCommandEvent(wxEVT_COMMAND_BUTTON_CLICKED, ID_BACKUPPROCESS);
			event->SetEventObject(process);
			event->SetInt(id);
			event->SetString(commands.Item(i));
			wxGetApp().QueueEvent(event);
			//If we are in console then we yield to make sure the event is processed
			if(!wxGetApp().IsGui()){
				wxGetApp().Yield();
			}
			while(wxGetApp().m_StatusMap[id] != true){
				if(!process->HasInput()){
					//If there was no input then sleep for a while so we don't thrash the CPU
					wxMilliSleep(100);
					if(!wxGetApp().IsGui()){
						wxGetApp().Yield();
					}
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
		if(data->GetFormat() == wxT("tar")){
			wxString ext = data->GetFileLocation().AfterLast('\\').AfterFirst('.');
			wxString path = data->GetFileLocation().Left(data->GetFileLocation().Length() - ext.Length()) + "tar";
			if(wxFileExists(path)){
				wxRemoveFile(path);
			}
		}
	}
	return NULL;
}
