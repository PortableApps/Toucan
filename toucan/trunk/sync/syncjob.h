/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SYNCJOB
#define H_SYNCJOB

class SyncData;
#include "../job.h"

/*!
 * A specialisation of the Job class for a Sync job.
 */
class SyncJob : public Job{

public:
	SyncJob(SyncData *Data);
	bool Execute();
};

#endif
