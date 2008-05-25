/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_BASICFUNCTIONS
#define H_BASICFUNCTIONS

#include <wx/tokenzr.h>
#include <wx/variant.h>
#include <wx/fileconf.h>
#include <wx/stdpaths.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/dir.h>
#include <wx/intl.h>
#include "frmprogress.h"
#include "toucan.h"


wxFFileOutputStream output(stderr);
wxTextOutputStream cout(output);

//Turns an array string into a string with the strings seperated by strSeperator, used when writing to ini files
wxString ArrayStringToString(wxArrayString arrStrings, wxString strSeperator){
	wxString strTemp;
	for(unsigned int i = 0; i < arrStrings.GetCount(); i++){
		strTemp = strTemp + strSeperator + arrStrings.Item(i);
	}
	return strTemp;
}

//Used for turning a string into an arraystring when the string are seperated by strSeperator, used when read from ini files
wxArrayString StringToArrayString(wxString strMain, wxString strSeperator){
	//Arraystring to create	
	wxArrayString arrMain;
	//Use a string tokeniser to seperate the given string
	wxStringTokenizer tkzMain(strMain, strSeperator, wxTOKEN_STRTOK);
	wxString strToken;
	while ( tkzMain.HasMoreTokens() ){  
		strToken = tkzMain.GetNextToken();   
		arrMain.Add(strToken);

	}
	return arrMain;
}

//Basic messagebox
void MessageBox(wxString strMessage, wxString strCaption){
	wxMessageBox(strMessage, strCaption, wxICON_INFORMATION);
}
//Messagebox with error icon
void ErrorBox(wxString strMessage){
	wxMessageBox(strMessage, _("Error"), wxICON_ERROR);
}

//Basic function to write output to progress form, used to keep code clean
void OutputProgress(wxString strValue){
	if(wxGetApp().ProgressWindow->IsShown()){
		wxGetApp().ProgressWindow->m_Text->AppendText(strValue + wxT("\n"));
	}
	else{
        cout<<strValue + wxT("\n");
	}		
}

/*Gets a files size in petabytes, should do for now but it may need changing in a few years ;)
Expects to be passed the result of a wxFilename.GetHumanReadableSize()*/
double GetInPB(wxString strValue){
	wxString strSize = strValue.Right(2);
	wxVariant var = strValue.Left(strValue.Length() - 2);
	double dSize = var.GetDouble();
	if(strSize = wxT("kB")){
		dSize = dSize/1024;
	}
	if(strSize = wxT("MB")){
		dSize = dSize/1024;
	}
	if(strSize = wxT("GB")){
		dSize = dSize/1024;
	}
	//Conveting to PB, should be plenty big for a while
	dSize = dSize/1024;
	return dSize;
}

//Adds a list of rules to a combobox
bool SetRulesBox(wxComboBox *box){
	//Clear the existin items incase any are out of date
	box->Clear();
	//Create a fileconfig item
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxGetApp().GetSettingsPath() + wxT("Rules.ini"));
	bool blCont;
	wxString strValue;
	long dummy;
	//Iterate through the groups adding them to the box
	blCont = config->GetFirstGroup(strValue, dummy);
	while (blCont){
		box->Append(strValue);
		blCont = config->GetNextGroup(strValue, dummy);
	}
	//Delete the fileconfig object
	delete config;
	return true;
}

//Adds a list of jobs to a combobox, needs to be passed a type to ensure only the relevent jobs are added
bool SetJobsBox(wxComboBox *box, wxString strType){
	//Create a fileconfig object	
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""),  wxGetApp().GetSettingsPath()+ wxT("Jobs.ini"));
	bool blCont;
	wxString strValue;
	long dummy;
	//Iterate through all of the groups
	blCont = config->GetFirstGroup(strValue, dummy);
	while (blCont){
		//If the group os of the correct type then add it to the combobox
		if(config->Read(strValue + wxT("/Type")) == strType){
			box->Append(strValue);
		}
		blCont = config->GetNextGroup(strValue, dummy);
	}
	//Remove the fileconfig object
	delete config;
	return true;
}

bool SetVariablesBox(wxComboBox *box){
	//Create a fileconfig object	
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""),  wxGetApp().GetSettingsPath()+ wxT("Variables.ini"));
	bool blCont;
	wxString strValue;
	long dummy;
	//Iterate through all of the groups
	blCont = config->GetFirstGroup(strValue, dummy);
	while (blCont){
		box->Append(strValue);
		blCont = config->GetNextGroup(strValue, dummy);
	}
	//Remove the fileconfig object
	delete config;
	return true;
}

wxArrayString GetLanguages(){
	wxArrayString arrLang;
	wxString strPath = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("lang") + wxFILE_SEP_PATH;
	wxDir dir(strPath);
	wxString strFilename;
	bool blDir = dir.GetFirst(&strFilename);
	if(blDir){
		do {
			if(wxDirExists(strPath + strFilename))
			{
				if(wxFileExists(strPath + strFilename + wxFILE_SEP_PATH + wxT("lang.ini"))){
					wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), strPath + strFilename + wxFILE_SEP_PATH + wxT("lang.ini"));
					wxString strLanguage = config->Read(wxT("General/LanguageCode"));
					arrLang.Add(wxLocale::FindLanguageInfo(strLanguage)->Description);
				}
			}
		}
		while (dir.GetNext(&strFilename) );
	} 
	return arrLang;
	
}
#endif
