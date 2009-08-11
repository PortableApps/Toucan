/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "securejob.h"
#include "secure.h"
#include "../toucan.h"
#include "../variables.h"
#include "../data/securedata.h"

#include <wx/filename.h>
#include <wx/textfile.h>

SecureJob::SecureJob(SecureData *Data) : Job(Data){
	;
}

bool SecureJob::Execute(){
	SecureData *data = static_cast<SecureData*>(GetData());
	for(unsigned int i = 0; i < data->GetLocations().GetCount(); i++){
		data->SetLocation(i, Normalise(data->GetLocation(i)));
	}
	Secure(data);
	return true;
}
