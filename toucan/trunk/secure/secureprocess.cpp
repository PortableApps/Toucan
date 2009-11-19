/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "../toucan.h"
#include "secureprocess.h"

void SecureProcess::OnTerminate(int WXUNUSED(pid), int status){
	//Update the status map with the returned value
	wxGetApp().m_ProcessStatusMap[m_id] = status;
	//Signal to toucan that we have finished
	wxGetApp().m_ProcessMap[m_id] = true;
}
