%module toucan
%include "wxstring.i"

%{
	#include "toucan.h"
	#include "script.h"
	#include "basicfunctions.h"
	#include "data/syncdata.h"
	#include "sync/syncjob.h"

	wxString GetSettingsPath(){
		return wxGetApp().GetSettingsPath();
	}

	void Sync(SyncData *data){
		SyncJob *job = new SyncJob(data);
		job->Execute();
	}

	void Sync(const wxString &source, const wxString &dest, const wxString &function, 
				bool timestamps = false, bool attributes = false, 
				bool ignorero = false, bool ignoredls = false)
	{
		SyncData *data = new SyncData(wxT("LastSyncJob"));
		data->SetSource(source);
		data->SetDest(dest);
		data->SetFunction(function);
		data->SetIgnoreRO(ignorero);
		data->SetAttributes(attributes);
		data->SetIgnoreDLS(ignoredls);
		data->SetTimeStamps(timestamps);
		if(data->IsReady()){
			Sync(data);
		}
		else{
			wxMessageBox(_("Not all of the required fields are filled"), _("Error"), wxICON_ERROR);
		}
	}
	
	void Sync(const wxString &jobname){
		SyncData *data = new SyncData(jobname);
		if(data->TransferFromFile()){
			if(data->IsReady()){
				Sync(data);
			}
			else{
				wxMessageBox(_("Not all of the required fields are filled"), _("Error"), wxICON_ERROR);
			}
		}
	}
%}

void OutputProgress(wxString message);
wxString GetSettingsPath();

void Sync(const wxString &jobname);
void Sync(const wxString &source, const wxString &dest, const wxString &function, bool timestamps = false);