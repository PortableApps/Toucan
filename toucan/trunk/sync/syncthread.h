/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SYNCTHREAD
#define H_SYNCTHREAD

#include <wx/thread.h>
#include "../rules.h"
#include "../data/syncdata.h"

/*!
 * The thread that does the actual sync work. We use a thread because 
 * the wxCopyFile calls block the thread they are running in.
 */
class SyncThread : public wxThread
{
public:
	/*!
	 * Constructs a new thread using the given SyncData. In some instances 
	 * a joinable thread is needed, i.e. in console mode, and this is specified
	 * by the wxThreadKind
	 */
	SyncThread(SyncData *data, wxThreadKind type) : wxThread(type){
		m_Data = data;
	}
	/*!
	 * The entry point for the thread, this the the function that actually does
	 * the work.
	 */
	virtual void *Entry();

private:
	SyncData *m_Data;
};


#endif
