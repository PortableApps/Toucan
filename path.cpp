/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include <wx/fileconf.h>
#include <wx/tokenzr.h>
#include <wx/datetime.h>
#include <wx/stdpaths.h>
#include <wx/utils.h>
#include <wx/filename.h>

#include "path.h"
#include "toucan.h"
#include "basicfunctions.h"

void Path::Create(const wxString &path){
    if(wxDirExists(path)){
        return;
    }

    wxFileName name = wxFileName::DirName(path);
    wxArrayString folders = name.GetDirs();
    wxString workingpath = name.GetVolume() + wxFileName::GetVolumeSeparator() + wxFILE_SEP_PATH;
    for(unsigned int i = 0; i < folders.GetCount(); i++){
        workingpath = workingpath + folders.Item(i) + wxFILE_SEP_PATH;
        if(!wxDirExists(workingpath) && !wxMkdir(workingpath)){
		    OutputProgress(_("Could not create") + " " + workingpath, true, true);
	    }
    }
}

wxString Path::Normalise(const wxString &path){
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
		if(token == "date"){
			token = now.FormatISODate();
			normalised += token;
			previousmatched = true;
		}
		else if(token == "time"){
			token = now.Format("%H") + "-" +  now.Format("%M");
			normalised += token;
			previousmatched = true;
		}
		else if(token == "YYYY" || token == "year"){
			token = now.Format("%Y");
			normalised += token;
			previousmatched = true;
		}
		else if(token == "MM" || token == "month"){
			token = now.Format("%m");
			normalised += token;
			previousmatched = true;
		}
		else if(token == "monthname"){
            token = wxDateTime::GetMonthName(wxDateTime::Now().GetMonth());
			normalised += token;
			previousmatched = true;
		}
		else if(token == "monthshortname"){
            token = wxDateTime::GetMonthName(wxDateTime::Now().GetMonth(), wxDateTime::Name_Abbr);
			normalised += token;
			previousmatched = true;
		}
		else if(token == "DD" || token == "day"){
			token = now.Format("%d");
			normalised += token;
			previousmatched = true;
		}
		else if(token == "dayname"){
            token = wxDateTime::GetWeekDayName(wxDateTime::Now().GetWeekDay());
			normalised += token;
			previousmatched = true;
		}
		else if(token == "dayshortname"){
            token = wxDateTime::GetWeekDayName(wxDateTime::Now().GetWeekDay(), wxDateTime::Name_Abbr);
			normalised += token;
			previousmatched = true;
		}
		else if(token == "hh" || token == "hour"){
			token = now.Format(wxT("%H"));
			normalised += token;
			previousmatched = true;
		}
		else if(token == "mm" || token == "minute"){
			token = now.Format("%M");
			normalised += token;
			previousmatched = true;
		}
		else if(token == "dayofweek"){
            int num = wxDateTime::Now().GetWeekDay();
            if(num == 0)
                num = 7;
            token = wxString::Format("%d", num);
			normalised += token;
			previousmatched = true;
		}
		else if(token == "weekofyear"){
            int num = wxDateTime::Now().GetWeekOfYear();
            token = wxString::Format("%d", num);
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
