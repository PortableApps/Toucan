/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 - 2011 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#ifndef LOGLISTCTRL_H
#define LOGLISTCTRL_H

#include <wx/log.h>

namespace boost{
    namespace interprocess{
        class message_queue;
    }
}

class LogMessageQueue : public wxLog
{
public:
    LogMessageQueue();
    ~LogMessageQueue();
    void DoLogRecord(wxLogLevel level, const wxString& msg, const wxLogRecordInfo& info);
private: 
    boost::interprocess::message_queue* queue;
};

#endif
