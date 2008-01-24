#ifndef H_BACKUPDATA
#define H_BACKUPDATA

class BackupData{

public:
	//Constuctor
	BackupData();

	//Functions
	bool TransferToFile(wxString strName);
	bool TransferFromFile(wxString strName);
	void TransferToForm(frmMain *window);
	bool TransferFromForm(frmMain*window);
	wxString CreateCommand();
	void Output();

	//Inline functions
	void SetLocations(wxArrayString locations) { arrLocations = locations; }
	wxArrayString GetLocations() { return arrLocations; }

	void SetFunction(wxString function) { strFunction = function; }
	wxString GetFunction() { return strFunction; }

	void SetFormat(wxString format) { strFormat = format; }
	wxString GetFormat() { return strFormat; }

	void SetRatio(wxString ratio) { strRatio = ratio; }
	wxString GetFunction() { return strFunction; }

	void SetPass(wxString pass) { strPass = pass; }
	wxString GetPass() { return strPass; }


private:
	wxArrayString arrLocations;
	wxString strFunction;
	wxString strFormat;
	wxString strRatio;
	wxString strPass;

}

bool BackupData::TransferFromFile(wxString strName){
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Left(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Length() - 11) + wxT("\\Data\\Jobs.ini") );
	
	bool blError;
	wxString strTemp;

	blError = config->Read(strName + wxT("/Locations"), &strTemp);
	if(!blError){ SetLocations(StringToArrayString(strTemp, wxT("#"))); }	
	blError = config->Read(strName + wxT("/Function"), &strTemp);
	if(!blError){ SetFunction(strTemp); }
	blError = config->Read(strName + wxT("/Format"), &strTemp);
	if(!blError){ SetFormat(strTemp); }
	blError = config->Read(strName + wxT("/Ratio"), &strTemp);
	if(!blError){ SetRatio(strTemp); }
	delete config;

	if(blError){
		ErrorMessage(wxT("There was an error reading from the jobs file, \nplease check it is not set as read only or in use."));
		return false;
	}
	return true;
}

bool BackupData::TransferToFile(wxString strName){
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Left(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Length() - 11) + wxT("\\Data\\Jobs.ini") );
	
	bool blError;

	blError = config->DeleteGroup(strName);
	blError = config->Write(strName + wxT("/Locations"),  ArrayStringToString(GetLocations(), wxT("#")));	
	blError = config->Write(strName + wxT("/Function"), GetFunction());	
	blError = config->Write(strName + wxT("/Format"), GetFormat());	
	blError = config->Write(strName + wxT("/Ratio"), GetRatio());
	delete config;
	
	if(blError){
		ErrorMessage(wxT("There was an error saving to the jobs file, \nplease check it is not set as read only or in use."));
		return false;
	}
	return true;
}

/*This function takes the data in BackupData and fills in the GUI*/
void BackupData::TransferToForm(frmMain *window){
	winodw->m_Backup_Tree->DeleteAllItems();
	AddDirToTree(window->m_Backup_Tree, GetLocations());
	m_Backup_Function->SetStringSelection(GetFunction));
	m_Backup_Format->SetStringSelection(GetFormat());
	m_Backup_Ratio->SetStringSelection(GetRatio());
	m_Backup_Pass->SetValue(GetPass());
	m_Backup_Repass->SetValue(GetPass());
	return;
}

/* This function sets all of the fields in SyncData based on the user inputted data in the
Main program window.*/
bool BackupData::TransferFromForm(frmMain *window){
	bool blNotFilled = false;
	SetLocations(wxGetApp().GetBackupLocations());
	if(GetLocations.GetCount() == 0){ blNotFilled = true; }
	if(m_Backup_Function->GetStringSelection() != wxEmptyString){ SetFunction(window->m_Backup_Function->GetStringSelection()); }
	else{ blNotFilled = true; }
	if(m_Backup_Format->GetStringSelection() != wxEmptyString){ SetFormat(window->m_Backup_Format->GetStringSelection()) ; }
	else{ blNotFilled = true; }
	if(m_Backup_Ratio->GetStringSelection() != wxEmptyString){ SetRatio(window->m_Backup_Ratio->GetStringSelection()); }
	else{ blNotFilled = true; }
	if(m_Backup_Pass->GetValue() != wxEmptyString){ wxString strPass = m_Backup_Pass->GetValue(); }
	else{ blNotFilled = true; }
	if(m_Backup_Repass->GetValue() != wxEmptyString){ wxString strRepass = m_Backup_Repass->GetValue(); }
	else{ blNotFilled = true; }
	if(blNotFilled){
		ErrorMessage(_("Not all of the required fields are filled"));
		return false;
	}
	if(strPass == strRepass){
		SetPass(strPass);
	}
	else{
		ErrorBox(_(The passwords to not match, please try again.));
		return false;
	}
	return true;	
}

