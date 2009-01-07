/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2008 Steven Lamerton
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

#include "basicfunctions.h"
#include "frmprogress.h"
#include "frmmain.h"
#include "toucan.h"

wxFFileOutputStream output(stderr);
wxTextOutputStream cout(output);

wxString ArrayStringToString(wxArrayString arrStrings, wxString strSeperator){
	wxString strTemp;
	for(unsigned int i = 0; i < arrStrings.GetCount(); i++){
		strTemp = strTemp + strSeperator + arrStrings.Item(i);
	}
	return strTemp;
}

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

void MessageBox(wxString strMessage, wxString strCaption){
	wxMessageBox(strMessage, strCaption, wxICON_INFORMATION);
}

void ErrorBox(wxString strMessage){
	wxMessageBox(strMessage, _("Error"), wxICON_ERROR);
}

void OutputProgress(wxString strValue){
	if(wxGetApp().blGUI == true){
		wxGetApp().ProgressWindow->m_Text->AppendText(strValue + wxT("\n"));
	}
	else{
		cout<<strValue + wxT("\n");
	}		
}

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

bool SetRulesBox(wxComboBox *box){
	//Clear the existin items incase any are out of date
	box->Clear();

	bool blCont;
	wxString strValue;
	long dummy;
	//Iterate through the groups adding them to the box
	blCont = wxGetApp().m_Rules_Config->GetFirstGroup(strValue, dummy);
	while (blCont){
		if(strValue != wxT("General")){
			box->Append(strValue);
		}
		blCont = wxGetApp().m_Rules_Config->GetNextGroup(strValue, dummy);
	}
	return true;
}

bool SetJobsBox(wxComboBox *box, wxString strType){
	bool blCont;
	wxString strValue;
	long dummy;
	//Iterate through all of the groups
	blCont = wxGetApp().m_Jobs_Config->GetFirstGroup(strValue, dummy);
	while (blCont){
		//If the group is of the correct type then add it to the combobox
		if(wxGetApp().m_Jobs_Config->Read(strValue + wxT("/Type")) == strType){
			box->Append(strValue);
		}
		blCont = wxGetApp().m_Jobs_Config->GetNextGroup(strValue, dummy);
	}
	return true;
}

bool SetVariablesBox(wxComboBox *box){
	bool blCont;
	wxString strValue;
	long dummy;
	//Iterate through all of the groups
	blCont = wxGetApp().m_Variables_Config->GetFirstGroup(strValue, dummy);
	while (blCont){
		box->Append(strValue);
		blCont = wxGetApp().m_Variables_Config->GetNextGroup(strValue, dummy);
	}
	return true;
}



bool SetScriptsBox(wxComboBox *box){
	//Clear the existin items incase any are out of date
	box->Clear();
	bool blCont;
	wxString strValue;
	long dummy;
	//Iterate through the groups adding them to the box
	blCont = wxGetApp().m_Scripts_Config->GetFirstGroup(strValue, dummy);
	while (blCont){
		if(strValue != wxT("General")){
			box->Append(strValue);
		}
		blCont = wxGetApp().m_Scripts_Config->GetNextGroup(strValue, dummy);
	}
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

wxArrayString GetTranslatorNames(){
	wxArrayString arrNames;
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
					arrNames.Add(config->Read(wxT("General/Translator")));
				}
			}
		}
		while (dir.GetNext(&strFilename) );
	} 
	return arrNames;
	
}

