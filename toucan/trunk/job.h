/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_JOB
#define H_JOB

#include "data/jobdata.h"

/*!
 * This is the base for all of the jobs in Toucan
 */
class Job{

public:
	Job(JobData *Data);
	virtual ~Job();

	virtual bool Execute() = 0;

	JobData *GetData() {return m_Data;}
	void SetData(JobData *Data){m_Data = Data;}
	
private:
	JobData *m_Data;
	
};

#endif
