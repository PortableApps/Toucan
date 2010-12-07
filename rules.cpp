/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include <wx/regex.h>
#include <wx/filename.h>
#include <wx/fileconf.h>
#include <wx/grid.h>
#include <wx/combobox.h>
#include <wx/msgdlg.h>
#include "rules.h"
#include "toucan.h"
#include "path.h"
#include "basicfunctions.h"
#include "forms/frmmain.h"

#include <boost/bimap.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/assign/list_inserter.hpp>

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

RuleResult Rule::Matches(wxFileName path){
    bool match = false;
    if(type == Simple){
        if(path.GetFullPath().Lower().Find(normalised.Lower()) != wxNOT_FOUND)
            match = true;
    }
    else if(type == Regex){
        wxRegEx regex; 
        regex.Compile(rule, wxRE_ICASE| wxRE_EXTENDED);
        if(regex.IsValid() && regex.Matches(normalised))
             match = true;
    }
    else if(type == Size){
        wxString filesize = path.GetHumanReadableSize();
        filesize.Replace(" ", "");
		double dfilesize = GetInPB(filesize);
		double dexcludesize = GetInPB(rule.Right(rule.length() - 1));
        if(rule.Left(1) == "<" && dfilesize < dexcludesize)
			match = true;
		if(rule.Left(1) == ">" && dfilesize > dexcludesize)
		    match = true;
    }
    else if(type == Date){
        wxDateTime date;
		bool valid = date.ParseDate(rule.Right(rule.length() - 1));
		if(valid && rule.Left(1) == "<" && path.GetModificationTime().IsEarlierThan(date))
			match = true; 
		if(valid && rule.Left(1) == ">" && path.GetModificationTime().IsLaterThan(date))
            match = true;
    }

    if(match){
        if(type == FileInclude || type == FolderInclude)
            return Included;
        else if(type == FileExclude || type == FolderExclude)
            return Excluded;
        else if(type == AbsoluteFolderExclude)
            return AbsoluteExcluded;
    }
    else{
        return NoMatch;
    }
}

RuleResult RuleSet::Matches(wxFileName path){
	if(rules.empty())
		return NoMatch;

    for(auto iter = rules.begin() ; iter != rules.end(); iter++){
        RuleResult result = (*iter).Matches(path);
        if(result != NoMatch)
            return result;
    }
	
	return NoMatch;
}

bool RuleSet::TransferFromFile(){
    std::unique_ptr<wxFileConfig> config(new wxFileConfig("", "", wxGetApp().GetSettingsPath() + wxFILE_SEP_PATH + "rules" + wxFILE_SEP_PATH + name + ".ini"));

    wxString temprule, tempfunction, temptype;

    //Iterate through all of the groups and read the rules
    for(unsigned int i = 0; i < config->GetNumberOfGroups(); i++){
        if(!config->Read(wxString::Format("%d", i) + "/Rule", &temprule)
        || !config->Read(wxString::Format("%d", i) + "/Function", &tempfunction)
        || !config->Read(wxString::Format("%d", i) + "/Type", &temptype)){
            wxMessageBox(_("There was an error reading from the rules file"), _("Error"), wxICON_ERROR);
            return false;
        }
        else{
            Rule rule;
            rule.rule = temprule;
            rule.function = functionmap.left.at(ToLang(tempfunction));
            rule.type = typemap.left.at(ToLang(temptype));
            rules.push_back(rule);
        }
    }

    return true;
}

bool RuleSet::TransferToFile(){
    if(!wxDirExists(wxGetApp().GetSettingsPath() + wxFILE_SEP_PATH + "rules")){
        wxMkdir(wxGetApp().GetSettingsPath() + wxFILE_SEP_PATH + "rules");
    }
    std::unique_ptr<wxFileConfig> config(new wxFileConfig("", "", wxGetApp().GetSettingsPath() + wxFILE_SEP_PATH + "rules" + wxFILE_SEP_PATH + name + ".ini"));
    config->DeleteAll();
    config->SetExpandEnvVars(false);

    for(unsigned int i = 0; i < rules.size(); i++){
        if(!config->Write(wxString::Format("%d", i) + "/Rule", rules.at(i).rule)
        || !config->Write(wxString::Format("%d", i) + "/Function", ToEn(functionmap.right.at(rules.at(i).function)))
        || !config->Write(wxString::Format("%d", i) + "/Type", ToEn(typemap.right.at(rules.at(i).type)))){
            wxMessageBox(_("There was an error saving to the rules file, \nplease check it is not set as read only or in use"), _("Error"), wxICON_ERROR);
		    return false;
        }
    }

    return true;
}

bool RuleSet::TransferFromForm(frmMain *window){
    if(!window){
        return false;
    }

    //Clear the existing rules
    rules.clear();
    for(int i = 0; i < window->m_RulesGrid->GetNumberRows(); i++){
        Rule rule;
        rule.rule  = window->m_RulesGrid->GetCellValue(i, 2);
        rule.function = functionmap.left.at(window->m_RulesGrid->GetCellValue(i, 0));
        rule.type = typemap.left.at(window->m_RulesGrid->GetCellValue(i, 1));
        rules.push_back(rule);
    }
    return true;
}

bool RuleSet::TransferToForm(frmMain *window){
    if(!window){
        return false;
    }

    //Set the rule name
    window->m_Rules_Name->SetStringSelection(GetName());

    //Create a new grid with enough rows for the new items;
    wxGrid* grid = window->m_RulesGrid;
    if(grid->GetNumberRows() > 0)
        grid->DeleteRows(0, grid->GetNumberRows());
    grid->AppendRows(rules.size());

    //Add the rules
    for(unsigned int i = 0; i < rules.size(); i++){
        //Set the editors
        grid->SetCellEditor(i, 0, new wxGridCellChoiceEditor(window->m_RulesChoices));
        grid->SetCellValue(i, 0, window->m_RulesChoices.Item(0));
        grid->SetCellEditor(i, 1, new wxGridCellChoiceEditor(window->m_RulesTypeChoices));
        grid->SetCellValue(i, 1, window->m_RulesTypeChoices.Item(0));
        //Add the items
        grid->SetCellValue(i, 0, functionmap.right.at(rules.at(i).function));
        grid->SetCellValue(i, 1, typemap.right.at(rules.at(i).type));
        grid->SetCellValue(i, 2, rules.at(i).rule);
    }
    return true;
}
