/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "settings.h"
#include "toucan.h"
#include <wx\fileconf.h>
#include <wx\stdpaths.h>
#include <wx\intl.h>

bool Settings::TransferToFile(){
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxGetApp().GetSettingsPath()+ wxT("Settings.ini"));
	config->Write(wxT("General/Tabs"), m_TabStyle);
	config->Write(wxT("General/Position"), m_Position);
	config->Write(wxT("General/LanguageCode"), m_LanguageCode);
	delete config;
	return true;
}

bool Settings::TransferFromFile(){
	m_TabStyle = wxEmptyString;
	m_Position = wxEmptyString;
	m_LanguageCode = wxEmptyString;
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxGetApp().GetSettingsPath()+ wxT("Settings.ini"));
	config->Read(wxT("General/Tabs"), &m_TabStyle,  _("Icons + Text"));
	config->Read(wxT("General/Position"), &m_Position,  _("Sync"));
	config->Read(wxT("General/LanguageCode"), &m_LanguageCode, wxT("en"));
	delete config;	
	return true;
}
