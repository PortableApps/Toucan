#ifndef H_RULES
#define H_RULES

//Size related stuff needs to be added to the matching routine

class Rules{

public:
	//Constructor
	Rules();

	//Functions
	bool TransferToFile(wxString strName);
	bool TransferFromFile(wxString strName);
	bool Matches(wxString strName, bool blIsDir); 

	//Inline Functions
	void SetFilesToExclude(wxArrayString filestoexclude){ arrFilesToExclude = filestoexclude; }
	wxArrayString GetFilesToExclude() { return arrFilesToExclude; }
	
	void SetFilesToInclude(wxArrayString filestoinclude){ arrFilesToInclude = filestoinclude; }
	wxArrayString GetFilesToExclude() { return arrFilesToInclude; }
	
	void SetFoldersToExclude(wxArrayString folderstoexclude){ arrFoldersToExclude = folderstoexclude; }
	wxArrayString GetFoldersToExclude() { return arrFoldersToExclude; }
	
	void SetRegex(wxArrayString regex){ arrRegex = regex; }
	wxArrayString GetRegex() { return arrRegex; }

private:
	wxArrayString arrFilesToExclude;
	wxArrayString arrFoldersToExclude;
	wxArrayString arrFilesToInclude;
	wxArrayString arrRegEx;

}	

bool Blacklist::Matches(wxString strName, bool blIsDir){
	bool blMatches = false;
	//Check to see if the file is always included, if so return true as there is no point checking the rest
	for(int i = 0; i < arrFilesToInclude; i++){
		if(strName.Left(arrFilesToInclude.Item(i).Length() == arrFilesToInclude.Item(i)){
					return false; 
				}
			}
			//Check to see if it matches the regex
			for(int r = 0; r < arrRegEx; r++){
				wxRegEx regMatch = arrRegEx.Item(r);
				if(regMatch.Matches(strName)){
			return true; 
		}
	}
	//If the name is a directory
	if(blIsDir){
		for(int j = 0; j < arrFoldersToExclude; j++){
			if(strName.Left(arrFilesToExclude.Item(j).Length() == arrFilesToExclude.Item(j)){
						return true; 
					}
				}
				for(int k = 0; k < arrFoldersToExclude; k++){
					if(strName.BeforeLast(wxFILE_SEP_PATH).AfterLast(wxFILE_SEP_PATH) == arrFilesToExclude.Item(k)){
				return true; 
			}
		}
	}
	//It is a file
	else{
		//Check to see if it is a size exclusion
		//Check to see if it is a date exclusion
		if(strFileName.Left(1) == wxT("<")){
			wxFilename flName(strFileName);
			wxDateTime Date
			for(int m = 0; m < arrFilesToExclude; m++){
				date.ParseDate(arrFilesToExclude.Item(m));
				if(flName.GetModificationTime.IsEarlierThan(date)){
					return true; 
				}
			}
		}	
		if(strFileName.Left(1) == wxT(">")){
			wxFilename flName(strFileName);
			wxDateTime Date
			for(int n = 0; n < arrFilesToExclude; n++){
				date.ParseDate(arrFilesToExclude.Item(n));
				if(flName.GetModificationTime.IsLaterThan(date)){
					return true; 
				}
			}
		}		
		//Run through all of the files to exclude and see if it matches either a folder location or the entire name
		for(int j = 0; j < arrFilesToExclude; j++){
			if(strName.Left(arrFilesToExclude.Item(j).Length() == arrFilesToExclude.Item(j)){
						return true; 
					}
				}
				//Run through files to exclude and see if it is a files name that matches
				for(int k = 0; k < arrFilesToExclude; k++){
					if(strName.AfterLast(wxFILE_SEP_PATH) == arrFilesToExclude.Item(k)){
				return true; 
			}
		}
		//Run through the files to exlude and see if it is an extension and it matches
		for(int l = 0; l < arrFilesToExclude; l++){
			wxFilename flName(strFileName);
			if(wxT(".") + flName.GetExt() == arrFilesToExclude.Item(k)){
				return true; 
			}
		}
	}
	return blMatches;
}

bool Rules::TransferFromFile(wxString strName){
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Left(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Length() - 11) + wxT("\\Data\\Rules.ini") );
	
	bool blError;
	wxString strTemp;

	blError = config->Read(strName + wxT("/FilesToInclude"), &strTemp);
	if(!blError){ SetFilesToInclude(StringToArrayString(strTemp, wxT("#"))); }	
	blError = config->Read(strName + wxT("/FilesToExclude"), &strTemp);
	if(!blError){ SetFilesToExclude(StringToArrayString(strTemp, wxT("#"))); }	
	blError = config->Read(strName + wxT("/FoldersToExclude"), &strTemp);
	if(!blError){ SetFoldersToExclude(StringToArrayString(strTemp, wxT("#"))); }	
	blError = config->Read(strName + wxT("/Regex"), &strTemp);
	if(!blError){ SetRegex(StringToArrayString(strTemp, wxT("#"))); }	
	delete config;

	if(blError){
		ErrorMessage(wxT("There was an error reading from the jobs file, \nplease check it is not set as read only or in use."));
		return false;
	}
	return true;
	'}

bool Rules::TransferToFile(wxString strName){
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Left(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Length() - 11) + wxT("\\Data\\Rules.ini") );
	
	bool blError;

	blError = config->DeleteGroup(strName);
	blError = config->Write(strName + wxT("/FilesToInclude"),  ArrayStringToString(GetFilesToInclude(), wxT("#")));	
	blError = config->Write(strName + wxT("/FilesToExclude"), ArrayStringToString(GetFilesToExclude(), wxT("#")));	
	blError = config->Write(strName + wxT("/FoldersToExclude"), ArrayStringToString(GetFoldersToExclude(), wxT("#")));	
	blError = config->Write(strName + wxT("/Regex"), ArrayStringToString(GetRegex(), wxT("#")));	
	delete config;
	
	if(blError){
		ErrorMessage(wxT("There was an error saving to the jobs file, \nplease check it is not set as read only or in use."));
		return false;
	}
	return true;
}

#endif
