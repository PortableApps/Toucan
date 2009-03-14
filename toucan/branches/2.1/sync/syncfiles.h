/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SYNCFILES
#define H_SYNCFILES

#include "syncbase.h"
#include "syncdata.h"
#include "../rules.h"

class SyncFiles : public SyncBase{
public:
	SyncFiles(wxString syncsource, wxString syncdest, SyncData* syncdata, Rules rules);
	bool Execute();
protected:
	virtual bool OnSourceNotDestFile(wxString path);
	virtual bool OnNotSourceDestFile(wxString path);
	virtual bool OnSourceAndDestFile(wxString path);
	virtual bool OnSourceNotDestFolder(wxString path);
	virtual bool OnNotSourceDestFolder(wxString path);
	virtual bool OnSourceAndDestFolder(wxString path);
	//Uses a temp file to copy neatly, currently only returns true, wxWidgets will output error messages
	bool CopyFile(wxString source, wxString dest);
	//Implement an MD5 check before a CopyFile
	bool CopyFileHash(wxString source, wxString dest);
	bool RemoveDirectory(wxString path);
	//Implement a timestamp check before a CopyFileHash
	bool UpdateFile(wxString source, wxString dest);
	bool CopyFolderTimestamp(wxString source, wxString dest);
};

#endif
