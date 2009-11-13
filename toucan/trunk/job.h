/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_JOB
#define H_JOB

#include "data/jobdata.h"

class Job : public wxThread {

public:

	Job(JobData *Data);
	virtual ~Job();

	virtual void* Entry() = 0;

	JobData *GetData() {return m_Data;}
	void SetData(JobData *Data){m_Data = Data;}
	
private:
	JobData *m_Data;
};

#endif
