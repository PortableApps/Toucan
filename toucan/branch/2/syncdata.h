#ifndef H_SYNCDATA
#define H_SYNCDATA

#include <wx/variant.h>

class SyncData{

public:
	//Constuctor
	SyncData();

	//Functions
	bool TransferToFile(wxString strName);
	bool TransferFromFile(wxString strName);
	void TransferToForm(frmMain *window);
	bool TransferFromForm(frmMain*window);
	void Output();

	//Inline functions
	void SetSource(wxString source) { strSource = source; }
	wxString GetSource() { return strSource; }

	void SetDest(wxString dest) { strDest = dest; }
	wxString GetDest() { return strDest; }

	void SetFunction(wxString function) { strFunction = function; }
	wxString GetFunction() { return strFunction; }

	void SetTimeStamps(bool timestamps) { blTimeStamps = timestamps; }
	bool GetTimeStamps() { return blTimeStamps; }

	void SetAttributes(bool attributes) { blAttributes = attributes; }
	bool GetAttributes() { return blAttributes; }

	void SetIgnoreRO(bool ignorero) { blIgnoreRO = ignorero; }
	bool  GetIgnoreRO() { return blIgnoreRO; }

	void SetIgnoreDLS(bool ignoredls) { blIgnoreDLS = ignoredls; }
	bool  GetIgnoreDLS() { return blIgnoreDLS; }

private:
	wxString strSource;
	wxString strDest;
	wxString strFunction;
	bool blTimeStamps;
	bool blAttributes;
	bool blIgnoreRO;
	bool blIgnoreDLS;

};

bool SyncData::TransferFromFile(wxString strName){
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Left(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Length() - 11) + wxT("\\Data\\Jobs.ini") );
	
	bool blError;
	wxString strTemp;
	bool blTemp;

	blError = config->Read(strName + wxT("/Source"), &strTemp);
	if(!blError){ SetSource(strTemp); }	
	blError = config->Read(strName + wxT("/Dest"), &strTemp);
	if(!blError){ SetDest(strTemp); }	
	blError = config->Read(strName + wxT("/Function"), &strTemp);
	if(!blError){ SetFunction(strTemp); }
	blError = config->Read(strName + wxT("/TimeStamps"), &blTemp);
	if(!blError){ SetTimeStamps(blTemp); }
	blError = config->Read(strName + wxT("/Attributes"), &blTemp);
	if(!blError){ SetAttributes(blTemp); }
	blError = config->Read(strName + wxT("/IgnoreReadOnly"), &blTemp);
	if(!blError){ SetIgnoreRO(blTemp); }
	blError = config->Read(strName + wxT("/IgnoreDaylightSavings"), &blTemp);
	if(!blError){ SetIgnoreDLS(blTemp); }
	delete config;
	
	if(blError){
		ErrorBox(wxT("There was an error reading from the jobs file, \nplease check it is not set as read only or in use."));
		return false;
	}
	return true;
}

bool SyncData::TransferToFile(wxString strName){
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Left(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()).Length() - 11) + wxT("\\Data\\Jobs.ini") );
	
	bool blError;

	blError = config->DeleteGroup(strName);
	blError = config->Write(strName + wxT("/Source"),  GetSource());	
	blError = config->Write(strName + wxT("/Dest"), GetDest());	
	blError = config->Write(strName + wxT("/Function"), GetFunction());
	blError = config->Write(strName + wxT("/TimeStamps"), GetTimeStamps());
	blError = config->Write(strName + wxT("/Attributes"), GetAttributes());
	blError = config->Write(strName + wxT("/IgnoreReadOnly"), GetIgnoreRO());
	blError = config->Write(strName + wxT("/IgnoreDaylightSavings"), GetIgnoreDLS());
	delete config;
	if(blError){
		ErrorBox(wxT("There was an error saving to the jobs file, \nplease check it is not set as read only or in use."));
		return false;
	}
	return true;
}

/*This function takes the data in SyncData and fills in the GUI*/
void SyncData::TransferToForm(frmMain *window){
	window->m_Sync_Source_Txt->SetValue(GetSource());
	window->m_Sync_Source_Tree->DeleteAllItems();
	AddDirToTree(GetSource(), window->m_Sync_Source_Tree, window);
	window->m_Sync_Dest_Txt->SetValue(GetDest());
	window->m_Sync_Dest_Tree->DeleteAllItems();
	AddDirToTree(GetDest(), window->m_Sync_Dest_Tree, window);
	window->m_Sync_Function->SetStringSelection(GetFunction());
	window->m_Sync_Timestamp->SetValue(GetTimeStamps());
	window->m_Sync_Attributes->SetValue(GetAttributes());
	window->m_Sync_Ignore_Readonly->SetValue(GetIgnoreRO());
	window->m_Sync_Ignore_DaylightS->SetValue(GetIgnoreDLS());
	return;
}

/* This function sets all of the fields in SyncData based on the user inputted data in the
Main program window.*/
bool SyncData::TransferFromForm(frmMain *window){
	bool blNotFilled = false;
	if(window->m_Sync_Source_Txt->GetValue() != wxEmptyString){ SetSource(window->m_Sync_Source_Txt->GetValue()); }
	else{ blNotFilled = true; }
	if(window->m_Sync_Dest_Txt->GetValue() != wxEmptyString){ SetDest(window->m_Sync_Dest_Txt->GetValue()); }
	else{ blNotFilled = true; }
	if(window->m_Sync_Function->GetStringSelection() != wxEmptyString){ SetFunction(window->m_Sync_Function->GetStringSelection()); }
	else{ blNotFilled = true; }
	SetTimeStamps(window->m_Sync_Timestamp->GetValue());
	SetAttributes(window->m_Sync_Attributes->GetValue());
	SetIgnoreRO(window->m_Sync_Ignore_Readonly->GetValue());
	SetIgnoreDLS(window->m_Sync_Ignore_DaylightS->GetValue());
	if(blNotFilled){
		ErrorBox(_("Not all of the required fields are filled"));
		return false;
	}
	return true;	
}

void SyncData::Output(){
	MessageBox (GetSource(), wxT("Source"));
	MessageBox(GetDest(), wxT("Destination"));
	MessageBox(GetFunction(), wxT("Function"));
	wxVariant varTemp;
	varTemp = GetTimeStamps();
	MessageBox(varTemp.GetString(), wxT("Timestamps?"));
	varTemp = GetAttributes();
	MessageBox(varTemp.GetString(), wxT("Attributes?"));
	varTemp = GetIgnoreRO();
	MessageBox(varTemp.GetString(), wxT("Ignore Readonly?"));
	varTemp = GetIgnoreDLS();
	MessageBox(varTemp.GetString(), wxT("Ignore Daylight Savings?"));
}

#endif
