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

//A pair of bimaps for easily converting between our enums and strings
static const boost::bimap<wxString, RuleType> typemap = boost::assign::list_of<boost::bimap<wxString, RuleType>::relation>
    (_("Simple"), Simple)
    (_("Regex"), Regex)
    (_("Size"), Size)
    (_("Date"), Date);

static const boost::bimap<wxString, RuleFunction> functionmap = boost::assign::list_of<boost::bimap<wxString, RuleFunction>::relation>
    (_("File Include"), FileInclude)
    (_("File Exclude"), FileExclude)
    (_("Folder Include"), FolderInclude)
    (_("Folder Exclude"), FolderExclude)
    (_("Absolute Folder Exclude"), AbsoluteFolderExclude);

class Rule{
public:
    Rule() {}
    Rule(const wxString &rule, RuleFunction function, RuleType type){
        this->rule = rule;
        this->normalised = Path::Normalise(rule);
        this->function = function;
        this->type = type;
    }

    RuleResult Matches(wxFileName path);
    bool IsValid();

    wxString rule;
    RuleFunction function;
    RuleType type;

private:
    wxString normalised;
};

class RuleSet{
public:
    RuleSet(const wxString &name) { this->name = name; }

    RuleResult Matches(wxFileName path);
    bool IsValid();

	bool TransferToFile();
	bool TransferFromFile();
	bool TransferToForm(frmMain *window);
	bool TransferFromForm(frmMain *window);

	void Add(Rule rule) { rules.push_back(rule); }

	const wxString& GetName() const {return name;}
private:
    std::vector<Rule> rules;
	wxString name;
};	

#endif
