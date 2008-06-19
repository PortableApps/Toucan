/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SETTINGS
#define H_SETTINGS

#include <wx\string.h>
#include <wx\intl.h>

class Settings{
	
public:
	bool TransferToFile();
	bool TransferFromFile();
	
	wxString GetPosition() { return m_Position; }
	void SetPosition(wxString position) { m_Position = position; }
	
	wxString GetTabStyle() { return m_TabStyle; }
	void SetTabStyle(wxString tabstyle) { m_TabStyle = tabstyle; }
	
	wxString GetLanguageCode() { return m_LanguageCode; }
	void SetLanguageCode(wxString langcode) { m_LanguageCode = langcode; }
	
	wxString GetFont() { return m_Font; }
	void SetFont(wxString font) { m_Font = font; }	
	
	int GetHeight() { return m_Height; }
	void SetHeight(int height) { m_Height = height; }	
	
	int GetWidth() { return m_Width; }
	void SetWidth(int width) { m_Width = width; }	
	
private:
	wxString m_Position;
	wxString m_TabStyle;
	wxString m_LanguageCode;
	wxString m_Font;
	int m_Height;
	int m_Width;
};

#endif
