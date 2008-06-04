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
#include <wx/cmdline.h>
#include <wx/fileconf.h>
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
	if(wxGetApp().ProgressWindow != NULL){
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



//Adds a list of rules to a combobox
bool SetScriptsBox(wxComboBox *box){
	//Clear the existin items incase any are out of date
	box->Clear();
	//Create a fileconfig item
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxGetApp().GetSettingsPath() + wxT("Scripts.ini"));
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

wxString InputPassword(){
	wxString strNewPass;
	if(wxGetApp().ProgressWindow != NULL){
		wxPasswordEntryDialog dialog(wxGetApp().ProgressWindow, _("Please enter your password"));
		if (dialog.ShowModal() == wxID_OK) {
			strNewPass = dialog.GetValue();
			wxPasswordEntryDialog dialog2(wxGetApp().ProgressWindow, _("Please enter your password"));
			if(dialog2.ShowModal() == wxID_OK){
				if(strNewPass == dialog2.GetValue()){
					return strNewPass;
				}
				else{
					ErrorBox(_("Sorry the passwords do not match"));
					return wxEmptyString;
				}
			}
		}
	}
	else{
		wxCmdLineParser cmdParser(wxGetApp().argc, wxGetApp().argv);
		int res;
		{
			wxLogNull log;
			res = cmdParser.Parse(false);
		}
		wxFileConfig config(wxT(""), wxT(""), wxGetApp().GetSettingsPath() + wxT("Jobs.ini"));
		if(cmdParser.GetParam(0) == _("Script")){
			if(cmdParser.GetParamCount() == 4 && cmdParser.GetParam(2) == cmdParser.GetParam(3)){
				return cmdParser.GetParam(2);
			}
			else{
				cout<<_("You need to repeat the password and ensure the two passowrds are identical");
				return wxEmptyString;
			}
		}
		else if(config.Read(cmdParser.GetParam(0) + wxT("/Type")) == wxT("Secure")){
			if(cmdParser.GetParamCount() == 3 && cmdParser.GetParam(1) == cmdParser.GetParam(2)){
				return cmdParser.GetParam(2);
			}
			else{
				cout<<_("You need to repeat the password and ensure the two passowrds are identical");
				return wxEmptyString;
			}
		}
		else if(config.Read(cmdParser.GetParam(0) + wxT("/Type")) == wxT("Backup")){
			if(cmdParser.GetParamCount() == 3){
				if(cmdParser.GetParam(1) == cmdParser.GetParam(2)){
					return cmdParser.GetParam(1);
				}
				else{
					cout<<_("You need to repeat the password and ensure the two passowrds are identical");
					return wxEmptyString;
				}
			}
			else{
				return wxT("Password not needed");
			}
		}
	}	
	
	return wxEmptyString;
}

void SetSliderText(){
	frmMain *window = wxGetApp().MainWindow;
	switch(window->m_Backup_Ratio->GetValue()){
		case 0:
			window->m_Backup_Ratio_Text->SetLabel(_("None"));
			break;
		case 1:
			window->m_Backup_Ratio_Text->SetLabel(_("Fastest"));
			break;
		case 2:
			window->m_Backup_Ratio_Text->SetLabel(_("Fast"));
			break;
		case 3:
			window->m_Backup_Ratio_Text->SetLabel(_("Default"));
			break;
		case 4:
			window->m_Backup_Ratio_Text->SetLabel(_("Maximum"));
			break;
		case 5:
			window->m_Backup_Ratio_Text->SetLabel(_("Ultra"));
			break;
	}
}
	
#endif
