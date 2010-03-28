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
	SyncFiles(const wxString &syncsource, const wxString &syncdest, SyncData* syncdata);
	bool Execute();

protected:
	virtual void OnSourceNotDestFile(const wxString &path);
	virtual void OnNotSourceDestFile(const wxString &path);
	virtual void OnSourceAndDestFile(const wxString &path);
	virtual void OnSourceNotDestFolder(const wxString &path);
	virtual void OnNotSourceDestFolder(const wxString &path);
	virtual void OnSourceAndDestFolder(const wxString &path);

	bool CopyIfNeeded(const wxString &source, const wxString &dest);
	bool CopyFile(const wxString &source, const wxString &dest);
	bool CopyFolderTimestamp(const wxString &source, const wxString &dest);
	bool SourceAndDestCopy(const wxString &source, const wxString &dest);

	bool RemoveDirectory(wxString path);
	bool RemoveFile(const wxString &path);
};

#ifdef __WXMSW__
	DWORD CALLBACK CopyProgressRoutine(LARGE_INTEGER TotalFileSize, LARGE_INTEGER TotalBytesTransferred, LARGE_INTEGER StreamSize,
							  LARGE_INTEGER StreamBytesTransferred, DWORD dwStreamNumber, DWORD dwCallbackReason,
							  HANDLE hSourceFile, HANDLE hDestinationFile, LPVOID lpData);
#endif

#endif
