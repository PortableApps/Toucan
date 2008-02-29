/////////////////////////////////////////////////////////////////////////////
// Name:        frmprogress.cpp
// Purpose:     
// Author:      Steven Lamerton
// Modified by: 
// Created:     21/12/2007 12:12:17
// RCS-ID:      
// Copyright:   Copyright (c) Steven Lamerton 2006-2007
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "frmprogress.h"

////@begin XPM images
////@end XPM images


/*!
 * frmProgress type definition
 */

IMPLEMENT_CLASS( frmProgress, wxFrame )


/*!
 * frmProgress event table definition
 */

BEGIN_EVENT_TABLE( frmProgress, wxFrame )

////@begin frmProgress event table entries
 EVT_BUTTON( wxID_OK, frmProgress::OnOkClick )

////@end frmProgress event table entries

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
////@begin frmProgress creation
 wxFrame::Create( parent, id, caption, pos, size, style );

 CreateControls();
 Centre();
////@end frmProgress creation
 return true;
}


/*!
 * frmProgress destructor
 */

frmProgress::~frmProgress()
{
////@begin frmProgress destruction
////@end frmProgress destruction
}


/*!
 * Member initialisation
 */

void frmProgress::Init()
{
////@begin frmProgress member initialisation
 m_Text = NULL;
 m_OK = NULL;
 m_Cancel = NULL;
 m_Save = NULL;
////@end frmProgress member initialisation
}


/*!
 * Control creation for frmProgress
 */

void frmProgress::CreateControls()
{    
////@begin frmProgress content construction
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

 m_Save = new wxButton( itemPanel2, wxID_SAVE, _("&Save"), wxDefaultPosition, wxDefaultSize, 0 );
 itemBoxSizer5->Add(m_Save, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end frmProgress content construction
}


/*!
 * Should we show tooltips?
 */

bool frmProgress::ShowToolTips()
{
 return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap frmProgress::GetBitmapResource( const wxString& name )
{
 // Bitmap retrieval
////@begin frmProgress bitmap retrieval
 wxUnusedVar(name);
 return wxNullBitmap;
////@end frmProgress bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon frmProgress::GetIconResource( const wxString& name )
{
 // Icon retrieval
////@begin frmProgress icon retrieval
 wxUnusedVar(name);
 return wxNullIcon;
////@end frmProgress icon retrieval
}




/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void frmProgress::OnOkClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK in frmProgress.
 // Before editing this code, remove the block markers.
 Destroy();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK in frmProgress. 
}

