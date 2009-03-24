/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "syncdata.h"
#include "syncthread.h"
#include "../basicfunctions.h"
#include "../variables.h"
#include "../toucan.h"
#include "../filecounter.h"
#include <wx/variant.h>
#include <wx/fileconf.h>
#include <wx/stdpaths.h>

bool SyncData::TransferFromFile(){
	wxString strName = GetName();
	
	bool blError;
	wxString strTemp;
	bool blTemp;

	blError = wxGetApp().m_Jobs_Config->Read(strName + wxT("/Source"), &strTemp);
	if(blError){ SetSource(strTemp); }	
	blError = wxGetApp().m_Jobs_Config->Read(strName + wxT("/Dest"), &strTemp);
	if(blError){ SetDest(strTemp); }	
	blError = wxGetApp().m_Jobs_Config->Read(strName + wxT("/Function"), &strTemp);
	if(blError){ SetFunction(strTemp); }
	blError = wxGetApp().m_Jobs_Config->Read(strName + wxT("/TimeStamps"), &blTemp);
	if(blError){ SetTimeStamps(blTemp); }
	blError = wxGetApp().m_Jobs_Config->Read(strName + wxT("/Attributes"), &blTemp);
	if(blError){ SetAttributes(blTemp); }
	blError = wxGetApp().m_Jobs_Config->Read(strName + wxT("/IgnoreReadOnly"), &blTemp);
	if(blError){ SetIgnoreRO(blTemp); }
	blError = wxGetApp().m_Jobs_Config->Read(strName + wxT("/IgnoreDaylightSavings"), &blTemp);
	if(blError){ SetIgnoreDLS(blTemp); }
	blError = wxGetApp().m_Jobs_Config->Read(strName + wxT("/Move"), &blTemp);
	if(blError){ SetIgnoreDLS(blTemp); }
	
	if(!blError){
		ErrorBox(_("There was an error reading from the jobs file, \nplease check it is not set as read only or in use."));
		return false;
	}
	return true;
}

bool SyncData::TransferToFile(){
	wxString strName = GetName();
	
	bool blError;

	blError = wxGetApp().m_Jobs_Config->DeleteGroup(strName);
	blError = wxGetApp().m_Jobs_Config->Write(strName + wxT("/Source"),  GetSource());	
	blError = wxGetApp().m_Jobs_Config->Write(strName + wxT("/Dest"), GetDest());	
	blError = wxGetApp().m_Jobs_Config->Write(strName + wxT("/Function"), GetFunction());
	blError = wxGetApp().m_Jobs_Config->Write(strName + wxT("/TimeStamps"), GetTimeStamps());
	blError = wxGetApp().m_Jobs_Config->Write(strName + wxT("/Attributes"), GetAttributes());
	blError = wxGetApp().m_Jobs_Config->Write(strName + wxT("/IgnoreReadOnly"), GetIgnoreRO());
	blError = wxGetApp().m_Jobs_Config->Write(strName + wxT("/IgnoreDaylightSavings"), GetIgnoreDLS());
	blError = wxGetApp().m_Jobs_Config->Write(strName + wxT("/Move"), GetMove());
	wxGetApp().m_Jobs_Config->Flush();
	if(!blError){
		ErrorBox(_("There was an error saving to the jobs file, \nplease check it is not set as read only or in use."));
		return false;
	}
	return true;
}

bool SyncData::TransferToForm(){
	frmMain *window = wxGetApp().MainWindow;
	if(window == NULL){
		return false;
	}
	window->m_Sync_Source_Txt->SetValue(GetSource());
	window->m_Sync_Source_Tree->DeleteAllItems();
	window->m_Sync_Source_Tree->AddRoot(wxT("Hidden text"));
	window->m_Sync_Source_Tree->AddNewPath(Normalise(Normalise(GetSource())));
	
	window->m_Sync_Dest_Txt->SetValue(GetDest());
	window->m_Sync_Dest_Tree->DeleteAllItems();
	window->m_Sync_Dest_Tree->AddRoot(wxT("Hidden text"));
	window->m_Sync_Dest_Tree->AddNewPath(Normalise(Normalise(GetDest())));
	
	window->m_Sync_Function->SetStringSelection(GetFunction());
	window->m_Sync_Timestamp->SetValue(GetTimeStamps());
	window->m_Sync_Attributes->SetValue(GetAttributes());
	window->m_Sync_Ignore_Readonly->SetValue(GetIgnoreRO());
	window->m_Sync_Ignore_DaylightS->SetValue(GetIgnoreDLS());
	window->m_Sync_Move->SetValue(GetMove());
	return false;
}

bool SyncData::TransferFromForm(){
	frmMain *window = wxGetApp().MainWindow;
	SetSource(window->m_Sync_Source_Txt->GetValue());
	SetDest(window->m_Sync_Dest_Txt->GetValue());
	SetFunction(window->m_Sync_Function->GetStringSelection());
	SetTimeStamps(window->m_Sync_Timestamp->GetValue());
	SetAttributes(window->m_Sync_Attributes->GetValue());
	SetIgnoreRO(window->m_Sync_Ignore_Readonly->GetValue());
	SetIgnoreDLS(window->m_Sync_Ignore_DaylightS->GetValue());
	SetMove(window->m_Sync_Move->GetValue());
	return true;	
}

void SyncData::Output(){
	MessageBox (GetSource(), wxT("Source"));
	MessageBox(GetDest(), wxT("Destination"));
	MessageBox(GetFunction(), wxT("Function"));
	wxVariant varTemp;
	varTemp = GetTimeStamps();
	MessageBox(varTemp.GetString(), wxT("Timestamps?"));
	varTemp = GetAttributes();
	MessageBox(varTemp.GetString(), wxT("Attributes?"));
	varTemp = GetIgnoreRO();
	MessageBox(varTemp.GetString(), wxT("Ignore Readonly?"));
	varTemp = GetIgnoreDLS();
	MessageBox(varTemp.GetString(), wxT("Ignore Daylight Savings?"));
	varTemp = GetMove();
	MessageBox(varTemp.GetString(), wxT("Move?"));
}

bool SyncData::Execute(Rules rules){
	SetSource(Normalise(Normalise(GetSource())));
	SetDest(Normalise(Normalise(GetDest())));
	//Create a new Sync thread and run it (needs to use Wait())
	SyncThread *thread = new SyncThread(*this, rules);
	thread->Create();
	thread->Run();
	return true;
}

bool SyncData::NeededFieldsFilled(){
	bool blFilled = true;
	if(GetSource() == wxEmptyString){
		blFilled = false;
	}
	if(GetDest() == wxEmptyString){
		blFilled = false;
	}
	if(GetFunction() == wxEmptyString){
		blFilled = false;
	}
	return blFilled;
}

