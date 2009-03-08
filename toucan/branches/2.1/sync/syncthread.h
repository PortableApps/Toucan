/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SYNC
#define H_SYNC

#include <map>
#include <list>
#include <wx/thread.h>
#include "syncdata.h"
	
enum location{
	SOURCE = 1,
	DEST = 2
};

class SyncThread : public wxThread
{
public:
	//Constructor
	SyncThread(SyncData data, Rules rules):wxThread(){
		m_Data = data;
		m_Rules = rules;
	}
	//Thread entry
	virtual void *Entry();

private:
	//Functions
	//Adds all the contents of a folder to a std::list
	std::list<wxString> FolderContentsToList(wxString path);
	//Merges two vectors into a map(wxString, location)
	std::map<wxString, location> MergeListsToMap(std::list<wxString> sourcelist, std::list<wxString> destlist);
	//Iterates through the map and calles the appropriate copying operations
	bool OperationCaller(std::map<wxString, location>);
	//The functions that do the copying
	bool OnSourceNotDest(wxString path);
	bool OnNotSourceDest(wxString path);
	bool OnSourceAndDest(wxString path);
	//Variables
	SyncData m_Data;
	Rules m_Rules;
};


#endif
