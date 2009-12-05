/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SYNCPREVIEW
#define H_SYNCPREVIEW

class SyncData;
#include "syncbase.h"
#include "../controls/vdtc.h"

class SyncPreview : public SyncBase{
public:

	SyncPreview(const wxString &syncsource, const wxString &syncdest, SyncData* syncdata, bool issource);
	VdtcTreeItemBaseArray Execute();

protected:
	bool OperationCaller(std::map<wxString, int> paths);
	void  OnSourceNotDestFile(const wxString &path);
	void  OnNotSourceDestFile(const wxString &path);
	void  OnSourceAndDestFile(const wxString &path);
	void  OnSourceNotDestFolder(const wxString &path);
	void  OnNotSourceDestFolder(const wxString &path);
	void  OnSourceAndDestFolder(const wxString &path);
	bool ShouldCopy(const wxString &source, const wxString &dest);

private:
	VdtcTreeItemBaseArray sourceitems;
	VdtcTreeItemBaseArray destitems;
	int GetItemLocation(const wxString &path, VdtcTreeItemBaseArray* array);
	bool sourcetree;
};

#endif
