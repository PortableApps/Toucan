%module toucan
%include "typemaps.i"

%{
	#include <wx/datetime.h>
	#include <wx/event.h>
	#include <wx/msgdlg.h> 
	#include "toucan.h"
	#include "script.h"
	#include "rules.h"
	#include "variables.h"
	#include "filecounter.h"
	#include "basicfunctions.h"
	#include "data/syncdata.h"
	#include "data/backupdata.h"
	#include "data/securedata.h"
	#include "sync/syncjob.h"
	#include "backup/backupjob.h"
	#include "secure/securejob.h"

	void Sync(SyncData *data){
		FileCounter counter;
		counter.AddPath(data->GetSource());
		if(data->GetFunction() == _("Equalise")){
			counter.AddPath(data->GetDest());
		}
		counter.Count();
		int count = counter.GetCount();
		wxCommandEvent *event = new wxCommandEvent(wxEVT_COMMAND_BUTTON_CLICKED, ID_PROGRESSSETUP);
		event->SetInt(count);
		wxGetApp().QueueEvent(event);
		SyncJob *job = new SyncJob(data);
		job->Create();
		job->Run();
		job->Wait();
	}

	void Sync(const wxString &source, const wxString &dest, const wxString &function, 
			  SyncChecks checks, SyncOptions options, const wxString &rules = wxEmptyString)
	{
		SyncData *data = new SyncData(wxT("LastSyncJob"));
		data->SetSource(source);
		data->SetDest(dest);
		data->SetFunction(function);
		data->SetCheckSize(checks.Size);
		data->SetCheckTime(checks.Time);
		data->SetCheckShort(checks.Short);
		data->SetCheckFull(checks.Full);
		data->SetIgnoreRO(options.IgnoreRO);
		data->SetAttributes(options.Attributes);
		data->SetIgnoreDLS(options.IgnoreDLS);
		data->SetTimeStamps(options.TimeStamps);
		data->SetRecycle(options.Recycle);
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
	
	void Backup(BackupData *data){
		FileCounter counter;
		counter.AddPaths(data->GetLocations());
		counter.Count();
		int count = counter.GetCount();
		wxCommandEvent *event = new wxCommandEvent(wxEVT_COMMAND_BUTTON_CLICKED, ID_PROGRESSSETUP);
		event->SetInt(count);
		wxGetApp().QueueEvent(event);
		if(data->GetUsesPassword()){
			if(wxGetApp().m_Password == wxEmptyString){
				wxCommandEvent *event = new wxCommandEvent(wxEVT_COMMAND_BUTTON_CLICKED, ID_GETPASSWORD);
				int id = wxDateTime::Now().GetTicks();
				event->SetInt(id);
				wxGetApp().QueueEvent(event);
				while(wxGetApp().m_StatusMap[id] != true){
					wxMilliSleep(100);
				}
			}
			data->SetPassword(wxGetApp().m_Password);
		}
		BackupJob *job = new BackupJob(data);
		job->Create();
		job->Run();
		job->Wait();
	}
	
	void Backup(const wxString &jobname){
		BackupData *data = new BackupData(jobname);
		if(data->TransferFromFile()){
			if(data->IsReady()){
				Backup(data);
			}
			else{
				wxMessageBox(_("Not all of the required fields are filled"), _("Error"), wxICON_ERROR);
			}
		}
	}
	
	void Backup(const wxArrayString &paths, const wxString &backuplocation, const wxString &function, 
				const wxString &format, int compressionlevel = 3, bool password = false, 
				bool test = false, const wxString &rules = wxEmptyString){
		BackupData *data = new BackupData(wxT("LastBackupJob"));
		data->SetLocations(paths);
		data->SetFileLocation(backuplocation);
		data->SetFunction(function);
		data->SetFormat(format);
		data->SetRatio(compressionlevel);
		data->SetUsesPassword(password);
		data->SetTest(test);
		data->SetRules(new Rules(rules, true));
		if(data->IsReady()){
			Backup(data);
		}
		else{
			wxMessageBox(_("Not all of the required fields are filled"), _("Error"), wxICON_ERROR);
		}
	}

	void Secure(SecureData *data){
		FileCounter counter;
		counter.AddPaths(data->GetLocations());
		counter.Count();
		int count = counter.GetCount();
		wxCommandEvent *event = new wxCommandEvent(wxEVT_COMMAND_BUTTON_CLICKED, ID_PROGRESSSETUP);
		event->SetInt(count);
		wxGetApp().QueueEvent(event);
		if(wxGetApp().m_Password == wxEmptyString){
			wxCommandEvent *event = new wxCommandEvent(wxEVT_COMMAND_BUTTON_CLICKED, ID_GETPASSWORD);
			int id = wxDateTime::Now().GetTicks();
			event->SetInt(id);
			wxGetApp().QueueEvent(event);
			while(wxGetApp().m_StatusMap[id] != true){
				wxMilliSleep(100);
			}
			data->SetPassword(wxGetApp().m_Password);
		}
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
	
	void Secure(const wxArrayString &paths, const wxString &function, const wxString &rules = wxEmptyString){
		SecureData *data = new SecureData(wxT("LastSecureJob"));
		data->SetLocations(paths);
		data->SetFunction(function);
		data->SetRules(new Rules(rules, true));
		Secure(data);
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

	int Execute(const wxString &path, bool async = false){
		wxString normpath = Normalise(path);
		wxCommandEvent *event = new wxCommandEvent(wxEVT_COMMAND_BUTTON_CLICKED, ID_PROCESS);
		int id = wxDateTime::Now().GetTicks();
		event->SetInt(id);
		event->SetString(normpath);
		wxGetApp().QueueEvent(event);
		if(!async){
			while(wxGetApp().m_StatusMap[id] != true){
				wxMilliSleep(100);
			}
		}
		return wxGetApp().m_ProcessStatusMap[id];
	}
	
	wxString GetScriptPath(const wxString &name){
		wxString path = wxGetApp().GetSettingsPath() + "scripts" + wxFILE_SEP_PATH + name + ".lua";
		if(wxFileExists(path)){
			return path;
		}
		else{
			return wxEmptyString;
		}
	}
	
	void InputPassword(){
		wxCommandEvent *event = new wxCommandEvent(wxEVT_COMMAND_BUTTON_CLICKED, ID_GETPASSWORD);
		int id = wxDateTime::Now().GetTicks();
		event->SetInt(id);
		wxGetApp().QueueEvent(event);
		while(wxGetApp().m_StatusMap[id] != true){
			wxMilliSleep(100);
		}
	}
%}

void OutputProgress(const wxString &message, bool time = false, bool error = false);

void Sync(const wxString &jobname);
void Sync(const wxString &source, const wxString &dest, const wxString &function, 
		  SyncChecks checks, SyncOptions options, const wxString &rules = wxEmptyString);

void Backup(const wxString &jobname);
void Backup(const wxArrayString &paths, const wxString &backuplocation, const wxString &function, 
			const wxString &format, int compressionlevel = 3, bool password = false, 
			bool test = false, const wxString &rules = wxEmptyString);

void Secure(const wxString &jobname);
void Secure(const wxArrayString &paths, const wxString &function, const wxString &rules = wxEmptyString);

wxString ExpandVariable(const wxString &variable);
wxString GetScriptPath(const wxString &name);
bool Delete(const wxString &path);
bool Copy(const wxString &source, const wxString &dest);
bool Move(const wxString &source, const wxString &dest);
bool Rename(const wxString &source, const wxString &dest);
int Execute(const wxString &path, bool async = false);
void InputPassword();
