/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_LUAMANAGER
#define H_LUAMANAGER

#include <wx/arrstr.h>
#include <wx/string.h>
#include <wx/datetime.h>

class frmProgress;
struct lua_State;

class LuaManager{

public:
	LuaManager();
	void Run(const wxString& line);
	void Run(const wxArrayString& lines);
	void RunNext();
	frmProgress* GetProgressWindow();

private:
	void DoRun(const wxString& line);
	void StartUp();
	void CleanUp();

	lua_State *m_State;
	frmProgress *m_Window;
	wxDateTime m_StartTime;
	unsigned int m_CurrentCommand;
	wxArrayString m_CommandList;
};

#endif
