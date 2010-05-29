/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007 - 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include "syncdata.h"
#include "../rules.h"
#include "../toucan.h"
#include "../path.h"
#include "../forms/frmmain.h"
#include "../controls/syncctrl.h"
#include <wx/variant.h>
#include <wx/fileconf.h>
#include <wx/msgdlg.h>
#include <wx/combobox.h>
#include <wx/radiobox.h>
#include <wx/checkbox.h>

void SyncData::TransferFromFile(){
	if(!wxGetApp().m_Jobs_Config->Exists(GetName())){
		throw std::invalid_argument(std::string(GetName() + " is not a valid job"));
	}

	SetSource(Read<wxString>("Source"));
	SetDest(Read<wxString>("Dest"));
	SetFunction(ToLang(Read<wxString>("Function")));
	SetCheckSize(Read<bool>("CheckSize"));
	SetCheckTime(Read<bool>("CheckTime"));
	SetCheckShort(Read<bool>("CheckShort"));
	SetCheckFull(Read<bool>("CheckFull"));
	SetTimeStamps(Read<bool>("TimeStamps"));
	SetAttributes(Read<bool>("Attributes"));
	SetIgnoreRO(Read<bool>("IgnoreReadOnly"));
	SetRecycle(Read<bool>("Recycle"));
	SetPreviewChanges(Read<bool>("PreviewChanges"));
	SetRules(new Rules(Read<wxString>("Rules"), true));
}

void SyncData::TransferToFile(){
	//We do not check for errors as it may just signify the group does not exist
	wxGetApp().m_Jobs_Config->DeleteGroup(GetName());

	Write<wxString>("Source", GetSource());
	Write<wxString>("Dest", GetDest());
	Write<wxString>("Function", ToEn(GetFunction()));
	Write<bool>("CheckSize", GetCheckSize());
	Write<bool>("CheckTime", GetCheckTime());
	Write<bool>("CheckShort", GetCheckShort());
	Write<bool>("CheckFull", GetCheckFull());
	Write<bool>("TimeStamps", GetTimeStamps());
	Write<bool>("Attributes", GetAttributes());
	Write<bool>("IgnoreReadOnly", GetIgnoreRO());
	Write<bool>("Recycle", GetRecycle());
	Write<bool>("PreviewChanges", GetPreviewChanges());
	Write<wxString>("Rules", GetRules() ? GetRules()->GetName() : "");
	Write<wxString>("Type", "Sync");

	if(!wxGetApp().m_Jobs_Config->Flush()){
		throw std::runtime_error("There was an error flushing the jobs file");
	}
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
	window->m_Sync_Source_Tree->AddItem(Path::Normalise(GetSource()));		
	window->m_Sync_Dest_Txt->SetValue(GetDest());
	window->m_Sync_Dest_Tree->AddItem(Path::Normalise(GetDest()));
	window->m_Sync_Function->SetStringSelection(GetFunction());
	window->m_SyncCheckSize->SetValue(GetCheckSize());
	window->m_SyncCheckTime->SetValue(GetCheckTime());
	window->m_SyncCheckShort->SetValue(GetCheckShort());
	window->m_SyncCheckFull->SetValue(GetCheckFull());
	window->m_Sync_Timestamp->SetValue(GetTimeStamps());
	window->m_Sync_Attributes->SetValue(GetAttributes());
	window->m_Sync_Ignore_Readonly->SetValue(GetIgnoreRO());
	window->m_Sync_Recycle->SetValue(GetRecycle());
	window->m_SyncPreviewChanges->SetValue(GetPreviewChanges());
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
	SetRecycle(window->m_Sync_Recycle->GetValue());
	SetPreviewChanges(window->m_SyncPreviewChanges->GetValue());
	SetRules(new Rules(window->m_Sync_Rules->GetStringSelection(), true));
	return true;	
}
