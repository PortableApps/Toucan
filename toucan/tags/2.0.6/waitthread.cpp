/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 Steven Lamerton
// Licence:     GNU GPL 2 (See readme for more info
/////////////////////////////////////////////////////////////////////////////////

#include "waitthread.h"

void *WaitThread::Entry(){
	while(wxProcess::Exists(m_PID)){
		m_Process->HasInput();
	}
	return NULL;
}
