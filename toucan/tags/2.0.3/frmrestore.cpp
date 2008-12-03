/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "frmrestore.h"
#include "frmprogress.h"
#include "backupprocess.h"
#include "toucan.h"
#include "waitthread.h"
#include "basicfunctions.h"

#include <wx/stdpaths.h>
#include <wx/checkbox.h>

/*!
 * frmRestore type definition
 */

IMPLEMENT_DYNAMIC_CLASS( frmRestore, wxDialog )


/*!
 * frmRestore event table definition
 */

BEGIN_EVENT_TABLE( frmRestore, wxDialog )

	EVT_BUTTON( wxID_OK, frmRestore::OnOkClick )

    EVT_BUTTON( wxID_CANCEL, frmRestore::OnCancelClick )
	
	EVT_BUTTON( ID_BUTTON_FILE, frmRestore::OnFileClick )

    EVT_BUTTON( ID_BUTTON_LOCATION, frmRestore::OnLocationClick )

END_EVENT_TABLE()


/*!
 * frmRestore constructors
 */

frmRestore::frmRestore()
{
    Init();
}

frmRestore::frmRestore( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * frmRestore creator
 */

bool frmRestore::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
    return true;
}


/*!
 * frmRestore destructor
 */

frmRestore::~frmRestore()
{
}


/*!
 * Member initialisation
 */

void frmRestore::Init()
{
    m_File = NULL;
    m_Location = NULL;
	m_IsPass = NULL;
}


/*!
 * Control creation for frmRestore
 */

void frmRestore::CreateControls()
{    
    frmRestore* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("Backup file:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemStaticText4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_File = new wxTextCtrl( itemDialog1, ID_FILE, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
	m_File->SetMinSize(wxSize(250,-1));
    itemBoxSizer3->Add(m_File, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton6 = new wxButton( itemDialog1, ID_BUTTON_FILE, wxT("..."), wxDefaultPosition, wxSize(25, -1), 0 );
    itemBoxSizer3->Add(itemButton6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer7, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText8 = new wxStaticText( itemDialog1, wxID_STATIC, _("Restore into:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer7->Add(itemStaticText8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_Location = new wxTextCtrl( itemDialog1, ID_LOCATION, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
	m_Location->SetMinSize(wxSize(250,-1));
    itemBoxSizer7->Add(m_Location, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton10 = new wxButton( itemDialog1, ID_BUTTON_LOCATION, wxT("..."), wxDefaultPosition, wxSize(25, -1), 0 );
    itemBoxSizer7->Add(itemButton10, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_IsPass = new wxCheckBox(itemDialog1, ID_ISPASS, _("Password"));
    itemBoxSizer2->Add(m_IsPass, 0, wxALL, 10);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer11 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer11, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxButton* itemButton12 = new wxButton( itemDialog1, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer11->AddButton(itemButton12);

    wxButton* itemButton13 = new wxButton( itemDialog1, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer11->AddButton(itemButton13);

    itemStdDialogButtonSizer11->Realize();
	
	wxString strPath = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH;
	this->SetIcon(wxIcon(strPath + wxT("Toucan.ico"), wxBITMAP_TYPE_ICO));
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void frmRestore::OnOkClick( wxCommandEvent& event )
{
	if(m_File->GetValue() != wxEmptyString && m_Location->GetValue() != wxEmptyString){
		wxString strPass;
		if(m_IsPass->IsChecked()){
			strPass = InputPassword();
			if(strPass == wxEmptyString){
				return;
			}
			else{
				strPass = wxT(" -p") + strPass;
			}
		}
		frmProgress *window = wxGetApp().ProgressWindow;
		window->m_Text->Clear();
		//Send all errors to the text control
		//wxLogTextCtrl* logTxt = new wxLogTextCtrl(window->m_Text);
		//delete wxLog::SetActiveTarget(logTxt);
		//Set up the buttons on the progress box
		window->m_OK->Enable(false);
		window->m_Save->Enable(false);
		window->m_Cancel->Enable(true);
		
		window->m_Text->AppendText(_("Starting...\n"));
		wxDateTime now = wxDateTime::Now();
		window->m_Text->AppendText(_("Time: ") + now.FormatISOTime() + wxT("\n"));
		//Show the window
		window->Update();
		//Create the data sets and fill them
		wxString strCommand =  wxT("7za.exe  x -aoa \"") + m_File->GetValue() + wxT("\" -o\"") + m_Location->GetValue() + wxT("\" * -r") + strPass;	
		
		window->Show();
		window->Refresh();
		window->Update();

		//Cretae the process, execute it
		PipedProcess *process = new PipedProcess(window);
		long lgPID = wxExecute(strCommand, wxEXEC_ASYNC|wxEXEC_NODISABLE, process);
		process->SetRealPid(lgPID);
		WaitThread *thread = new WaitThread(lgPID, process);
		thread->Create();
		thread->Run();
		thread->Wait();
		while(process->HasInput())
			;
		window->m_OK->Enable(true);
		window->m_Save->Enable(true);
		window->m_Cancel->Enable(false);
		now = wxDateTime::Now();
		window->m_Text->AppendText(_("Time: ") + now.FormatISOTime() + wxT("\n"));
		window->m_Text->AppendText(_("Finished"));
		wxGetApp().SetAbort(false);
	}
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
 */

void frmRestore::OnCancelClick( wxCommandEvent& event )
{
	this->EndModal(wxID_CANCEL);
	this->Destroy();
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_LOCATION
 */

void frmRestore::OnLocationClick( wxCommandEvent& event )
{
	wxDirDialog dialog(this,_("Please select the destination folder."), wxEmptyString);
	if (dialog.ShowModal() == wxID_OK) {
		m_Location->SetValue(dialog.GetPath());
	}
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_FILE
 */

void frmRestore::OnFileClick( wxCommandEvent& event )
{
	wxFileDialog dialog(this,_("Please choose your backup file"), wxEmptyString, wxEmptyString, wxT("7 Zip (*.7z)|*.7z|Zip Files (*.zip)|*.zip|All Files (*.*)|*.*"), wxFD_OPEN);
	if (dialog.ShowModal() == wxID_OK) {
		m_File->SetValue(dialog.GetPath());
	}
}
