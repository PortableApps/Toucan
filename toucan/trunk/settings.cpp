/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "settings.h"
#include "toucan.h"
#include <wx/fileconf.h>
#include <wx/stdpaths.h>
#include <wx/intl.h>

bool Settings::TransferToFile(){
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxGetApp().GetSettingsPath()+ wxT("Settings.ini"));
	config->Write(wxT("General/Tabs"), m_TabStyle);
	config->Write(wxT("General/Position"), m_Position);
	config->Write(wxT("General/LanguageCode"), m_LanguageCode);
	config->Write(wxT("General/Font"), m_Font);
	config->Write(wxT("General/Height"), m_Height);
	config->Write(wxT("General/Width"), m_Width);
	config->Write(wxT("General/X"), m_X);
	config->Write(wxT("General/Y"), m_Y);
	config->Write(wxT("General/RememberSync"), m_RememberSync);
	config->Write(wxT("General/RememberBackup"), m_RememberBackup);
	config->Write(wxT("General/RememberSecure"), m_RememberSecure);
	delete config;
	return true;
}

bool Settings::TransferFromFile(){
	//Set the default values
	m_TabStyle = _("Icons and Text");
	m_Position = _("Sync");
	m_LanguageCode = wxT("en");
	wxFont temp = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
	m_Font = temp.GetNativeFontInfoDesc();
	m_Height = 1;
	m_Width = 1;
	m_Y = 0;
	m_X = 0;
	m_RememberSync = false;
	m_RememberBackup = false;
	m_RememberSecure = false;
	
	//Read from the settings file
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxGetApp().GetSettingsPath()+ wxT("Settings.ini"));
	config->Read(wxT("General/Tabs"), &m_TabStyle);
	config->Read(wxT("General/Position"), &m_Position);
	config->Read(wxT("General/LanguageCode"), &m_LanguageCode);
	config->Read(wxT("General/Font"), &m_Font);
	config->Read(wxT("General/Height"), &m_Height);
	config->Read(wxT("General/Width"), &m_Width);
	config->Read(wxT("General/X"), &m_X);
	config->Read(wxT("General/Y"), &m_Y);
	config->Read(wxT("General/RememberSync"), &m_RememberSync);
	config->Read(wxT("General/RememberBackup"), &m_RememberBackup);
	config->Read(wxT("General/RememberSecure"), &m_RememberSecure);
	delete config;	
	return true;
}
