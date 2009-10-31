/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include <wx/listctrl.h>

#include "luamanager.h"
#include "basicfunctions.h"
#include "forms/frmprogress.h"
#include "controls/loglistctrl.h"
#include <lua.hpp>

extern "C" {
	extern int luaopen_toucan(lua_State*L);
}

LuaManager::LuaManager(){
	m_State = luaL_newstate();
	luaL_openlibs(m_State);
	luaopen_toucan(m_State);
	wxString setprint = wxT("print=toucan.OutputProgress");
	luaL_dostring(m_State, setprint.mb_str());
}

void LuaManager::StartUp(){
	if(m_Window){
		m_Window = new frmProgress(NULL, wxID_ANY, _("Progress"), wxDefaultPosition, wxSize(640, 480), wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);
		m_Window->Show();
	}
	else{
		m_Window->m_List->DeleteAllItems();
	}
	//Send all errors to the list control
	LogListCtrl* logList = new LogListCtrl(m_Window->m_List);
	delete wxLog::SetActiveTarget(logList);
	m_StartTime = wxDateTime::Now();
	wxMessageBox(_("Outputting"));
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
}

void LuaManager::Run(const wxString &line){
	if(line == wxEmptyString){
		return;
	}
	StartUp();
	DoRun(line);
	CleanUp();
}

void LuaManager::Run(const wxArrayString &lines){
	if(lines.Count() == 0){
		return;
	}
	m_CurrentCommand = 0;
	m_CommandList = lines;
	StartUp();
	DoRun(lines.Item(0));
}

void LuaManager::RunNext(){
	//We have finished all of the jobs
	if(m_CommandList.Count() == m_CurrentCommand++){
		CleanUp();
	}
	DoRun(m_CommandList.Item(m_CurrentCommand));
}

void LuaManager::DoRun(const wxString &line){
	if (luaL_loadstring(m_State, line.mb_str()) || lua_pcall(m_State, 0, 0, 0)) {
		OutputProgress(wxT("Cannot run lua file: ") + wxString(lua_tostring(m_State, -1), wxConvUTF8));
	}
}

frmProgress* LuaManager::GetProgressWindow(){
	return m_Window;
}
