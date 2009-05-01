/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "tests.h"
#include "basicfunctions.h"
#include "variables.h"
#include "rules.h"
#include <wx/wx.h>
#include <map>

Tests::Tests(){
	total = 18;
	SetGaugeRange(total);
	OutputBlank();
	failures = 0;
	VariablesTest();
	RulesTest();
	OutputProgress(wxString::Format(wxT("%d failures"), failures));
	OutputProgress(wxString::Format(wxT("%d successes"), total - failures));
}

void Tests::VariablesTest(){
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
	//Test for @ in path
	list[wxT("C:\\thisisa@test\\path.doc")] = wxT("C:\\thisisa@test\\path.doc");
	//A more complex test
	list[wxT("@drive@\\testdir\\@date@.zip")] = wxT("C:\\testdir\\") + wxDateTime::Now().FormatISODate() + wxT(".zip");

	for(std::map<wxString, wxString>::iterator iter = list.begin(); iter != list.end(); ++iter){
		if(Normalise(Normalise((*iter).first)) != (*iter).second){
			OutputProgress((*iter).first + wxT(" failed"));
			failures++;
		}
		IncrementGauge();
	}
	return;
}

void Tests::RulesTest(){
	wxArrayString fileexclude, folderexclude, locationinclude;
	Rules rules;
	//Filepath and is it a directory
	std::map<wxString, bool> list;
	//Set up the paths
	//Should exclude?
	list[(wxT("C:\\"))] = false; //Included
	list[(wxT("C:\\test.doc"))] = true; //Excluded
	list[(wxT("C:\\Users\\Test\\Documents\\"))] = false; //Included
	list[(wxT("C:\\Users\\Test\\Documents\\TEST.txt"))] = true; //Excluded
	list[(wxT("C:\\Users\\Test\\Documents\\test.txt"))] = true; //Excluded
	list[(wxT("C:\\Users\\Testexclude\\test.txt"))] = true; //Excluded
	list[(wxT("C:\\Users\\Testexclude\\specialchars++.txt"))] = false; //Included
	//Add the exclusions and inclusions
	fileexclude.Add(wxT(".doc"));
	fileexclude.Add(wxT("test"));
	rules.SetFilesToExclude(fileexclude);
	folderexclude.Add(wxT("\\testexclude"));
	rules.SetFoldersToExclude(folderexclude);
	locationinclude.Add(wxT("++"));
	rules.SetLocationsToInclude(locationinclude);
	//Run the tests
	for(std::map<wxString, bool>::iterator iter = list.begin(); iter != list.end(); ++iter){
		bool isdir = false;
		if((*iter).first.Right(1) == wxFILE_SEP_PATH){
			isdir = true;	
		}
		if(rules.ShouldExclude((*iter).first, isdir) != (*iter).second){
			OutputProgress((*iter).first + wxT(" failed"));
			failures++;
		}
		IncrementGauge();
	}
}
