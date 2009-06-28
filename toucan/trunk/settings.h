/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 - 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SETTINGS
#define H_SETTINGS

#include <wx/string.h>
class wxFileConfig;

class Settings{

public:
	Settings(wxString path);
	~Settings();

	bool TransferToFile();
	bool TransferFromFile();

	wxFileConfig* GetConfig() { return config; }

	void SetFont(const wxString& Font) {this->m_Font = Font;}
	void SetHeight(const double& Height) {this->m_Height = Height;}
	void SetLanguageCode(const wxString& LanguageCode) {this->m_LanguageCode = LanguageCode;}
	void SetPosition(const wxString& Position) {this->m_Position = Position;}
	void SetRememberBackup(const bool& RememberBackup) {this->m_RememberBackup = RememberBackup;}
	void SetRememberSecure(const bool& RememberSecure) {this->m_RememberSecure = RememberSecure;}
	void SetRememberSync(const bool& RememberSync) {this->m_RememberSync = RememberSync;}
	void SetTabStyle(const wxString& TabStyle) {this->m_TabStyle = TabStyle;}
	void SetWidth(const double& Width) {this->m_Width = Width;}
	void SetX(const double& X) {this->m_X = X;}
	void SetY(const double& Y) {this->m_Y = Y;}
	void SetEnableTooltips(const bool& EnableTooltips) {this->m_EnableTooltips = EnableTooltips;}
	void SetSmallBorders(const bool& SmallBorders) {this->m_SmallBorders = SmallBorders;}
	const bool& GetDisableLog() const {return m_DisableLog;}
	const bool& GetDisableStream() const {return m_DisableStream;}
	const wxString& GetFont() const {return m_Font;}
	const double& GetHeight() const {return m_Height;}
	const wxString& GetLanguageCode() const {return m_LanguageCode;}
	const wxString& GetPosition() const {return m_Position;}
	const bool& GetRememberBackup() const {return m_RememberBackup;}
	const bool& GetRememberSecure() const {return m_RememberSecure;}
	const bool& GetRememberSync() const {return m_RememberSync;}
	const wxString& GetTabStyle() const {return m_TabStyle;}
	const double& GetWidth() const {return m_Width;}
	const double& GetX() const {return m_X;}
	const double& GetY() const {return m_Y;}
	const bool& GetEnableTooltips() const {return m_EnableTooltips;}
	const bool& GetSmallBorders() const {return m_SmallBorders;}

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
	bool m_DisableStream;
	bool m_DisableLog;
	bool m_EnableTooltips;
	bool m_SmallBorders;
	wxFileConfig* config;
};

#endif

