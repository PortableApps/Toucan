/////////////////////////////////////////////////////////////////////////////
// Name:        wizardff.cpp
// Purpose:     
// Author:      Steven Lamerton
// Modified by: 
// Created:     19/10/2007 14:56:42
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

#include "wizardff.h"

////@begin XPM images
////@end XPM images


/*!
 * WizardFF type definition
 */

IMPLEMENT_DYNAMIC_CLASS( WizardFF, wxWizard )


/*!
 * WizardFF event table definition
 */

BEGIN_EVENT_TABLE( WizardFF, wxWizard )

////@begin WizardFF event table entries
////@end WizardFF event table entries

END_EVENT_TABLE()


/*!
 * WizardFF constructors
 */

WizardFF::WizardFF()
{
 Init();
}

WizardFF::WizardFF( wxWindow* parent, wxWindowID id, const wxPoint& pos )
{
 Init();
 Create(parent, id, pos);
}


/*!
 * WizardFF creator
 */

bool WizardFF::Create( wxWindow* parent, wxWindowID id, const wxPoint& pos )
{
////@begin WizardFF creation
 wxBitmap wizardBitmap(GetBitmapResource(wxT("wizard.png")));
 wxWizard::Create( parent, id, _("Copy Firefox Profile"), wizardBitmap, pos, wxDEFAULT_DIALOG_STYLE|wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX );

 CreateControls();
////@end WizardFF creation
 return true;
}


/*!
 * WizardFF destructor
 */

WizardFF::~WizardFF()
{
////@begin WizardFF destruction
////@end WizardFF destruction
}


/*!
 * Member initialisation
 */

void WizardFF::Init()
{
////@begin WizardFF member initialisation
////@end WizardFF member initialisation
}


/*!
 * Control creation for WizardFF
 */

void WizardFF::CreateControls()
{    
////@begin WizardFF content construction
 wxWizard* itemWizard1 = this;

 wxWizardPageSimple* lastPage = NULL;
////@end WizardFF content construction
}


/*!
 * Runs the wizard.
 */

bool WizardFF::Run()
{
 wxWindowList::compatibility_iterator node = GetChildren().GetFirst();
 while (node)
 {
  wxWizardPage* startPage = wxDynamicCast(node->GetData(), wxWizardPage);
  if (startPage) return RunWizard(startPage);
  node = node->GetNext();
 }
 return false;
}


/*!
 * Should we show tooltips?
 */

bool WizardFF::ShowToolTips()
{
 return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap WizardFF::GetBitmapResource( const wxString& name )
{
 // Bitmap retrieval
////@begin WizardFF bitmap retrieval
 wxUnusedVar(name);
 if (name == _T("wizard.png"))
 {
  wxBitmap bitmap(_T("wizard.png"), wxBITMAP_TYPE_PNG);
  return bitmap;
 }
 return wxNullBitmap;
////@end WizardFF bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon WizardFF::GetIconResource( const wxString& name )
{
 // Icon retrieval
////@begin WizardFF icon retrieval
 wxUnusedVar(name);
 return wxNullIcon;
////@end WizardFF icon retrieval
}
