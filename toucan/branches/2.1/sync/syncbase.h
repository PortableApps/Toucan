/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SYNCBASE
#define H_SYNCBASE

#include <map>
#include <list>
#include <wx/string.h>

enum location{
	SOURCE = 1,
	DEST = 2
};

class SyncBase{
public:
	SyncBase();
	virtual ~SyncBase();

	//Adds all the contents of a folder to a std::list
	std::list<wxString> FolderContentsToList(wxString path);
	//Merges two lists into a map(wxString, location), will location specifying where the items were merged from
	std::map<wxString, location> MergeListsToMap(std::list<wxString> sourcelist, std::list<wxString> destlist);
	//Iterates through the map and calles the appropriate copying operations
	bool OperationCaller(std::map<wxString, location>);
	//The functions that do the copying
	virtual bool OnSourceNotDestFile(wxString path) = 0;
	virtual bool OnNotSourceDestFile(wxString path) = 0;
	virtual bool OnSourceAndDestFile(wxString path) = 0;
	virtual bool OnSourceNotDestFolder(wxString path) = 0;
	virtual bool OnNotSourceDestFolder(wxString path) = 0;
	virtual bool OnSourceAndDestFolder(wxString path) = 0;

private:
	wxString sourceroot;
	wxString destroot;
};

#endif
