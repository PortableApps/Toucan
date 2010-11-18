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
	void Run(const wxString& line, const bool showprogress = true);
	frmProgress* GetProgressWindow();
	void NullWindow() { m_Window = NULL; }


private:

	frmProgress *m_Window;
};

#endif
