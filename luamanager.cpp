/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 - 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include <wx/gauge.h>
#include <wx/button.h>
#include <wx/bmpbuttn.h>
#include <wx/listctrl.h>

#include "toucan.h"
#include "luathread.h"
#include "luamanager.h"
#include "basicfunctions.h"
#include "forms/frmmain.h"
#include "forms/frmprogress.h"

#ifdef __WXMSW__
	#include <windows.h>
	#include <wx/msw/winundef.h>
#endif

LuaManager::LuaManager(){
	m_Window = NULL;
}

void LuaManager::Run(const wxString &line, const bool showprogress){
    wxGetApp().SetAbort(false);
    //If we are in gui mode then reset as new runs should prompt again
    //otherwise do not clear as the password is set from the commandline
    if(wxGetApp().IsGui()){
        //We initially set the password to blank
        wxGetApp().m_Password = "";
    }

	if(line == wxEmptyString)
		return;

	LuaThread *thread = new LuaThread(line);
	thread->Create();
	thread->Run();

    if(wxGetApp().IsGui()){
        m_Window = new frmProgress(wxGetApp().MainWindow, wxID_ANY, _("Progress"));
        m_Window->SetTaskbar(wxGetApp().MainWindow->m_Taskbar);

        if(!showprogress){
		    m_Window->m_Gauge->Show(false);
		    m_Window->SetSize(m_Window->GetSize() + wxSize(1, 1));
	    }

        m_Window->ShowModal();
    }
}


frmProgress* LuaManager::GetProgressWindow(){
	return m_Window;
}
