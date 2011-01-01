/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 - 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SYNCJOB
#define H_SYNCJOB

class SyncData;
#include "../job.h"
#include "syncbase.h"
#include <wx/string.h>


class SyncJob : public Job
{
public:
	SyncJob(SyncData *Data);
	virtual void* Entry();
};

class SyncFiles : public SyncBase
{
public:
	SyncFiles(const wxFileName &syncsource, const wxFileName &syncdest, SyncData* syncdata);
	bool Execute();

protected:
	virtual void OnSourceNotDestFile(const wxFileName &source, const wxFileName &dest);
	virtual void OnNotSourceDestFile(const wxFileName &source, const wxFileName &dest);
	virtual void OnSourceAndDestFile(const wxFileName &source, const wxFileName &dest);
	virtual void OnSourceNotDestFolder(const wxFileName &source, const wxFileName &dest);
	virtual void OnNotSourceDestFolder(const wxFileName &source, const wxFileName &dest);
	virtual void OnSourceAndDestFolder(const wxFileName &source, const wxFileName &dest);

	bool CopyIfNeeded(const wxFileName &source, const wxFileName &dest);
	bool CopyFile(const wxFileName &source, const wxFileName &dest);
	bool CopyFolderTimestamp(const wxFileName &source, const wxFileName &dest);
	bool SourceAndDestCopy(const wxFileName &source, const wxFileName &dest);

	bool DeleteDirectory(const wxFileName &path);
	bool RemoveFile(const wxFileName &path);
};

#endif
