/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_FILECOUNTER
#define H_FILECOUNTER

#include <wx/arrstr.h>

class FileCounter{
public:
	FileCounter();
	void AddPath(wxString path);
	void AddPaths(wxArrayString paths);
	long GetCount();
	bool Count();
	bool Clear();

private:
	//This is the actualy counting function
	bool CountFolder(wxString path);

	wxArrayString m_Paths;
	long m_Count;
};	

#endif
