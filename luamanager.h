/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 - 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_LUAMANAGER
#define H_LUAMANAGER

#include <wx/string.h>
#include <wx/datetime.h>

class frmProgress;

class LuaManager{

public:
	LuaManager();
	void Run(const wxString& line, const bool showprogress = true);
	frmProgress* GetProgressWindow();
	void NullWindow() { m_Window = NULL; }
	void CleanUp();

private:
	void StartUp();

	frmProgress *m_Window;
	wxDateTime m_StartTime;
};

#endif
