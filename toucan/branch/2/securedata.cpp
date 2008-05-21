/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "securedata.h"
#include "basicfunctions.h"
#include "toucan.h"
#include <wx/fileconf.h>
#include <wx/stdpaths.h>

//Grab all of the fields from the form*/
bool SecureData::TransferFromFile(wxString strName){
	//Create a new ficonfig 	
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""),  wxGetApp().GetSettingsPath()+ wxT("Jobs.ini"));
	
	bool blError;
	wxString strTemp;

	//Read the fields from the file, if the read works then add them
	blError = config->Read(strName + wxT("/Locations"), &strTemp);
	if(blError){ SetLocations(StringToArrayString(strTemp, wxT("#"))); }	
	blError = config->Read(strName + wxT("/Function"), &strTemp);
	if(blError){ SetFunction(strTemp); }
	blError = config->Read(strName + wxT("/Format"), &strTemp);
	if(blError){ SetFormat(strTemp); }
	//Delete the fileconfig as we are finished with it	
	delete config;

	//If error output a message
	if(!blError){
		ErrorBox(_("There was an error reading from the jobs file, \nplease check it is not set as read only or in use."));
		return false;
	}
	return true;
}

//For saving to an ini file
bool SecureData::TransferToFile(wxString strName){
	//Create a new fileconfig object
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""),  wxGetApp().GetSettingsPath()+ wxT("Jobs.ini"));
	
	bool blError;
	//Delete a group if it already exists
	blError = config->DeleteGroup(strName);
	//Write the fields to the ini file
	blError = config->Write(strName + wxT("/Locations"),  ArrayStringToString(GetLocations(), wxT("#")));	
	blError = config->Write(strName + wxT("/Function"), GetFunction());	
	blError = config->Write(strName + wxT("/Format"), GetFormat());	
	
	//Write the contents to the file and then delete the fileconfig object
	config->Flush();
	delete config;

	//Output an error message if needed
	if(!blError){
		ErrorBox(wxT("There was an error saving to the jobs file, \nplease check it is not set as read only or in use."));
		return false;
	}
	return true;
}

/*This function takes the data in BackupData and fills in the GUI*/
void SecureData::TransferToForm(frmMain *window){
	//Delete all of the existing items in the treectrl and readd a root
	window->m_Secure_TreeCtrl->DeleteAllItems();
	window->m_Secure_TreeCtrl->AddRoot(wxT("Hidden root"));
	//Remove all of the filepaths from the list
	for(unsigned int i = 0; i < wxGetApp().MainWindow->m_SecureLocations->GetCount(); i++){
		wxGetApp().MainWindow->m_SecureLocations->RemoveAt(i);
	}
	//Add the new filepaths to the list and the treectrl
	for(unsigned int k = 0; k < GetLocations().Count(); k++){
		wxGetApp().MainWindow->m_SecureLocations->Add(GetLocations().Item(k));
		window->m_Secure_TreeCtrl->AddNewPath(GetLocations().Item(k));
	}
	//Set up the rest of the window
	window->m_Secure_Function->SetStringSelection(GetFunction());
	window->m_Secure_Format->SetStringSelection(GetFormat());
	window->m_Secure_Pass->SetValue(GetPass());
	window->m_Secure_Repass->SetValue(GetPass());
	return;
}

/* This function sets all of the fields in SyncData based on the user inputted data in the
Main program window.*/
bool SecureData::TransferFromForm(frmMain *window, bool blShowError){
	bool blNotFilled = false;
	wxString strPass, strRepass;

	//Set the global secure path lists and make sure there are some locations
	SetLocations(*wxGetApp().MainWindow->m_SecureLocations);
	if(GetLocations().GetCount() == 0){ blNotFilled = true; }

	//Get the function, ensuring something is selected
	if(window->m_Secure_Function->GetStringSelection() != wxEmptyString){ SetFunction(window->m_Secure_Function->GetStringSelection()); }
	else{ blNotFilled = true; }

	//Get the format, ensuring something is selected
	if(window->m_Secure_Format->GetStringSelection() != wxEmptyString){ SetFormat(window->m_Secure_Format->GetStringSelection()) ; }
	else{ blNotFilled = true; }
	
	//Get the two passowrd, but do not add them to the data
	if(window->m_Secure_Pass->GetValue() != wxEmptyString){strPass = window->m_Secure_Pass->GetValue(); }
	else{ blNotFilled = true; }
	if(window->m_Secure_Repass->GetValue() != wxEmptyString){strRepass = window->m_Secure_Repass->GetValue(); }
	else{ blNotFilled = true; }

	//Output an error message if needed
	if(blNotFilled && blShowError){
		ErrorBox(_("Not all of the required fields are filled"));
		return false;
	}

	//If the two passwords are the same then add them to the data
	if(strPass == strRepass){
		SetPass(strPass);
	}
	else{
		ErrorBox(_("The passwords to not match, please try again."));
		return false;
	}
	return true;	
}

/*Output function, for debugging only*/
void SecureData::Output(){
	for(unsigned int i = 0; i < GetLocations().GetCount(); i++){
		MessageBox(GetLocations().Item(i), wxT("Location"));
	}
	MessageBox(GetFunction(), wxT("Function"));
	MessageBox(GetFormat(), wxT("Format"));
	MessageBox(GetPass(), wxT("Pass"));
}

