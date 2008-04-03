/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007 Steven Lamerton
// Licence:     GNU GPL 2 (See readme for more info
/////////////////////////////////////////////////////////////////////////////////

#ifndef _BASICOPS_H_
#define _BASICOPS_H_

#include <wx/textfile.h>
#include <wx/file.h>
#include <wx/fileconf.h>
#include <wx/arrstr.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>

#include "frmprogress.h"
#include "frmmain.h"

wxFFileOutputStream output( stderr );
wxTextOutputStream cout( output );

/*PrepareTextFile is a destructive routine that either creates a new text file
of the name or clears an existing one*/
bool PrepareTextFile(wxString strFileName)
{
    wxTextFile file;
    if(wxFile::Exists(strFileName))
    {	
        file.Open(strFileName);
        file.Clear();
        file.Write();
    }
    else
    {
        file.Create(strFileName);
    }
    file.Close();
    return true;
}

/*GetVariables returns a wxArrayString containing the names of all the
existing Portable Variables and is used be the insert Portable Variable buttons.*/
wxArrayString GetVariables()
{
   // wxMessageBox(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Left(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Length() - 11));
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Left(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Length() - 11) + wxT("\\Data\\Variables.ini") );
	wxFileConfig::Set( config );	
	wxString str;
	long dummy;
	wxArrayString arrVars;
	bool bCont = config->GetFirstGroup(str, dummy);
	while ( bCont ) {
   	arrVars.Add(wxT("@") + str + wxT("@"));
	bCont = config->GetNextGroup(str, dummy);
	}
	arrVars.Add(wxT("@docs@"));
	arrVars.Add(wxT("@drive@"));
	arrVars.Add(wxT("@date@"));
	arrVars.Add(wxT("@time@"));
	return arrVars;
}

/*JobNameExists is used when creating a new job to ensure name individuality,*/
bool JobNameExists(wxString strName)
{
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""),wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Left(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Length() - 11) + wxT("\\Data\\Jobs.ini"));
	wxFileConfig::Set( config );
	if(config->HasGroup(strName)){
	return true;
	}
	else{
	return false;
	}
}

/*Save the settings of the main form*/
bool SaveSettings(frmMain* window)
{
    wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Left(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Length() - 11) + wxT("\\Data\\Settings.ini"));
	wxFileConfig::Set( config );
	config->Write(wxT("General/Tabstyle") , window->m_TabStyle->GetStringSelection());
	config->Write(wxT("General/Tooltip") , window->m_Tooltip->GetStringSelection());
    if(window->m_Lang->GetStringSelection() == wxT("English")){
        config->Write(wxT("General/Language") , wxT("wxLANGUAGE_ENGLSH")); 
    }
    if(window->m_Lang->GetStringSelection() == wxT("Deutsch")){
        config->Write(wxT("General/Language") , wxT("wxLANGUAGE_GERMAN"));
    }
    if(window->m_Lang->GetStringSelection() == wxT("Francais")){
        config->Write(wxT("General/Language") , wxT("wxLANGUAGE_FRENCH"));
    }
    if(window->m_Lang->GetStringSelection() == wxT("Nederlands")){
        config->Write(wxT("General/Language") , wxT("wxLANGUAGE_DUTCH"));
    }
    if(window->m_Lang->GetStringSelection() == wxT("Eesti")){
        config->Write(wxT("General/Language") , wxT("wxLANGUAGE_ESTONIAN"));
    }
    if(window->m_Lang->GetStringSelection() == wxT("Espanol")){
        config->Write(wxT("General/Language") , wxT("wxLANGUAGE_SPANISH"));
    }
    if(window->m_Lang->GetStringSelection() == wxT("Nihongo")){
        config->Write(wxT("General/Language") , wxT("wxLANGUAGE_JAPANESE"));
    }
	config->Flush();
    return true;
}

/*Insert a portable variable using a single choice popup box*/
wxString InsertPV()
{
	wxArrayString arrPortVar = GetVariables();
	int iSel = wxGetSingleChoiceIndex(_("Please select the portable variable that you would like"), _("Insert Portable Variable"), arrPortVar, NULL);
	if(iSel != -1){
        return arrPortVar.Item(iSel);
	}
	else{
        return wxEmptyString;
	}
}

