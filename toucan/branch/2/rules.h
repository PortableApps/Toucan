#ifndef H_RULES
#define H_RULES

#include <wx\regex.h>
#include "basicfunctions.h"

//Ssize related stuff needs to be added to the matching routine

class Rules{
public:
	//Constructor
	//Rules();

	//Functions
	bool TransferToFile(wxString strName);
	bool TransferFromFile(wxString strName);
	bool ShouldExclude(wxString strName, bool blIsDir); 
	bool ShouldDelete(wxString strFilename);

	//Inline Functions
	void SetFilesToExclude(wxArrayString filestoexclude){ arrFilesToExclude = filestoexclude; }
	wxArrayString GetFilesToExclude() { return arrFilesToExclude; }
	
	void SetLocationsToInclude(wxArrayString locationstoinclude){ arrLocationsToInclude = locationstoinclude; }
	wxArrayString GetLocationsToIncude() { return arrLocationsToInclude; }
	
	void SetFoldersToExclude(wxArrayString folderstoexclude){ arrFoldersToExclude = folderstoexclude; }
	wxArrayString GetFoldersToExclude() { return arrFoldersToExclude; }
	
	void SetFilesToDelete(wxArrayString filestodelete){ arrFilesToDelete = filestodelete; }
	wxArrayString GetFilesToDelete(){ return arrFilesToDelete; }
private:

	wxArrayString arrFoldersToExclude;
	wxArrayString arrLocationsToInclude;
	wxArrayString arrFilesToDelete;
	wxArrayString arrFilesToExclude;

};	

bool Rules::ShouldExclude(wxString strName, bool blIsDir){
	//wxMessageBox(_("Into the match"));
	bool blMatches = false;
	//Check to see if there are any matches for also include, if so then immediately retun as no other options need to be checked
	for(unsigned int r = 0; r < arrLocationsToInclude.Count(); r++){
		wxRegEx regMatch; 
		regMatch.Compile(arrLocationsToInclude.Item(r));
		if(regMatch.IsValid()){
			if(regMatch.Matches(strName)){
				return false; 
			}
		}
	}
	//If the name is a directory
	if(blIsDir){
		//wxMessageBox(_("Tis a folder"));
		for(unsigned int j = 0; j < arrFoldersToExclude.Count(); j++){
		wxRegEx regMatch;
		regMatch.Compile(arrFoldersToExclude.Item(j));
			if(regMatch.IsValid()){
				if(regMatch.Matches(strName)){
					wxMessageBox(_("Excluding folder"), strName);
					return true; 
				}
			}
		}
	}
	//It is a file
	else{
	//wxMessageBox(_("Tis a file"));
		for(unsigned int j = 0; j < arrFilesToExclude.Count(); j++){
			//Create  strExclusion and set it to the current exclusion we are checking against
			wxString strExclusion = arrFilesToExclude.Item(j);
			//Check to see if it is a filesize based exclusion
			if(strExclusion.Left(1) == wxT("<") || strExclusion.Left(1) == wxT(">")){
				if(strExclusion.Right(2) == wxT("kB") || strExclusion.Right(2) == wxT("MB") || strExclusion.Right(2) == wxT("GB")){
					//We can now be sure that this is a size exclusion
					wxFileName flName(strName);
					wxString strNameSize = flName.GetHumanReadableSize();
					double dFileSize = GetInPB(strNameSize);
					double dExclusionSize = GetInPB(strExclusion.Right(strExclusion.Length() - 2));
					if(strExclusion.Left(1) == wxT("<")){
						if(dFileSize < dExclusionSize){
							wxMessageBox(_("Excluded by size"));
							return true;
						}
					}
					if(strExclusion.Left(1) == wxT(">")){
						if(dFileSize > dExclusionSize){
							wxMessageBox(_("Excluded by size"));
							return true;
						}
					}
				}
			}
			//Check to see if it is a date, but NOT a size
			if(strExclusion.Left(1) == wxT("<") && strExclusion.Right(1) != wxT("B")){
				wxFileName flName(strName);
				wxDateTime date;								
				date.ParseDate(arrFilesToExclude.Item(j));
				if(flName.GetModificationTime().IsEarlierThan(date)){
					//wxMessageBox(_("Excluded by date"));
					return true; 
				}
			}
			//Other date direction
			if(strExclusion.Left(1) == wxT(">") && strExclusion.Right(1) != wxT("B")){
				wxFileName flName(strName);
				wxDateTime date;								
				date.ParseDate(arrFilesToExclude.Item(j));
				if(flName.GetModificationTime().IsLaterThan(date)){
					wxMessageBox(_("Excluded by date"));
					return true; 
				}
			}
			//Check to see if there is a match in the filename, including any regex bits
			wxRegEx regMatch;
			regMatch.Compile(strExclusion, wxRE_ICASE| wxRE_EXTENDED);
			if(regMatch.IsValid()){
				if(regMatch.Matches(strName)){
					wxMessageBox(_("Excluded by regex"));
					return true; 
				}
			}
			else{
				wxMessageBox(_("Error with Regex"));
			}
		}
		//Need to check to see if it is in an excluded folder as in this case it still needs to be excluded
		for(unsigned int j = 0; j < arrFoldersToExclude.Count(); j++){
			wxRegEx regMatch;
			regMatch.Compile(arrFoldersToExclude.Item(j));
			//wxMessageBox(strName);
			//wxMessageBox(arrFoldersToExclude.Item(j));
				if(regMatch.IsValid()){
					if(regMatch.Matches(strName)){
						wxMessageBox(_("Excluding folder"));
						return true; 
					}
				}
			}
	}
	//wxMessageBox(_("Returning as normal"));
	return blMatches;
}

