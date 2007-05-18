#ifndef H_NORMALISE
#define H_NORMALISE

#include <wx/fileconf.h>
#include <wx/tokenzr.h>
#include <wx/datetime.h>
#include <wx/stdpaths.h>

wxString Normalise(wxString strFilePath)
{
	wxString token;
	wxString strReturn = _("");
	wxDateTime now = wxDateTime::Now();  
	wxStringTokenizer tkz(strFilePath, wxT("@"), wxTOKEN_STRTOK);
	while ( tkz.HasMoreTokens() )
	{
		token = tkz.GetNextToken();
		if(token.Left(1) != wxT("\\") && token.Right(1) != wxT("\\"))
		{
			
			if(token == wxT("date"))
			{
				token = now.FormatISODate();
				strReturn = strReturn + token;
			}
			else if(token == wxT("time"))
			{
				token = now.Format(wxT("%H")) + wxT("-") +  now.Format(wxT("%M"));
				strReturn = strReturn + token;       
			}
			else if(token == wxT("drive"))
			{
				strReturn = strReturn + wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Left(2);
			}
			else if(token == wxT("docs"))
			{	
				strReturn = strReturn + wxStandardPaths::Get().GetDocumentsDir();
			}
			else
			{
				wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxT("\\Data\\Variables.ini") );
				wxFileConfig::Set( config );
				
				wxString strRead;
				if(config->Read(token + wxT("/") + wxGetFullHostName(), & strRead) == false)
				{
					strReturn = strReturn.Left(strReturn.Length() -1);
					if(config->Read(token + wxT("/") + _("Other"), & strRead) == false)
					{
						strReturn = strReturn.Left(strReturn.Length() -1);
						strReturn += token;                 
					}
					else
					{
						strReturn += strRead;
					}
					
				}
				else
				{
					strReturn += strRead;
				}
			}
		}
		else
		{
			
			strReturn = strReturn + token;
			
		}
	}
	return strReturn;
}
#endif