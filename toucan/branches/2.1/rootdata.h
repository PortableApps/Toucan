/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_ROOTDATA
#define H_ROOTDATA

#include <wx/string.h>
#include "frmmain.h"

//This class serves as the basis for all of the data classes

class RootData{

public:
	
	RootData();
	virtual ~RootData();
	
	//Functions
	virtual bool TransferToFile() = 0;
	virtual bool TransferFromFile() = 0;
	virtual bool TransferToForm() = 0;
	virtual bool TransferFromForm() = 0;
	
	virtual void Output() = 0;
	
	virtual bool Execute(Rules rules) = 0;
	
	virtual bool NeedsPassword() = 0;
	
	virtual bool NeededFieldsFilled() = 0;
	
	void SetName(wxString name) { m_Name = name; }
	wxString GetName(){ return m_Name; }
	
	void SetPassword(wxString password) { m_Password = password; }
	wxString GetPassword(){ return m_Password; }
	
private:
	
	wxString m_Name;
	wxString m_Password;
	
};

#endif

