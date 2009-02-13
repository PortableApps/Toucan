/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "restoredata.h"
#include "toucan.h"
#include "frmrestore.h"
#include "script.h"
#include "basicfunctions.h"

#include <wx/checkbox.h>
#include <wx/listctrl.h>
#include <wx/stdpaths.h>

//frmRestore event table
BEGIN_EVENT_TABLE(frmRestore, wxDialog)
	EVT_BUTTON(wxID_OK, frmRestore::OnOkClick)
    EVT_BUTTON(wxID_CANCEL, frmRestore::OnCancelClick)
	EVT_BUTTON(ID_FILE, frmRestore::OnFileClick)
    EVT_BUTTON(ID_DESTINATION, frmRestore::OnDestinationClick)
END_EVENT_TABLE()

//Constructor
frmRestore::frmRestore(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style){
    Init();
    wxDialog::Create(parent, id, caption, pos, size, style);

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
}

//frmRestore initialisation
void frmRestore::Init(){
    m_File = NULL;
    m_Destination = NULL;
	m_Pass = NULL;
}

//Create controls
void frmRestore::CreateControls(){   
	//Create a pointer so that we have something to add the items to	
    frmRestore* Dialog = this;

    wxBoxSizer* TopSizer = new wxBoxSizer(wxVERTICAL);
    Dialog->SetSizer(TopSizer);

	wxBoxSizer* BackupFileSizer = new wxBoxSizer(wxHORIZONTAL);
	TopSizer->Add(BackupFileSizer, 0, wxGROW|wxALL, 5);

	wxStaticText* BackupFileText = new wxStaticText(Dialog, wxID_STATIC, _("Backup file:"), wxDefaultPosition, wxDefaultSize, 0);
	BackupFileSizer->Add(BackupFileText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_File = new wxTextCtrl(Dialog, ID_TEXT_FILE, _T(""), wxDefaultPosition, wxDefaultSize, 0);
	m_File->SetMinSize(wxSize(250,-1));
	BackupFileSizer->Add(m_File, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxButton* BackupFileButton = new wxButton(Dialog, ID_FILE, wxT("..."), wxDefaultPosition, wxSize(25, -1), 0);
	BackupFileSizer->Add(BackupFileButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* BackupDestinationSizer = new wxBoxSizer(wxHORIZONTAL);
	TopSizer->Add(BackupDestinationSizer, 0, wxGROW|wxALL, 5);

	wxStaticText* BackupDestinationText = new wxStaticText(Dialog, wxID_STATIC, _("Restore into:"), wxDefaultPosition, wxDefaultSize, 0);
	BackupDestinationSizer->Add(BackupDestinationText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_Destination = new wxTextCtrl(Dialog, ID_TEXT_DESTINATION, _T(""), wxDefaultPosition, wxDefaultSize, 0);
	m_Destination->SetMinSize(wxSize(250,-1));
	BackupDestinationSizer->Add(m_Destination, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxButton* BackupDestinationButton = new wxButton(Dialog, ID_DESTINATION, wxT("..."), wxDefaultPosition, wxSize(25, -1), 0 );
	BackupDestinationSizer->Add(BackupDestinationButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_Pass = new wxCheckBox(Dialog, ID_PASS, _("Password"));
	TopSizer->Add(m_Pass, 0, wxALL, 10);

	wxStdDialogButtonSizer* ButtonSizer = new wxStdDialogButtonSizer;
	TopSizer->Add(ButtonSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
		
	wxButton* OKButton = new wxButton(Dialog, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0);
	ButtonSizer->AddButton(OKButton);

	wxButton* CancelButton = new wxButton(Dialog, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0);
	ButtonSizer->AddButton(CancelButton);

	ButtonSizer->Realize();

	wxString strPath = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH;
	this->SetIcon(wxIcon(strPath + wxT("Toucan.ico"), wxBITMAP_TYPE_ICO));
}

//This code needs to be reintegrated in the script system
void frmRestore::OnOkClick(wxCommandEvent& event){
	RestoreData* data = new RestoreData;
	data->SetName(wxT("LastRestoreJob"));
	data->SetBackupFile(m_File->GetValue());
	data->SetBackupFolder(m_Destination->GetValue());
	data->IsPassword = m_Pass->GetValue();
	if(data->NeededFieldsFilled()){
		if(data->TransferToFile()){
			wxGetApp().m_Jobs_Config->Write(wxT("LastRestoreJob/Type"),  wxT("Restore"));
			wxGetApp().m_Jobs_Config->Flush();	
			wxArrayString arrScript;
			arrScript.Add(wxT("Restore \"LastRestoreJob\""));
			wxGetApp().SetAbort(false);
			wxGetApp().m_Script->SetScript(arrScript);
			wxGetApp().m_Script->Execute();
		}
	}
	else{
		ErrorBox(_("Not all of the required fields are filled"));
	}
	delete data;
}

void frmRestore::OnCancelClick(wxCommandEvent& event){
	this->EndModal(wxID_CANCEL);
	this->Destroy();
}

void frmRestore::OnDestinationClick(wxCommandEvent& event){
	wxDirDialog dialog(this,_("Please select the destination folder."), wxEmptyString);
	if (dialog.ShowModal() == wxID_OK) {
		m_Destination->SetValue(dialog.GetPath());
	}
}

void frmRestore::OnFileClick(wxCommandEvent& event){
	wxFileDialog dialog(this,_("Please choose your backup file"), wxEmptyString, wxEmptyString, wxT("7 Zip (*.7z)|*.7z|Zip Files (*.zip)|*.zip|All Files (*.*)|*.*"), wxFD_OPEN);
	if (dialog.ShowModal() == wxID_OK) {
		m_File->SetValue(dialog.GetPath());
	}
}
