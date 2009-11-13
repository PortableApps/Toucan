/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SECUREJOB
#define H_SECUREJOB

class SecureData;
#include "../job.h"

/*!
 * A specialisation of the Job class for a Secure job.
 */
class SecureJob : public Job{

public:
	SecureJob(SecureData *Data);
	virtual void* Entry();
};

#endif
