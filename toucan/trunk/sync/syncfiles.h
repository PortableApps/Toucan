/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SYNCFILES
#define H_SYNCFILES

class SyncData;
class Rules;
#include "syncbase.h"
#include <wx/string.h>

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
	bool CopyFilePlain(wxString source, wxString dest);
	bool CopyFileHash(wxString source, wxString dest);
	bool CopyFileTimestamp(wxString source, wxString dest);
	bool CopyFolderTimestamp(wxString source, wxString dest);
	bool DeleteDirectory(wxString path);
	bool RemoveFile(wxString path);
	//Copy the newer file to the older location
	bool SourceAndDestCopy(wxString source, wxString dest);
};

#endif
