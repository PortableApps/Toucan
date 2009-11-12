/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_LUATHREAD
#define H_LUATHREAD

#include <wx/thread.h>
#include <wx/string.h>

struct lua_State;

class LuaThread : public wxThread {

public:
	LuaThread(const wxString &line);

	virtual void *Entry();

private:
	lua_State *m_State;
	wxString m_Command;
};

#endif
