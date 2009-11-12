/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include <wx/listctrl.h>

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
	//TODO : Move this to a bindings file
	wxString setprint = wxT("print=toucan.OutputProgress");
	luaL_dostring(m_State, setprint.mb_str());
	wxString setsync  = wxT("sync=toucan.Sync"); 
	luaL_dostring(m_State, setsync.mb_str());
	wxString setexpand = wxT("expand=toucan.ExpandVariable");
	luaL_dostring(m_State, setexpand.mb_str());
}

void *LuaThread::Entry(){
	if (luaL_loadstring(m_State, m_Command.mb_str()) || lua_pcall(m_State, 0, 0, 0)) {
		OutputProgress(wxT("Cannot run lua file: ") + wxString(lua_tostring(m_State, -1), wxConvUTF8));
	}
	wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, ID_FINISH);
	wxPostEvent(&wxGetApp(), event);
	return NULL;
}
