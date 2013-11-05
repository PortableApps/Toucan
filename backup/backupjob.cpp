/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 - 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////


#include "backupjob.h"
#include "backupprocess.h"
#include "../toucan.h"
#include "../path.h"
#include "../basicfunctions.h"
#include "../data/backupdata.h"
#include "../forms/frmprogress.h"

#include <wx/filename.h>
#include <wx/textfile.h>
#include <wx/stdpaths.h>


BackupJob::BackupJob(BackupData *Data) : Job(Data){
	;
}

//This is a utility function for properlly creating the include/exclude file
boost::shared_ptr<wxTextFile> BackupJob::CreateSysFile(const wxString &filename) {
    boost::shared_ptr<wxTextFile> sysfile(new wxTextFile(wxGetApp().GetSettingsPath() + filename));
    if(wxFileExists(sysfile->GetName())){
        sysfile->Open();
        sysfile->Clear();
        sysfile->Write();
    }
    else{
        sysfile->Create();
    }
    return sysfile;
}

void* BackupJob::Entry(){
	BackupData *data = static_cast<BackupData*>(GetData());

	//Expand all of the variables
	for(unsigned int i = 0; i < data->GetLocations().Count(); i++){
        data->SetLocation(i, Path::Normalise(data->GetLocation(i)));
	}
    data->SetFileLocation(Path::Normalise(data->GetFileLocation()));

    //If we are not running a restore job we need to create exclude & possibly include files
    boost::shared_ptr<wxTextFile> includefile, excludefile;
    if(data->GetFunction() != _("Restore")){
        excludefile = CreateSysFile(wxT("Excludes.txt"));
        //Include file is only used for Complete and Update functions
        if(data->GetFunction() == _("Complete") || data->GetFunction() == _("Update")) {
            includefile = CreateSysFile(wxT("Includes.txt"));
        }
    }

    OutputProgress(_("Creating file list, this may take some time."), Message);
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
		//If we are not running a restore job we need to create exclude & possibly include files
		if(data->GetFunction() != _("Restore")){
			wxFileName filename(path);
			int length;

			//Include file is only used for Complete and Update functions
            if(data->GetFunction() == _("Complete") || data->GetFunction() == _("Update")) {
                includefile->AddLine(data->GetLocation(i));
                includefile->Write();
            }
			//If we have a directory then take of the last dir and take off one for the remaining slash
			if(isDir){
				if (filename.GetDirCount() != 0){
					filename.RemoveLastDir();
				}
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
			if(!data->CreateList(excludefile, data->GetLocation(i), length)){
				return false;
			}
			excludefile->Write();
		}
	}

    wxArrayString commands = data->CreateCommands();
    
    for(unsigned int i = 0; i < commands.Count(); ++i){
        int id = wxDateTime::Now().GetTicks();
        BackupProcess *process = new BackupProcess(id);
        wxCommandEvent *event = new wxCommandEvent(wxEVT_COMMAND_BUTTON_CLICKED, ID_BACKUPPROCESS);
        event->SetEventObject(process);
        event->SetInt(id);
        event->SetString(commands.Item(i));
        wxGetApp().QueueEvent(event);
        while(wxGetApp().m_StatusMap[id] != true){
            if(!process->HasInput()){
                //If there was no input then sleep for a while so we don't thrash the CPU
                wxMilliSleep(100);
            }
        }

        //Grab any remaining output
        while(process->HasInput());

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
	return NULL;
}
