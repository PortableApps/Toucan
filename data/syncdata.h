/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SYNCDATA
#define H_SYNCDATA

class frmMain;

#include "jobdata.h"
#include <wx/string.h>

class SyncData : public JobData{

public:
	SyncData(const wxString &name) : JobData(name){
		;
	}

	bool TransferToFile();
	bool TransferFromFile();
	bool TransferToForm(frmMain *window);
	bool TransferFromForm(frmMain *window);

	bool IsReady();

	void SetSource(const wxString& Source) {this->m_Source = Source;}
	void SetDest(const wxString& Dest) {this->m_Dest = Dest;}
	void SetFunction(const wxString& Function) {this->m_Function = Function;}
	void SetCheckSize(const bool& CheckSize) {this->m_CheckSize = CheckSize;}
	void SetCheckTime(const bool& CheckTime) {this->m_CheckTime = CheckTime;}
	void SetCheckShort(const bool& CheckShort) {this->m_CheckShort = CheckShort;}
	void SetCheckFull(const bool& CheckFull) {this->m_CheckFull = CheckFull;}
	void SetIgnoreDLS(const bool& IgnoreDLS) {this->m_IgnoreDLS = IgnoreDLS;}
	void SetIgnoreRO(const bool& IgnoreRO) {this->m_IgnoreRO = IgnoreRO;}
	void SetTimeStamps(const bool& TimeStamps) {this->m_TimeStamps = TimeStamps;}
	void SetAttributes(const bool& Attributes) {this->m_Attributes = Attributes;}
	void SetRecycle(const bool& Recycle) {this->m_Recycle = Recycle;}

	const wxString& GetSource() const {return m_Source;}
	const wxString& GetDest() const {return m_Dest;}
	const wxString& GetFunction() const {return m_Function;}
	const bool& GetCheckSize() const {return m_CheckSize;}
	const bool& GetCheckTime() const {return m_CheckTime;}
	const bool& GetCheckShort() const {return m_CheckShort;}
	const bool& GetCheckFull() const {return m_CheckFull;}
	const bool& GetIgnoreDLS() const {return m_IgnoreDLS;}
	const bool& GetIgnoreRO() const {return m_IgnoreRO;}
	const bool& GetTimeStamps() const {return m_TimeStamps;}
	const bool& GetAttributes() const {return m_Attributes;}
	const bool& GetRecycle() const {return m_Recycle;}

private:
	wxString m_Source;
	wxString m_Dest;
	wxString m_Function;
	wxString m_PreText;
	bool m_CheckSize;
	bool m_CheckTime;
	bool m_CheckShort;
	bool m_CheckFull;
	bool m_TimeStamps;
	bool m_Attributes;
	bool m_IgnoreRO;
	bool m_IgnoreDLS;
	bool m_Recycle;
};

#endif
