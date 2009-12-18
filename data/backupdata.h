/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 - 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_BACKUPDATA
#define H_BACKUPDATA

class wxTextFile;

#include "jobdata.h"
#include <wx/string.h>
#include <wx/arrstr.h>


class BackupData : public JobData{

public:
	BackupData(const wxString &name) : JobData(name){
		;
	}

	bool TransferToFile();
	bool TransferFromFile();
	bool TransferToForm(frmMain *window);
	bool TransferFromForm(frmMain *window);

	bool IsReady();
	void Output();
	
	wxString CreateCommand(int i);
	bool CreateList(wxTextFile *file, wxString path, int length);

	void SetFileLocation(const wxString& FileLocation) {this->m_FileLocation = FileLocation;}
	void SetFormat(const wxString& Format) {this->m_Format = Format;}
	void SetFunction(const wxString& Function) {this->m_Function = Function;}
	void SetRatio(const int& Ratio) {this->m_Ratio = Ratio;}
	void SetLocations(const wxArrayString& Locations) {this->m_Locations = Locations;}
	void SetLocation(const int& i, const wxString& Location) {this->m_Locations.Item(i) = Location;}
	void SetUsesPassword(const bool& UsesPassword) {this->m_UsesPassword = UsesPassword;}
	void SetTest(const bool& Test) {this->m_Test = Test;}
	void SetPassword(const wxString& Password) {this->m_Password = Password;}

	const wxString& GetFileLocation() const {return m_FileLocation;}
	const wxString& GetFormat() const {return m_Format;}
	const wxString& GetFunction() const {return m_Function;}
	const int& GetRatio() const {return m_Ratio;}
	const wxArrayString& GetLocations() const {return m_Locations;}
	const wxString& GetLocation(const int& i) const {return m_Locations.Item(i);}
	const bool& GetUsesPassword() const {return m_UsesPassword;}
	const bool& GetTest() const {return m_Test;}
	const wxString& GetPassword() const {return m_Password;}

private:
	wxString m_FileLocation;
	wxArrayString m_Locations;
	wxString m_Function;
	wxString m_Format;
	wxString m_Password;
	int m_Ratio;
	bool m_UsesPassword;
	bool m_Test;
};

#endif
