/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007 - 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include "securedata.h"
#include "../basicfunctions.h"
#include "../toucan.h"
#include "../rules.h"
#include "../variables.h"
#include "../forms/frmmain.h"
#include "../controls/previewctrl.h"
#include <wx/fileconf.h>
#include <wx/stdpaths.h>
#include <wx/msgdlg.h>
#include <wx/combobox.h>
#include <wx/radiobox.h>
#include <wx/checkbox.h>

void SecureData::TransferFromFile(){
	if(!wxGetApp().m_Jobs_Config->Exists(GetName())){
		throw std::invalid_argument(std::string(GetName() + " is not a valid job"));
	}

	SetLocations(Read("Locations"));
	SetFunction(Read<wxString>("Function"));
	SetRules(new Rules(Read<wxString>("Rules"), true));
}

void SecureData::TransferToFile(){
	wxGetApp().m_Jobs_Config->DeleteGroup(GetName());

	Write("Locations", GetLocations());
	Write("Function", ToEn(GetFunction()));
	Write("Rules", GetRules() ? GetRules()->GetName() : wxT(""));
	Write("Type", "Secure");

	if(!wxGetApp().m_Jobs_Config->Flush()){
		throw std::runtime_error("There was an error flushing the jobs file");
	}
}

bool SecureData::TransferToForm(frmMain *window){
	if(!window){
		return false;
	}

	if(GetName() != wxT("SecureRemember") && window->m_Secure_Job_Select->GetStringSelection() != GetName()){
		if(window->m_Secure_Job_Select->FindString(GetName(), true) == wxNOT_FOUND){
			window->m_Secure_Job_Select->AppendString(GetName());
		}
		window->m_Secure_Job_Select->SetStringSelection(GetName());
	}

	window->m_Secure_TreeCtrl->DeleteChildren(window->m_Secure_TreeCtrl->GetRootItem());

	for(unsigned int i = 0; i < window->m_SecureLocations->GetCount(); i++){
		window->m_SecureLocations->RemoveAt(i);
	}
	for(unsigned int k = 0; k < GetLocations().Count(); k++){
		window->m_SecureLocations->Add(GetLocations().Item(k));
		window->m_Secure_TreeCtrl->AddItem(Normalise(GetLocations().Item(k)));
	}

	window->m_Secure_Function->SetStringSelection(GetFunction());
	window->m_Secure_Rules->SetStringSelection(GetRules()->GetName());

	//Notify the window that we have updated the rules
	wxCommandEvent event;
	window->OnSecureRulesSelected(event);

	return true;
}

bool SecureData::TransferFromForm(frmMain *window){
	if(!window){
		return false;
	}

	SetLocations(*window->m_SecureLocations);
	SetFunction(window->m_Secure_Function->GetStringSelection());
	SetRules(new Rules(window->m_Secure_Rules->GetStringSelection(), true));

	return true;	
}