bool Rules::TransferFromFile(wxString strName){
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("Rules.ini"));
	
	wxString strTemp;

	config->Read(strName + wxT("/FilesToInclude"), &strTemp);
	SetLocationsToInclude(StringToArrayString(strTemp, wxT("#")));	
	config->Read(strName + wxT("/FilesToExclude"), &strTemp);
	SetFilesToExclude(StringToArrayString(strTemp, wxT("#"))); 	
	config->Read(strName + wxT("/FoldersToExclude"), &strTemp);
	SetFoldersToExclude(StringToArrayString(strTemp, wxT("#"))); 
	config->Read(strName + wxT("/FilesToDelete"), &strTemp);
	SetFilesToDelete(StringToArrayString(strTemp, wxT("#"))); 
	delete config;
	return true;
}

bool Rules::TransferToFile(wxString strName){
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("Rules.ini"));
	
	bool blError;
	config->DeleteGroup(strName);
	blError = config->Write(strName + wxT("/FilesToInclude"),  ArrayStringToString(GetLocationsToIncude(), wxT("#")));	
	blError = config->Write(strName + wxT("/FilesToExclude"), ArrayStringToString(GetFilesToExclude(), wxT("#")));	
	blError = config->Write(strName + wxT("/FoldersToExclude"), ArrayStringToString(GetFoldersToExclude(), wxT("#")));
	blError = config->Write(strName + wxT("/FilesToDelete"), ArrayStringToString(GetFilesToDelete(), wxT("#")));	
	config->Flush();
	delete config;
	
	if(!blError){
		ErrorBox(wxT("There was an error saving to the rules file, \nplease check it is not set as read only or in use."));
		return false;
	}
	return true;
}

bool Rules::ShouldDelete(wxString strFilename){
	//Checks to see if it was a filepath that was passed
	for(unsigned int i = 0; i < GetFilesToDelete().GetCount(); i++){
		if(strFilename == GetFilesToDelete().Item(i)){
			return true;
		}
	}
	//Checks to see if it was a filename that was passed
	for(unsigned int j = 0; j < GetFilesToDelete().GetCount(); j++){
		if(strFilename == GetFilesToDelete().Item(j).AfterLast(wxFILE_SEP_PATH)){
			return true;
		}
	}
	return false;
}

#endif
