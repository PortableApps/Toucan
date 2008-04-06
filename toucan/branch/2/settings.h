#ifndef _H_SETTINGS_
#define _H_SETTINGS_


//Saves the basic settings, need to be expanded for language support, window position & size

class Settings{
	
public:
	//Functions
	Settings(frmMain *window) { m_Window = window; } 
	bool TransferToFile();
	bool TransferFromFile();
	bool TransferFromForm();
	bool TransferToForm();
	
private:
	
	bool m_Position;
	wxString m_Tabs;
	frmMain *m_Window;
};

bool Settings::TransferToFile(){
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("Settings.ini"));
	config->Write(wxT("General/Tabs"), m_Tabs);
	config->Write(wxT("General/Position"), m_Position);
	delete config;
	return true;
}

bool Settings::TransferFromFile(){
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("Settings.ini"));
	m_Tabs = config->Read(wxT("General/Tabs"));
	config->Read(wxT("General/Position"), m_Position);
	delete config;	
	return true;
}

bool Settings::TransferFromForm(){
	m_Tabs = m_Window->m_Settings_Tabs->GetStringSelection();
	m_Position = m_Window->m_Settings_Position->GetValue();
	return true;
}

bool Settings::TransferToForm(){
	m_Window->m_Settings_Tabs->SetStringSelection(m_Tabs);
	m_Window->m_Settings_Position->SetValue(m_Position);
	return true;
}

#endif
