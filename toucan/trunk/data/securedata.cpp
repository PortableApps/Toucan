/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "securedata.h"
#include "../basicfunctions.h"
#include "../toucan.h"
#include "../variables.h"
#include "../forms/frmmain.h"
#include "../controls/vdtc.h"
#include <wx/fileconf.h>
#include <wx/stdpaths.h>

bool SecureData::TransferFromFile(){
	bool error = false;
	wxString stemp;

	if(!wxGetApp().m_Jobs_Config->Exists(GetName())){
		return false;
	}

	if(wxGetApp().m_Jobs_Config->Read(GetName() + wxT("/Locations"), &stemp)) SetLocations(StringToArrayString(stemp, wxT("|")));
		else error = true;
	if(wxGetApp().m_Jobs_Config->Read(GetName() + wxT("/Function"), &stemp)) SetFunction(ToLang(stemp));
		else error = true;
	if(wxGetApp().m_Jobs_Config->Read(GetName() + wxT("/Rules"), &stemp))  SetRules(new Rules(stemp, true));
		else error = true;

	if(error){
		wxMessageBox(_("There was an error reading from the jobs file"), _("Error"), wxICON_ERROR);
		return false;
	}
	return true;
}

bool SecureData::TransferToFile(){
	bool error = false;

	wxGetApp().m_Jobs_Config->DeleteGroup(GetName());

	if(!wxGetApp().m_Jobs_Config->Write(GetName() + wxT("/Locations"),  ArrayStringToString(GetLocations(), wxT("|")))) error = true;
	if(!wxGetApp().m_Jobs_Config->Write(GetName() + wxT("/Function"), ToEn(GetFunction()))) error = true;
	if(!wxGetApp().m_Jobs_Config->Write(GetName() + wxT("/Rules"), GetRules() ? GetRules()->GetName() : wxT(""))) error = true;
	if(!wxGetApp().m_Jobs_Config->Write(GetName() + wxT("/Type"),  wxT("Secure"))) error = true;

	wxGetApp().m_Jobs_Config->Flush();

	if(error){
		wxMessageBox(_("There was an error saving to the jobs file, \nplease check it is not set as read only or in use"), _("Error"), wxICON_ERROR);
		return false;
	}
	return true;
}

bool SecureData::TransferToForm(frmMain *window){
	if(!window){
		return false;
	}

	window->m_Secure_TreeCtrl->DeleteChildren(window->m_Secure_TreeCtrl->GetRootItem());

	for(unsigned int i = 0; i < window->m_SecureLocations->GetCount(); i++){
		window->m_SecureLocations->RemoveAt(i);
	}
	for(unsigned int k = 0; k < GetLocations().Count(); k++){
		window->m_SecureLocations->Add(GetLocations().Item(k));
		window->m_Secure_TreeCtrl->AddNewPath(Normalise(GetLocations().Item(k)));
	}

	window->m_Secure_Function->SetStringSelection(GetFunction());

	return true;
}

bool SecureData::TransferFromForm(frmMain *window){
	if(!window){
		return false;
	}

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

bool SecureData::IsReady(){
	if(GetLocations().Count() == 0 || GetFunction() == wxEmptyString){
		return false;
	}
	else{
		return true;
	}
}
