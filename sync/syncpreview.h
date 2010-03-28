/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 - 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SYNCPREVIEW
#define H_SYNCPREVIEW

class SyncData;
#include "syncbase.h"
#include "../controls/dirctrl.h"


class SyncPreview : public SyncBase{
public:

	SyncPreview(const wxString &syncsource, const wxString &syncdest, SyncData* syncdata, bool issource);
	DirCtrlItemArray Execute();

protected:
	bool OperationCaller(std::map<wxString, int> paths);
	void OnSourceNotDestFile(const wxString &path);
	void OnNotSourceDestFile(const wxString &path);
	void OnSourceAndDestFile(const wxString &path);
	void OnSourceNotDestFolder(const wxString &path);
	void OnNotSourceDestFolder(const wxString &path);
	void OnSourceAndDestFolder(const wxString &path);

	bool CopyIfNeeded(const wxString &source, const wxString &dest);

private:
	DirCtrlItemArray sourceitems;
	DirCtrlItemArray destitems;
	bool sourcetree;
};

#endif