/*This function sends the correct output based on whether the app is in console mode or not*/
bool OutputProgress(wxString strMessage, frmProgress* window, bool blVisible)
{
    if(blVisible){
        window->m_Progress_Text->AppendText(strMessage + wxT("\n"));
    }
    else{
        cout<<strMessage + wxT("\n");
    }        
    return true;
}

/*Checks to see if a given string matches any of the specified exclusions*/
bool IsExcluded(wxString strFileName, wxArrayString arrExclusions, bool blDir){
	strFileName = strFileName.MakeLower();
    //wxMessageBox(strFileName);
	bool blExclude = false;
	for(int i = 0; i < arrExclusions.GetCount(); i++){
		arrExclusions.Item(i).MakeLower();
	}
	wxFileName flFileName(strFileName);
	if(blDir){
		//For each exclusion
        wxString strTemp;
        if(arrExclusions.GetCount() != 0){
            for(int j = 0; j < arrExclusions.GetCount(); j++){
				if(arrExclusions.Item(j) != wxEmptyString){		
					if(arrExclusions.Item(j) == strFileName.BeforeLast(wxFILE_SEP_PATH).AfterLast(wxFILE_SEP_PATH)){
						blExclude = true;
					}
				}
            }	
        }
        if(arrExclusions.GetCount() != 0){
            for(int j = 0; j < arrExclusions.GetCount(); j++){
				if(arrExclusions.Item(j) != wxEmptyString){	
					if(arrExclusions.Item(j) == strFileName.AfterLast(wxFILE_SEP_PATH)){
						blExclude = true;
					}
				}				
            }	
        }
	}
	else{
        //wxMessageBox(_("IS file"));
		for(int j = 0; j < arrExclusions.GetCount(); j++){
			if(arrExclusions.Item(j) == wxT(".") + flFileName.GetExt()){
				blExclude = true;
			}
		}
        for(int j = 0; j < arrExclusions.GetCount(); j++){
                if(arrExclusions.Item(j) == strFileName.BeforeLast(wxFILE_SEP_PATH).AfterLast(wxFILE_SEP_PATH)){
                    blExclude = true;
                }
            }	
	}
	return blExclude;
}

bool IsLocationOK(int i, frmMain* window, wxString strType){
    if(strType == _("Sync")){
        if(i == 0){
            wxFileName fl = window->m_Sync_First->GetValue();
            if(fl.IsDir()){
                return true;
            }
            else{
                return false;
            }
        }
        if(i == 1){
            wxFileName fl = window->m_Sync_Second->GetValue();
            if(fl.IsDir()){
                return true;
            }
            else{
                return false;
            }
        }
        
    }
   else if(strType == _("Backup")){
        if(i == 0){
            wxFileName fl = window->m_Backup_First->GetValue();
            if(window->m_BackupType->GetStringSelection() == _("Complete")){
                if(wxFileName(window->m_Backup_First->GetValue()).IsDir()){
                    return true;
                }
                else{
                    return false;
                }
            }
            else if(window->m_BackupType->GetStringSelection() == _("Update")){
                if(fl.IsDir()){
                    return true;
                }
                else{
                    return false;
                }
            }
            else if(window->m_BackupType->GetStringSelection() == _("Restore")){
                if(!fl.IsDir()){
                    return true;
                }
                else{
                    return false;
                }	
            }
            else if(window->m_BackupType->GetStringSelection() == _("Incremental")){
                if(fl.IsDir()){
                    return true;
                }
                else{
                    return false;
                }
            }
        }
        if(i == 1){
            wxFileName fl = window->m_Backup_First->GetValue();
            //wxMessageBox(window->m_Backup_First->GetValue());
            if(window->m_BackupType->GetStringSelection() == _("Complete")){
                if(wxFileName(window->m_Backup_First->GetValue()).IsDir()){
                    return true;
                }
                else{
                    return false;
                }
            }
            else if(window->m_BackupType->GetStringSelection() == _("Update")){
                if(!fl.IsDir()){
                    return true;
                }
                else{
                    return false;
                }
            }
            else if(window->m_BackupType->GetStringSelection() == _("Restore")){
                if(!fl.IsDir()){
                    return true;
                }
                else{
                    return false;
                }	
            }
            else if(window->m_BackupType->GetStringSelection() == _("Incremental")){
                if(fl.IsDir()){
                    return true;
                }
                else{
                    return false;
                }
            }
        }
    }
}
    


#endif