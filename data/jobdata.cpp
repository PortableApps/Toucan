/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 - 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "jobdata.h"
#include "../rules.h"

JobData::JobData(const wxString& name){
	m_Name = name;
	m_Rules = NULL;
}

JobData::~JobData(){
	delete m_Rules;
}

