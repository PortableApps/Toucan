/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 - 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_BACKUPDATA
#define H_BACKUPDATA

class wxTextFile;

#include "jobdata.h"
#include <wx/string.h>
#include <wx/arrstr.h>

/*!
 * A specialisation of the JobData class for use with a Backup job.
 */
class BackupData : public JobData{

public:
	/*! 
	 * Construct a new set of BackupData with the given name
	 */
	BackupData(const wxString &name) : JobData(name){
		;
	}

	bool TransferToFile();
	bool TransferFromFile();
	bool TransferToForm(frmMain *window);
	bool TransferFromForm(frmMain *window);

	bool IsReady();
	void Output();
	
	/*!
	 * Creates a 7-Zip compatible command to be passed to wxExecute.
	 * \param i The number of the location to create the command for
	 */
	wxString CreateCommand(int i);
	/*!
	 * Creates a 7-Zip compatible exclusions list.
	 * \param file The files to write the list into
	 * \param path The top level path to generate the list from
	 * \param length The length to be chopped off the start of the filenames
	 */
	bool CreateList(wxTextFile *file, wxString path, int length);

	/*!
	 * Sets the location of the backup archive
	 */
	void SetFileLocation(const wxString& FileLocation) {this->m_FileLocation = FileLocation;}
	/*!
	 * Sets the backup format, Zip or 7Zip
	 */
	void SetFormat(const wxString& Format) {this->m_Format = Format;}
	/*!
	 * Sets the backup function, currently Compete, Update, Differential or 
	 * Restore
	 */	
	void SetFunction(const wxString& Function) {this->m_Function = Function;}
	/*!
	 * Sets the backup compression level, between 1 and 5
	 */
	void SetRatio(const int& Ratio) {this->m_Ratio = Ratio;}
	/*!
	 * Sets the locations to be backed up
	 */
	void SetLocations(const wxArrayString& Locations) {this->m_Locations = Locations;}
	/*!
	 * Sets a specific location in the backup list
	 */
	void SetLocation(const int& i, const wxString& Location) {this->m_Locations.Item(i) = Location;}
	/*!
	 * Sets whether this job uses a password
	 */
	void SetUsesPassword(const bool& UsesPassword) {this->m_UsesPassword = UsesPassword;}
	/*!
	 * Sets the password for this job (not saved to disk)
	 */
	void SetPassword(const wxString& Password) {this->m_Password = Password;}

	/*!
	 * Gets the location of the archive
	 */
	const wxString& GetFileLocation() const {return m_FileLocation;}
	/*!
	 * Gets the format of the backup
	 */
	const wxString& GetFormat() const {return m_Format;}
	/*!
	 * Gets the backup function
	 */
	const wxString& GetFunction() const {return m_Function;}
	/*!
	 * Gets the compression ratio for this job
	 */
	const int& GetRatio() const {return m_Ratio;}
	/*!
	 * Gets the locations to be backed up
	 */
	const wxArrayString& GetLocations() const {return m_Locations;}
	/*!
	 * Gets a specific location from the list to be backed up
	 */
	const wxString& GetLocation(const int& i) const {return m_Locations.Item(i);}
	/*!
	 * Gets whether this job uses a password
	 */
	const bool& GetUsesPassword() const {return m_UsesPassword;}
	/*!
	 * Gets the password used by this job
	 */
	const wxString& GetPassword() const {return m_Password;}

private:
	wxString m_FileLocation;
	wxArrayString m_Locations;
	wxString m_Function;
	wxString m_Format;
	wxString m_Password;
	int m_Ratio;
	bool m_UsesPassword;
};

#endif
