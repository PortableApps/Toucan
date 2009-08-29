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
	SyncFiles(wxString syncsource, wxString syncdest, SyncData* syncdata);
	bool Execute();
protected:
	virtual void OnSourceNotDestFile(wxString path);
	virtual void OnNotSourceDestFile(wxString path);
	virtual void OnSourceAndDestFile(wxString path);
	virtual void OnSourceNotDestFolder(wxString path);
	virtual void OnNotSourceDestFolder(wxString path);
	virtual void OnSourceAndDestFolder(wxString path);
	bool CopyFilePlain(wxString source, wxString dest);
	bool CopyFileStream(wxString source, wxString dest);
	bool CopyFileTimestamp(wxString source, wxString dest);
	bool CopyFolderTimestamp(wxString source, wxString dest);
	bool RemoveDirectory(wxString path);
	bool RemoveFile(wxString path);
	//Copy the newer file to the older location
	bool SourceAndDestCopy(wxString source, wxString dest);
};

#endif
