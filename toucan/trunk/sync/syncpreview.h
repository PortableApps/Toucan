/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SYNCPREVIEW
#define H_SYNCPREVIEW

#include "syncfiles.h"
#include "../rules.h"
#include "../data/syncdata.h"
#include "../controls/vdtc.h"

class SyncPreview : public SyncFiles{
public:
	SyncPreview(wxString syncsource, wxString syncdest, SyncData* syncdata, bool issource);
	VdtcTreeItemBaseArray Execute();
protected:
	bool OperationCaller(std::map<const wxString, short> paths);
	//This tells us on which side of the operation we are on
	bool sourcetree;
	//Get the position of a given string
	int GetItemLocation(wxString path, VdtcTreeItemBaseArray* array);
	//Thes are the working arrays, at the end of the function will choose the correct one to return
	VdtcTreeItemBaseArray sourceitems;
	VdtcTreeItemBaseArray destitems;
	//Overwrite the functions so we can add the previewing
	void  OnSourceNotDestFile(wxString path);
	void  OnNotSourceDestFile(wxString path);
	void  OnSourceAndDestFile(wxString path);
	void  OnSourceNotDestFolder(wxString path);
	void  OnNotSourceDestFolder(wxString path);
	void  OnSourceAndDestFolder(wxString path);
	//This function does the file stream stuff
	bool ShouldCopy(wxString source, wxString dest);
};

#endif
