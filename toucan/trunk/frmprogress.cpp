/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "toucan.h"
#include "frmprogress.h"

#include <wx/stdpaths.h>

/*!
 * frmProgress type definition
 */

IMPLEMENT_CLASS( frmProgress, wxFrame )


/*!
 * frmProgress event table definition
 */

BEGIN_EVENT_TABLE( frmProgress, wxFrame )

	EVT_CLOSE( frmProgress::OnCloseWindow )

	EVT_BUTTON( wxID_OK, frmProgress::OnOkClick )

	EVT_BUTTON( wxID_CANCEL, frmProgress::OnCancelClick )

	EVT_BUTTON( wxID_SAVE, frmProgress::OnSaveClick )

END_EVENT_TABLE()


/*!
 * frmProgress constructors
 */

frmProgress::frmProgress()
{
	Init();
}

frmProgress::frmProgress( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
	Init();
	Create( parent, id, caption, pos, size, style );
}


/*!
 * frmProgress creator
 */

bool frmProgress::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
	wxFrame::Create( parent, id, caption, pos, size, style );

	CreateControls();
	Centre();
	return true;
}


/*!
 * frmProgress destructor
 */

frmProgress::~frmProgress()
{
}


/*!
 * Member initialisation
 */

void frmProgress::Init()
{
	m_Text = NULL;
	m_OK = NULL;
	m_Cancel = NULL;
	m_Save = NULL;
}


/*!
 * Control creation for frmProgress
 */

void frmProgress::CreateControls()
{
	frmProgress* itemFrame1 = this;

	wxPanel* itemPanel2 = new wxPanel( itemFrame1, ID_PANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );

	wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
	itemPanel2->SetSizer(itemBoxSizer3);

	m_Text = new wxTextCtrl( itemPanel2, ID_TEXTCTRL, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_RICH2 );
	itemBoxSizer3->Add(m_Text, 1, wxGROW|wxALL, 5);

	wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer3->Add(itemBoxSizer5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	m_OK = new wxButton( itemPanel2, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer5->Add(m_OK, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_Cancel = new wxButton( itemPanel2, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer5->Add(m_Cancel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_Save = new wxButton( itemPanel2, wxID_SAVE, _("Save"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer5->Add(m_Save, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	wxString strPath = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH;
	this->SetIcon(wxIcon(strPath + wxT("Toucan.ico"), wxBITMAP_TYPE_ICO));
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void frmProgress::OnOkClick( wxCommandEvent& event )
{
	this->Show(false);
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANEL
 */

void frmProgress::OnCancelClick( wxCommandEvent& event )
{
	wxGetApp().SetAbort(true);
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_SAVE
 */

void frmProgress::OnSaveClick( wxCommandEvent& event )
{
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



void frmProgress::OnCloseWindow( wxCloseEvent& event )
{
	this->Show(false);
}

