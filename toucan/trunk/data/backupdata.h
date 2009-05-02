/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_BACKUPDATA
#define H_BACKUPDATA

#include <wx/textfile.h>
#include <wx/string.h>
#include "../forms/frmmain.h"
#include "rootdata.h"

/*The backupdata class contains all of the information needed for backups, except the ruels. Includes
functiosn for writing the data to and from the gui and an ini file*/
class BackupData : public RootData{
	//ATTN : Replace IsPassword
public:
	//Functions
	bool TransferToFile();
	bool TransferFromFile();
	bool TransferToForm();
	bool TransferFromForm();
	
	bool NeededFieldsFilled();

	void Output();
	
	bool Execute(Rules rules);
	
	bool NeedsPassword() { return IsPassword; }
	
	//Used to create the command to exectue
	wxString CreateCommand(int i);
	
	//Used to create the exclusions list
	bool CreateList(wxTextFile *file, Rules rules, wxString strPath, int iRootLength);

	//Setters / Getters
	void SetFileLocation(const wxString& FileLocation) {this->m_FileLocation = FileLocation;}
	void SetFormat(const wxString& Format) {this->m_Format = Format;}
	void SetFunction(const wxString& Function) {this->m_Function = Function;}
	void SetRatio(const int& Ratio) {this->m_Ratio = Ratio;}
	void SetSourceLocations(const wxArrayString& SourceLocations) {this->m_SourceLocations = SourceLocations;}
	void SetSourceLocation(const int& i, const wxString& SourceLocation) {this->m_SourceLocations.Item(i) = SourceLocation;}
	const wxString& GetFileLocation() const {return m_FileLocation;}
	const wxString& GetFormat() const {return m_Format;}
	const wxString& GetFunction() const {return m_Function;}
	const int& GetRatio() const {return m_Ratio;}
	const wxArrayString& GetSourceLocations() const {return m_SourceLocations;}
	const wxString& GetSourceLocation(const int& i) const {return m_SourceLocations.Item(i);}
	
	//Is a password needed?
	bool IsPassword;
	
private:
	wxString m_FileLocation;
	wxArrayString m_SourceLocations;
	wxString m_Function;
	wxString m_Format;
	int m_Ratio;
};

#endif
