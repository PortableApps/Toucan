/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2007 Steven Lamerton
// Licence:     GNU GPL 2 (See readme for more info
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_NORMALISE
#define H_NORMALISE

#include <wx/fileconf.h>
#include <wx/tokenzr.h>
#include <wx/datetime.h>
#include <wx/stdpaths.h>
#include <wx/utils.h>
#include <wx/filename.h>

/*This code creates the correct filenames from ones including Portable Variables
if the name to be returned is not valid then the function returns an empty string
and this is dealt with by the calling function*/
wxString Normalise(wxString strFilePath)
{
	wxString token;
	wxString strReturn = wxEmptyString;
	wxDateTime now = wxDateTime::Now();  
	wxStringTokenizer tkz(strFilePath, wxT("@"), wxTOKEN_STRTOK);
	while ( tkz.HasMoreTokens() )
	{
        token = tkz.GetNextToken();
		if(token.Left(1) != wxT("\\") && token.Right(1) != wxT("\\")){
			wxString strValue;
			if(token == wxT("date")){
				token = now.FormatISODate();
				strReturn += token;
			}
			else if(token == wxT("time")){
				token = now.Format(wxT("%H")) + wxT("-") +  now.Format(wxT("%M"));
				strReturn += token;       
			}
			else if(token == wxT("drive")){
				strReturn += wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Left(2);
			}
			else if(token == wxT("docs")){	
				strReturn += wxStandardPaths::Get().GetDocumentsDir();
			}
			else if(wxGetEnv(token , &strValue)){
				strReturn += strValue;
			}
			else{
				wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Left(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Length() - 11)+ wxT("\\Data\\Variables.ini") );
				wxFileConfig::Set( config );
				
				wxString strRead;
				//If there is no value in the field with the computers name, or it is empty, then read the value in the 'other' field
				if(config->Read(token + wxT("/") + wxGetFullHostName(), & strRead) == false || config->Read(token + wxT("/") + wxGetFullHostName()) == wxEmptyString)
				{
					//If there is no value in the 'other' field then simply add the name of that section as it must me a folder name	
					if(config->Read(token + wxT("/") + _("Other"), & strRead) == false){
						strReturn = strReturn + token;                 
					}
					else{
						strReturn += strRead;
					}		
				}
				else{
					strReturn += strRead;
				}
			}
		}
		else
		{
			strReturn += token;
		}
	}

	if(strReturn.Length() == 2 && strReturn.Right(1) == wxT(":")){
		strReturn += wxFILE_SEP_PATH;
	}	
	wxFileName flReturn(strReturn);
	if(flReturn.IsOk()){
		return strReturn;
	}
	else{
		return wxEmptyString;
	}
}
#endif
