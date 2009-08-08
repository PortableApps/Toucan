/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 - 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_JOBDATA
#define H_JOBDATA

#include <wx/string.h>

class frmMain;
class Rules;

class JobData{

public:
	/*!
	 * Constructs a new job with the given name
	 */
	JobData(const wxString &name);
	
	/*!
	 * Cleans up job data, including destroying the associated set of Rules 
	 */
	virtual ~JobData();

	/*!
	 * Transfers the data to the specificed path, using GetName() as a group
	 * seperator
	 */
	virtual bool TransferToFile(const wxString &path) = 0;
	
	/*!
	 * Transfers the data from the specificed path
	 */
	virtual bool TransferFromFile(const wxString &path) = 0;

	/*!
	 * Transfers the data to the main window
	 */
	virtual bool TransferToForm(frmMain* window) = 0;
	
	/*!
	 * Transfers the data from the main window
	 */
	virtual bool TransferFromForm(frmMain* window) = 0;

	/*!
	 * Output all of the information related to the job, this is intended for 
	 * debugging
	 */
	virtual void Output() = 0;

	/*!
	 * Checks to see if the job is ready to be used, i.e. all of the information
	 * needed is present
	 */
	virtual bool NeededFieldsFilled() = 0;

	/*!
	 * Set the job name
	 */
	void SetName(const wxString& Name) {this->m_Name = Name;}
	
	/*!
	 * Set the Rules to be used in this job, the job takes over control of the 
	 * rules and destoys them when it has finished.
	 */
	void SetRules(Rules *Rules) {this->m_Rules = Rules;}
	
	/*!
	 * Get the job name
	 */
	const wxString& GetName() const {return m_Name;}
	
	/*!
	 * Get the set of Rules to be used in this job, do not destoy them as they 
	 * will be destroyed when the job is finished.
	 */
	Rules* GetRules() const {return m_Rules;}

private:
	wxString m_Name;
	Rules *m_Rules;
};

#endif

