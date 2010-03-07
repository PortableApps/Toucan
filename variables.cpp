/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2009 Steven Lamerton
// Licence:     GNU GPL 2 (See readme for more info
/////////////////////////////////////////////////////////////////////////////////

#include <wx/fileconf.h>
#include <wx/tokenzr.h>
#include <wx/datetime.h>
#include <wx/stdpaths.h>
#include <wx/utils.h>
#include <wx/filename.h>

#include "variables.h"
#include "toucan.h"

wxString Normalise(const wxString &path){
#ifdef __WXMSW__
	//We only need to set this up once, and do it the first time
	if(wxGetApp().m_DriveLabels.empty()){
		TCHAR drives[256];  
		if(GetLogicalDriveStrings(256, drives)){  
			LPTSTR drive = drives;
			int offset = _tcslen(drive) + 1;  
			while(*drive){  
				wxString volumename = wxEmptyString;
				TCHAR label[256]; 
				if(GetVolumeInformation(drive, label, sizeof(label), NULL, 0, NULL, NULL, 0)){
					volumename.Printf(wxT("%s"),label); 
					if(volumename != wxEmptyString){
						wxGetApp().m_DriveLabels[volumename] = wxString(drive).Left(2);
					}
				}
				drive += offset;  
			}
		}
	}
#endif
	if(path.find("@") == wxNOT_FOUND){
		return path;
	}
	wxString token;
	wxString normalised = wxEmptyString;
	wxDateTime now = wxDateTime::Now();  
	wxStringTokenizer tkz(path, wxT("@"), wxTOKEN_RET_EMPTY_ALL);
	bool previousmatched = true;
	while(tkz.HasMoreTokens()){
        token = tkz.GetNextToken();
		wxString strValue, read;
		if(token == wxT("date")){
			token = now.FormatISODate();
			normalised += token;
			previousmatched = true;
		}
		else if(token == wxT("time")){
			token = now.Format(wxT("%H")) + wxT("-") +  now.Format(wxT("%M"));
			normalised += token;
			previousmatched = true;
		}
		else if(token == wxT("YYYY")){
			token = now.Format(wxT("%Y"));
			normalised += token;
			previousmatched = true;
		}
		else if(token == wxT("MM")){
			token = now.Format(wxT("%m"));
			normalised += token;
			previousmatched = true;
		}
		else if(token == wxT("DD")){
			token = now.Format(wxT("%d"));
			normalised += token;
			previousmatched = true;
		}
		else if(token == wxT("hh")){
			token = now.Format(wxT("%H"));
			normalised += token;
			previousmatched = true;
		}
		else if(token == wxT("mm")){
			token = now.Format(wxT("%M"));
			normalised += token;
			previousmatched = true;
		}
		else if(token == wxT("drive")){
			normalised += wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Left(2);
			previousmatched = true;
		}
		else if(token == wxT("docs")){
			normalised += wxStandardPaths::Get().GetDocumentsDir();
			previousmatched = true;
		}
		else if(token == wxT("volume")){
			#ifdef __WXMSW__
				wxString strName = wxEmptyString;
				WCHAR volumeLabel[256]; 
				GetVolumeInformation(wxGetApp().GetSettingsPath().Left(3), volumeLabel, sizeof(volumeLabel), NULL, 0, NULL, NULL, 0);
				strName.Printf(wxT("%s"),volumeLabel); 
				normalised += strName;
			#endif
			previousmatched = true;
		}
		else if(token == wxT("label")){
		 	wxFileConfig* autorun = new wxFileConfig(wxT(""), wxT(""), wxGetApp().GetSettingsPath().Left(3) + wxFILE_SEP_PATH + wxT("autorun.inf"));
			wxString label = autorun->Read(wxT("Autorun/Label"));
			normalised += label;
			delete autorun;
			previousmatched = true;
		}
		else if(wxGetApp().m_DriveLabels[token] != wxEmptyString){
			normalised += wxGetApp().m_DriveLabels[token];
			previousmatched = true;
		}
		else if(wxGetEnv(token , &strValue)){
			normalised += strValue;
			previousmatched = true;
		}
		else if(wxGetApp().m_Variables_Config->HasGroup(token) && wxGetApp().m_Variables_Config->Read(token + wxT("/") + wxGetFullHostName(), &read)){
			normalised += read;
			previousmatched = true;
		}
		else if(wxGetApp().m_Variables_Config->HasGroup(token) && wxGetApp().m_Variables_Config->Read(token + wxT("/") + _("Other"), &read)){
			normalised += read;
			previousmatched = true;
		}
		else{
			if(previousmatched){
				normalised += token;
			}
			else{
				normalised = normalised + wxT("@") + token;
			}
			//This time we did not match
			previousmatched = false;
		}
	}
	if(normalised.Length() == 2 && normalised.Right(1) == wxT(":")){
		normalised += wxFILE_SEP_PATH;
	}
	wxFileName flReturn(normalised);
	if(flReturn.IsOk()){
		//If we havent made any changes in this run then return, else scan again
		//as new variables may have been added
		return normalised == path ? path : Normalise(normalised);
	}
	return wxEmptyString;
}
