/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 - 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "jobdata.h"
#include "../rules.h"

JobData::JobData(const wxString& name){
	m_Name = name;
	rules = NULL;
}

JobData::~JobData(){
	delete rules;
}

