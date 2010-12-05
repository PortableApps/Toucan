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
	/*bool error = false;
	wxString stemp;

	if(!wxGetApp().m_Rules_Config->Exists(GetName())){
		return false;
	}

	if(wxGetApp().m_Rules_Config->Read(GetName() + wxT("/FilesToInclude"), &stemp)) SetIncludedLocations(StringToArrayString(stemp, wxT("|")));
		else error = true;
	if(wxGetApp().m_Rules_Config->Read(GetName() + wxT("/FilesToExclude"), &stemp)) SetExcludedFiles(StringToArrayString(stemp, wxT("|")));
		else error = true; 	
	if(wxGetApp().m_Rules_Config->Read(GetName() + wxT("/FoldersToExclude"), &stemp))  SetExcludedFolders(StringToArrayString(stemp, wxT("|")));
		else error = true;
	
	if(error){
		wxMessageBox(_("There was an error reading from the rules file"), _("Error"), wxICON_ERROR);
		return false;
	}*/

	return true;
}

bool RuleSet::TransferToFile(){
	/*bool error = false;

	wxGetApp().m_Rules_Config->DeleteGroup(GetName());

	if(!wxGetApp().m_Rules_Config->Write(GetName() + wxT("/FilesToInclude"),  ArrayStringToString(GetIncludedLocations(), wxT("|")))) error = true;	
	if(!wxGetApp().m_Rules_Config->Write(GetName() + wxT("/FilesToExclude"), ArrayStringToString(GetExcludedFiles(), wxT("|")))) error = true;	
	if(!wxGetApp().m_Rules_Config->Write(GetName() + wxT("/FoldersToExclude"), ArrayStringToString(GetExcludedFolders(), wxT("|")))) error = true;
	
	wxGetApp().m_Rules_Config->Flush();
	
	if(error){
		wxMessageBox(_("There was an error saving to the rules file, \nplease check it is not set as read only or in use"), _("Error"), wxICON_ERROR);
		return false;
	}*/
	return true;
}

bool RuleSet::TransferFromForm(frmMain *window){
	/*if(!window){
		return false;
	}

    //Clear the existing rules
	Clear();
	for(int i = 0; i < window->m_RulesGrid->GetNumberRows(); i++){
		//Rules type is in column one and the rule in column two
        const wxString type = window->m_RulesGrid->GetCellValue(i, 0);
        const wxString rule = window->m_RulesGrid->GetCellValue(i, 1);
		if(type == _("File to Exclude")){
			m_ExcludedFiles.Add(rule);
		}
		else if(type == _("Folder to Exclude")){
			m_ExcludedFolders.Add(rule);
		}
		else if(type == _("Location to Include")){
			m_IncludedLocations.Add(rule);
		}
	}*/
	return true;
}

bool RuleSet::TransferToForm(frmMain *window){
	/*if(!window){
		return false;
	}

    //Set the rule name
	window->m_Rules_Name->SetStringSelection(GetName());

    wxGrid* grid = window->m_RulesGrid;
    const int count = m_ExcludedFiles.Count() + m_ExcludedFolders.Count() + m_IncludedLocations.GetCount();
    int runningpos = 0;
    //Create a new grid with enough rows for the new items;
    grid->ClearGrid();
    grid->DeleteRows(0, grid->GetNumberRows());
    grid->AppendRows(count);
    //Set the editors
    for(int i = 0; i < count; i++){
        grid->SetCellEditor(i, 0, new wxGridCellChoiceEditor(window->m_RulesChoices));
        grid->SetCellValue(i, 0, window->m_RulesChoices.Item(0));
    }

    //Add the individual rules
	for(unsigned int i = 0; i < m_ExcludedFiles.Count(); i++){
		grid->SetCellValue(_("File to Exclude"), runningpos, 0);
		grid->SetCellValue(m_ExcludedFiles.Item(i), runningpos++, 1);
	}
	for(unsigned int i = 0; i < m_ExcludedFolders.Count(); i++){
		grid->SetCellValue(_("Folder to Exclude"), runningpos, 0);
		grid->SetCellValue(m_ExcludedFolders.Item(i), runningpos++, 1);
	}
	for(unsigned int i = 0; i < m_IncludedLocations.GetCount(); i++){
		grid->SetCellValue(_("Location to Include"), runningpos, 0);
		grid->SetCellValue(m_IncludedLocations.Item(i), runningpos++, 1);
    }*/
	return true;
}
