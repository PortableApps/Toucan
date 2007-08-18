/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007 Steven Lamerton
// Licence:     GNU GPL 2 (See readme for more info
/////////////////////////////////////////////////////////////////////////////////

#include <wx/textfile.h>
#include <wx/file.h>
#include <wx/fileconf.h>
#include <wx/arrstr.h>

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
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxT("\\Data\\Variables.ini") );
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
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxT("\\Data\\Jobs.ini") );
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
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxT("\\Data\\Settings.ini") );
	wxFileConfig::Set( config );
	config->Write(wxT("General/Tabstyle") , window->m_TabStyle->GetStringSelection());
	config->Write(wxT("General/Tooltip") , window->m_Tooltip->GetStringSelection());
	config->Flush();
	return true;
}

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