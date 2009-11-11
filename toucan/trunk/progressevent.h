/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_PROGRESSEVENT
#define H_PROGRESSEVENT

#include <wx/event.h>

class ProgressEvent : public wxEvent{

public:
	ProgressEvent(wxEventType type, int id, wxString message, bool time = false, bool error = false) : 
	wxEvent(id, type), m_Message(message), m_Time(time), m_Error(error){
	}

	virtual wxEvent* Clone() const;
	const wxString& GetMessage();
	bool ShowTime();
	bool ShowError();

private:
	wxString m_Message;
	bool m_Time;
	bool m_Error;
};

wxDEFINE_EVENT(PROGRESS_SEND, ProgressEvent);

#endif
