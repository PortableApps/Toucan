/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 - 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
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
    m_StartTime = wxDateTime::Now();

	m_State = luaL_newstate();
	luaL_openlibs(m_State);
	luaopen_toucan(m_State);
	wxString path = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("bindings.lua");
	if (luaL_loadfile(m_State, path.mb_str()) || lua_pcall(m_State, 0, 0, 0)) {
		OutputProgress(wxT("Cannot run lua file: ") + wxString(lua_tostring(m_State, -1), wxConvUTF8), Error);
	}
}

void *LuaThread::Entry(){
    OutputProgress("Toucan 3.1.3-pre.1", StartingLine);
    OutputProgress(_("Date:") + " " + m_StartTime.FormatISODate(), StartingInfo);
	OutputProgress(_("Computer Name:") + " " + wxGetHostName(), StartingInfo);
	OutputProgress(_("Operating System:") + " " + wxGetOsDescription(), StartingInfo);
	OutputProgress(wxEmptyString, Message);

	if (luaL_loadstring(m_State, m_Command.mb_str()) || lua_pcall(m_State, 0, 0, 0)) {
		OutputProgress(wxT("Cannot run lua file: ") + wxString(lua_tostring(m_State, -1), wxConvUTF8), Error);
	}

    OutputProgress(wxEmptyString, Message);
    OutputProgress(_("Elapsed:") + wxT(" ") + wxDateTime::Now().Subtract(m_StartTime).Format(), FinishingInfo);
    OutputProgress(_("Finished"), FinishingLine);
	return NULL;
}
