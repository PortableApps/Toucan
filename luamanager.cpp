/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 - 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include <wx/gauge.h>
#include <wx/button.h>
#include <wx/listctrl.h>

#include "toucan.h"
#include "luathread.h"
#include "luamanager.h"
#include "basicfunctions.h"
#include "forms/frmmain.h"
#include "forms/frmprogress.h"
#include "controls/loglistctrl.h"

#ifdef __WXMSW__
	#include <windows.h>
	#include <wx/msw/winundef.h>
#endif

LuaManager::LuaManager(){
	m_Window = NULL;
}

void LuaManager::StartUp(){
	wxGetApp().SetAbort(false);
	m_StartTime = wxDateTime::Now();
	if(wxGetApp().IsGui()){
		if(!m_Window){
			m_Window = new frmProgress(wxGetApp().MainWindow, wxID_ANY, _("Progress"), wxDefaultPosition, wxSize(640, 480), wxDEFAULT_FRAME_STYLE|wxRESIZE_BORDER);
			m_Window->Show();
		}
		else{
			m_Window->m_List->DeleteAllItems();
		}
		LogListCtrl* logList = new LogListCtrl(m_Window->m_List);
		delete wxLog::SetActiveTarget(logList);
	}
	OutputProgress("Toucan 3.0 Pre-Release 1", true);
	OutputProgress(_("Date:") + " " + m_StartTime.FormatISODate());
	OutputProgress(_("Computer Name:") + " " + wxGetHostName());
	OutputProgress(_("Operating System:") + " " + wxGetOsDescription());
	OutputProgress(wxEmptyString);
	if(wxGetApp().IsGui()){
		m_Window->m_List->SetColumnWidth(1, m_Window->m_List->GetClientSize().GetWidth() - m_Window->m_List->GetColumnWidth(0));
		m_Window->m_OK->Enable(false);
		m_Window->m_Save->Enable(false);
		m_Window->m_Cancel->Enable(true);
	}
}

void LuaManager::CleanUp(){
	if(wxGetApp().IsGui()){
		//Send all errors to the standard gui
		delete wxLog::SetActiveTarget(new wxLogGui);
		OutputProgress(wxEmptyString);
	}
	OutputProgress(_("Elapsed:") + wxT(" ") + wxDateTime::Now().Subtract(m_StartTime).Format());
	OutputProgress(_("Finished"), true);
	if(wxGetApp().IsGui()){
		m_Window->m_OK->Enable(true);
		m_Window->m_Save->Enable(true);
		m_Window->m_Cancel->Enable(false);

		//Let the user know we have finished
		m_Window->FinishGauge();
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
		m_Window->RequestUserAttention();
#endif
	}
}

void LuaManager::Run(const wxString &line, const bool showprogress){
	if(line == wxEmptyString){
		return;
	}
	StartUp();
	if(!showprogress){
		m_Window->m_Gauge->Show(false);
		m_Window->SetSize(m_Window->GetSize() + wxSize(1, 1));
	}
	LuaThread *thread = new LuaThread(line);
	thread->Create();
	thread->Run();
}


frmProgress* LuaManager::GetProgressWindow(){
	return m_Window;
}
