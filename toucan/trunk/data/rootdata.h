/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 - 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_ROOTDATA
#define H_ROOTDATA

#include "../toucan.h"
#include <wx/string.h>

class Rules;

//This class serves as the basis for all of the data classes
class RootData{

public:
	RootData();
	virtual ~RootData();
	
	//Functions
	virtual bool TransferToFile() = 0;
	virtual bool TransferFromFile() = 0;
	virtual bool TransferToForm(frmMain* window) = 0;
	virtual bool TransferToForm(){
		return TransferToForm(wxGetApp().MainWindow);
	}
	virtual bool TransferFromForm() = 0;

	virtual void Output() = 0;
	virtual bool Execute(Rules rules) = 0;
	virtual bool NeedsPassword() = 0;
	virtual bool NeededFieldsFilled() = 0;

	//Setters / Getters
	void SetName(const wxString& Name) {this->m_Name = Name;}
	void SetPassword(const wxString& Password) {this->m_Password = Password;}
	void SetStartLength(const int& StartLength) {this->m_StartLength = StartLength;}
	const wxString& GetName() const {return m_Name;}
	const wxString& GetPassword() const {return m_Password;}
	const int& GetStartLength() const {return m_StartLength;}

private:
	wxString m_Name;
	wxString m_Password;
	int m_StartLength;
};

#endif

