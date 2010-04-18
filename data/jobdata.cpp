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

template<> wxArrayString JobData::Read(const wxString& key){
	wxString temp;
	if(!wxGetApp().m_Jobs_Config->Read(GetName() + "/" +  key, &temp)){
		throw std::runtime_error(std::string("There was an error reading from the jobs file, looking for " + key));
	}
	wxArrayString strings;
	wxStringTokenizer tkz(temp, "|", wxTOKEN_STRTOK);
	while(tkz.HasMoreTokens()){  
		strings.Add(tkz.GetNextToken());
	}
	return strings;
}

template<> void JobData::Write(const wxString& key, wxArrayString value){
	wxString temp;
	for(unsigned int i = 0; i < value.GetCount(); i++){
		temp = temp + "|" + value.Item(i);
	}
	if(!wxGetApp().m_Jobs_Config->Write(GetName() + "/" +  key, temp)){
		throw std::runtime_error(std::string("There was an error writing to the jobs file"));
	}
}

