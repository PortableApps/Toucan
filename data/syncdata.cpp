/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007 - 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include "syncdata.h"
#include "../rules.h"
#include "../toucan.h"
#include "../variables.h"
#include "../forms/frmmain.h"
#include "../controls/syncctrl.h"
#include <wx/variant.h>
#include <wx/fileconf.h>
#include <wx/msgdlg.h>
#include <wx/combobox.h>
#include <wx/radiobox.h>
#include <wx/checkbox.h>

bool SyncData::TransferFromFile(){
	bool error = false;
	bool btemp;
	wxString stemp;

	if(!wxGetApp().m_Jobs_Config->Exists(GetName())){
		return false;
	}

	if(wxGetApp().m_Jobs_Config->Read(GetName() + wxT("/Source"), &stemp)) SetSource(stemp);
		else error = true;
	if(wxGetApp().m_Jobs_Config->Read(GetName() + wxT("/Dest"), &stemp))  SetDest(stemp);
		else error = true;
	if(wxGetApp().m_Jobs_Config->Read(GetName() + wxT("/Function"), &stemp))  SetFunction(ToLang(stemp));
		else error = true;
	if(wxGetApp().m_Jobs_Config->Read(GetName() + wxT("/CheckSize"), &btemp))  SetCheckSize(btemp);
		else error = true;
	if(wxGetApp().m_Jobs_Config->Read(GetName() + wxT("/CheckTime"), &btemp))  SetCheckTime(btemp);
		else error = true;
	if(wxGetApp().m_Jobs_Config->Read(GetName() + wxT("/CheckShort"), &btemp))  SetCheckShort(btemp);
		else error = true;
	if(wxGetApp().m_Jobs_Config->Read(GetName() + wxT("/CheckFull"), &btemp))  SetCheckFull(btemp);
		else error = true;
	if(wxGetApp().m_Jobs_Config->Read(GetName() + wxT("/TimeStamps"), &btemp))  SetTimeStamps(btemp);
		else error = true;
	if(wxGetApp().m_Jobs_Config->Read(GetName() + wxT("/Attributes"), &btemp))  SetAttributes(btemp);
		else error = true;
	if(wxGetApp().m_Jobs_Config->Read(GetName() + wxT("/IgnoreReadOnly"), &btemp))  SetIgnoreRO(btemp);
		else error = true;
	if(wxGetApp().m_Jobs_Config->Read(GetName() + wxT("/IgnoreDaylightSavings"), &btemp))  SetIgnoreDLS(btemp);
		else error = true;
	if(wxGetApp().m_Jobs_Config->Read(GetName() + wxT("/Recycle"), &btemp))  SetRecycle(btemp);
		else error = true;
	if(wxGetApp().m_Jobs_Config->Read(GetName() + wxT("/Rules"), &stemp))  SetRules(new Rules(stemp, true));
		else error = true;

	if(error){
		wxMessageBox(_("There was an error reading from the jobs file"), _("Error"), wxICON_ERROR);
		return false;
	}
	return true;
}

bool SyncData::TransferToFile(){
	bool error = false;

	wxGetApp().m_Jobs_Config->DeleteGroup(GetName());

	if(!wxGetApp().m_Jobs_Config->Write(GetName() + wxT("/Source"),  GetSource())) error = true;	
	if(!wxGetApp().m_Jobs_Config->Write(GetName() + wxT("/Dest"), GetDest())) error = true;
	if(!wxGetApp().m_Jobs_Config->Write(GetName() + wxT("/Function"), ToEn(GetFunction()))) error = true;
	if(!wxGetApp().m_Jobs_Config->Write(GetName() + wxT("/CheckSize"), GetCheckSize())) error = true;
	if(!wxGetApp().m_Jobs_Config->Write(GetName() + wxT("/CheckTime"), GetCheckTime())) error = true;
	if(!wxGetApp().m_Jobs_Config->Write(GetName() + wxT("/CheckShort"), GetCheckShort())) error = true;
	if(!wxGetApp().m_Jobs_Config->Write(GetName() + wxT("/CheckFull"), GetCheckFull())) error = true;
	if(!wxGetApp().m_Jobs_Config->Write(GetName() + wxT("/TimeStamps"), GetTimeStamps())) error = true;
	if(!wxGetApp().m_Jobs_Config->Write(GetName() + wxT("/Attributes"), GetAttributes())) error = true;
	if(!wxGetApp().m_Jobs_Config->Write(GetName() + wxT("/IgnoreReadOnly"), GetIgnoreRO())) error = true;
	if(!wxGetApp().m_Jobs_Config->Write(GetName() + wxT("/IgnoreDaylightSavings"), GetIgnoreDLS())) error = true;
	if(!wxGetApp().m_Jobs_Config->Write(GetName() + wxT("/Recycle"), GetRecycle())) error = true;
	if(!wxGetApp().m_Jobs_Config->Write(GetName() + wxT("/Rules"), GetRules() ? GetRules()->GetName() : wxT(""))) error = true;
	if(!wxGetApp().m_Jobs_Config->Write(GetName() + wxT("/Type"),  wxT("Sync"))) error = true;

	wxGetApp().m_Jobs_Config->Flush();

	if(error){
		wxMessageBox(_("There was an error saving to the jobs file, \nplease check it is not set as read only or in use"), _("Error"), wxICON_ERROR);
		return false;
	}
	return true;
}

