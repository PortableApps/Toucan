/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_RULES
#define H_RULES

#include <wx/arrstr.h>

class Rules{

public:
	//Functions
	bool IsEmpty();
	bool TransferToFile(wxString strName);
	bool TransferFromFile(wxString strName);
	bool ShouldExclude(wxString strName, bool blIsDir); 
	
	void SetFilesToExclude(const wxArrayString& FilesToExclude) {this->m_FilesToExclude = FilesToExclude;}
	void SetFoldersToExclude(const wxArrayString& FoldersToExclude) {this->m_FoldersToExclude = FoldersToExclude;}
	void SetLocationsToInclude(const wxArrayString& LocationsToInclude) {this->m_LocationsToInclude = LocationsToInclude;}
	const wxArrayString& GetFilesToExclude() const {return m_FilesToExclude;}
	const wxArrayString& GetFoldersToExclude() const {return m_FoldersToExclude;}
	const wxArrayString& GetLocationsToInclude() const {return m_LocationsToInclude;}
	
	void Clear(){
		m_FoldersToExclude.Clear();
		m_LocationsToInclude.Clear();
		m_FilesToExclude.Clear();
	}
	
private:
	wxArrayString m_FoldersToExclude;
	wxArrayString m_LocationsToInclude;
	wxArrayString m_FilesToExclude;

};	

#endif
