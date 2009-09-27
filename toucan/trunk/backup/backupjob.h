/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_BACKUPJOB
#define H_BACKUPJOB

class BackupData;
#include "../job.h"

/*!
 * A specialisation of the Job class for a Backup job.
 */
class BackupJob : public Job{

public:
	BackupJob(BackupData *Data);
	bool Execute();
};

#endif
