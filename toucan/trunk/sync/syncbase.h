/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SYNCBASE
#define H_SYNCBASE

class SyncData;
class Rules;

#include <map>
#include <list>
#include <wx/string.h>

/*!
 * An abstract base class for all Sync operations. 
 */
class SyncBase{

public:
	/*!
	 * Constructor
	 */
	SyncBase();
	/*!
	 * Destructor
	 */
	virtual ~SyncBase();
protected:
	/*!
	 * Add the contents of a folder to a std::list in string form
	 */
	std::list<const wxString> FolderContentsToList(wxString path);
	/*!
	 * Merges a pair of std::list<const wxString>s into a single 
	 * std::map<const wxString, short> where the short indicates where the string 
	 * was originally located:
	 * \li 1 = sourcelist
	 * \li 2 = destlist
	 * \li 3 = both
	 */
	std::map<const wxString, short> MergeListsToMap(std::list<const wxString> sourcelist, std::list<const wxString> destlist);
	/*!
	 * Iterates through a std::map<const wxString, short> calling the appropriate 
	 * On[Not]Source[Not]Dest[File/Folder] function
	 */
	bool OperationCaller(std::map<const wxString, short> paths);

	/*!
	 * Called when we only have a source file
	 */
	virtual void OnSourceNotDestFile(const wxString &path) = 0;
	/*!
	 * Called when we only have a destination file
	 */
	virtual void OnNotSourceDestFile(const wxString &path) = 0;
	/*!
	 * Called when we have a source file and a destination file
	 */
	virtual void OnSourceAndDestFile(const wxString &path) = 0;
	/*!
	 * Called when we only have a source folder
	 */
	virtual void OnSourceNotDestFolder(const wxString &path) = 0;
	/*!
	 * Called when we only have a destination folder
	 */
	virtual void OnNotSourceDestFolder(const wxString &path) = 0;
	/*!
	 * Called when we have a source file and a destination folder
	 */
	virtual void OnSourceAndDestFolder(const wxString &path) = 0;

	/*!
	 * Indicates if we are in preview mode
	 */
	bool preview;
	/*!
	 * Indicates if streams should not be used in sync operations
	 */
	bool disablestreams;
	/*!
	 * The current source folder. This may differ from data->GetSource() as 
	 * GetSource() is not changed through a Sync job, it points to the true 
	 * root directory
	 */
	wxString sourceroot;
	/*!
	 * The current destination folder. This may differ from data->Dest() as 
	 * GetDest() is not changed through a Sync job, it points to the true 
	 * root directory
	 */
	wxString destroot;
	/*!
	 * Pointer to the SyncData we are using
	 */
	SyncData *data;
};

#endif
