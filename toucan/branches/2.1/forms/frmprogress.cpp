/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "frmprogress.h"
#include "../script.h"
#include "../toucan.h"

#include <wx/wx.h>
#include <wx/stdpaths.h>
#include <wx/listctrl.h>

//frmProgress event table
BEGIN_EVENT_TABLE(frmProgress, wxFrame)
	EVT_CLOSE(frmProgress::OnCloseWindow)
	EVT_BUTTON(wxID_OK, frmProgress::OnOkClick)
	EVT_BUTTON(wxID_CANCEL, frmProgress::OnCancelClick)
	EVT_BUTTON(wxID_SAVE, frmProgress::OnSaveClick)
	
	//ScriptManager
	EVT_BUTTON(ID_SCRIPTFINISH, frmProgress::OnScriptFinish)
	EVT_BUTTON(ID_SCRIPTPROGRESS, frmProgress::OnScriptProgress)
	EVT_BUTTON(ID_SCRIPTBLANK, frmProgress::OnScriptBlank)

END_EVENT_TABLE()

//Constructor
frmProgress::frmProgress(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style){
	Init();
	wxFrame::Create( parent, id, caption, pos, size, style );
	CreateControls();
	Centre();
}

//frmProgress initialisation
void frmProgress::Init(){
	m_List = NULL;
	m_OK = NULL;
	m_Cancel = NULL;
	m_Save = NULL;
	m_Gauge = NULL;
}

//Create controls
void frmProgress::CreateControls(){
	frmProgress* Frame = this;

	wxPanel* Panel = new wxPanel(Frame, ID_PANEL_PROGRESS, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );

	wxBoxSizer* TopSizer = new wxBoxSizer(wxVERTICAL);
	Panel->SetSizer(TopSizer);

	m_Gauge = new wxGauge(Panel, ID_PROGRESS_GAUGE, 100);
	TopSizer->Add(m_Gauge, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxEXPAND, 5);

	m_List = new wxListCtrl(Panel, ID_PROGRESS_LIST, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_VRULES);
	TopSizer->Add(m_List, 1, wxGROW|wxALL, 5);

	wxBoxSizer* ButtonSizer = new wxBoxSizer(wxHORIZONTAL);
	TopSizer->Add(ButtonSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	m_OK = new wxButton(Panel, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	ButtonSizer->Add(m_OK, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_Cancel = new wxButton(Panel, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	ButtonSizer->Add(m_Cancel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_Save = new wxButton(Panel, wxID_SAVE, _("Save"), wxDefaultPosition, wxDefaultSize, 0 );
	ButtonSizer->Add(m_Save, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
	//Add columns
	m_List->InsertColumn(0, _("Time"));
	m_List->InsertColumn(1, _("Status"));
	
	//Set the form icon
	wxString strPath = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH;
	this->SetIcon(wxIcon(strPath + wxT("Toucan.ico"), wxBITMAP_TYPE_ICO));
}

void frmProgress::OnOkClick(wxCommandEvent& event){
	this->MakeModal(false);
	this->Show(false);
}

void frmProgress::OnCancelClick(wxCommandEvent& event){
	wxGetApp().SetAbort(true);
}

void frmProgress::OnSaveClick(wxCommandEvent& event){
	/*wxString strCaption = _("Save");
	wxString strWildcard;
	strWildcard = wxT("Text Files (*.txt)|*.txt");
	wxString defaultFilename = wxEmptyString;
	wxString defaultDir = wxT("/");
	wxFileDialog dialog(this, strCaption, defaultDir, defaultFilename, strWildcard, wxSAVE);
	if (dialog.ShowModal() == wxID_OK) {
		m_Text->SaveFile(dialog.GetPath());
	}*/
}

void frmProgress::OnCloseWindow(wxCloseEvent& event){
	this->MakeModal(false);
	this->Show(false);
}

//ID_SCRIPTFINISH
void frmProgress::OnScriptFinish(wxCommandEvent& event){
	if(wxGetApp().m_Script->GetCommand() < wxGetApp().m_Script->GetCount()){
		wxGetApp().m_Script->SetCommand(wxGetApp().m_Script->GetCommand() + 1);
		wxGetApp().m_Script->ParseCommand(wxGetApp().m_Script->GetCommand() - 1);
	}
	else{
		wxGetApp().m_Script->CleanUp();
		wxGetApp().m_Script->SetCommand(wxGetApp().m_Script->GetCommand() + 1);
	}
}

//ID_SCRIPTPROGRESS
void frmProgress::OnScriptProgress(wxCommandEvent& event){
	long index = m_List->InsertItem(m_List->GetItemCount() - 1, wxEmptyString);
	if(event.GetInt() == -1){
		m_List->SetItem(index, 1, event.GetString());
	}
	//There was also a date, so spilt the string up
	else{
		m_List->SetItem(index, 0, event.GetString().Left(event.GetInt()));
		m_List->SetItem(index, 1, event.GetString().Right(event.GetString().Length() - event.GetInt()));
	}
	m_List->EnsureVisible(index);
	Update();
}

//ID_SCRIPTBLANK
void frmProgress::OnScriptBlank(wxCommandEvent& event){
	long index = m_List->InsertItem(m_List->GetItemCount() - 1, wxEmptyString);
	m_List->SetItem(index, 1, wxEmptyString);
}
