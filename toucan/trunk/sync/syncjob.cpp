/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "syncjob.h"
#include "../toucan.h"
#include "../variables.h"
#include "../data/syncdata.h"
#include "../sync/syncthread.h"

#include <wx/thread.h>

SyncJob::SyncJob(SyncData *Data) : Job(Data){
	;
}

bool SyncJob::Execute(){
	SyncData *data = static_cast<SyncData*>(GetData());
	data->SetSource(Normalise(data->GetSource()));
	data->SetDest(Normalise(data->GetDest()));
	//Create a new Sync thread and run it
	//If we are running in the command line then we need joinable so the app doesnt return early
	wxThreadKind type = wxTHREAD_DETACHED;
	if(!wxGetApp().GetUsesGUI()){
		type = wxTHREAD_JOINABLE;
	}
	SyncThread *thread = new SyncThread(data, type);
	thread->Create();
	thread->Run();
	if(!wxGetApp().GetUsesGUI()){
		thread->Wait();
	}
	return true;
}
