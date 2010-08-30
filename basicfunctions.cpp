/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include <wx/tokenzr.h>
#include <wx/variant.h>
#include <wx/fileconf.h>
#include <wx/stdpaths.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/dir.h>
#include <wx/intl.h>
#include <wx/cmdline.h>
#include <wx/fileconf.h>
#include <wx/msgdlg.h>

#include "toucan.h"
#include "settings.h"
#include "basicfunctions.h"
#include "forms/frmprogress.h"
#include "forms/frmpassword.h"

//ATTN : This needs clearing up into smaller files
 
const wxString& ToLang(const wxString &en){
	return wxGetApp().m_EnToLang[en];
}

const wxString& ToEn(const wxString &lang){
	return wxGetApp().m_LangToEn[lang];
}

wxString ArrayStringToString(const wxArrayString &strings, const wxString &seperator){
	wxString temp;
	for(unsigned int i = 0; i < strings.GetCount(); i++){
		temp = temp + seperator + strings.Item(i);
	}
	return temp;
}

wxArrayString StringToArrayString(const wxString &string, const wxString &seperator){
	wxArrayString strings;
	//Use a string tokeniser to seperate the given string
	wxStringTokenizer tkz(string, seperator, wxTOKEN_STRTOK);
	while(tkz.HasMoreTokens()){  
		strings.Add(tkz.GetNextToken());
	}
	return strings;
}

void OutputProgress(const wxString &message, bool time, bool error){
	int type = 0;
	if(time && error){
		type = 1;
	}
	else if(error){
		type = 2;
	}
	else if(time){
		type = 3;
	}
	if(wxGetApp().IsGui()){
		wxCommandEvent *event = new wxCommandEvent(wxEVT_COMMAND_BUTTON_CLICKED, ID_OUTPUT);
		event->SetString(message);
		event->SetInt(type);
		wxGetApp().QueueEvent(event);
	}
	else{
		wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, ID_OUTPUT);
		event.SetString(message);
		event.SetInt(type);
		wxGetApp().ProcessEvent(event);
	}
}

void IncrementGauge(){
	wxCommandEvent *event = new wxCommandEvent(wxEVT_COMMAND_BUTTON_CLICKED, ID_PROGRESS);
	wxGetApp().QueueEvent(event);
}

double GetInPB(const wxString &value){
	wxString size;
	wxVariant var;
	//The size is in bytes so the length is all but one
	if(value.Right(2).Left(1) != wxT("k") && value.Right(2).Left(1) != wxT("M") && value.Right(2).Left(1) != wxT("G")){
		if(value.Right(1) == wxT("B")){
			var = value.Left(value.Length() - 1);
			size = value.Right(1);
		}
		else{
			//We do not know what unit is being used
			return 0;
		}
	}
	//The size is in a larger unit, so it all but two
	else{
		var = value.Left(value.Length() - 2);
		size = value.Right(2);
	}
	double dSize = var.GetDouble();
	if(size == wxT("B")){
		dSize = dSize/1024;
		size = wxT("kB");
	}
	if(size == wxT("kB")){
		dSize = dSize/1024;
		size = wxT("MB");
	}
	if(size == wxT("MB")){
		dSize = dSize/1024;
		size = wxT("GB");
	}
	if(size == wxT("GB")){
		dSize = dSize/1024;
	}
	//Conveting to PB, should be plenty big for a while
	dSize = dSize/1024;
	return dSize;
}

wxArrayString GetJobs(const wxString &type){
	bool blCont;
	wxString value;
	long dummy;
	wxArrayString jobs;
	//Iterate through all of the groups
	blCont = wxGetApp().m_Jobs_Config->GetFirstGroup(value, dummy);
	while (blCont){
		//If the group is of the correct type then add it to the combobox
		if(wxGetApp().m_Jobs_Config->Read(value + wxT("/Type")) == type){
			if(value != wxT("SyncRemember") && value != wxT("BackupRemember") && value != wxT("SecureRemember")){
				jobs.Add(value);				
			}
		}
		else if(type == wxEmptyString){
			jobs.Add(value);
		}
		blCont = wxGetApp().m_Jobs_Config->GetNextGroup(value, dummy);
	}
	if(type == wxEmptyString){
		jobs.Add(wxT("SyncRemember"));
		jobs.Add(wxT("BackupRemember"));
		jobs.Add(wxT("SecureRemember"));
	}
	return jobs;	
}

