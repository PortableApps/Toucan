/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SECUREJOB
#define H_SECUREJOB

class SecureData;
#include "../job.h"

class SecureJob : public Job{

public:
	SecureJob(SecureData *Data);
	virtual void* Entry();

private:
	bool Crypt(const wxString &path, SecureData *data);
	bool CryptFile(const wxString &path, SecureData *data);
};

#endif
