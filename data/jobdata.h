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
	template<class T> T Read(const wxString& key){
		T temp;
		if(!wxGetApp().m_Jobs_Config->Read(GetName() + "/" +  key, &temp)){
			throw std::runtime_error(std::string("There was an error reading from the jobs file, looking for " + key));
		}
		return temp;
	}

	template<class T> void Write(const wxString& key, T value){
		if(!wxGetApp().m_Jobs_Config->Write(GetName() + "/" +  key, value)){
			throw std::runtime_error(std::string("There was an error writing to the jobs file"));
		}
	}

	wxArrayString Read(const wxString& key){
		wxString temp;
		if(!wxGetApp().m_Jobs_Config->Read(GetName() + "/" +  key, &temp)){
			throw std::runtime_error(std::string("There was an error reading from the jobs file, looking for " + key));
		}
		wxArrayString strings;
		wxStringTokenizer tkz(temp, "|", wxTOKEN_STRTOK);
		while(tkz.HasMoreTokens()){  
			strings.Add(tkz.GetNextToken());
		}
		return strings;
	}

	void Write(const wxString& key, wxArrayString value){
		wxString temp;
		for(unsigned int i = 0; i < value.GetCount(); i++){
			temp = temp + "|" + value.Item(i);
		}
		if(!wxGetApp().m_Jobs_Config->Write(GetName() + "/" +  key, temp)){
			throw std::runtime_error(std::string("There was an error writing to the jobs file"));
		}
	}


private:
	wxString m_Name;
	Rules *m_Rules;
};

#endif
