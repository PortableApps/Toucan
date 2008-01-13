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
WX_DEFINE_LIST(m_Processes);


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


bool Toucan::RegisterProcess(wxProcess* process)
{
    if (!m_Processes.Find(process))
        m_Processes.Append(process);

    if (!m_wakeUpTimer.IsRunning())
        m_wakeUpTimer.Start(100);
    return true;
}

/// UnregisterProcess
bool Toucan::UnregisterProcess(wxProcess* process)
{
    bool success = m_Processes.DeleteObject(process);
    if (!HasProcesses())
    {
        if (m_wakeUpTimer.IsRunning())
            m_wakeUpTimer.Stop();
    }
    return success;
}

void Toucan::OnTimer(wxTimerEvent& WXUNUSED(event))
{
    wxWakeUpIdle();
}

/// Handle any pending input, in idle time
bool Toucan::HandleProcessInput()
{
    if (!HasProcesses())
        return false;

    bool hasInput = false;

    wxNode* node = m_Processes.GetFirst();
    while (node)
    {
        PipedProcess* process = wxDynamicCast(node->GetData(), PipedProcess);
        if (process && process->HasInput())
            hasInput = true;
        node = node->GetNext();
    }
    return hasInput;
}

void Toucan::OnIdle(wxIdleEvent& event)
{
    if (HandleProcessInput())
        event.RequestMore();
    event.Skip();
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

