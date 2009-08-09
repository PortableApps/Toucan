/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_RULES
#define H_RULES

class frmMain;

#include <wx/arrstr.h>

/*!
 * A class that describes a set of rules to be used in any operation in Toucan.
 */
class Rules{

public:

	/*
	 * Construct a set of Rules with the given name and optionally load it from 
	 * the rules file
	 */
	Rules(const wxString &name, bool loadfromfile = false);

	/*!
	 * Transfers the rules to the rules file, using GetName() as the group
	 * name
	 * \return Returns true is the rules could be written, false otherwise
	 */
	bool TransferToFile();
	/*!
	 * Transfers the rules from the rules file, using GetName() as the group
	 * name
	 * \return Returns true if the rules could be read, false otherwise
	 */
	bool TransferFromFile();
	/*!
	 * Transfers the rules to the main window
	 * \return Returns true is the rules could be copied to the form, false 
	 * otherwise
	 */
	bool TransferToForm(frmMain *window);
	/*!
	 * Transfers the data from the main window
	 * \return Returns true is the rules could be read from the form, false 
	 * otherwise
	 */
	bool TransferFromForm(frmMain *window);

	/*!
	 * \return Returns true if there are no rules specified, false otherwise
	 */
	bool IsEmpty();
	/*!
	 * Clears the Rules
	 */
	void Clear(){
		m_ExcludedFiles.Clear();
		m_ExcludedFolders.Clear();
		m_IncludedLocations.Clear();
	}

	/*!
	 * \param name The path to compare against the rules
	 * \param directory Is the path a directory?
	 * \return Returns true if the specifed file / folder should be excluded, 
	 * false otherwise
	 */
	bool ShouldExclude(wxString path, bool directory); 
	
	/*!
	 * Sets the name of the Rules
	 */
	void SetName(const wxString& Name) {this->m_Name = Name;}
	/*!
	 * Sets Rules governing file exclusions, this supports dates prefixed with a 
	 * less than or greater than sign, files sizes prefixed with a less than or 
	 * greater than sign and suffixed with B, kB, MB or GB, a regex prefixed 
	 * with an asterisk or plain text
	 */	
	void SetExcludedFiles(const wxArrayString& FilesToExclude) {this->m_ExcludedFiles = FilesToExclude;}
	/*!
	 * Sets Rules governing folder exclusions, this supports a regex prefixed 
	 * with an asterisk or plain text
	 */	
	void SetExcludedFolders(const wxArrayString& FoldersToExclude) {this->m_ExcludedFolders = FoldersToExclude;}
	/*!
	 * Sets Rules governing location inclusions, this supports a regex prefixed 
	 * with an asterisk or plain text
	 */	
	void SetIncludedLocations(const wxArrayString& LocationsToInclude) {this->m_IncludedLocations = LocationsToInclude;}

	/*!
	 * Gets the name of the Rules
	 */
	const wxString& GetName() const {return m_Name;}
	/*!
	 * Gets the Rules governing file exclusion
	 */
	const wxArrayString& GetExcludedFiles() const {return m_ExcludedFiles;}
	/*!
	 * Gets the Rules governing folder exclusion
	 */
	const wxArrayString& GetExcludedFolders() const {return m_ExcludedFolders;}
	/*!
	 * Gets the Rules governing location inclusions
	 */
	const wxArrayString& GetIncludedLocations() const {return m_IncludedLocations;}

private:
	wxArrayString m_ExcludedFiles;
	wxArrayString m_ExcludedFolders;
	wxArrayString m_IncludedLocations;
	wxString m_Name;
};	

#endif
