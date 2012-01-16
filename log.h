/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 - 2011 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#ifndef LOGLISTCTRL_H
#define LOGLISTCTRL_H

#include <wx/log.h>

class wxTextFile;

//An empty log class to use when we are not logging to a file
class LogBlank : public wxLog
{};

class LogMessageQueue : public wxLog
{
public:
    void DoLogRecord(wxLogLevel level, const wxString& msg, const wxLogRecordInfo& info);
};

//Log to a (user specified) file
class LogFile : public wxLog
{
public:
    LogFile(wxTextFile* file);
    void DoLogRecord(wxLogLevel level, const wxString& msg, const wxLogRecordInfo& info);

private:
    wxTextFile *file;
    int count;
};

#endif
