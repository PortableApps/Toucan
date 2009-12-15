/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_TEST
#define H_TEST

#include "basicfunctions.h"
#include "variables.h"
#include "rules.h"
#include "filecounter.h"
#include <wx/string.h>
#include <wx/datetime.h>
#include <wx/arrstr.h>
#include <wx/stdpaths.h>
#include <wx/textfile.h>
#include <cxxtest/TestSuite.h>
#include <algorithm>
#include <vector>

class GeneralTests : public CxxTest::TestSuite{
public:
	void testVariables(){
		TS_TRACE("Need test for @label@");
		TS_TRACE("Need test for @volume@");
		TS_TRACE("Need test for @drive@");
		TS_TRACE("Need test for @docs@");

		TS_ASSERT_EQUALS(Normalise(wxT("@date@")), wxDateTime::Now().FormatISODate());
		TS_ASSERT_EQUALS(Normalise(wxT("@time@")), wxDateTime::Now().Format(wxT("%H")) + wxT("-") +  wxDateTime::Now().Format(wxT("%M")));
		TS_ASSERT_EQUALS(Normalise(wxT("@YYYY@")), wxDateTime::Now().Format(wxT("%Y")));
		TS_ASSERT_EQUALS(Normalise(wxT("@MM@")), wxDateTime::Now().Format(wxT("%m")));
		TS_ASSERT_EQUALS(Normalise(wxT("@DD@")), wxDateTime::Now().Format(wxT("%d")));
		TS_ASSERT_EQUALS(Normalise(wxT("@hh@")), wxDateTime::Now().Format(wxT("%H")));
		TS_ASSERT_EQUALS(Normalise(wxT("@mm@")), wxDateTime::Now().Format(wxT("%M")));
		TS_ASSERT_EQUALS(Normalise(wxT("C:\\thisisa@test\\path.doc")), wxT("C:\\thisisa@test\\path.doc"));
		TS_ASSERT_EQUALS(Normalise(wxT("C:\\testdir\\@date@.zip")), wxT("C:\\testdir\\") + wxDateTime::Now().FormatISODate() + wxT(".zip"));
		TS_ASSERT_EQUALS(Normalise(wxT("C:\\@YYYY@@MM@\\@date@.zip")), wxT("C:\\") + wxDateTime::Now().Format(wxT("%Y")) + wxDateTime::Now().Format(wxT("%m")) + wxT("\\") + wxDateTime::Now().FormatISODate()+ wxT(".zip"));
		TS_ASSERT_EQUALS(Normalise(wxT("C:\\testdir\\@@date@.zip")), wxT("C:\\testdir\\@") + wxDateTime::Now().FormatISODate() + wxT(".zip"));
	}
	void testRules(){
		TS_TRACE("Need more complex rules tests");
		wxArrayString fileexclude, folderexclude, locationinclude;
		Rules rules(wxT("TestRules"));
		//Add the exclusions and inclusions
		fileexclude.Add(wxT(".doc"));
		fileexclude.Add(wxT("test"));
		rules.SetExcludedFiles(fileexclude);
		folderexclude.Add(wxT("\\testexclude"));
		rules.SetExcludedFolders(folderexclude);
		locationinclude.Add(wxT("++"));
		rules.SetIncludedLocations(locationinclude);
		TS_ASSERT_EQUALS(rules.ShouldExclude(wxT("C:\\"), true), false); //Included
		TS_ASSERT_EQUALS(rules.ShouldExclude(wxT("C:\\test.doc"), false), true); //Excluded
		TS_ASSERT_EQUALS(rules.ShouldExclude(wxT("C:\\Users\\Test\\Documents\\"), true), false); //Included
		TS_ASSERT_EQUALS(rules.ShouldExclude(wxT("C:\\Users\\Test\\Documents\\TEST.txt"), false), true); //Excluded
		TS_ASSERT_EQUALS(rules.ShouldExclude(wxT("C:\\Users\\Test\\Documents\\test.txt"), false), true); //Excluded
		TS_ASSERT_EQUALS(rules.ShouldExclude(wxT("C:\\Users\\Testexclude\\test.txt"), false), true); //Excluded
		TS_ASSERT_EQUALS(rules.ShouldExclude(wxT("C:\\Users\\Testexclude\\specialchars++.txt"), false), false); //Included
	}
};

class FileCounterTests : public CxxTest::TestSuite{
public:
	void setUp(){
		const wxString testdir = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + "unittests";
		std::vector<wxString> folderlist;
		folderlist.push_back(testdir);
		folderlist.push_back(testdir + wxFILE_SEP_PATH + "subdir1");
		folderlist.push_back(testdir + wxFILE_SEP_PATH + "subdir2");
		folderlist.push_back(testdir + wxFILE_SEP_PATH + "subdir1" + wxFILE_SEP_PATH + "subdir1");
		folderlist.push_back(testdir + wxFILE_SEP_PATH + "subdir1" + wxFILE_SEP_PATH + "subdir2");
		
		std::for_each(folderlist.begin(), folderlist.end(), makedir);
		std::for_each(folderlist.begin(), folderlist.end(), createfiles);
	}

	void tearDown(){
		const wxString testdir = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + "unittests";
		std::vector<wxString> folderlist;
		folderlist.push_back(testdir);
		folderlist.push_back(testdir + wxFILE_SEP_PATH + "subdir1");
		folderlist.push_back(testdir + wxFILE_SEP_PATH + "subdir2");
		folderlist.push_back(testdir + wxFILE_SEP_PATH + "subdir1" + wxFILE_SEP_PATH + "subdir1");
		folderlist.push_back(testdir + wxFILE_SEP_PATH + "subdir1" + wxFILE_SEP_PATH + "subdir2");

		std::reverse(folderlist.begin(), folderlist.end());
		std::for_each(folderlist.begin(), folderlist.end(), deletefiles);
		std::for_each(folderlist.begin(), folderlist.end(), deletedir);
	}

	void testCounter(){
		const wxString unittestdir = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + "unittests";
		FileCounter counter;
		counter.AddPath(unittestdir);
		counter.Count();
		TS_ASSERT_EQUALS(counter.GetCount(), 10);
	}
};

#endif
