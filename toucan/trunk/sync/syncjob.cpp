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
	SyncThread *thread = new SyncThread(data);
	thread->Create();
	thread->Run();
	return true;
}
