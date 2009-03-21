/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include <wx/wx.h>
#include <wx/regex.h>
#include <wx/filename.h>
#include <wx/fileconf.h>
#include "rules.h"
#include "toucan.h"
#include "basicfunctions.h"

bool Rules::IsEmpty(){
	if(GetFilesToExclude().Count() == 0 && GetFoldersToExclude().Count() == 0 && GetLocationsToInclude().Count() == 0){
		return true;
	}
	return false;
}

bool Rules::ShouldExclude(wxString strName, bool blIsDir){
	//If there are no rules then return false
	if(IsEmpty()){
		return false;
	}
	//If there are any matches for inclusions then immediately retun as no other options need to be checked
	for(unsigned int r = 0; r < arrLocationsToInclude.Count(); r++){
		wxString strInclusion = arrLocationsToInclude.Item(r);
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
	for(unsigned int j = 0; j < arrFoldersToExclude.Count(); j++){
		wxString strFolderExclusion = arrFoldersToExclude.Item(j);
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
		for(unsigned int j = 0; j < arrFilesToExclude.Count(); j++){
			wxString strExclusion = arrFilesToExclude.Item(j);
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
			else if(strExclusion.Left(1) == wxT("<") && strExclusion.Right(1) != wxT("B")){
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
				else{
					wxMessageBox(_("Error with Regex"));
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

bool Rules::TransferFromFile(wxString strName){
	wxString strTemp;

	wxGetApp().m_Rules_Config->Read(strName + wxT("/FilesToInclude"), &strTemp);
	SetLocationsToInclude(StringToArrayString(strTemp, wxT("|")));	
	wxGetApp().m_Rules_Config->Read(strName + wxT("/FilesToExclude"), &strTemp);
	SetFilesToExclude(StringToArrayString(strTemp, wxT("|"))); 	
	wxGetApp().m_Rules_Config->Read(strName + wxT("/FoldersToExclude"), &strTemp);
	SetFoldersToExclude(StringToArrayString(strTemp, wxT("|"))); 
	return true;
}

bool Rules::TransferToFile(wxString strName){
	bool blError;
	wxGetApp().m_Rules_Config->DeleteGroup(strName);
	blError = wxGetApp().m_Rules_Config->Write(strName + wxT("/FilesToInclude"),  ArrayStringToString(GetLocationsToInclude(), wxT("|")));	
	blError = wxGetApp().m_Rules_Config->Write(strName + wxT("/FilesToExclude"), ArrayStringToString(GetFilesToExclude(), wxT("|")));	
	blError = wxGetApp().m_Rules_Config->Write(strName + wxT("/FoldersToExclude"), ArrayStringToString(GetFoldersToExclude(), wxT("|")));
	wxGetApp().m_Rules_Config->Flush();
	
	if(!blError){
		ErrorBox(wxT("There was an error saving to the rules file, \nplease check it is not set as read only or in use."));
		return false;
	}
	return true;
}
