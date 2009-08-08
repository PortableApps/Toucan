/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_FILECOUNTER
#define H_FILECOUNTER

#include <wx/arrstr.h>

/*!
 * A general class for counting files. Paths are Normalised when they are added. 
 * Both files and folders can be added and will be counted correctly.
 */
class FileCounter{

public:
	/*!
	 * Constructs a FileCounter instance.
	 */
	FileCounter();
	/*!
	 * Adds a single path to the list of paths to be counted.
	 */
	void AddPath(const wxString &path);
	/*!
	 * Adds a set of paths to the list of paths to be counted.
	 */
	void AddPaths(const wxArrayString &paths);
	/*!
	 * Returns the count as last calculated by Count().
	 */
	long GetCount();
	/*!
	 * Counts the numer of files in the folders specified in the list
	 */
	bool Count();
	
private:
	bool CountFolder(wxString path);
	wxArrayString m_Paths;
	long m_Count;
};	

#endif
