%module toucan
%include "wxstring.i"

%{
	#include "toucan.h"
	#include "script.h"
	#include "rules.h"
	#include "variables.h"
	#include "basicfunctions.h"
	#include "data/syncdata.h"
	#include "sync/syncjob.h"

	wxString GetSettingsPath(){
		return wxGetApp().GetSettingsPath();
	}
	
	wxString ExpandVariable(const wxString &variable){
		return Normalise(variable);
	}

	void Sync(SyncData *data){
		SyncJob *job = new SyncJob(data);
		job->Execute();
	}

	void Sync(const wxString &source, const wxString &dest, const wxString &function, 
				bool timestamps = false, bool attributes = false, 
				bool ignorero = false, bool ignoredls = false, const wxString &rules = wxEmptyString)
	{
		SyncData *data = new SyncData(wxT("LastSyncJob"));
		data->SetSource(source);
		data->SetDest(dest);
		data->SetFunction(function);
		data->SetIgnoreRO(ignorero);
		data->SetAttributes(attributes);
		data->SetIgnoreDLS(ignoredls);
		data->SetTimeStamps(timestamps);
		data->SetRules(new Rules(rules, true));
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

void OutputProgress(const wxString &message, bool time = false, bool error = false);
wxString ExpandVariable(const wxString &variable);
wxString GetSettingsPath();

void Sync(const wxString &jobname);
void Sync(const wxString &source, const wxString &dest, const wxString &function, bool timestamps = false);