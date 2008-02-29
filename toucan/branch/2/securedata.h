#ifndef H_SECUREDATA
#define H_SECUREDATA

#include "basicfunctions.h"
#include "toucan.h"
#include <wx/fileconf.h>
#include <wx/stdpaths.h>

class SecureData{

public:
	//Constuctor
	SecureData();

	//Functions
	bool TransferToFile(wxString strName);
	bool TransferFromFile(wxString strName, bool blShowError);
	void TransferToForm(frmMain *window);
	bool TransferFromForm(frmMain*window, bool blShowError);
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

};

SecureData::SecureData(){
	;
}


bool SecureData::TransferFromFile(wxString strName, bool blShowError){
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxPathOnly(wxStandardPaths::Get().GetExecutablePath())+ wxFILE_SEP_PATH + wxT("Jobs.ini") );
	
	bool blError;
	wxString strTemp;

	blError = config->Read(strName + wxT("/Locations"), &strTemp);
	if(blError){ SetLocations(StringToArrayString(strTemp, wxT("#"))); }	
	blError = config->Read(strName + wxT("/Function"), &strTemp);
	if(blError){ SetFunction(strTemp); }
	blError = config->Read(strName + wxT("/Format"), &strTemp);
	if(blError){ SetFormat(strTemp); }
	delete config;

	if(!blError && blShowError){
		ErrorBox(wxT("There was an error reading from the jobs file, \nplease check it is not set as read only or in use."));
		return false;
	}
	return true;
}

bool SecureData::TransferToFile(wxString strName){
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("Jobs.ini") );
	
	bool blError;

	blError = config->DeleteGroup(strName);
	wxMessageBox(ArrayStringToString(GetLocations(), wxT("#")));
	blError = config->Write(strName + wxT("/Locations"),  ArrayStringToString(GetLocations(), wxT("#")));	
	blError = config->Write(strName + wxT("/Function"), GetFunction());	
	blError = config->Write(strName + wxT("/Format"), GetFormat());	
	
	delete config;

	if(!blError){
		ErrorBox(wxT("There was an error saving to the jobs file, \nplease check it is not set as read only or in use."));
		return false;
	}
	return true;
}

/*This function takes the data in BackupData and fills in the GUI*/
void SecureData::TransferToForm(frmMain *window){
	//Delete all of the existing items in the treectrl
	window->m_Secure_TreeCtrl->DeleteAllItems();
	//Remove all of the filepaths from the list
	for(unsigned int i = 0; i < wxGetApp().GetSecureLocations().GetCount(); i++){
		wxGetApp().RemoveSecureLocation(i);
	}
	//Add the new filepaths to the list and the treectrl
	for(unsigned int k = 0; k < GetLocations().Count(); k++){
		wxGetApp().AppendSecureLocation(GetLocations().Item(k));
		window->m_Secure_TreeCtrl->SetRootPath(GetLocations().Item(k));
	}
	//Set up the rest of the window
	window->m_Secure_Function->SetStringSelection(GetFunction());
	window->m_Secure_Format->SetStringSelection(GetFormat());
	window->m_Secure_Pass->SetValue(GetPass());
	window->m_Secure_Repass->SetValue(GetPass());
	return;
}

/* This function sets all of the fields in SyncData based on the user inputted data in the
Main program window.*/
bool SecureData::TransferFromForm(frmMain *window, bool blShowError){
	bool blNotFilled = false;
	wxString strPass;
	wxString strRepass;
	SetLocations(wxGetApp().GetSecureLocations());
	if(GetLocations().GetCount() == 0){ blNotFilled = true; }
	if(window->m_Secure_Function->GetStringSelection() != wxEmptyString){ SetFunction(window->m_Secure_Function->GetStringSelection()); }
	else{ blNotFilled = true; }
	if(window->m_Secure_Format->GetStringSelection() != wxEmptyString){ SetFormat(window->m_Secure_Format->GetStringSelection()) ; }
	else{ blNotFilled = true; }
	if(window->m_Secure_Pass->GetValue() != wxEmptyString){strPass = window->m_Secure_Pass->GetValue(); }
	else{ blNotFilled = true; }
	if(window->m_Secure_Repass->GetValue() != wxEmptyString){strRepass = window->m_Secure_Repass->GetValue(); }
	else{ blNotFilled = true; }
	if(blNotFilled && blShowError){
		ErrorBox(_("Not all of the required fields are filled"));
		return false;
	}
	if(strPass == strRepass){
		SetPass(strPass);
	}
	else{
		ErrorBox(_("The passwords to not match, please try again."));
		return false;
	}
	return true;	
}

void SecureData::Output(){
	for(unsigned int i = 0; i < GetLocations().GetCount(); i++){
		MessageBox(GetLocations().Item(i), wxT("Location"));
	}
	MessageBox(GetFunction(), wxT("Function"));
	MessageBox(GetFormat(), wxT("Format"));
	MessageBox(GetPass(), wxT("Pass"));
}


#endif
