/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SECUREPROCESS
#define H_SECUREPROCESS

#include "../signalprocess.h"

class SecureProcess : public SignalProcess{
	
public:
	SecureProcess(int id) : SignalProcess(id)
	{
    }

	void OnTerminate(int pid, int status);
};

#endif
