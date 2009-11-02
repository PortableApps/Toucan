/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "progressevent.h"

wxEvent* ProgressEvent::Clone() const{ 
	return new ProgressEvent(*this); 
}

const wxString& ProgressEvent::GetMessage(){ 
	return m_Message;
}

bool ProgressEvent::ShowTime(){ 
	return m_Time;
}

bool ProgressEvent::ShowError(){ 
	return m_Error;
}

wxDECLARE_EVENT(PROGRESS_SEND, ProgressEvent);
#define EVT_PROGRESS(id, func) wx__DECLARE_EVT1(PROGRESS_SEND, id, &func)
