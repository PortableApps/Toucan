/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SETTINGS
#define H_SETTINGS

#include <wx/string.h>
#include <wx/intl.h>

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
	
	double GetHeight() { return m_Height; }
	void SetHeight(float height) { m_Height = height; }	
	
	double GetWidth() { return m_Width; }
	void SetWidth(float width) { m_Width = width; }	
	
	double GetY() { return m_Y; }
	void SetY(double y) { m_Y = y; }	
	
	double GetX() { return m_X; }
	void SetX(double x) { m_X = x; }	
	
	double GetRememberSync() { return m_RememberSync; }
	void SetRememberSync(bool remember) { m_RememberSync = remember; }	
	
	double GetRememberBackup() { return m_RememberBackup; }
	void SetRememberBackup(bool remember) { m_RememberBackup = remember; }	
	
	double GetRememberSecure() { return m_RememberSecure; }
	void SetRememberSecure(bool remember) { m_RememberSecure = remember; }	
	
private:
	wxString m_Position;
	wxString m_TabStyle;
	wxString m_LanguageCode;
	wxString m_Font;
	double m_Height;
	double m_Width;
	double m_X;
	double m_Y;
	bool m_RememberSync;
	bool m_RememberBackup;
	bool m_RememberSecure;
};

#endif

