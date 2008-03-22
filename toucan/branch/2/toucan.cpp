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

#include <wx/list.h>
#include <wx/listimpl.cpp> 
#include <wx/snglinst.h>
#include <wx/splash.h>

#include "toucan.h"
#include "frmmain.h"
#include "backupprocess.h"


/*!
* Application instance implementation
*/
IMPLEMENT_APP( Toucan )

/*!
* Toucan type definition
*/

IMPLEMENT_CLASS( Toucan, wxApp )

/*!
* Toucan event table definition
*/

BEGIN_EVENT_TABLE( Toucan, wxApp )
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
	blAbort = false;
}

/*!
* Initialisation for Toucan
*/

bool Toucan::OnInit()
{    

	wxInitAllImageHandlers();
	frmMain* mainWindow = new frmMain( NULL, ID_AUIFRAME );
	mainWindow->Show();
	mainWindow->Raise();
	mainWindow->Update();
	return true;
}


bool Toucan::RegisterProcess(PipedProcess *process)
{
	if ( m_Running.IsEmpty() ){
		m_wakeUpTimer.Start(20);
	}
	m_Running.Add(process);
	return true;
}

/// UnregisterProcess
bool Toucan::UnregisterProcess(PipedProcess *process)
{
	m_Running.Remove(process);
	if ( m_Running.IsEmpty() ){
		m_wakeUpTimer.Stop();
	}
	return true;
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
	return wxApp::OnExit();
}

