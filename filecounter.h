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

	void AddPath(const wxString &path);
	void AddPaths(const wxArrayString &paths);
	long GetCount() const;
	bool Count();
	
private:
	bool CountFolder(wxString path);
	wxArrayString m_Paths;
	long m_Count;
};	

#endif
