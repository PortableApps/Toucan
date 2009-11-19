/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SIGNALPROCESS
#define H_SIGNALPROCESS

#include <wx/process.h>

class SignalProcess : public wxProcess{
	
public:
	SignalProcess(int id) : m_id(id)
	{
    }

	void OnTerminate(int pid, int status);

protected:
	int m_id;
};

#endif
