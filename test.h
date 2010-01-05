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

class VariablesTests : public CxxTest::TestSuite{
public:
	void testDate(){
		TS_ASSERT_EQUALS(Normalise(wxT("@date@")), wxDateTime::Now().FormatISODate());
	}

	void testTime(){
		TS_ASSERT_EQUALS(Normalise(wxT("@time@")), wxDateTime::Now().Format(wxT("%H")) + wxT("-") +  wxDateTime::Now().Format(wxT("%M")));
	}

	void testYear(){
		TS_ASSERT_EQUALS(Normalise(wxT("@YYYY@")), wxDateTime::Now().Format(wxT("%Y")));
	}

	void testMonth(){
		TS_ASSERT_EQUALS(Normalise(wxT("@MM@")), wxDateTime::Now().Format(wxT("%m")));
	}

	void testDay(){
		TS_ASSERT_EQUALS(Normalise(wxT("@DD@")), wxDateTime::Now().Format(wxT("%d")));
	}

	void testHour(){
		TS_ASSERT_EQUALS(Normalise(wxT("@hh@")), wxDateTime::Now().Format(wxT("%H")));
	}

	void testMinute(){
		TS_ASSERT_EQUALS(Normalise(wxT("@mm@")), wxDateTime::Now().Format(wxT("%M")));
	}

	void testAtEmbedded(){
		TS_ASSERT_EQUALS(Normalise(wxT("C:\\thisisa@test\\path.doc")), wxT("C:\\thisisa@test\\path.doc"));
	}

	void testDateEmbedded(){
		TS_ASSERT_EQUALS(Normalise(wxT("C:\\testdir\\@date@.zip")), wxT("C:\\testdir\\") + wxDateTime::Now().FormatISODate() + wxT(".zip"));
	}

	void testBunchedEmbeddded(){
		TS_ASSERT_EQUALS(Normalise(wxT("C:\\@YYYY@@MM@\\@date@.zip")), wxT("C:\\") + wxDateTime::Now().Format(wxT("%Y")) + wxDateTime::Now().Format(wxT("%m")) + wxT("\\") + wxDateTime::Now().FormatISODate()+ wxT(".zip"));
	}

	void testExtraAt(){
		TS_ASSERT_EQUALS(Normalise(wxT("C:\\testdir\\@@date@.zip")), wxT("C:\\testdir\\@") + wxDateTime::Now().FormatISODate() + wxT(".zip"));
		TS_ASSERT_EQUALS(Normalise(wxT("C:\\testdir\\@@date@@.zip")), wxT("C:\\testdir\\@") + wxDateTime::Now().FormatISODate() + wxT("@.zip"));
	}
};

class RulesTests : public CxxTest::TestSuite{
public:
	Rules *rules;
	wxArrayString fileexclude, folderexclude, locationinclude;

	void setUp(){
		rules = new Rules("testrules");

		//Set up excluded files
		fileexclude.Add(wxT(".doc"));
		fileexclude.Add(wxT("testex"));
		rules->SetExcludedFiles(fileexclude);

		//Set up excluded folders
		folderexclude.Add(wxT("\\testex"));
		rules->SetExcludedFolders(folderexclude);

		//Set up included locations
		locationinclude.Add("++");
		locationinclude.Add("testinc.txt");
		rules->SetIncludedLocations(locationinclude);		
	}

	void tearDown(){
		delete rules;
	}

	void testIncludeDefault(){
		TS_ASSERT_EQUALS(rules->ShouldExclude(wxT("C:\\"), true), false); //Included
		TS_ASSERT_EQUALS(rules->ShouldExclude(wxT("C:\\file.txt"), false), false); //Included
	}

	void testExcludeFile(){
		TS_ASSERT_EQUALS(rules->ShouldExclude(wxT("C:\\test.doc"), false), true); //Excluded
		TS_ASSERT_EQUALS(rules->ShouldExclude(wxT("C:\\testex.txt"), false), true); //Excluded
	}

	void testExcludeFolder(){
		TS_ASSERT_EQUALS(rules->ShouldExclude(wxT("C:\\Users\\Testexclude\\"), true), true); //Excluded
		TS_ASSERT_EQUALS(rules->ShouldExclude(wxT("C:\\Users\\Testexclude\\test.txt"), false), true); //Excluded
	}

	void testIncludeLocation(){
		TS_ASSERT_EQUALS(rules->ShouldExclude(wxT("C:\\Users\\Testexclude\\testinc.txt"), false), false); //Included
	}
	
	void testCaseInsensative(){
		TS_ASSERT_EQUALS(rules->ShouldExclude(wxT("C:\\Users\\Test\\Documents\\TESTEX.txt"), false), true); //Excluded
	}

	void testPlusSigns(){
		TS_ASSERT_EQUALS(rules->ShouldExclude(wxT("C:\\Users\\Test\\specialchars++.txt"), false), false); //Included
	}
};

class FileCounterTests : public CxxTest::TestSuite{
public:

	wxString testdir;
	std::vector<wxString> folderlist;

	void setUp(){
		testdir = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + "unittests" + wxFILE_SEP_PATH;
		folderlist.push_back(testdir);
		folderlist.push_back(testdir + "subdir1");
		folderlist.push_back(testdir + "subdir2");
		folderlist.push_back(testdir + "subdir1" + wxFILE_SEP_PATH + "subdir1");
		folderlist.push_back(testdir + "subdir1" + wxFILE_SEP_PATH + "subdir2");
		
		std::for_each(folderlist.begin(), folderlist.end(), makedir);
		std::for_each(folderlist.begin(), folderlist.end(), createfiles);
	}

	void tearDown(){
		//Flip the list as files must be removed before folders
		std::reverse(folderlist.begin(), folderlist.end());
		std::for_each(folderlist.begin(), folderlist.end(), deletefiles);
		std::for_each(folderlist.begin(), folderlist.end(), deletedir);
	}

	void testDirectory(){
		FileCounter counter;
		counter.AddPath(testdir);
		counter.Count();
		TS_ASSERT_EQUALS(counter.GetCount(), 10);
	}

	void testFiles(){
		FileCounter counter;
		counter.AddPath(testdir + "file1");
		counter.AddPath(testdir + "subdir1" + wxFILE_SEP_PATH + "file1");
		counter.Count();
		TS_ASSERT_EQUALS(counter.GetCount(), 2);
	}
};

#endif
