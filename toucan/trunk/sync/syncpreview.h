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

/*!
 * A specialisation of the SyncFiles class for previewing, it does not perform 
 * any file copies
 */
class SyncPreview : public SyncFiles{
public:
	/*!
	 * Constructs a new SyncPreview instance, issource indicates whether we 
	 * should pass the source or destination array back when the preview has 
	 * finished
	 */
	SyncPreview(wxString syncsource, wxString syncdest, SyncData* syncdata, bool issource);
	/*!
	 * Runs the preview, returning an array suitable for use in a wxVirtualDirTreeCtrl
	 */
	VdtcTreeItemBaseArray Execute();
protected:
	bool OperationCaller(std::map<const wxString, short> paths);
	void  OnSourceNotDestFile(const wxString &path);
	void  OnNotSourceDestFile(const wxString &path);
	void  OnSourceAndDestFile(const wxString &path);
	void  OnSourceNotDestFolder(const wxString &path);
	void  OnNotSourceDestFolder(const wxString &path);
	void  OnSourceAndDestFolder(const wxString &path);
	/*!
	 * A copy of the SyncFiles::CopyFileStream function but without any file 
	 * writing to check if a file should be copied
	 */
	bool ShouldCopy(const wxString &source, const wxString &dest);
private:
	VdtcTreeItemBaseArray sourceitems;
	VdtcTreeItemBaseArray destitems;
	int GetItemLocation(const wxString &path, VdtcTreeItemBaseArray* array);
	bool sourcetree;
};

#endif