bool SyncData::TransferToForm(frmMain *window){
	if(!window){
		return false;
	}

	//If the job name isnt in the list then we need to add it
	if(GetName() != wxT("SyncRemember") && window->m_Sync_Job_Select->GetStringSelection() != GetName()){
		if(window->m_Sync_Job_Select->FindString(GetName(), true) == wxNOT_FOUND){
			window->m_Sync_Job_Select->AppendString(GetName());
		}
		window->m_Sync_Job_Select->SetStringSelection(GetName());
	}

	window->m_Sync_Source_Txt->SetValue(GetSource());
	window->m_Sync_Source_Tree->DeleteAllItems();
	//ATTN : Move the wxEmptyString checks into the DirCtrl code 
	if(GetSource() != wxEmptyString){
		window->m_Sync_Source_Tree->AddItem(Normalise(GetSource()));		
	}

	window->m_Sync_Dest_Txt->SetValue(GetDest());
	window->m_Sync_Dest_Tree->DeleteChildren(window->m_Sync_Dest_Tree->GetRootItem());
	if(GetDest() != wxEmptyString){
		window->m_Sync_Dest_Tree->AddItem(Normalise(GetDest()));		
	}

	window->m_Sync_Function->SetStringSelection(GetFunction());
	window->m_SyncCheckSize->SetValue(GetCheckSize());
	window->m_SyncCheckTime->SetValue(GetCheckTime());
	window->m_SyncCheckShort->SetValue(GetCheckShort());
	window->m_SyncCheckFull->SetValue(GetCheckFull());
	window->m_Sync_Timestamp->SetValue(GetTimeStamps());
	window->m_Sync_Attributes->SetValue(GetAttributes());
	window->m_Sync_Ignore_Readonly->SetValue(GetIgnoreRO());
	window->m_Sync_Ignore_DaylightS->SetValue(GetIgnoreDLS());
	window->m_Sync_Recycle->SetValue(GetRecycle());
	window->m_Sync_Rules->SetStringSelection(GetRules()->GetName());
	return true;
}

bool SyncData::TransferFromForm(frmMain *window){
	if(!window){
		return false;
	}
	SetSource(window->m_Sync_Source_Txt->GetValue());
	SetDest(window->m_Sync_Dest_Txt->GetValue());
	SetFunction(window->m_Sync_Function->GetStringSelection());
	SetCheckSize(window->m_SyncCheckSize->GetValue());
	SetCheckTime(window->m_SyncCheckTime->GetValue());
	SetCheckShort(window->m_SyncCheckShort->GetValue());
	SetCheckFull(window->m_SyncCheckFull->GetValue());
	SetTimeStamps(window->m_Sync_Timestamp->GetValue());
	SetAttributes(window->m_Sync_Attributes->GetValue());
	SetIgnoreRO(window->m_Sync_Ignore_Readonly->GetValue());
	SetIgnoreDLS(window->m_Sync_Ignore_DaylightS->GetValue());
	SetRecycle(window->m_Sync_Recycle->GetValue());
	SetRules(new Rules(window->m_Sync_Rules->GetStringSelection(), true));
	return true;	
}

bool SyncData::IsReady(){
	if(GetSource() == wxEmptyString || GetDest() == wxEmptyString || GetFunction() == wxEmptyString){
		return false;
	}
	else{
		return true;
	}
}
