%module toucan
%include "wxstring.i"

%{
	#include <wx/datetime.h>
	#include <wx/event.h>
	#include <wx/msgdlg.h> 
	#include "toucan.h"
	#include "script.h"
	#include "rules.h"
	#include "variables.h"
	#include "basicfunctions.h"
	#include "data/syncdata.h"
	#include "data/securedata.h"
	#include "sync/syncjob.h"
	#include "secure/securejob.h"

	void Sync(SyncData *data){
		SyncJob *job = new SyncJob(data);
		job->Create();
		job->Run();
		job->Wait();
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

	void Secure(SecureData *data){
		SecureJob *job = new SecureJob(data);
		job->Create();
		job->Run();
		job->Wait();
	}
	
	void Secure(const wxString &jobname){
		SecureData *data = new SecureData(jobname);
		if(data->TransferFromFile()){
			if(data->IsReady()){
				Secure(data);
			}
			else{
				wxMessageBox(_("Not all of the required fields are filled"), _("Error"), wxICON_ERROR);
			}
		}
	}

	//Helper functions
	wxString ExpandVariable(const wxString &variable){
		return Normalise(variable);
	}

	bool Delete(const wxString &path){
		wxString normpath = Normalise(path);
		if(wxRemoveFile(path)){
			OutputProgress(_("Deleted ") + normpath);	
		}
		else{
			OutputProgress(_("Failed to delete ") + normpath, true, true);
			return false;
		}
		return true;
	}

	bool Copy(const wxString &source, const wxString &dest){
		wxString normsource = Normalise(source);
		wxString normdest = Normalise(dest);
		if(wxCopyFile(normsource, normdest, true)){
			OutputProgress(_("Copied ") + normsource);	
		}
		else{
			OutputProgress(_("Failed to copy ") + normsource, true, true);
			return false;
		}	
		return true;
	}

	bool Move(const wxString &source, const wxString &dest){
		wxString normsource = Normalise(source);
		wxString normdest = Normalise(dest);
		if(Copy(normsource, normdest)){
			if(wxRemoveFile(normsource)){
				OutputProgress(_("Moved") + normsource);	
			}
			else{
				OutputProgress(_("Failed to move ") + normsource, true, true);
				return false;
			}
		}
		return true;
	}

	bool Rename(const wxString &source, const wxString &dest){
		wxString normsource = Normalise(source);
		wxString normdest = Normalise(dest);
		if(wxRenameFile(normsource, normdest, true)){
			OutputProgress(_("Renamed ") + normsource);	
		}
		else{
			OutputProgress(_("Failed to rename ") + normsource, true, true);
			return false;
		}
		return true;
	}

	void Execute(const wxString &path, bool async = false){
		wxString normpath = Normalise(path);
		wxCommandEvent *event = new wxCommandEvent(wxEVT_COMMAND_BUTTON_CLICKED, ID_PROCESS);
		int id = wxDateTime::Now().GetTicks();
		event.SetInt(id);
		event.SetString(normpath);
		wxGetApp().QueueEvent(event);
		if(!async){
			while(wxGetApp().m_ProcessMap[id] != true){
				wxMilliSleep(100);
			}
		}
		OutputProgress(_("Executed ") + normpath);
		return;
	}
%}

void OutputProgress(const wxString &message, bool time = false, bool error = false);

void Sync(const wxString &jobname);
void Sync(const wxString &source, const wxString &dest, const wxString &function, 
          bool timestamps = false, bool attributes = false, bool ignorero = false, 
          bool ignoredls = false, const wxString &rules = wxEmptyString);

void Secure(const wxString &jobname);

wxString ExpandVariable(const wxString &variable);
bool Delete(const wxString &path);
bool Copy(const wxString &source, const wxString &dest);
bool Move(const wxString &source, const wxString &dest);
bool Rename(const wxString &source, const wxString &dest);
void Execute(const wxString &path, bool async = false);
