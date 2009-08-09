/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SYNCDATA
#define H_SYNCDATA

class frmMain;

#include "../data/jobdata.h"
#include <wx/string.h>

/*!
 * A specialisation of the JobData class for use with a Sync job.
 */
class SyncData : public JobData{

public:
	/*! 
	 * Construct a new set of SyncData with the given name
	 */
	SyncData(const wxString &name) : JobData(name){
		;
	}

	bool TransferToFile(const wxString &path);
	bool TransferFromFile(const wxString &path);
	bool TransferToForm(frmMain *window);
	bool TransferFromForm(frmMain *window);
	
	void Output();
	bool IsReady();

	/*!
	 * Set whether file attributes will be copied, i.e. hidden or system 
	 * attributes
	 */
	void SetAttributes(const bool& Attributes) {this->m_Attributes = Attributes;}
	/*!
	 * Set the destination for the sync job
	 */
	void SetDest(const wxString& Dest) {this->m_Dest = Dest;}
	/*!
	 * Set the type of sync to perform, currently Copy, Update, Mirror, Clean, 
	 * Equalise
	 */
	void SetFunction(const wxString& Function) {this->m_Function = Function;}
	/*!
	 * Set whether to ignore changes due to daylight savings when comparing
	 * file times
	 */
	void SetIgnoreDLS(const bool& IgnoreDLS) {this->m_IgnoreDLS = IgnoreDLS;}
	/*! 
	 * Set if the job should ignore the read only flags on files
	 */
	void SetIgnoreRO(const bool& IgnoreRO) {this->m_IgnoreRO = IgnoreRO;}
	/*!
	 * Set the source of the sync job
	 */
	void SetSource(const wxString& Source) {this->m_Source = Source;}
	/*!
	 * Set whether to copy timestamps
	 */
	void SetTimeStamps(const bool& TimeStamps) {this->m_TimeStamps = TimeStamps;}
	/*!
	 * Get if the job is copying attributes
	 */
	const bool& GetAttributes() const {return m_Attributes;}
	/*!
	 * Get the desination for the job
	 */
	const wxString& GetDest() const {return m_Dest;}
	/*!
	 * Get the function used in this job
	 */
	const wxString& GetFunction() const {return m_Function;}
	/*!
	 * Get whether to ignore daylight savings changes
	 */
	const bool& GetIgnoreDLS() const {return m_IgnoreDLS;}
	/*!
	 * Get whether to ignore read only flags
	 */
	const bool& GetIgnoreRO() const {return m_IgnoreRO;}
	/*!
	 * Get the source for the job
	 */
	const wxString& GetSource() const {return m_Source;}
	/*!
	 * Get if the job should copy timestamps
	 */
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

