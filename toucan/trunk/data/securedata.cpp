/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "securedata.h"
#include "../basicfunctions.h"
#include "../toucan.h"
#include "../variables.h"
#include "../secure.h"
#include "../forms/frmmain.h"
#include <wx/fileconf.h>
#include <wx/stdpaths.h>

bool SecureData::TransferFromFile(){
	bool error = false;
	wxString strTemp;

	if(!wxGetApp().m_Jobs_Config->Exists(GetName())){
		return false;
	}

	if(wxGetApp().m_Jobs_Config->Read(GetName() + wxT("/Locations"), &strTemp)){
		SetLocations(StringToArrayString(strTemp, wxT("|")));
	}
	else{ error = true; }
	if(wxGetApp().m_Jobs_Config->Read(GetName() + wxT("/Function"), &strTemp)){
		SetFunction(strTemp);
	}
	else{ error = true; }

	if(error){
		wxMessageBox(_("There was an error reading from the jobs file, \nplease check it is not set as read only or in use."), _("Error"), wxICON_ERROR);
		return false;
	}
	return true;
}

//For saving to an ini file
bool SecureData::TransferToFile(){
	//ATTN : Needs cleanup
	bool blError;
	//Delete a group if it already exists
	blError = wxGetApp().m_Jobs_Config->DeleteGroup(GetName());
	//Write the fields to the ini file
	blError = wxGetApp().m_Jobs_Config->Write(GetName() + wxT("/Locations"),  ArrayStringToString(GetLocations(), wxT("|")));	
	blError = wxGetApp().m_Jobs_Config->Write(GetName() + wxT("/Function"), GetFunction());	

	//Write the contents to the file
	wxGetApp().m_Jobs_Config->Flush();

	//Output an error message if needed
	if(!blError){
		wxMessageBox(_("There was an error saving to the jobs file, \nplease check it is not set as read only or in use."), _("Error"), wxICON_ERROR);
		return false;
	}
	return true;
}

/*This function takes the data in BackupData and fills in the GUI*/
bool SecureData::TransferToForm(){
	//ATTN : Need to move to all use window pointer
	frmMain *window = wxGetApp().MainWindow;
	if(window == NULL){
		return false;
	}
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
		window->m_Secure_TreeCtrl->AddNewPath(Normalise(Normalise(GetLocations().Item(k))));
	}
	//Set up the rest of the window
	window->m_Secure_Function->SetStringSelection(GetFunction());
	return true;
}

/* This function sets all of the fields in SyncData based on the user inputted data in the
Main program window.*/
bool SecureData::TransferFromForm(){
	//ATTN : Need to move to all use window pointer
	frmMain *window = wxGetApp().MainWindow;

	SetLocations(*wxGetApp().MainWindow->m_SecureLocations);
	SetFunction(window->m_Secure_Function->GetStringSelection());

	return true;	
}

/*Output function, for debugging only*/
void SecureData::Output(){
	for(unsigned int i = 0; i < GetLocations().GetCount(); i++){
		wxMessageBox(GetLocations().Item(i), wxT("Location"));
	}
	wxMessageBox(GetFunction(), wxT("Function"));
	wxMessageBox(GetPassword(), wxT("Pass"));
}

bool SecureData::Execute(Rules rules){
	for(unsigned int i = 0; i < GetLocations().GetCount(); i++){
		SetLocation(i, Normalise(Normalise(GetLocation(i))));
	}
	Secure(*this, rules, wxGetApp().ProgressWindow);
	return true;
}


bool SecureData::NeededFieldsFilled(){
	//ATTN : Needs cleanup
	bool blFilled = true;
	if(GetLocations().Count() == 0){
		blFilled = false;
	}
	if(GetFunction() == wxEmptyString){
		blFilled = false;
	}
	return blFilled;
}
