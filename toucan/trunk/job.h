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
	/*!
	 * Constructs a new Job with the given JobData
	 */
	Job(JobData *Data);
	/*!
	 * Cleans up job, including deleting the associated JobData
	 */
	virtual ~Job();

	/*!
	 * Runs the job using the given JobData
	 */
	virtual bool Execute() = 0;

	/*!
	 * Gets the JobData associated with this job
	 */
	JobData *GetData() {return m_Data;}
	/*!
	 * sets the JobData associated with this job. Note the job takes control of 
	 * the data and destroys it when it is finished
	 */
	void SetData(JobData *Data){m_Data = Data;}
	
private:
	JobData *m_Data;
	
};

#endif
