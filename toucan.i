%module toucan
%include "typemaps.i"

%{
	#include <wx/datetime.h>
	#include <wx/event.h>
	#include "toucan.h"
	#include "rules.h"
	#include "path.h"
	#include "fileops.h"
	#include "filecounter.h"
	#include "basicfunctions.h"
	#include "data/syncdata.h"
	#include "data/backupdata.h"
	#include "data/securedata.h"
	#include "sync/syncjob.h"
	#include "backup/backupjob.h"
	#include "secure/securejob.h"

	void Sync(SyncData *data){
		if(data->GetSource().GetFullPath() == wxEmptyString || !data->GetSource().IsDir()){
			throw std::invalid_argument("The source path is invalid");
		}
		if(data->GetDest().GetFullPath() == wxEmptyString || !data->GetDest().IsDir()){
			throw std::invalid_argument("The destination path  is invalid");
		}
		if(data->GetFunction() == wxEmptyString){
			throw std::invalid_argument("A valid function must be selected");
		}
		FileCounter counter;
		if(data->GetFunction() != _("Mirror")){
			counter.AddPath(data->GetSource().GetFullPath());		
		}
		else{
			counter.AddPath(data->GetDest().GetFullPath());	
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
			  SyncChecks checks = SyncChecks(), SyncOptions options = SyncOptions(), 
			  const wxString &rules = wxEmptyString)
	{
		SyncData *data = new SyncData(wxT("LastSyncJob"));
		data->SetSource(wxFileName::DirName(source));
		data->SetDest(wxFileName::DirName(dest));
		data->SetFunction(function);
		data->SetCheckSize(checks.Size);
		data->SetCheckTime(checks.Time);
		data->SetCheckShort(checks.Short);
		data->SetCheckFull(checks.Full);
		data->SetIgnoreRO(options.IgnoreRO);
		data->SetAttributes(options.Attributes);
		data->SetTimeStamps(options.TimeStamps);
		data->SetRecycle(options.Recycle);
		data->SetPreviewChanges(options.PreviewChanges);
        RuleSet *ruleset = new RuleSet(rules);
        ruleset->TransferFromFile();
		data->SetRules(ruleset);
		try{
			Sync(data);
		}
		catch(std::exception &arg){
			OutputProgress(arg.what(), Error);
		}
	}
	
	void Sync(const wxString &jobname){
		SyncData *data = new SyncData(jobname);
		try{
			data->TransferFromFile();
			Sync(data);
		}
		catch(std::exception &arg){
			OutputProgress(arg.what(), Error);
		}
	}
	
	void Backup(BackupData *data){
		if(data->GetLocations().Count() == 0){
			throw std::invalid_argument("You must select some paths to backup");
		}
		if(data->GetFileLocation() == wxEmptyString || wxDirExists(data->GetFileLocation())){
			throw std::invalid_argument("The backup archive must be specified");
		}
		if(data->GetFunction() == wxEmptyString){
			throw std::invalid_argument("A valid function must be selected");
		}
		if(data->GetFormat() == wxEmptyString){
			throw std::invalid_argument("A valid format must be selected");
		}
		FileCounter counter;
		counter.AddPaths(data->GetLocations());
		counter.Count();
		int count = counter.GetCount();
		if(data->GetTest()){
			count = count * 2;
		}
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
		try{
			data->TransferFromFile();
			Backup(data);
		}
		catch(std::exception &arg){
			OutputProgress(arg.what(), Error);
		}
	}
	
	void Backup(const wxArrayString &paths, const wxString &backuplocation, const wxString &function, 
				const wxString &format, int compressionlevel = 3, 
				BackupOptions options = BackupOptions(), const wxString &rules = wxEmptyString){
		BackupData *data = new BackupData(wxT("LastBackupJob"));
		data->SetLocations(paths);
		data->SetFileLocation(backuplocation);
		data->SetFunction(function);
		data->SetFormat(format);
		data->SetRatio(compressionlevel);
		data->SetUsesPassword(options.Password);
		data->SetTest(options.Test);
		data->SetSolid(options.Solid);
        RuleSet *ruleset = new RuleSet(rules);
        ruleset->TransferFromFile();
		data->SetRules(ruleset);
		try{
			Backup(data);
		}
		catch(std::exception &arg){
			OutputProgress(arg.what(), Error);
		}
	}

	void Secure(SecureData *data){
		if(data->GetLocations().Count() == 0){
			throw std::invalid_argument("You must select some paths to secure");
		}
		if(data->GetFunction() == wxEmptyString){
			throw std::invalid_argument("A valid function must be selected");
		}
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
		try{
			data->TransferFromFile();
			Secure(data);
		}
		catch(std::exception &arg){
			OutputProgress(arg.what(), Error);
		}
	}
	
	void Secure(const wxArrayString &paths, const wxString &function, const wxString &rules = wxEmptyString){
		SecureData *data = new SecureData(wxT("LastSecureJob"));
		data->SetLocations(paths);
		data->SetFunction(function);
        RuleSet *ruleset = new RuleSet(rules);
        ruleset->TransferFromFile();
		data->SetRules(ruleset);
		try{
			Secure(data);
		}
		catch(std::exception &arg){
			OutputProgress(arg.what(), Error);
		}
	}

	//Helper functions
	wxString ExpandVariable(const wxString &variable){
		return Path::Normalise(variable);
	}

	bool Delete(const wxString &path){
		wxString normpath = Path::Normalise(path);
		if(File::Delete(normpath, false, false)){
			OutputProgress(_("Deleted ") + normpath, Message);	
		}
		else{
			OutputProgress(_("Failed to delete ") + normpath, Error);
			return false;
		}
		return true;
	}

	bool Copy(const wxString &source, const wxString &dest){
		wxString normsource = Path::Normalise(source);
		wxString normdest = Path::Normalise(dest);
		if(File::Copy(normsource, normdest)){
			OutputProgress(_("Copied ") + normsource, Message);	
		}
		else{
			OutputProgress(_("Failed to copy ") + normsource, Error);
			return false;
		}	
		return true;
	}

	bool Move(const wxString &source, const wxString &dest){
		wxString normsource = Path::Normalise(source);
		wxString normdest = Path::Normalise(dest);
		if(File::Rename(normsource, normdest, true)){
			OutputProgress(_("Moved") + normsource, Message);	
		}
		else{
			OutputProgress(_("Failed to move ") + normsource, Error);
			return false;
		}
		return true;
	}

	bool Rename(const wxString &source, const wxString &dest){
		wxString normsource = Path::Normalise(source);
		wxString normdest = Path::Normalise(dest);
		if(File::Rename(normsource, normdest, true)){
			OutputProgress(_("Moved") + normsource, Message);	
		}
		else{
			OutputProgress(_("Failed to move ") + normsource, Error);
			return false;
		}
		return true;
	}

	int Execute(const wxString &path, bool async = false){
		wxString normpath = Path::Normalise(path);
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
	
	bool Shutdown(){
		return wxShutdown();
	}
%}

void OutputProgress(const wxString &message, OutputType type);

void Sync(const wxString &jobname);
void Sync(const wxString &source, const wxString &dest, const wxString &function, 
		  SyncChecks checks = SyncChecks(), SyncOptions options = SyncOptions(), 
		  const wxString &rules = wxEmptyString);

void Backup(const wxString &jobname);
void Backup(const wxArrayString &paths, const wxString &backuplocation, const wxString &function, 
			const wxString &format, int compressionlevel = 3, 
			BackupOptions options = BackupOptions(), const wxString &rules = wxEmptyString);

void Secure(const wxString &jobname);
void Secure(const wxArrayString &paths, const wxString &function, const wxString &rules = wxEmptyString);

wxString ExpandVariable(const wxString &variable);
wxString GetScriptPath(const wxString &name);
bool Delete(const wxString &path);
bool Copy(const wxString &source, const wxString &dest);
bool Move(const wxString &source, const wxString &dest);
bool Rename(const wxString &source, const wxString &dest);
int Execute(const wxString &path, bool async = false);
bool Shutdown();
void InputPassword();
