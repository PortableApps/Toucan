/////////////////////////////////////////////////////////////////////////////
// Name:        toucan.cpp
// Purpose:     
// Author:      Steven Lamerton
// Modified by: 
// Created:     28/09/2007 20:20:52
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

#include <wx/list.h>
#include <wx/listimpl.cpp> 

#include "toucan.h"
#include "frmmain.h"
#include "backupprocess.h"
////@begin XPM images
////@end XPM images


/*!
* Application instance implementation
*/

////@begin implement app
IMPLEMENT_APP( Toucan )
////@end implement app


/*!
* Toucan type definition
*/

IMPLEMENT_CLASS( Toucan, wxApp )


/*
*Define the list that holds PipedProecesses
*/
//WX_DEFINE_LIST(m_Processes);


/*!
* Toucan event table definition
*/

BEGIN_EVENT_TABLE( Toucan, wxApp )

////@begin Toucan event table entries
////@end Toucan event table entries
	EVT_IDLE(Toucan::OnIdle)
    EVT_TIMER(wxID_ANY, Toucan::OnTimer)
END_EVENT_TABLE()


/*!
* Constructor for Toucan
*/

Toucan::Toucan()
{
	Init();
}


/*!
* Member initialisation
*/

void Toucan::Init()
{
	////@begin Toucan member initialisation
	////@end Toucan member initialisation
}

/*!
* Initialisation for Toucan
*/

bool Toucan::OnInit()
{    
	////@begin Toucan initialisation
	// Remove the comment markers above and below this block
	// to make permanent changes to the code.

#if wxUSE_XPM
	wxImage::AddHandler(new wxXPMHandler);
#endif
#if wxUSE_LIBPNG
	wxImage::AddHandler(new wxPNGHandler);
#endif
#if wxUSE_LIBJPEG
	wxImage::AddHandler(new wxJPEGHandler);
#endif
#if wxUSE_GIF
	wxImage::AddHandler(new wxGIFHandler);
#endif
	frmMain* mainWindow = new frmMain( NULL, ID_AUIFRAME );
	mainWindow->Show(true);
	////@end Toucan initialisation

	return true;
}


bool Toucan::RegisterProcess(PipedProcess *process)
{
	if ( m_Running.IsEmpty() ){
		m_wakeUpTimer.Start(100);
	}
	m_Running.Add(process);
}

/// UnregisterProcess
bool Toucan::UnregisterProcess(PipedProcess *process)
{
	m_Running.Remove(process);
	if ( m_Running.IsEmpty() ){
		m_wakeUpTimer.Stop();
	}
}

void Toucan::OnTimer(wxTimerEvent& WXUNUSED(event))
{
    wxWakeUpIdle();
}


void Toucan::OnIdle(wxIdleEvent& event)
{
	wxYield();
    size_t count = m_Running.GetCount();
    for ( size_t n = 0; n < count; n++ )
    {
        if ( m_Running[n]->HasInput())
        {
            event.RequestMore();
        }
    }
}



/*!
* Cleanup for Toucan
*/

int Toucan::OnExit()
{    
	////@begin Toucan cleanup
	return wxApp::OnExit();
	////@end Toucan cleanup
}

