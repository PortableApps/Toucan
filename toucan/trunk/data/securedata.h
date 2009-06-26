/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SECUREDATA
#define H_SECUREDATA

#include <wx/string.h>
#include <wx/arrstr.h>
#include "rootdata.h"

class SecureData : public RootData{

public:
	bool TransferToFile();
	bool TransferFromFile();
	bool TransferToForm(frmMain *window);
	bool TransferFromForm();
	void Output();
	
	bool NeededFieldsFilled();
	bool Execute(Rules rules);
	bool NeedsPassword() { return true; }
	
	void SetFunction(const wxString& Function) {this->m_Function = Function;}
	void SetLocations(const wxArrayString& Locations) {this->m_Locations = Locations;}
	void SetLocation(const int& i, const wxString& Location) {this->m_Locations.Item(i) = Location;}
	const wxString& GetFunction() const {return m_Function;}
	const wxArrayString& GetLocations() const {return m_Locations;}
	const wxString& GetLocation(const int& i) const {return m_Locations.Item(i);}

private:
	wxArrayString m_Locations;
	wxString m_Function;
};

#endif
