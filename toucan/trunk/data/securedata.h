/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SECUREDATA
#define H_SECUREDATA

#include "jobdata.h"
#include <wx/string.h>
#include <wx/arrstr.h>

/*!
 * A specialisation of the JobData class for use with a Secure job.
 */
class SecureData : public JobData{

public:
	/*! 
	 * Construct a new set of SecureData with the given name
	 */
	SecureData(const wxString &name) : JobData(name){
		;
	}

	bool TransferToFile();
	bool TransferFromFile();
	bool TransferToForm(frmMain *window);
	bool TransferFromForm(frmMain *window);
	
	void Output();
	bool IsReady();
	
	/*! 
	 * Sets the secure function, Encrypt or Decrypt
	 */
	void SetFunction(const wxString& Function) {this->m_Function = Function;}
	/*! 
	 * Sets the password for this job (not saved to disk)
	 */
	void SetPassword(const wxString& Password) {this->m_Password = Password;}
	/*!
	 * Sets the locations to be secured
	 */
	void SetLocations(const wxArrayString& Locations) {this->m_Locations = Locations;}
	/*!
	 * Sets a specific location in the secure list
	 */
	void SetLocation(const int& i, const wxString& Location) {this->m_Locations.Item(i) = Location;}
	/*! 
	 * Gets the secure function
	 */
	const wxString& GetFunction() const {return m_Function;}
	/*! 
	 * Gets the password for this job
	 */
	const wxString& GetPassword() const {return m_Password;}
	/*! 
	 * Gets the locations to be secured
	 */
	const wxArrayString& GetLocations() const {return m_Locations;}
	/*! 
	 * Gets a specific location on the secure list
	 */
	const wxString& GetLocation(const int& i) const {return m_Locations.Item(i);}

private:
	wxArrayString m_Locations;
	wxString m_Function;
	wxString m_Password;
};

#endif
