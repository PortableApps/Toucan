/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_RULES
#define H_RULES

class frmMain;

#include "path.h"

#include <wx/arrstr.h>
#include <wx/filename.h>
#include <wx/longlong.h>

#include <vector>

#include <boost/bimap.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/assign/list_inserter.hpp>

enum RuleFunction{
    FileInclude,
    FileExclude,
    FolderInclude,
    FolderExclude,
    AbsoluteFolderExclude
};

enum RuleType{
    Simple,
    Regex,
    Date,
    Size
};

enum RuleResult{
    NoMatch,
    Included,
    Excluded,
    AbsoluteExcluded
};

namespace{
    /*Gets a files size in petabytes, should do for now but it may need changing in a few years ;)
    Expects to be passed the result of a wxFilename.GetHumanReadableSize()*/
    double GetInPB(const wxString &value);
}

//A pair of bimaps for easily converting between our enums and strings
static const boost::bimap<wxString, RuleType> typemap = boost::assign::list_of<boost::bimap<wxString, RuleType>::relation>
    ("Simple", Simple)
    ("Regex", Regex)
    ("Size", Size)
    ("Date", Date);

static const boost::bimap<wxString, RuleFunction> functionmap = boost::assign::list_of<boost::bimap<wxString, RuleFunction>::relation>
    ("File Include", FileInclude)
    ("File Exclude", FileExclude)
    ("Folder Include", FolderInclude)
    ("Folder Exclude", FolderExclude)
    ("Absolute Folder Exclude", AbsoluteFolderExclude);

class Rule{
public:
    Rule(const wxString &rule, RuleFunction function, RuleType type){
        this->rule = rule;
        this->normalised = Path::Normalise(rule);
        this->function = function;
        this->type = type;
        this->valid = Validate();
    }

    RuleResult Matches(wxFileName path);
    bool IsValid() const { return valid; }

    wxString rule;
    RuleFunction function;
    RuleType type;

private:
    wxString normalised;
    bool valid;
    bool Validate();
};

class RuleSet{
public:
    RuleSet(const wxString &name) { this->name = name; }

    RuleResult Matches(wxFileName path);
    bool IsValid();

	bool TransferToFile();
	bool TransferFromFile();

	void Add(Rule rule) { rules.push_back(rule); }

	const wxString& GetName() const {return name;}
    const std::vector<Rule>& GetRules() const {return rules;}
private:
    std::vector<Rule> rules;
	wxString name;
};	

#endif
