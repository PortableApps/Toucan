/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SYNCBASE
#define H_SYNCBASE

class SyncData;
class Rules;

#include <map>
#include <list>
#include <wx/string.h>

class SyncBase
{
public:
	SyncBase(const wxString &syncsource, const wxString &syncdest, SyncData* syncdata);
	virtual ~SyncBase();

protected:
	std::list<const wxString> FolderContentsToList(wxString path);
	std::map<const wxString, int> MergeListsToMap(std::list<const wxString> sourcelist, std::list<const wxString> destlist);
	bool OperationCaller(std::map<const wxString, int> paths);

	virtual void OnSourceNotDestFile(const wxString &path) = 0;
	virtual void OnNotSourceDestFile(const wxString &path) = 0;
	virtual void OnSourceAndDestFile(const wxString &path) = 0;
	virtual void OnSourceNotDestFolder(const wxString &path) = 0;
	virtual void OnNotSourceDestFolder(const wxString &path) = 0;
	virtual void OnSourceAndDestFolder(const wxString &path) = 0;

	bool preview;
	bool disablestreams;
	wxString sourceroot;
	wxString destroot;
	SyncData *data;
};

#endif
