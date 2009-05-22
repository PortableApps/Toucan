/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SYNCDATA
#define H_SYNCDATA

#include "../forms/frmmain.h"
#include "../data/rootdata.h"
#include <wx/string.h>

class SyncData : public RootData{

public:
	bool TransferToFile();
	bool TransferFromFile();
	bool TransferToForm();
	bool TransferFromForm();
	
	void Output();
	bool Execute(Rules rules);
	bool NeedsPassword() { return false; }
	bool NeededFieldsFilled();

	//Setters / Getters
	void SetAttributes(const bool& Attributes) {this->m_Attributes = Attributes;}
	void SetDest(const wxString& Dest) {this->m_Dest = Dest;}
	void SetFunction(const wxString& Function) {this->m_Function = Function;}
	void SetIgnoreDLS(const bool& IgnoreDLS) {this->m_IgnoreDLS = IgnoreDLS;}
	void SetIgnoreRO(const bool& IgnoreRO) {this->m_IgnoreRO = IgnoreRO;}
	void SetPreText(const wxString& PreText) {this->m_PreText = PreText;}
	void SetSource(const wxString& Source) {this->m_Source = Source;}
	void SetTimeStamps(const bool& TimeStamps) {this->m_TimeStamps = TimeStamps;}
	const bool& GetAttributes() const {return m_Attributes;}
	const wxString& GetDest() const {return m_Dest;}
	const wxString& GetFunction() const {return m_Function;}
	const bool& GetIgnoreDLS() const {return m_IgnoreDLS;}
	const bool& GetIgnoreRO() const {return m_IgnoreRO;}
	const wxString& GetPreText() const {return m_PreText;}
	const wxString& GetSource() const {return m_Source;}
	const bool& GetTimeStamps() const {return m_TimeStamps;}

private:
	wxString m_Source;
	wxString m_Dest;
	wxString m_Function;
	wxString m_PreText;
	bool m_TimeStamps;
	bool m_Attributes;
	bool m_IgnoreRO;
	bool m_IgnoreDLS;
};

#endif

