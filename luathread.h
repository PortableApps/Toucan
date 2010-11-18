/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 - 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_LUATHREAD
#define H_LUATHREAD

#include <wx/thread.h>
#include <wx/string.h>
#include <wx/datetime.h>

struct lua_State;

class LuaThread : public wxThread {

public:
	LuaThread(const wxString &line);

	virtual void *Entry();

private:
    wxDateTime m_StartTime;
	lua_State *m_State;
	wxString m_Command;
};

#endif
