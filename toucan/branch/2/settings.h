//Saves the basic settings, need to be expanded for language support, window position & size

bool SaveSettings(frmMain *window){
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("Settings.ini"));
	config->Write(wxT("General/Tabs"), window->m_Settings_Tabs->GetStringSelection());
	delete config;
	return true;
}