wxString InputPassword(){
	wxString strNewPass;
	if(wxGetApp().blGUI == true){
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
		int iArgs = wxGetApp().argc;
		if(iArgs == 4){
			cmdParser.AddParam(_("Job name"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
			cmdParser.AddParam(_("Password"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
			cmdParser.AddParam(_("Repeated password"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
		}
		//Backup job all specified with password
		else if(iArgs == 10){
			cmdParser.AddParam(_("Operation"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
			cmdParser.AddParam(_("Backup file"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
			cmdParser.AddParam(_("File of paths"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
			cmdParser.AddParam(_("Function"),  wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
			cmdParser.AddParam(_("Format"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
			cmdParser.AddParam(_("Compression level"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
			cmdParser.AddParam(_("Rules"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
			cmdParser.AddParam(_("Password"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
			cmdParser.AddParam(_("Repeated password"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
		}
		//Secure all specified
		else if(iArgs == 7){
			cmdParser.AddParam(_("Operation"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
			cmdParser.AddParam(_("File of paths"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
			cmdParser.AddParam(_("Function"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
			cmdParser.AddParam(_("Rules"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);		
			cmdParser.AddParam(_("Password"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
			cmdParser.AddParam(_("Repeated password"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
		}
		int res;
		{
			wxLogNull log;
			res = cmdParser.Parse(false);
		}
		if(cmdParser.GetParam(0) == wxT("Script")){
			if(cmdParser.GetParamCount() == 4 && cmdParser.GetParam(2) == cmdParser.GetParam(3)){
				return cmdParser.GetParam(2);
			}
			else{
				cout<<_("You need to repeat the password and ensure the two passwords are identical");
				return wxEmptyString;
			}
		}
		else if(wxGetApp().m_Jobs_Config->Read(cmdParser.GetParam(0) + wxT("/Type")) == wxT("Secure")){
			if(cmdParser.GetParamCount() == 3 && cmdParser.GetParam(1) == cmdParser.GetParam(2)){
				return cmdParser.GetParam(2);
			}
			else{
				cout<<_("You need to repeat the password and ensure the two passwords are identical");
				return wxEmptyString;
			}
		}
		else if(wxGetApp().m_Jobs_Config->Read(cmdParser.GetParam(0) + wxT("/Type")) == wxT("Backup")){
			if(cmdParser.GetParamCount() == 3){
				if(cmdParser.GetParam(1) == cmdParser.GetParam(2)){
					return cmdParser.GetParam(1);
				}
				else{
					cout<<_("You need to repeat the password and ensure the two passwords are identical");
					return wxEmptyString;
				}
			}
			else{
				return wxT("Password not needed");
			}
		}
		else if(cmdParser.GetParam(0) == wxT("Secure")){
			if(cmdParser.GetParamCount() == 6){
				if(cmdParser.GetParam(4) == cmdParser.GetParam(5)){
					return cmdParser.GetParam(4);
				}
				else{
					cout<<_("You need to repeat the password and ensure the two passwords are identical");
					return wxEmptyString;
				}
			}
		}
		else if(cmdParser.GetParam(0) == wxT("Backup")){
			if(cmdParser.GetParamCount() == 9){
				if(cmdParser.GetParam(7) == cmdParser.GetParam(8)){
					return cmdParser.GetParam(7);
				}
				else{
					cout<<_("You need to repeat the password and ensure the two passwords are identical");
					return wxEmptyString;
				}
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

bool UpdateJobs(int version){
	//Back up the existing file
	if(wxFileExists(wxGetApp().GetSettingsPath() + wxT("Jobs.ini"))){
		wxCopyFile(wxGetApp().GetSettingsPath() + wxT("Jobs.ini"), wxGetApp().GetSettingsPath() + wxT("Jobs.old"), true);		
	}
	if(version == 1){
		bool blCont;
		wxString strValue;
		long dummy;
		//Iterate through the groups adding them to the box
		blCont = wxGetApp().m_Jobs_Config->GetFirstGroup(strValue, dummy);
		while(blCont){
			if(wxGetApp().m_Jobs_Config->Read(strValue + wxT("/Type")) == wxT("Sync")){
				
				wxGetApp().m_Jobs_Config->DeleteEntry(strValue + wxT("/Exclusions"));
				wxGetApp().m_Jobs_Config->DeleteEntry(strValue + wxT("/Preview"));
				
				wxString strTemp;
				strTemp = wxGetApp().m_Jobs_Config->Read(strValue + wxT("/1"));
				if(wxGetApp().m_Jobs_Config->Read(strValue + wxT("/Source")) == wxEmptyString){
					wxGetApp().m_Jobs_Config->Write(strValue + wxT("/Source"), strTemp);
				}
				strTemp = wxGetApp().m_Jobs_Config->Read(strValue + wxT("/2"));
				if(wxGetApp().m_Jobs_Config->Read(strValue + wxT("/Dest")) == wxEmptyString){
					wxGetApp().m_Jobs_Config->Write(strValue + wxT("/Dest"), strTemp);
				}
				
				wxGetApp().m_Jobs_Config->DeleteEntry(strValue + wxT("/1"));
				wxGetApp().m_Jobs_Config->DeleteEntry(strValue + wxT("/2"));
				wxGetApp().m_Jobs_Config->Write(strValue + wxT("/IgnoreReadOnly"), 0);
				wxGetApp().m_Jobs_Config->Write(strValue + wxT("/IgnoreDaylightSavings"), 0);
				wxGetApp().m_Jobs_Config->Flush();
			}
			else if(wxGetApp().m_Jobs_Config->Read(strValue + wxT("/Type")) == wxT("Backup")){
				wxGetApp().m_Jobs_Config->DeleteEntry(strValue + wxT("/Exclusions"));
				
				wxString strTemp;
				strTemp = wxGetApp().m_Jobs_Config->Read(strValue + wxT("/1"));
				if(wxGetApp().m_Jobs_Config->Read(strValue + wxT("/Locations")) == wxEmptyString){
					wxGetApp().m_Jobs_Config->Write(strValue + wxT("/Locations"), wxT("#") + strTemp);
				}
				strTemp = wxGetApp().m_Jobs_Config->Read(strValue + wxT("/2"));
				if(wxGetApp().m_Jobs_Config->Read(strValue + wxT("/BackupLocation")) == wxEmptyString){
					wxGetApp().m_Jobs_Config->Write(strValue + wxT("/BackupLocation"), strTemp);
				}
				wxGetApp().m_Jobs_Config->DeleteEntry(strValue + wxT("/1"));
				wxGetApp().m_Jobs_Config->DeleteEntry(strValue + wxT("/2"));
				
				if(wxGetApp().m_Jobs_Config->Read(strValue + wxT("/Function")) == wxT("Incremental")){
					wxGetApp().m_Jobs_Config->Write(strValue + wxT("/Function"), _("Differential"));
				}
				if(wxGetApp().m_Jobs_Config->Read(strValue + wxT("/Format")) == wxT("7 Zip")){
					wxGetApp().m_Jobs_Config->Write(strValue + wxT("/Format"), _("7-Zip"));
				}
				if(wxGetApp().m_Jobs_Config->Read(strValue + wxT("/Ratio")) == wxT("Normal")){
					wxGetApp().m_Jobs_Config->Write(strValue + wxT("/Ratio"), wxT("3"));
				}
				if(wxGetApp().m_Jobs_Config->Read(strValue + wxT("/Ratio")) == wxT("Ultra")){
					wxGetApp().m_Jobs_Config->Write(strValue + wxT("/Ratio"), wxT("5"));
				}
				wxGetApp().m_Jobs_Config->Write(strValue + wxT("/IsPass"), 0);
				wxGetApp().m_Jobs_Config->Flush();
			}
			else if(wxGetApp().m_Jobs_Config->Read(strValue + wxT("/Type")) == wxT("Secure")){
				wxString strTemp;
				strTemp = wxGetApp().m_Jobs_Config->Read(strValue + wxT("/Files"));
				strTemp.Replace(wxT("|"), wxT("#"));
				if(wxGetApp().m_Jobs_Config->Read(strValue + wxT("/Locations")) == wxEmptyString){
					wxGetApp().m_Jobs_Config->Write(strValue + wxT("/Locations"), strTemp);
				}
				wxGetApp().m_Jobs_Config->DeleteEntry(strValue + wxT("/Files"));
				strTemp = wxGetApp().m_Jobs_Config->Read(strValue + wxT("/Routine"));
				if(wxGetApp().m_Jobs_Config->Read(strValue + wxT("/Format")) == wxEmptyString){
					wxGetApp().m_Jobs_Config->Write(strValue + wxT("/Format"), strTemp);
				}
				wxGetApp().m_Jobs_Config->DeleteEntry(strValue + wxT("/Routine"));
				
				wxGetApp().m_Jobs_Config->Flush();
			}
				
			blCont = wxGetApp().m_Jobs_Config->GetNextGroup(strValue, dummy);
		}
		wxGetApp().m_Jobs_Config->Flush();
		version = 200;
	}
	if(version == 200){
		bool blCont;
		wxString strValue;
		long dummy;
		//Iterate through the groups adding them to the box
		blCont = wxGetApp().m_Jobs_Config->GetFirstGroup(strValue, dummy);
		while (blCont){
			if(wxGetApp().m_Jobs_Config->Read(strValue + wxT("/Type")) == wxT("Backup")){
				wxString strTemp;
				strTemp = wxGetApp().m_Jobs_Config->Read(strValue + wxT("/Locations"));
				strTemp.Replace(wxT("#"), wxT("|"));		
				wxGetApp().m_Jobs_Config->Write(strValue + wxT("/Locations"), strTemp);
				wxGetApp().m_Jobs_Config->Flush();
			}
			else if(wxGetApp().m_Jobs_Config->Read(strValue + wxT("/Type")) == wxT("Secure")){
				wxString strTemp;
				strTemp = wxGetApp().m_Jobs_Config->Read(strValue + wxT("/Locations"));
				strTemp.Replace(wxT("#"), wxT("|"));
				wxGetApp().m_Jobs_Config->Write(strValue + wxT("/Locations"), strTemp);
				wxGetApp().m_Jobs_Config->Flush();
			}
				
			blCont = wxGetApp().m_Jobs_Config->GetNextGroup(strValue, dummy);
		}
		wxGetApp().m_Jobs_Config->Flush();
	}
	return true;
}

bool UpdateRules(int version){
	//Back up the existing file
	if(wxFileExists(wxGetApp().GetSettingsPath() + wxT("Rules.ini"))){
		wxCopyFile(wxGetApp().GetSettingsPath() + wxT("Rules.ini"), wxGetApp().GetSettingsPath() + wxT("Rules.old"), true);		
	}
	if(version == 1){
		bool blCont;
		wxString strValue;
		long dummy;
		//Iterate through the groups adding them to the box
		blCont = wxGetApp().m_Rules_Config->GetFirstGroup(strValue, dummy);
		while (blCont){
			if(strValue != wxT("General")){
				if(wxGetApp().m_Rules_Config->Read(strValue + wxT("/FilesToInclude")) != wxEmptyString){
					wxString strTemp;
					strTemp = wxGetApp().m_Rules_Config->Read(strValue + wxT("/FilesToInclude"));
					strTemp.Replace(wxT("#"), wxT("|"));
					wxGetApp().m_Rules_Config->Write(strValue + wxT("/FilesToInclude"), strTemp);
					wxGetApp().m_Rules_Config->Flush();
				}	
				if(wxGetApp().m_Rules_Config->Read(strValue + wxT("/FilesToExclude")) != wxEmptyString){
					wxString strTemp;
					strTemp = wxGetApp().m_Rules_Config->Read(strValue + wxT("/FilesToExclude"));
					strTemp.Replace(wxT("#"), wxT("|"));
					wxGetApp().m_Rules_Config->Write(strValue + wxT("/FilesToExclude"), strTemp);
					wxGetApp().m_Rules_Config->Flush();
				}	
				if(wxGetApp().m_Rules_Config->Read(strValue + wxT("/FoldersToExclude")) != wxEmptyString){
					wxString strTemp;
					strTemp = wxGetApp().m_Rules_Config->Read(strValue + wxT("/FoldersToExclude"));
					strTemp.Replace(wxT("#"), wxT("|"));
					wxGetApp().m_Rules_Config->Write(strValue + wxT("/FoldersToExclude"), strTemp);
					wxGetApp().m_Rules_Config->Flush();
				}			
			}
			blCont = wxGetApp().m_Rules_Config->GetNextGroup(strValue, dummy);
		}
		wxGetApp().m_Rules_Config->Flush();
	}
	return true;
}

bool UpdateScripts(int version){
	//Back up the existing file	
	if(wxFileExists(wxGetApp().GetSettingsPath() + wxT("Scripts.ini"))){
		wxCopyFile(wxGetApp().GetSettingsPath() + wxT("Scripts.ini"), wxGetApp().GetSettingsPath() + wxT("Scripts.old"), true);		
	}
	if(version == 1){
		bool blCont;
		wxString strValue;
		long dummy;
		blCont = wxGetApp().m_Scripts_Config->GetFirstGroup(strValue, dummy);
		while (blCont){
			if(strValue != wxT("General")){
				if(wxGetApp().m_Scripts_Config->Read(strValue + wxT("/Script")) != wxEmptyString){
					wxString strTemp;
					strTemp = wxGetApp().m_Scripts_Config->Read(strValue + wxT("/Script"));
					strTemp.Replace(wxT("#"), wxT("|"));
					wxGetApp().m_Scripts_Config->Write(strValue + wxT("/Script"), strTemp);
					wxGetApp().m_Scripts_Config->Flush();
				}			
			}
			blCont = wxGetApp().m_Scripts_Config->GetNextGroup(strValue, dummy);
		}
		wxGetApp().m_Scripts_Config->Flush();
	}
	return true;
}
