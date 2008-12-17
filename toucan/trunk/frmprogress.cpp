/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "toucan.h"
#include "frmprogress.h"

#include <wx/stdpaths.h>

//frmProgress event table
BEGIN_EVENT_TABLE(frmProgress, wxFrame)
	EVT_CLOSE(frmProgress::OnCloseWindow)
	EVT_BUTTON(wxID_OK, frmProgress::OnOkClick)
	EVT_BUTTON(wxID_CANCEL, frmProgress::OnCancelClick)
	EVT_BUTTON(wxID_SAVE, frmProgress::OnSaveClick)
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
	m_Text = NULL;
	m_OK = NULL;
	m_Cancel = NULL;
	m_Save = NULL;
}

//Create controls
void frmProgress::CreateControls(){
	//Create a pointer so that we have something to add the items to	
	frmProgress* Frame = this;

	wxPanel* Panel = new wxPanel(Frame, ID_PANEL_PROGRESS, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );

	wxBoxSizer* TopSizer = new wxBoxSizer(wxVERTICAL);
	Panel->SetSizer(TopSizer);

		m_Text = new wxTextCtrl(Panel, ID_PROGRESS_TEXT, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_RICH2 );
		TopSizer->Add(m_Text, 1, wxGROW|wxALL, 5);

		wxBoxSizer* ButtonSizer = new wxBoxSizer(wxHORIZONTAL);
		TopSizer->Add(ButtonSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

			m_OK = new wxButton(Panel, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
			ButtonSizer->Add(m_OK, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

			m_Cancel = new wxButton(Panel, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
			ButtonSizer->Add(m_Cancel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

			m_Save = new wxButton(Panel, wxID_SAVE, _("Save"), wxDefaultPosition, wxDefaultSize, 0 );
			ButtonSizer->Add(m_Save, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
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
	wxString strCaption = _("Save");
	wxString strWildcard;
	strWildcard = wxT("Text Files (*.txt)|*.txt");
	wxString defaultFilename = wxEmptyString;
	wxString defaultDir = wxT("/");
	wxFileDialog dialog(this, strCaption, defaultDir, defaultFilename, strWildcard, wxSAVE);
	if (dialog.ShowModal() == wxID_OK) {
		m_Text->SaveFile(dialog.GetPath());
	}
}

void frmProgress::OnCloseWindow(wxCloseEvent& event){
	this->MakeModal(false);
	this->Show(false);
}
