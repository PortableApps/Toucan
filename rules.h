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
    Rule(const wxString rule, RuleFunction function, RuleType type){
        this->rule = rule;
        this->normalised = Path::Normalise(rule);
        this->function = function;
        this->type = type;
    }

    RuleResult Matches(wxFileName path);


private:
    wxString rule;
    wxString normalised;
    RuleFunction function;
    RuleType type;
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

	/*bool IsEmpty();
	void Clear(){
		m_ExcludedFiles.Clear();
		m_ExcludedFolders.Clear();
		m_IncludedLocations.Clear();
	}

	bool ShouldExclude(wxString path, bool directory); 
	void SetName(const wxString& Name) {this->m_Name = Name;}
	void SetExcludedFiles(const wxArrayString& FilesToExclude){
		this->m_ExcludedFiles = FilesToExclude;
		m_Normalised = false;
	}
	void SetExcludedFolders(const wxArrayString& FoldersToExclude){
		this->m_ExcludedFolders = FoldersToExclude;
		m_Normalised = false;
	}
	void SetIncludedLocations(const wxArrayString& LocationsToInclude){
		this->m_IncludedLocations = LocationsToInclude;
		m_Normalised = false;
	}

	const wxString& GetName() const {return m_Name;}
	const wxArrayString& GetExcludedFiles() const {return m_ExcludedFiles;}
	const wxArrayString& GetExcludedFolders() const {return m_ExcludedFolders;}
	const wxArrayString& GetIncludedLocations() const {return m_IncludedLocations;}*/

private:
    std::vector<Rule> rules;
	wxString name;
};	

#endif
