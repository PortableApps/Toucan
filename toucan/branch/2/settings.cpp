/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "settings.h"
#include <wx\fileconf.h>
#include <wx\stdpaths.h>

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
