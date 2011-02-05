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

namespace{
    double GetInPB(const wxString &value){
        wxString size;
    	wxVariant var;
    	//The size is in bytes so the length is all but one
    	if(value.Right(2).Left(1) != wxT("k") && value.Right(2).Left(1) != wxT("M") && value.Right(2).Left(1) != wxT("G")){
    		if(value.Right(1) == wxT("B")){
    			var = value.Left(value.Length() - 1);
    			size = value.Right(1);
    		}
    		else{
    			//We do not know what unit is being used
    			return 0;
    		}
    	}
    	//The size is in a larger unit, so it all but two
    	else{
    		var = value.Left(value.Length() - 2);
    		size = value.Right(2);
    	}
    	double dSize = var.GetDouble();
    	if(size == wxT("B")){
    		dSize = dSize/1024;
    		size = wxT("kB");
    	}
    	if(size == wxT("kB")){
    		dSize = dSize/1024;
    		size = wxT("MB");
    	}
    	if(size == wxT("MB")){
    		dSize = dSize/1024;
    		size = wxT("GB");
    	}
    	if(size == wxT("GB")){
    		dSize = dSize/1024;
    	}
    	//Conveting to PB, should be plenty big for a while
    	dSize = dSize/1024;
    	return dSize;
    }
}

RuleResult Rule::Matches(wxFileName path){
    //If we have an invalid rule then don't try to match
    if(!valid || normalised == "" || rule == "")
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
    wxFileConfig config("", "",  Locations::GetSettingsPath() + "rules" + wxFILE_SEP_PATH + name + ".ini");
    wxString temprule, tempfunction, temptype;

    //Iterate through all of the groups and read the rules
    for(unsigned int i = 0; i < config.GetNumberOfGroups(); i++){
        if(!config.Read(wxString::Format("%d", i) + "/Rule", &temprule)
        || !config.Read(wxString::Format("%d", i) + "/Function", &tempfunction)
        || !config.Read(wxString::Format("%d", i) + "/Type", &temptype)){
            wxMessageBox(_("There was an error reading from the rules file"), _("Error"), wxICON_ERROR);
            return false;
        }
        else{
            Rule rule(temprule, functionmap.left.at(tempfunction), typemap.left.at(temptype));
            rules.push_back(rule);
        }
    }

    return true;
}

bool RuleSet::TransferToFile(){
    wxFileConfig config("", "", Locations::GetSettingsPath() + "rules" + wxFILE_SEP_PATH + name + ".ini");
    config.DeleteAll();
    config.SetExpandEnvVars(false);

    for(unsigned int i = 0; i < rules.size(); i++){
        if(!config.Write(wxString::Format("%d", i) + "/Rule", rules.at(i).rule)
        || !config.Write(wxString::Format("%d", i) + "/Function", functionmap.right.at(rules.at(i).function))
        || !config.Write(wxString::Format("%d", i) + "/Type", typemap.right.at(rules.at(i).type))){
            wxMessageBox(_("There was an error saving to the rules file, \nplease check it is not set as read only or in use"), _("Error"), wxICON_ERROR);
		    return false;
        }
    }

    return true;
}
