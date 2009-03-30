/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "tests.h"
#include "basicfunctions.h"
#include "variables.h"
#include <wx/wx.h>
#include <map>

Tests::Tests(){
	total = 9;
	SetGaugeRange(total);
	OutputBlank();
	failures = 0;
	Variables();
	OutputProgress(wxString::Format(wxT("%d failures"), failures));
	OutputProgress(wxString::Format(wxT("%d successes"), total - failures));
}

void Tests::Variables(){
	//ATTN : Needs tests for label and volume
	std::map<wxString, wxString> list;
	list[wxT("@date@")] = wxDateTime::Now().FormatISODate();
	list[wxT("@time@")] = wxDateTime::Now().Format(wxT("%H")) + wxT("-") +  wxDateTime::Now().Format(wxT("%M"));
	list[wxT("@YYYY@")] = wxDateTime::Now().Format(wxT("%Y"));
	list[wxT("@MM@")] = wxDateTime::Now().Format(wxT("%m"));
	list[wxT("@DD@")] = wxDateTime::Now().Format(wxT("%d"));
	list[wxT("@hh@")] = wxDateTime::Now().Format(wxT("%H"));
	list[wxT("@mm@")] = wxDateTime::Now().Format(wxT("%M"));
	list[wxT("@drive@")] =  wxT("C:\\");
	list[wxT("@docs@")] =  wxT("C:\\Users\\Steven\\Documents");

	for(std::map<wxString, wxString>::iterator iter = list.begin(); iter != list.end(); ++iter){
		if(Normalise(Normalise((*iter).first)) != (*iter).second){
			OutputProgress((*iter).first + wxT(" failed"));
			failures++;
		}
		IncrementGauge();
	}
	return;
}
