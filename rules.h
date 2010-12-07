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
