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
#include "../controls/virtualdirtreectrl.h"
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

bool SecureData::TransferToFile(){
	bool error = false;

	wxGetApp().m_Jobs_Config->DeleteGroup(GetName());
	wxGetApp().m_Jobs_Config->Flush();

	if(!wxGetApp().m_Jobs_Config->Write(GetName() + wxT("/Locations"),  ArrayStringToString(GetLocations(), wxT("|")))){
		error = true;
	}
	if(!wxGetApp().m_Jobs_Config->Write(GetName() + wxT("/Function"), GetFunction())){
		error = true;
	}

	wxGetApp().m_Jobs_Config->Flush();

	if(error){
		wxMessageBox(_("There was an error saving to the jobs file, \nplease check it is not set as read only or in use."), _("Error"), wxICON_ERROR);
		return false;
	}
	return true;
}

bool SecureData::TransferToForm(){
	frmMain *window = wxGetApp().MainWindow;
	if(window == NULL){
		return false;
	}

	window->m_Secure_TreeCtrl->DeleteAllItems();
	window->m_Secure_TreeCtrl->AddRoot(wxT("Hidden root"));

	for(unsigned int i = 0; i < window->m_SecureLocations->GetCount(); i++){
		window->m_SecureLocations->RemoveAt(i);
	}
	for(unsigned int k = 0; k < GetLocations().Count(); k++){
		window->m_SecureLocations->Add(GetLocations().Item(k));
		window->m_Secure_TreeCtrl->AddNewPath(Normalise(Normalise(GetLocations().Item(k))));
	}

	window->m_Secure_Function->SetStringSelection(GetFunction());
	return true;
}

bool SecureData::TransferFromForm(){
	frmMain *window = wxGetApp().MainWindow;

	SetLocations(*window->m_SecureLocations);
	SetFunction(window->m_Secure_Function->GetStringSelection());

	return true;	
}

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
	bool blFilled = true;
	if(GetLocations().Count() == 0){
		blFilled = false;
	}
	if(GetFunction() == wxEmptyString){
		blFilled = false;
	}
	return blFilled;
}
