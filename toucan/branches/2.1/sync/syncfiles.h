/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SYNCFILES
#define H_SYNCFILES

#include "syncbase.h"
#include "syncdata.h"

class SyncFiles : public SyncBase{
public:
	SyncFiles(wxString syncsource, wxString syncdest, SyncData* syncdata);
	bool Execute();
private:
	bool OnSourceNotDestFile(wxString path);
	bool OnNotSourceDestFile(wxString path);
	bool OnSourceAndDestFile(wxString path);
	bool OnSourceNotDestFolder(wxString path);
	bool OnNotSourceDestFolder(wxString path);
	bool OnSourceAndDestFolder(wxString path);
	//Uses a temp file to copy neatly, currently only returns true, wxWidgets will output error messages
	bool CopyFile(wxString source, wxString dest);
};

#endif
