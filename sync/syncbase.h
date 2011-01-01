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
#include <wx/filename.h>

enum Location{
    Source,
    Dest,
    SourceAndDest
};

class SyncBase
{
public:
	SyncBase(const wxFileName &syncsource, const wxFileName &syncdest, SyncData* syncdata);
	virtual ~SyncBase();

protected:
	std::list<wxString> FolderContentsToList(const wxFileName &path);
    std::map<wxString, Location> MergeListsToMap(std::list<wxString> sourcelist, std::list<wxString> destlist);
    void OperationCaller(std::map<wxString, Location> paths);

	virtual void OnSourceNotDestFile(const wxFileName &source, const wxFileName &dest) = 0;
	virtual void OnNotSourceDestFile(const wxFileName &source, const wxFileName &dest) = 0;
	virtual void OnSourceAndDestFile(const wxFileName &source, const wxFileName &dest) = 0;
	virtual void OnSourceNotDestFolder(const wxFileName &source, const wxFileName &dest) = 0;
	virtual void OnNotSourceDestFolder(const wxFileName &source, const wxFileName &dest) = 0;
	virtual void OnSourceAndDestFolder(const wxFileName &source, const wxFileName &dest) = 0;

	bool ShouldCopySize(const wxFileName &source, const wxFileName &dest);
	bool ShouldCopyTime(const wxFileName &source, const wxFileName &dest);
	bool ShouldCopyShort(const wxFileName &source, const wxFileName &dest);
	bool ShouldCopyFull(const wxFileName &source, const wxFileName &dest);

	wxFileName sourceroot;
	wxFileName destroot;
	SyncData *data;
};

#endif