wxArrayString GetVariables(bool builtin = false){
	bool cont;
	wxString value;
	long dummy;
	wxArrayString variables;
	//Iterate through all of the groups
	cont = wxGetApp().m_Variables_Config->GetFirstGroup(value, dummy);
	while(cont){
		variables.Add(value);
		cont = wxGetApp().m_Variables_Config->GetNextGroup(value, dummy);
	}
	if(builtin){
		variables.Add("date");
		variables.Add("time");
		variables.Add("docs");
		variables.Add("drive");
		variables.Add("volume");
		variables.Add("label");
		variables.Add("year");
		variables.Add("month");
        variables.Add("monthname");
        variables.Add("monthshortname");
		variables.Add("day");
		variables.Add("dayname");
		variables.Add("dayshortname");
		variables.Add("hour");
		variables.Add("minute");
        variables.Add("dayofweek");
        variables.Add("weekofyear");
	}
	return variables;	
}

wxArrayString GetRules(){
	bool cont;
	wxString value;
	long dummy;
	wxArrayString rules;

	cont = wxGetApp().m_Rules_Config->GetFirstGroup(value, dummy);
	while(cont){
		if(value != wxT("General")){
			rules.Add(value);
		}
		cont = wxGetApp().m_Rules_Config->GetNextGroup(value, dummy);
	}
	return rules;
}

wxArrayString GetScripts(){
	if(!wxDirExists(wxGetApp().GetSettingsPath() + "scripts" + wxFILE_SEP_PATH)){
		wxMkdir(wxGetApp().GetSettingsPath() + "scripts" + wxFILE_SEP_PATH);
	}
	wxArrayString scripts;
	wxString filename;
	wxDir dir(wxGetApp().GetSettingsPath() + "scripts" + wxFILE_SEP_PATH);
	if(dir.GetFirst(&filename)){
		do{
			wxFileName fname(filename);
			if(!fname.IsDir() && fname.GetExt() == "lua"){
				scripts.Add(fname.GetName());
			}
		}
		while(dir.GetNext(&filename));
	}
	return scripts;
}

