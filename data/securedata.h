/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 - 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SECUREDATA
#define H_SECUREDATA

#include "jobdata.h"
#include <wx/string.h>
#include <wx/arrstr.h>

class SecureData : public JobData{

public:
	SecureData(const wxString &name) : JobData(name){
		;
	}

	void TransferToFile();
	void TransferFromFile();
	bool TransferToForm(frmMain *window);
	bool TransferFromForm(frmMain *window);
	
	void Output();

	void SetFunction(const wxString& Function) {this->m_Function = Function;}
	void SetPassword(const wxString& Password) {this->m_Password = Password;}
	void SetLocations(const wxArrayString& Locations) {this->m_Locations = Locations;}
	void SetLocation(const int& i, const wxString& Location) {this->m_Locations.Item(i) = Location;}

	const wxString& GetFunction() const {return m_Function;}
	const wxString& GetPassword() const {return m_Password;}
	const wxArrayString& GetLocations() const {return m_Locations;}
	const wxString& GetLocation(const int& i) const {return m_Locations.Item(i);}

private:
	wxArrayString m_Locations;
	wxString m_Function;
	wxString m_Password;
};

#endif
