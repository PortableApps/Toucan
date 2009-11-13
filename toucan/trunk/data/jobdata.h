/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 - 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_JOBDATA
#define H_JOBDATA

#include <wx/string.h>
#include "../toucan.h"
//We include this to get access to ToEn and ToLang
#include "../basicfunctions.h"

class frmMain;
class Rules;

/*!
 * JobData is the root for all of the Toucan data classes and if used to supply 
 * information to the Job classes. It must be inherited to be used correctly.
 */
class JobData{

public:
	/*!
	 * Constructs a new set of job data with the given name
	 */
	JobData(const wxString &name);
	
	/*!
	 * Cleans up job data, including destroying the associated set of Rules 
	 */
	virtual ~JobData();

	/*!
	 * Transfers the data to the jobs file, using GetName() as the group
	 * name
	 * \return Returns true is the data could be written, false otherwise
	 */
	virtual bool TransferToFile() = 0;
	/*!
	 * Transfers the data from the jobs file, using GetName() as the group
	 * name
	 * \return Returns true if the data could be read, false otherwise
	 */
	virtual bool TransferFromFile() = 0;
	/*!
	 * Transfers the data to the main window
	 * \return Returns true is the data could be copied to the form, false 
	 * otherwise
	 */
	virtual bool TransferToForm(frmMain* window) = 0;
	/*!
	 * Transfers the data from the main window. Use IsReady() to check that the 
	 * data is ready to be used in a job.
	 * \return Returns true is the data could be read from the form, false 
	 * otherwise
	 */
	virtual bool TransferFromForm(frmMain* window) = 0;

	/*!
	 * Checks to see if the job is ready to be used, i.e. all of the information
	 * needed is present
	 */
	virtual bool IsReady() = 0;

	/*!
	 * Sets the job name
	 */
	void SetName(const wxString& Name) {this->m_Name = Name;}
	/*!
	 * Sets the Rules to be used in this job, the job takes over control of the 
	 * rules and destoys them when it has finished.
	 */
	void SetRules(Rules *Rules) {this->m_Rules = Rules;}
	/*!
	 * Gets the job name
	 */
	const wxString& GetName() const {return m_Name;}
	/*!
	 * Gets the set of Rules to be used in this job, do not destoy them as they 
	 * will be destroyed when the job is finished.
	 */
	Rules* GetRules() const {return m_Rules;}

private:
	wxString m_Name;
	Rules *m_Rules;
};

#endif

