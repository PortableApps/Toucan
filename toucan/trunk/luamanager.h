/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_LUAMANAGER
#define H_LUAMANAGER

#include <wx/string.h>
#include <wx/datetime.h>

class frmProgress;

class LuaManager{

public:
	LuaManager();
	void Run(const wxString& line);
	frmProgress* GetProgressWindow();
	void CleanUp();

private:
	void StartUp();

	frmProgress *m_Window;
	wxDateTime m_StartTime;
};

#endif
