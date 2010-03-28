/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 - 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
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
	std::list<wxString> FolderContentsToList(wxString path);
	std::map<wxString, int> MergeListsToMap(std::list<wxString> sourcelist, std::list<wxString> destlist);
	bool OperationCaller(std::map<wxString, int> paths);

	virtual void OnSourceNotDestFile(const wxString &path) = 0;
	virtual void OnNotSourceDestFile(const wxString &path) = 0;
	virtual void OnSourceAndDestFile(const wxString &path) = 0;
	virtual void OnSourceNotDestFolder(const wxString &path) = 0;
	virtual void OnNotSourceDestFolder(const wxString &path) = 0;
	virtual void OnSourceAndDestFolder(const wxString &path) = 0;

	bool ShouldCopySize(const wxString &source, const wxString &dest);
	bool ShouldCopyTime(const wxString &source, const wxString &dest);
	bool ShouldCopyShort(const wxString &source, const wxString &dest);
	bool ShouldCopyFull(const wxString &source, const wxString &dest);

	bool preview;
	wxString sourceroot;
	wxString destroot;
	SyncData *data;
};

#endif
