/////////////////////////////////////////////////////////////////////////////
// Name:        frmmain.cpp
// Purpose:     
// Author:      Steven Lamerton
// Modified by: 
// Created:     20/08/2007 16:44:04
// RCS-ID:      
// Copyright:   Copyright (C)  2007 Steven Lamerton
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

#include "frmmain.h"

////@begin XPM images
////@end XPM images


/*!
 * frmMain type definition
 */

IMPLEMENT_CLASS( frmMain, wxFrame )


/*!
 * frmMain event table definition
 */

BEGIN_EVENT_TABLE( frmMain, wxFrame )

////@begin frmMain event table entries
////@end frmMain event table entries

END_EVENT_TABLE()


/*!
 * frmMain constructors
 */

frmMain::frmMain()
{
    Init();
}

frmMain::frmMain( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create( parent, id, caption, pos, size, style );
}


/*!
 * frmMain creator
 */

bool frmMain::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin frmMain creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    Centre();
////@end frmMain creation
    return true;
}


/*!
 * frmMain destructor
 */

frmMain::~frmMain()
{
////@begin frmMain destruction
    GetAuiManager().UnInit();
////@end frmMain destruction
}


/*!
 * Member initialisation
 */

void frmMain::Init()
{
////@begin frmMain member initialisation
////@end frmMain member initialisation
}


/*!
 * Control creation for frmMain
 */

void frmMain::CreateControls()
{    
////@begin frmMain content construction
    frmMain* itemFrame1 = this;

    GetAuiManager().SetManagedWindow(this);

    wxAuiNotebook* itemAuiNotebook2 = new wxAuiNotebook( itemFrame1, ID_AUINOTEBOOK1, wxDefaultPosition, wxDefaultSize, wxAUI_NB_TAB_FIXED_WIDTH|wxAUI_NB_SCROLL_BUTTONS );

    wxPanel* itemPanel3 = new wxPanel( itemAuiNotebook2, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );

    itemAuiNotebook2->AddPage(itemPanel3, _("Tab"), false);

    wxPanel* itemPanel4 = new wxPanel( itemAuiNotebook2, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );

    itemAuiNotebook2->AddPage(itemPanel4, _("Tab"), false);

    wxPanel* itemPanel5 = new wxPanel( itemAuiNotebook2, ID_PANEL3, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );

    itemAuiNotebook2->AddPage(itemPanel5, _("Tab"), false);

    itemFrame1->GetAuiManager().AddPane(itemAuiNotebook2, wxAuiPaneInfo()
        .Name(_T("Pane1")).CaptionVisible(false).CloseButton(false).DestroyOnClose(false).Resizable(true).Floatable(false));

    GetAuiManager().Update();

////@end frmMain content construction
}


/*!
 * Should we show tooltips?
 */

bool frmMain::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap frmMain::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin frmMain bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end frmMain bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon frmMain::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin frmMain icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end frmMain icon retrieval
}
