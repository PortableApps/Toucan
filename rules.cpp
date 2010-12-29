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
#include "controls/rulesgrid.h"

RuleResult Rule::Matches(wxFileName path){
    //If we have an invalid rule then don't try to match
    if(!valid)
        return NoMatch;
    //If we have a folder then a file rule isn't applied, but not vice-versa
    if(path.IsDir() && (function == FileInclude || function == FileExclude))
        return NoMatch;
    bool match = false;
    if(type == Simple){
        if(path.GetFullPath().Lower().Find(normalised.Lower()) != wxNOT_FOUND
        || path.GetFullPath().Matches(normalised))
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
        if(function == FileInclude || function == FolderInclude)
            return Included;
        else if(function == FileExclude || function == FolderExclude)
            return Excluded;
        else if(function == AbsoluteFolderExclude)
            return AbsoluteExcluded;
    }
    else{
        return NoMatch;
    }
}

bool Rule::Validate(){
    //We do not support folder size exclusions
    if((function == FolderInclude || function == FolderExclude)
    &&(type == Size))
        return false;
    if(type == Regex){
        wxLogNull nulllog;
        wxRegEx regex(rule, wxRE_ICASE| wxRE_EXTENDED);
        if(regex.IsValid())
            return true;
        else
            return false;      
    }
    else if(type == Date){
        wxDateTime date;
        //We strip the leading < or >
        if(rule.Left(1) == wxT("<") || rule.Left(1) == wxT(">") && rule.length() > 1 && date.ParseDate(rule.Right(rule.length() - 1)))
            return true;
        else
            return false;
    }
    else if(type == Size){
        wxRegEx regex(wxT("(([0-9]+)(kB|MB|GB))"));
        //We strip the leading < or >
        if((rule.Left(1) == wxT("<") || rule.Left(1) == wxT(">")) && rule.length() > 1 && regex.Matches(rule.Right(rule.length() - 1))){
			//Check that we are matching the whole length of the string
            if(regex.GetMatch(rule.Right(rule.length() - 1)).length() == rule.length() - 1)
                return true;
            else
                return false;
        }
        else
            return false;
    }
    else{
        return true;
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

bool RuleSet::IsValid(){
    if(rules.empty())
        return true;

    for(auto iter = rules.begin() ; iter != rules.end(); iter++){
        if(!(*iter).IsValid())
            return false;
    }
	
	return true;
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
            Rule rule(temprule, functionmap.left.at(ToLang(tempfunction)), typemap.left.at(ToLang(temptype)));
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
        Rule rule(window->m_RulesGrid->GetCellValue(i, 2),
                  functionmap.left.at(window->m_RulesGrid->GetCellValue(i, 0)),
                  typemap.left.at(window->m_RulesGrid->GetCellValue(i, 1)));
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
    RulesGrid* grid = window->m_RulesGrid;
    grid->Clear();
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
        grid->ValidateRow(i);
    }
    return true;
}
