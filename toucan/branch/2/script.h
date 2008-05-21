/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SCRIPT
#define H_SCRIPT

#include <wx/arrstr.h>

bool ParseScript(wxArrayString arrScript){
	
	//First check that the whole script is valid (basic number of parameters check)
	wxString strLine;
	bool blParseError = false;
	for(unsigned int i = 0; i < arrScript.Count(); i++){
		strLine = arrScript.Item(i);
		wxStringTokenizer tkz(strLine, wxT("\t"), wxTOKEN_STRTOK);
		wxString strToken = tkz.GetNextToken();
		if(strToken == _("Sync")){
			if(tkz.CountTokens() != 2){
				wxLogError(strToken + _(" has an incorrect number of parameters"));
				blParseError = true;
			}
		}
		else if(strToken == _("Backup")){
			if(tkz.CountTokens() != 2){
				wxLogError(strToken + _(" has an incorrect number of parameters"));
				blParseError = true;
			}
		}
		else if(strToken == _("Secure")){
			if(tkz.CountTokens() != 2){
				wxLogError(strToken + _(" has an incorrect number of parameters"));
				blParseError = true;
			}
		}
		else if(strToken == _("Delete")){
			if(tkz.CountTokens() != 2){
				wxLogError(strToken + _(" has an incorrect number of parameters"));
				blParseError = true;
			}
		}
		else if(strToken == _("Move")){
			if(tkz.CountTokens() != 3){
				wxLogError(strToken + _(" has an incorrect number of parameters"));
				blParseError = true;
			}
		}
		else if(strToken == _("Copy")){
			if(tkz.CountTokens() != 3){
				wxLogError(strToken + _(" has an incorrect number of parameters"));
				blParseError = true;
			}
		}
		else if(strToken == _("Execute")){
			if(tkz.CountTokens() != 2){
				wxLogError(strToken + _(" has an incorrect number of parameters"));
				blParseError = true;
			}
		}
		else{
			blParseError = true;
			wxLogError(strToken + _(" not recognised"));
		}
	}
	if(blParseError){
		return false;
	}

	
	//End of parsing for errors, now execute!
	
	for(unsigned int i = 0; i < arrScript.Count(); i++){
		strLine = arrScript.Item(i);
		wxStringTokenizer tkz(strLine, wxT("\t"), wxTOKEN_STRTOK);
		wxString strToken = tkz.GetNextToken();
		if(strToken == _("Sync")){
			
		}
		else if(strToken == _("Backup")){
			
		}
		else if(strToken == _("Secure")){
			
		}
		else if(strToken == _("Delete")){
			
		}
		else if(strToken == _("Move")){
			
		}
		else if(strToken == _("Copy")){
			
		}
		else if(strToken == _("Execute")){
			
		}
	}
	return true;
}
		
#endif
