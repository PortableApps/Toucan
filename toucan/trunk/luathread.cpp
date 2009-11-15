/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include <wx/listctrl.h>
#include <wx/stdpaths.h>

#include "toucan.h"
#include "luathread.h"
#include "basicfunctions.h"
#include <lua.hpp>

extern "C" {
	extern int luaopen_toucan(lua_State*L);
}

LuaThread::LuaThread(const wxString &line) : wxThread(), m_Command(line){
	m_State = luaL_newstate();
	luaL_openlibs(m_State);
	luaopen_toucan(m_State);
	wxString path = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("bindings.lua");
	if (luaL_loadfile(m_State, path.mb_str()) || lua_pcall(m_State, 0, 0, 0)) {
		OutputProgress(wxT("Cannot run lua file: ") + wxString(lua_tostring(m_State, -1), wxConvUTF8));
	}
}

void *LuaThread::Entry(){
	if (luaL_loadstring(m_State, m_Command.mb_str()) || lua_pcall(m_State, 0, 0, 0)) {
		OutputProgress(wxT("Cannot run lua file: ") + wxString(lua_tostring(m_State, -1), wxConvUTF8));
	}
	wxCommandEvent *event = new wxCommandEvent(wxEVT_COMMAND_BUTTON_CLICKED, ID_FINISH);
	wxGetApp().QueueEvent(event);
	return NULL;
}