bool UpdateJobs(){
	long version;
	wxFileConfig *config = wxGetApp().m_Jobs_Config;
	if(!wxFileExists(wxGetApp().GetSettingsPath() + wxT("Jobs.ini"))){
		if(!wxGetApp().IsReadOnly()){
			config->Write(wxT("General/Version"), 301);
			config->Flush();
			return true;
		}
		else{
			return false;
		}
	}
	config->Read(wxT("General/Version"), &version, 1);
	//Return if we are up to date
	if(version == 301){
		return true;
	}
	if(wxGetApp().IsReadOnly()){
		wxMessageBox(_("Please run Toucan from a device that allows writes so it can update itself"), _("Error"), wxCENTRE|wxICON_ERROR);
		return false;
	}
	//Back up the existing file
	if(wxFileExists(wxGetApp().GetSettingsPath() + wxT("Jobs.ini"))){
		wxCopyFile(wxGetApp().GetSettingsPath() + wxT("Jobs.ini"), wxGetApp().GetSettingsPath() + wxT("Jobs.old"), true);		
	}
	//We no longer support upgrading such old configs
	if(version < 200){
		wxMessageBox(_("Upgrading to this version of Toucan is not supported"), _("Error"), wxCENTRE|wxICON_ERROR);
		return false;
	}
	if(version == 200){
		bool blCont;
		wxString strValue;
		long dummy;
		//Iterate through all of the groups
		blCont = config->GetFirstGroup(strValue, dummy);
		while (blCont){
			if(config->Read(strValue + wxT("/Type")) == wxT("Backup")){
				wxString strTemp;
				strTemp = config->Read(strValue + wxT("/Locations"));
				strTemp.Replace(wxT("#"), wxT("|"));		
				config->Write(strValue + wxT("/Locations"), strTemp);
				config->Flush();
			}
			else if(config->Read(strValue + wxT("/Type")) == wxT("Secure")){
				wxString strTemp;
				strTemp = config->Read(strValue + wxT("/Locations"));
				strTemp.Replace(wxT("#"), wxT("|"));
				config->Write(strValue + wxT("/Locations"), strTemp);
				config->Flush();
			}
			blCont = config->GetNextGroup(strValue, dummy);
		}
		config->Flush();
		version = 202;
	}
	if(version == 202){
		bool blCont;
		wxString strValue;
		long dummy;
		//Iterate through all of the groups
		blCont = config->GetFirstGroup(strValue, dummy);
		while (blCont){
			if(config->Read(strValue + wxT("/Type")) == wxT("Sync")){
				wxString strTemp;
				strTemp = config->Read(strValue + wxT("/Function"));
				if(strTemp == _("Mirror (Copy)") || strTemp == _("Mirror (Update)")){
					strTemp = _("Mirror");
				}
				config->Write(strValue + wxT("/Function"), strTemp);
			}
			blCont = config->GetNextGroup(strValue, dummy);
		}
		version = 204;
	}
	if(version == 204){
		wxString value;
		long dummy;
		bool exists = config->GetFirstGroup(value, dummy);
		while(exists){
			//All of the functions need to use english only
			config->Write(value + wxT("/Function"), ToEn(config->Read(value + wxT("/Function"))));
			exists = config->GetNextGroup(value, dummy);
		}
		version = 213;
	}
	if(version == 213){
		wxString value;
		long dummy;
		bool exists = config->GetFirstGroup(value, dummy);
		while(exists){
			if(config->Read(value + wxT("/Type")) == wxT("Sync")){
				wxString temp = config->Read(value + wxT("/Function"));
				if(temp == _("Update")){
					config->Write(value + wxT("/Function"), _("Copy"));
					config->Write(value + wxT("/CheckTime"), true);
				}
				else if(temp == _("Copy") || temp == _("Mirror") || temp == _("Equalise")
						|| temp == _("Clean") || temp == _("Move")){
					config->Write(value + wxT("/CheckSize"), false);
					config->Write(value + wxT("/CheckTime"), false);
					config->Write(value + wxT("/CheckShort"), false);
					config->Write(value + wxT("/CheckFull"), true);
				}
			}
			exists = config->GetNextGroup(value, dummy);
		}
        version = 300;
	}
	if(version == 300){
		wxString value;
		long dummy;
		bool exists = config->GetFirstGroup(value, dummy);
		while(exists){
			if(config->Read(value + wxT("/Type")) == wxT("Sync")){
                if(!config->Read("CheckSize")){
					config->Write(value + wxT("/CheckSize"), false);
                }
                if(!config->Read("CheckTime")){
					config->Write(value + wxT("/CheckTime"), false);
                }
                if(!config->Read("CheckShort")){
					config->Write(value + wxT("/CheckShort"), false);
                }
                if(!config->Read("CheckFull")){
					config->Write(value + wxT("/CheckFull"), false);
                }
                if(!config->Read("Recycle")){
					config->Write(value + wxT("/Recycle"), false);
                }
                if(!config->Read("PreviewChanges")){
					config->Write(value + wxT("/PreviewChanges"), false);
                }
			}
			else if(config->Read(value + wxT("/Type")) == wxT("Backup")){
                if(!config->Read("Test")){
					config->Write(value + wxT("/Test"), false);
                }
                if(!config->Read("Solid")){
					config->Write(value + wxT("/Solid"), false);
                }
			}
			exists = config->GetNextGroup(value, dummy);
		}
        version = 301;
	}
	config->Write(wxT("General/Version"), 301);
	config->Flush();
	return true;
}

bool UpdateRules(){
	long version;
	wxFileConfig *config = wxGetApp().m_Rules_Config;
	if(!wxFileExists(wxGetApp().GetSettingsPath() + wxT("Rules.ini"))){
		if(!wxGetApp().IsReadOnly()){
			config->Write(wxT("General/Version"), 300);
			config->Flush();
			return true;
		}
		else{
			return false;
		}
	}
	config->Read(wxT("General/Version"), &version, 1);
	//Return if we are up to date
	if(version == 300){
		return true;
	}
	if(wxGetApp().IsReadOnly()){
		wxMessageBox(_("Please run Toucan from a device that allows writes so it can update itself"), _("Error"), wxCENTRE|wxICON_ERROR);
		return false;
	}
	//Back up the existing file
	if(wxFileExists(wxGetApp().GetSettingsPath() + wxT("Rules.ini"))){
		wxCopyFile(wxGetApp().GetSettingsPath() + wxT("Rules.ini"), wxGetApp().GetSettingsPath() + wxT("Rules.old"), true);		
	}
	//We no longer support upgrading such old configs
	if(version < 200){
		wxMessageBox(_("Upgrading to this version of Toucan is not supported"), _("Error"), wxCENTRE|wxICON_ERROR);
		return false;
	}
	config->Write(wxT("General/Version"), 300);
	config->Flush();
	return true;
}

