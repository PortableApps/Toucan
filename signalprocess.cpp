/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "toucan.h"
#include "signalprocess.h"

void SignalProcess::OnTerminate(int WXUNUSED(pid), int status){
	//Set the status
	wxGetApp().m_ProcessStatusMap[m_id] = status;
	//Signal to toucan that we have finished
	wxGetApp().m_StatusMap[m_id] = true;
}
