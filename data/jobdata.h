/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 - 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_JOBDATA
#define H_JOBDATA

#include <stdexcept>
#include <wx/string.h>
#include <wx/tokenzr.h>
#include <wx/fileconf.h>
#include "../toucan.h"
#include "../basicfunctions.h"

class frmMain;
class Rules;

class JobData{
public:
	JobData(const wxString &name);
	virtual ~JobData();

	virtual void TransferToFile() = 0;
	virtual void TransferFromFile() = 0;
	virtual bool TransferToForm(frmMain* window) = 0;
	virtual bool TransferFromForm(frmMain* window) = 0;

	void SetName(const wxString& Name) {this->m_Name = Name;}
	void SetRules(Rules *Rules) {this->m_Rules = Rules;}
	const wxString& GetName() const {return m_Name;}
	Rules* GetRules() const {return m_Rules;}

protected:
	template<typename T> T Read(const wxString& key);
	template<typename T> void Write(const wxString& key, T value);

private:
	wxString m_Name;
	Rules *m_Rules;
};

#endif

