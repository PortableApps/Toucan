/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SYNCFILES
#define H_SYNCFILES

class SyncData;
class Rules;

#include "syncbase.h"
#include <wx/string.h>

/*!
 * A specialisation of the SyncBase class for syncing files
 */
class SyncFiles : public SyncBase{
public:
	/*!
	 * Constructs a new SyncFiles instance with the given source, destination and data
	 */
	SyncFiles(wxString syncsource, wxString syncdest, SyncData* syncdata);
	/*!
	 * Runs the sync
	 */
	bool Execute();
protected:
	virtual void OnSourceNotDestFile(const wxString &path);
	virtual void OnNotSourceDestFile(const wxString &path);
	virtual void OnSourceAndDestFile(const wxString &path);
	virtual void OnSourceNotDestFolder(const wxString &path);
	virtual void OnNotSourceDestFolder(const wxString &path);
	virtual void OnSourceAndDestFolder(const wxString &path);
	/*!
	 * Copies a file from source to dest
	 */
	bool CopyFilePlain(const wxString &source, const wxString &dest);
	/*!
	 * Copies a file from source to dest using streams to see if the files are 
	 * identical or not
	 */
	bool CopyFileStream(const wxString &source, const wxString &dest);
	/*!
	 * Copies a file from source to dest but only if the source file is newer
	 */
	bool CopyFileTimestamp(const wxString &source, const wxString &dest);
	/*!
	 * Copies the folder timestamps from source to dest
	 */
	bool CopyFolderTimestamp(const wxString &source, const wxString &dest);
	/*!
	 * Removes the given directory and any sub directories and files
	 */
	bool RemoveDirectory(wxString path);
	/*!
	 * Removes the given file
	 */
	bool RemoveFile(const wxString &path);
	/*!
	 * Chooses which file to copy based on which is newer
	 */
	bool SourceAndDestCopy(const wxString &source, const wxString &dest);
};

#endif
