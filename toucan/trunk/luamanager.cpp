/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include <wx/button.h>
#include <wx/listctrl.h>

#include "luathread.h"
#include "luamanager.h"
#include "basicfunctions.h"
#include "forms/frmprogress.h"
#include "controls/loglistctrl.h"

#ifdef __WXMSW__
	#include <windows.h>
	#include <wx/msw/winundef.h>
#endif

LuaManager::LuaManager(){
	;
}

void LuaManager::StartUp(){
	if(m_Window){
		m_Window = new frmProgress(NULL, wxID_ANY, _("Progress"), wxDefaultPosition, wxSize(640, 480), wxDEFAULT_FRAME_STYLE|wxRESIZE_BORDER);
		m_Window->Show();
	}
	else{
		m_Window->m_List->DeleteAllItems();
	}
	//Send all errors to the list control
	LogListCtrl* logList = new LogListCtrl(m_Window->m_List);
	delete wxLog::SetActiveTarget(logList);
	m_StartTime = wxDateTime::Now();
	OutputProgress(_("Starting"), true);
	OutputProgress(wxEmptyString);
	m_Window->m_OK->Enable(false);
	m_Window->m_Save->Enable(false);
	m_Window->m_Cancel->Enable(true);
}

void LuaManager::CleanUp(){
	//Send all errors to the standard gui
	delete wxLog::SetActiveTarget(new wxLogGui);
	OutputProgress(wxEmptyString);
	OutputProgress(_("Elapsed") + wxT(" ") + wxDateTime::Now().Subtract(m_StartTime).Format());
	OutputProgress(_("Finished"), true);
	m_Window->m_OK->Enable(true);
	m_Window->m_Save->Enable(true);
	m_Window->m_Cancel->Enable(false);

	//Let the user know we have finished
#ifdef __WXMSW__
	FLASHWINFO info;
	info.uCount = 3;
	info.dwTimeout = 0;
	info.hwnd = static_cast<HWND>(m_Window->GetHWND());
	info.cbSize = sizeof(info);
	if(wxWindow::FindFocus() == m_Window){
		info.dwFlags = FLASHW_ALL|FLASHW_TIMERNOFG;			
	}
	else{
		info.dwFlags = FLASHW_ALL;					
	}
	FlashWindowEx(&info);
#else
	RequestUserAttention();
#endif
}

void LuaManager::Run(const wxString &line){
	if(line == wxEmptyString){
		return;
	}
	StartUp();
	LuaThread *thread = new LuaThread(line);
	thread->Create();
	thread->Run();
}


frmProgress* LuaManager::GetProgressWindow(){
	return m_Window;
}