bool UpdateScripts(){
	long version;
	wxFileConfig *config = wxGetApp().m_Scripts_Config;
	if(!wxFileExists(wxGetApp().GetSettingsPath() + wxT("Scripts.ini"))){
		if(!wxGetApp().IsReadOnly()){
			config->Write(wxT("General/Version"), 300);
			config->Flush();
			return true;
		}
		else{
			return false;
		}
	}
	config->Read(wxT("General/Version"), &version, 1);
	//Return if we are up to date
	if(version == 300){
		return true;
	}
	if(wxGetApp().IsReadOnly()){
		wxMessageBox(_("Please run Toucan from a device that allows writes so it can update itself"), _("Error"), wxCENTRE|wxICON_ERROR);
		return false;
	}
	//Back up the existing file
	if(wxFileExists(wxGetApp().GetSettingsPath() + wxT("Scripts.ini"))){
		wxCopyFile(wxGetApp().GetSettingsPath() + wxT("Scripts.ini"), wxGetApp().GetSettingsPath() + wxT("Jobs.old"), true);		
	}
	//We no longer support upgrading such old configs
	if(version < 200){
		wxMessageBox(_("Upgrading to this version of Toucan is not supported"), _("Error"), wxCENTRE|wxICON_ERROR);
		return false;
	}
	config->Write(wxT("General/Version"), 300);
	config->Flush();
	return true;
}

bool UpdateSettings(){
	long version;
	wxFileConfig *config = new wxFileConfig(wxT(""), wxT(""), wxGetApp().GetSettingsPath() + wxT("Settings.ini"));
	if(!wxFileExists(wxGetApp().GetSettingsPath() + wxT("Settings.ini"))){
		if(!wxGetApp().IsReadOnly()){
			config->Write(wxT("General/Version"), 300);
			config->Flush();
			return true;
		}
		else{
			return false;
		}
	}
	config->Read(wxT("General/Version"), &version, 1);
	//Return if we are up to date
	if(version == 300){
		delete config;
		return true;
	}
	if(wxGetApp().IsReadOnly()){
		wxMessageBox(_("Please run Toucan from a device that allows writes so it can update itself"), _("Error"), wxCENTRE|wxICON_ERROR);
		delete config;
		return false;
	}
	//Back up the existing file	
	if(wxFileExists(wxGetApp().GetSettingsPath() + wxT("Settings.ini"))){
		wxCopyFile(wxGetApp().GetSettingsPath() + wxT("Settings.ini"), wxGetApp().GetSettingsPath() + wxT("Settings.old"), true);		
	}
	if(version == 1){
		//All of the functions need to use english only
		config->Write(wxT("General/Tabs"), ToEn(config->Read(wxT("General/Tabs"))));
		config->Write(wxT("General/Position"), ToEn(config->Read(wxT("General/Position"))));
		config->Flush();
	}
	config->Write(wxT("General/Version"), 300);
	config->Flush();
	delete config;
	return true;
}

//Used by for_each in test.h
void makedir(const wxString &path){
	wxMkdir(path);
}

//Used by for_each in test.h
void createfiles(const wxString &path){
	wxTextFile file;
	file.Create(path + wxFILE_SEP_PATH + "file1");
	file.Create(path + wxFILE_SEP_PATH + "file2");
}

//Used by for_each in test.h
void deletedir(const wxString &path){
	wxRmDir(path);
}

//Used by for_each in test.h
void deletefiles(const wxString &path){
	wxRemoveFile(path + wxFILE_SEP_PATH + "file1");
	wxRemoveFile(path + wxFILE_SEP_PATH + "file2"); 
}
