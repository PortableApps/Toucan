/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SYNCTHREAD
#define H_SYNCTHREAD

#include <wx/thread.h>
#include "syncdata.h"

class SyncThread : public wxThread
{
public:
	//Constructor
	SyncThread(SyncData data, Rules rules):wxThread(){
		m_Data = data;
		m_Rules = rules;
	}
	//Thread entry
	virtual void *Entry();

private:
	//Variables
	SyncData m_Data;
	Rules m_Rules;
};


#endif
