#ifndef H_SECUREDATA
#define H_SECUREDATA

class SecureData{

public:
	//Constuctor
	SecureData();

	//Functions
	bool TransferToFile(wxString strName);
	bool TransferFromFile(wxString strName);
	void TransferToForm(frmMain *window);
	bool TransferFromForm(frmMain*window);
	void Output();

	//Inline functions
	void SetLocations(wxArrayString locations) { arrLocations = locations; }
	wxArrayString GetLocations() { return arrLocations; }

	void SetFunction(wxString function) { strFunction = function; }
	wxString GetFunction() { return strFunction; }

	void SetFormat(wxString format) { strFormat = format; }
	wxString GetFormat() { return strFormat; }

	void SetPass(wxString pass) { strPass = pass; }
	wxString GetPass() { return strPass; }


private:
	wxArrayString arrLocations;
	wxString strFunction;
	wxString strFormat;
	wxString strPass;

}

bool SecureData::TransferFromFile(wxString strName){
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Left(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Length() - 11) + wxT("\\Data\\Jobs.ini") );
	
	bool blError;
	wxString strTemp;

	blError = config->Read(strName + wxT("/Locations"), &strTemp);
	if(!blError){ SetLocations(StringToArrayString(strTemp, wxT("#"))); }	
	blError = config->Read(strName + wxT("/Function"), &strTemp);
	if(!blError){ SetFunction(strTemp); }
	blError = config->Read(strName + wxT("/Format"), &strTemp);
	if(!blError){ SetFormat(strTemp); }
	delete config;

	if(blError){
		ErrorMessage(wxT("There was an error reading from the jobs file, \nplease check it is not set as read only or in use."));
		return false;
	}
	return true;
}

bool SecureData::TransferToFile(wxString strName){
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Left(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Length() - 11) + wxT("\\Data\\Jobs.ini") );
	
	bool blError;

	blError = config->DeleteGroup(strName);
	blError = config->Write(strName + wxT("/Locations"),  ArrayStringToString(GetLocations(), wxT("#")));	
	blError = config->Write(strName + wxT("/Function"), GetFunction());	
	blError = config->Write(strName + wxT("/Format"), GetFormat());	
	delete config;

	if(blError){
		ErrorMessage(wxT("There was an error saving to the jobs file, \nplease check it is not set as read only or in use."));
		return false;
	}
	return true;
}

/*This function takes the data in BackupData and fills in the GUI*/
void SecureData::TransferToForm(frmMain *window){
	winodw->m_Secure_Tree->DeleteAllItems();
	AddDirToTree(window->m_Secure_Tree, GetLocations());
	m_Secure_Function->SetStringSelection(GetFunction));
	m_Secure_Format->SetStringSelection(GetFormat());
	m_Secure_Pass->SetValue(GetPass);
	m_Secure_Repass->SetValue(GetPass);
	return;
}

/* This function sets all of the fields in SyncData based on the user inputted data in the
Main program window.*/
bool SecureData::TransferFromForm(frmMain *window){
	bool blNotFilled = false;
	SetLocations(wxGetApp().GetSecureLocations());
	if(GetLocations.GetCount() == 0){ blNotFilled = true; }
	if(m_Secure_Function->GetStringSelection() != wxEmptyString){ SetFunction(window->m_Secure_Function->GetStringSelection()); }
	else{ blNotFilled = true; }
	if(m_Secure_Format->GetStringSelection() != wxEmptyString){ SetFormat(window->m_Secure_Format->GetStringSelection()) ; }
	else{ blNotFilled = true; }
	if(m_Secure_Pass->GetValue() != wxEmptyString){ wxString strPass = m_Secure_Pass->GetValue(); }
	else{ blNotFilled = true; }
	if(m_Secure_Repass->GetValue() != wxEmptyString){ wxString strRepass = m_Secure_Repass->GetValue(); }
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

SecureData::Output(){
	for(int i = 0; i < GetLocations.GetCount(); i++){
		MessageBox(GetLocations.Item(i), wxT("Location"))
	}
	MessageBox(GetFunction, wxT("Function"));
	MessageBox(GetFormat, wxT("Format"));
	MessageBox(GetPass, wxT("Pass"));
}

#endif
