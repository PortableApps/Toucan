/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_BACKUPJOB
#define H_BACKUPJOB

class BackupData;
#include <boost/shared_ptr.hpp>
#include "../job.h"

/*!
 * A specialisation of the Job class for a Backup job.
 */
class BackupJob : public Job{

public:
	BackupJob(BackupData *Data);
	virtual void* Entry();
protected:
    boost::shared_ptr<wxTextFile> CreateSysFile(const wxString &filename);
};

#endif
