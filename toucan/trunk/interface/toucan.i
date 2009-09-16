%module toucan
%include "wxstring.i"

%{
#include "../script.h"
#include "../data/syncdata.h"
#include "../sync/syncjob.h"
#include "../basicfunctions.h"
#include "../toucan.h"

wxString GetSettingsPath(){
	return wxGetApp().GetSettingsPath();
}

void Sync(const wxString &source, const wxString &dest){
	SyncData* data = new SyncData(wxT("LastSyncJob"));
	data->SetSource(source);
	data->SetDest(dest);
	data->SetFunction(_("Copy"));
	data->SetIgnoreRO(false);
	data->SetAttributes(false);
	data->SetIgnoreDLS(false);
	data->SetTimeStamps(false);
	if(data->IsReady()){
		if(data->TransferToFile()){
			wxArrayString arrScript;
			arrScript.Add(wxT("Sync \"LastSyncJob\""));
			wxGetApp().SetAbort(false);
			wxGetApp().m_Script->SetScript(arrScript);
			wxGetApp().m_Script->Execute();
		}
	}
	else{
		wxMessageBox(_("Not all of the required fields are filled"), _("Error"), wxICON_ERROR);
	}
	delete data;
}

%}

void OutputProgress(wxString message);
wxString GetSettingsPath();
void Sync(const wxString &source, const wxString &dest);