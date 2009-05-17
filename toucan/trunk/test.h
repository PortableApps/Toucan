/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_TEST
#define H_TEST

#include "variables.h"
#include "rules.h"
#include <wx/string.h>
#include <wx/datetime.h>
#include <wx/arrstr.h>
#include <wx/app.h>
#include <cxxtest/TestSuite.h>

//Please note when modifying this file the descriptors at the end of cmdline.cpp need to be updated too

class Tests : public CxxTest::TestSuite{
public:
	void testVariables(){
		TS_TRACE("Need test for @label@");
		TS_TRACE("Need test for @volume@");
		TS_TRACE("Need test for @drive@");
		TS_TRACE("Need test for @docs@");

		TS_ASSERT_EQUALS(Normalise(Normalise(wxT("@date@"))), wxDateTime::Now().FormatISODate());
		TS_ASSERT_EQUALS(Normalise(Normalise(wxT("@time@"))), wxDateTime::Now().Format(wxT("%H")) + wxT("-") +  wxDateTime::Now().Format(wxT("%M")));
		TS_ASSERT_EQUALS(Normalise(Normalise(wxT("@YYYY@"))), wxDateTime::Now().Format(wxT("%Y")));
		TS_ASSERT_EQUALS(Normalise(Normalise(wxT("@MM@"))), wxDateTime::Now().Format(wxT("%m")));
		TS_ASSERT_EQUALS(Normalise(Normalise(wxT("@DD@"))), wxDateTime::Now().Format(wxT("%d")));
		TS_ASSERT_EQUALS(Normalise(Normalise(wxT("@hh@"))), wxDateTime::Now().Format(wxT("%H")));
		TS_ASSERT_EQUALS(Normalise(Normalise(wxT("@mm@"))), wxDateTime::Now().Format(wxT("%M")));
		TS_ASSERT_EQUALS(Normalise(Normalise(wxT("C:\\thisisa@test\\path.doc"))), wxT("C:\\thisisa@test\\path.doc"));
		TS_ASSERT_EQUALS(Normalise(Normalise(wxT("C:\\testdir\\@date@.zip"))), wxT("C:\\testdir\\") + wxDateTime::Now().FormatISODate() + wxT(".zip"));
		TS_ASSERT_EQUALS(Normalise(Normalise(wxT("C:\\@YYYY@@MM@\\@date@.zip"))), wxT("C:\\") + wxDateTime::Now().Format(wxT("%Y")) + wxDateTime::Now().Format(wxT("%m")) + wxT("\\") + wxDateTime::Now().FormatISODate()+ wxT(".zip"));
		TS_ASSERT_EQUALS(Normalise(Normalise(wxT("C:\\testdir\\@@date@.zip"))), wxT("C:\\testdir\\@") + wxDateTime::Now().FormatISODate() + wxT(".zip"));
	}
	void testRules(){
		TS_TRACE("Need more complex rules tests");
		wxArrayString fileexclude, folderexclude, locationinclude;
		Rules rules;
		//Filepath and is it a directory
		std::map<wxString, bool> list;
		//Add the exclusions and inclusions
		fileexclude.Add(wxT(".doc"));
		fileexclude.Add(wxT("test"));
		rules.SetFilesToExclude(fileexclude);
		folderexclude.Add(wxT("\\testexclude"));
		rules.SetFoldersToExclude(folderexclude);
		locationinclude.Add(wxT("++"));
		rules.SetLocationsToInclude(locationinclude);
		TS_ASSERT_EQUALS(rules.ShouldExclude(wxT("C:\\"), true), false); //Included
		TS_ASSERT_EQUALS(rules.ShouldExclude(wxT("C:\\test.doc"), false), true); //Excluded
		TS_ASSERT_EQUALS(rules.ShouldExclude(wxT("C:\\Users\\Test\\Documents\\"), true), false); //Included
		TS_ASSERT_EQUALS(rules.ShouldExclude(wxT("C:\\Users\\Test\\Documents\\TEST.txt"), false), true); //Excluded
		TS_ASSERT_EQUALS(rules.ShouldExclude(wxT("C:\\Users\\Test\\Documents\\test.txt"), false), true); //Excluded
		TS_ASSERT_EQUALS(rules.ShouldExclude(wxT("C:\\Users\\Testexclude\\test.txt"), false), true); //Excluded
		TS_ASSERT_EQUALS(rules.ShouldExclude(wxT("C:\\Users\\Testexclude\\specialchars++.txt"), false), false); //Included
	}
};

#endif
