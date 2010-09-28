/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
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

Rules::Rules(const wxString &name, bool loadfromfile) : m_Name(name){
	if(loadfromfile){
		TransferFromFile();
	}
	m_Normalised = false;
}

bool Rules::IsEmpty(){
	if(GetExcludedFiles().Count() == 0 && GetExcludedFolders().Count() == 0 && GetIncludedLocations().Count() == 0){
		return true;
	}
	return false;
}

bool Rules::ShouldExclude(wxString strName, bool blIsDir){
	//If there are no rules then return false
	if(IsEmpty()){
		return false;
	}
	//If this is the first time we have run then we need to expand the rules if they have variables in them
	if(!m_Normalised){
		for(unsigned int i = 0; i < m_ExcludedFiles.GetCount(); i++){
            m_ExcludedFiles.Item(i) = Path::Normalise(m_ExcludedFiles.Item(i));
		}
		for(unsigned int i = 0; i < m_ExcludedFolders.GetCount(); i++){
            m_ExcludedFolders.Item(i) = Path::Normalise(m_ExcludedFolders.Item(i));
		}
		for(unsigned int i = 0; i < m_IncludedLocations.GetCount(); i++){
            m_IncludedLocations.Item(i) = Path::Normalise(m_IncludedLocations.Item(i));
		}
		m_Normalised = true;
	}
	//If there are any matches for inclusions then immediately retun as no other options need to be checked
	for(unsigned int r = 0; r < GetIncludedLocations().Count(); r++){
		wxString strInclusion = GetIncludedLocations().Item(r);
		//If it is a regex then regex match it
		if(strInclusion.Left(1) == wxT("*")){
			wxRegEx regMatch; 
			regMatch.Compile(strInclusion.Right(strInclusion.Length() -1), wxRE_ICASE| wxRE_EXTENDED);
			if(regMatch.IsValid()){
				if(regMatch.Matches(strName)){
					return false; 
				}
			}
		}
		//Otherwise plain text match
		else{
			if(strName.Lower().Find(strInclusion.Lower()) != wxNOT_FOUND){
				return false;
			}
		}
	}
	//Always check the directory exclusions as a file that is in an excluded directory should be excluded
	for(unsigned int j = 0; j < GetExcludedFolders().Count(); j++){
		wxString strFolderExclusion = GetExcludedFolders().Item(j);
		//If it is a regex then regex match it
		if(strFolderExclusion.Left(1) == wxT("*")){
			wxRegEx regMatch;
			regMatch.Compile(strFolderExclusion.Right(strFolderExclusion.Length() - 1), wxRE_ICASE| wxRE_EXTENDED);
			if(regMatch.IsValid()){
				if(regMatch.Matches(strName)){
					return true; 
				}
			}
		}
		//Otherwise plain text match
		else{
			if(strName.Lower().Find(strFolderExclusion.Lower()) != wxNOT_FOUND){
				return true;
			}
		}
	}
	//It is a file so run the extra checks as well
	if(!blIsDir){
		for(unsigned int j = 0; j < GetExcludedFiles().Count(); j++){
			wxString strExclusion = GetExcludedFiles().Item(j);
			//Check to see if it is a filesize based exclusion
			if(strExclusion.Left(1) == wxT("<") || strExclusion.Left(1) == wxT(">")){
				if(strExclusion.Right(2) == wxT("kB") || strExclusion.Right(2) == wxT("MB") || strExclusion.Right(2) == wxT("GB")){
					//We can now be sure that this is a size exclusion
					wxFileName flName(strName);
					wxString strFileSize = flName.GetHumanReadableSize();
					strFileSize.Replace(wxT(" "), wxT(""));
					double dFileSize = GetInPB(strFileSize);
					wxString strSize = strExclusion.Right(strExclusion.Length() - 1);
					double dExclusionSize = GetInPB(strSize);
					if(strExclusion.Left(1) == wxT("<")){
						if(dFileSize < dExclusionSize){
							return true;
						}
					}
					if(strExclusion.Left(1) == wxT(">")){
						if(dFileSize > dExclusionSize){
							return true;
						}
					}
				}
			}
			//Check to see if it is a date, but NOT a size
			if(strExclusion.Left(1) == wxT("<") && strExclusion.Right(1) != wxT("B")){
				wxFileName flName(strName);
				wxDateTime date;								
				date.ParseDate(strExclusion.Right(strExclusion.Length() - 1));
				if(flName.GetModificationTime().IsEarlierThan(date)){
					return true; 
				}
			}
			//Other date direction
			else if(strExclusion.Left(1) == wxT(">") && strExclusion.Right(1) != wxT("B")){
				wxFileName flName(strName);
				wxDateTime date;								
				date.ParseDate(strExclusion.Right(strExclusion.Length() - 1));
				if(flName.GetModificationTime().IsLaterThan(date)){
					return true; 
				}
			}
			//Check to see if it is a regex
			else if(strExclusion.Left(1) == wxT("*")){
				//Check to see if there is a match in the filename, including any regex bits
				wxRegEx regMatch;
				regMatch.Compile(strExclusion.Right(strExclusion.Length() - 1), wxRE_ICASE| wxRE_EXTENDED);
				if(regMatch.IsValid()){
					if(regMatch.Matches(strName)){
						return true; 
					}
				}
			}
			//Else plain text match it
			else{
				if(strName.Lower().Find(strExclusion.Lower()) != wxNOT_FOUND){
					return true;
				}
			}
		}
	}
	return false;
}

bool Rules::TransferFromFile(){
	bool error = false;
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
	}

	return true;
}

bool Rules::TransferToFile(){
	bool error = false;

	wxGetApp().m_Rules_Config->DeleteGroup(GetName());

	if(!wxGetApp().m_Rules_Config->Write(GetName() + wxT("/FilesToInclude"),  ArrayStringToString(GetIncludedLocations(), wxT("|")))) error = true;	
	if(!wxGetApp().m_Rules_Config->Write(GetName() + wxT("/FilesToExclude"), ArrayStringToString(GetExcludedFiles(), wxT("|")))) error = true;	
	if(!wxGetApp().m_Rules_Config->Write(GetName() + wxT("/FoldersToExclude"), ArrayStringToString(GetExcludedFolders(), wxT("|")))) error = true;
	
	wxGetApp().m_Rules_Config->Flush();
	
	if(error){
		wxMessageBox(_("There was an error saving to the rules file, \nplease check it is not set as read only or in use"), _("Error"), wxICON_ERROR);
		return false;
	}
	return true;
}

bool Rules::TransferFromForm(frmMain *window){
	if(!window){
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
	}
	return true;
}

bool Rules::TransferToForm(frmMain *window){
	if(!window){
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
    }
	return true;
}
