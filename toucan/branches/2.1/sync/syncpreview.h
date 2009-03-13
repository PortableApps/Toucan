/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SYNCPREVIEW
#define H_SYNCPREVIEW

#include "syncbase.h"
#include "syncdata.h"
#include "../rules.h"
#include "../controls/virtualdirtreectrl.h"

class SyncPreview : public SyncBase{
public:
	SyncPreview(wxString syncsource, wxString syncdest, SyncData* syncdata, Rules syncrules, bool issource);
	VdtcTreeItemBaseArray Execute();
private:
	//We have our own operation caller that doesnt call the ruls check as it is done by the individual functions
	bool OperationCaller(std::map<wxString, short> paths);
	bool OnSourceNotDestFile(wxString path);
	bool OnNotSourceDestFile(wxString path);
	bool OnSourceAndDestFile(wxString path);
	bool OnSourceNotDestFolder(wxString path);
	bool OnNotSourceDestFolder(wxString path);
	bool OnSourceAndDestFolder(wxString path);
	//This tells us on which side of the operation we are on
	bool sourcetree;
	VdtcTreeItemBaseArray items;
};

#endif
