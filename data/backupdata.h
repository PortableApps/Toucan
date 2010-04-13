/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 - 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_BACKUPDATA
#define H_BACKUPDATA

class wxTextFile;

#include "jobdata.h"
#include <wx/string.h>
#include <wx/arrstr.h>

struct BackupOptions{
	bool Password;
	bool Test;
	bool Solid;

	BackupOptions() : Password(false), Test(false), Solid(true)
	{}
};


class BackupData : public JobData{

public:
	BackupData(const wxString &name) : JobData(name){
		;
	}

	void TransferToFile();
	void TransferFromFile();
	bool TransferToForm(frmMain *window);
	bool TransferFromForm(frmMain *window);

	void Output();
	
	wxArrayString CreateCommands();
	bool CreateList(wxTextFile *file, wxString path, int length);

	void SetFileLocation(const wxString& FileLocation) {this->m_FileLocation = FileLocation;}
	void SetFormat(const wxString& Format) {this->m_Format = Format;}
	void SetFunction(const wxString& Function) {this->m_Function = Function;}
	void SetRatio(const int& Ratio) {this->m_Ratio = Ratio;}
	void SetLocations(const wxArrayString& Locations) {this->m_Locations = Locations;}
	void SetLocation(const int& i, const wxString& Location) {this->m_Locations.Item(i) = Location;}
	void SetUsesPassword(const bool& UsesPassword) {this->m_Options.Password = UsesPassword;}
	void SetTest(const bool& Test) {this->m_Options.Test = Test;}
	void SetSolid(const bool& Solid) {this->m_Options.Solid = Solid;}
	void SetPassword(const wxString& Password) {this->m_Password = Password;}

	const wxString& GetFileLocation() const {return m_FileLocation;}
	const wxString& GetFormat() const {return m_Format;}
	const wxString& GetFunction() const {return m_Function;}
	const int& GetRatio() const {return m_Ratio;}
	const wxArrayString& GetLocations() const {return m_Locations;}
	const wxString& GetLocation(const int& i) const {return m_Locations.Item(i);}
	const bool& GetUsesPassword() const {return m_Options.Password;}
	const bool& GetTest() const {return m_Options.Test;}
	const bool& GetSolid() const {return m_Options.Solid;}
	const wxString& GetPassword() const {return m_Password;}

private:
	wxString m_FileLocation;
	wxArrayString m_Locations;
	wxString m_Function;
	wxString m_Format;
	wxString m_Password;
	int m_Ratio;
	BackupOptions m_Options;
};

#endif
