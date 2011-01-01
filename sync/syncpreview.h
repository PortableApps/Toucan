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
	virtual void OnSourceNotDestFile(const wxFileName &source, const wxFileName &dest);
	virtual void OnNotSourceDestFile(const wxFileName &source, const wxFileName &dest);
	virtual void OnSourceAndDestFile(const wxFileName &source, const wxFileName &dest);
	virtual void OnSourceNotDestFolder(const wxFileName &source, const wxFileName &dest);
	virtual void OnNotSourceDestFolder(const wxFileName &source, const wxFileName &dest);
	virtual void OnSourceAndDestFolder(const wxFileName &source, const wxFileName &dest);

	bool CopyIfNeeded(const wxFileName &source, const wxFileName &dest);

private:
    DirCtrlIter FindPath(DirCtrlItemArray* items, const wxFileName &path);

	DirCtrlItemArray sourceitems;
	DirCtrlItemArray destitems;
	bool sourcetree;
};

#endif