%module toucan
%include "wxstring.i"
%include "sync.i"

%{
	#include "../basicfunctions.h"
	#include "../toucan.h"

	wxString GetSettingsPath(){
		return wxGetApp().GetSettingsPath();
	}
%}

void OutputProgress(wxString message);
wxString GetSettingsPath();

void Sync(const wxString &jobname);
void Sync(const wxString &source, const wxString &dest, const wxString &function, bool timestamps = false);