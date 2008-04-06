#ifndef _H_SETTINGS_
#define _H_SETTINGS_

//Saves the basic settings, need to be expanded for language support, window position & size

class Settings{
	
public:
	bool TransferToFile();
	bool TransferFromFile();
	
	wxString GetPosition() { return m_Position; }
	void SetPosition(wxString position) { m_Position = position; }
	
	wxString GetTabStyle() { return m_TabStyle; }
	void SetTabStyle(wxString tabstyle) { m_TabStyle = tabstyle; }
	
private:
	
	wxString m_Position;
	wxString m_TabStyle;
};

bool Settings::TransferToFile(){
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("Settings.ini"));
	config->Write(wxT("General/Tabs"), m_TabStyle);
	config->Write(wxT("General/Position"), m_Position);
	delete config;
	return true;
}

bool Settings::TransferFromFile(){
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("Settings.ini"));
	m_TabStyle = config->Read(wxT("General/Tabs"));
	m_Position = config->Read(wxT("General/Position"));
	delete config;	
	return true;
}

#endif