BackupData::Output(){
	for(int i = 0; i < GetLocations.GetCount(); i++){
		MessageBox(GetLocations.Item(i), wxT("Location"))
	}
	MessageBox(GetFunction, wxT("Function"));
	MessageBox(GetFormat, wxT("Format"));
	MessageBox(GetRatio, wxT("Ratio"));
	MessageBox(GetPass, wxT("Pass"));
}

wxString BackupData::CreateCommand(){
	
	//General string until this gets setup to use the class
	wxString strFormat, strRatio, strPass, strCommand;
	//Setting up to use the first item in the array.
	wxString strFirst = GetLocations().Item(0);
	
	//strSolid is only used if 7zip is chosen to allow updating
	wxString strSolid = wxEmptyString;

	//Format based stuff
	if (GetFormat() == wxT("Zip")){
        strFormat = wxT("zip");
    }
    else if (GetFormat() == wxT("7 Zip")){
        strFormat = wxT("7z");
		strSolid = wxT(" -ms=off");
    }
    
	//Ratio stuff, will need to be updated for 
    if (GetRatio() == _("Normal")){
        strRatio = wxT("5");
    }
    else if (GetRatio() == _("Maximum")){
        strRatio = wxT("9");
    }
	
	//Checking to see if there is a password used
	if(GetPass() != wxEmptyString){
		strPass = wxT(" -p") + strPass;
        if(GetFormat() = wxT("7z")){
            strPass += wxT(" -mhe=on");
        }
	}
   
    if(GetType() == _("Complete")){
        strCommand = wxT("7za.exe a -t") + strFormat + strPass + wxT(" -mx") + strRatio + wxT(" \"") + strSecond + wxT("\" \"") + strFirst + wxT("\\*\"") + wxT(" -x@\"") + wxPathOnly(wxStandardPaths::Get().GetExecutablePath())+ wxFILE_SEP_PATH + wxT("temp-exclusions.txt") + wxT("\"") + strSolid;    
        //wxMessageBox(command);
    }
	else if(GetType() == _("Update")){
        strCommand = wxT("7za.exe  u -t") + strFormat + wxT(" \"") + strSecond + wxT("\" \"") + strFirst + wxT("\\*\"")  + wxT(" -x@\"") + wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) +  + wxFILE_SEP_PATH + wxT("temp-exclusions.txt") + wxT("\"")  + strPass+ strSolid;
	}
	else if(GetType() == _("Restore")){
        strCommand = wxT("7za.exe  x  \"") + strFirst + wxT("\" -o\"") + strSecond + wxT("\" * -r")  + wxT(" -x@\"") + wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH  + wxT("temp-exclusions.txt") + wxT("\"") + strPass + strSolid;
	}
	//With the incremental type the first use creates a file called base file. On subsequent runs a file is created with a filename based on both the date and time.    
	else if(GetType() == _("Incremental")){
        if(wxFileExists(strSecond + wxFILE_SEP_PATH + wxT("BaseFile.") + strFormat)){
            wxDateTime now = wxDateTime::Now();
            if (strFirst[strFirst.length()-1] != wxFILE_SEP_PATH) {
                strFirst += wxFILE_SEP_PATH;       
            }
			strCommand = wxT("7za.exe u \"") + strSecond + wxFILE_SEP_PATH + wxT("BaseFile.") + strFormat + wxT("\" -u- -up0q3x2z0!\"") + strSecond + wxFILE_SEP_PATH + now.FormatISODate()+ wxT("-") + now.Format(wxT("%H")) + wxT("-") +  now.Format(wxT("%M")) + wxT(".") + strFormat + wxT("\" \"") + strFirst + wxT("*\"") + wxT(" -x@\"") + wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("temp-exclusions.txt") + wxT("\"") + strSolid + strPass;
        }
        else{
			strCommand = wxT("7za.exe a -t") + strFormat + wxT(" -mx") + strRatio + wxT(" \"") + strSecond + wxFILE_SEP_PATH + wxT("BaseFile.") + strFormat + wxT("\" \"") + strFirst + wxT("\\*\"")  + wxT(" -x@\"") + wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH  + wxT("temp-exclusions.txt") + wxT("\"") + strSolid + strPass;    
        }
    }
    return command;
}
#endif
