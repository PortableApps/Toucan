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
	config->Write(wxT("General/Language"), m_LanguageCode);
	delete config;
	return true;
}

bool Settings::TransferFromFile(){
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxGetApp().GetSettingsPath()+ wxT("Settings.ini"));
	config->Read(wxT("General/Tabs"), &m_TabStyle,  _("Icons + Text"));
	config->Read(wxT("General/Position"), &m_Position,  _("Sync"));
	int temp;
	config->Read(wxT("General/Language"), &temp, wxLanguage(wxLANGUAGE_ENGLISH));
	m_LanguageCode = wxLanguage(temp);
	delete config;	
	return true;
}
