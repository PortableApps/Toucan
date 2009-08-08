/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_DRAGNDROP
#define H_DRAGNDROP

class wxVirtualDirTreeCtrl;
class wxTextCtrl;

#include <wx/dnd.h>

/*!
 * The file drop target for a wxVirtualDirTreeCtrl, multiple files are 
 * supported. This is used in Backup and Secure.
 */
class DnDFileTree : public wxFileDropTarget
{
public:
	/*!
	 * Constructs the drop target, still needs to be set with 
	 * wxWindow->SetDropTarget
	 */
	DnDFileTree(wxVirtualDirTreeCtrl *tree) {
		m_Tree = tree;
	}

	/*!
	 * Called when a file is dropped onto the wxVirtualDirTreeCtrl
	 */
	bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& arrFilenames);
	
private:
	wxVirtualDirTreeCtrl *m_Tree;
};

/*!
 * The file drop target for a wxVirtualDirTreeCtrl and wxTextCtrl as used in 
 * Sync. Only a single folder is supported, others are discarded.
 */
class DnDFileTreeText : public wxFileDropTarget
{
public:
	/*!
	 * Constructs the drop target, still needs to be set with 
	 * wxWindow->SetDropTarget
	 */
	DnDFileTreeText(wxTextCtrl *text, wxVirtualDirTreeCtrl *tree) {
		m_Text = text; 
		m_Tree = tree;
	}
	
	/*!
	 * Called when a file is dropped onto the wxVirtualDirTreeCtrl or the wxTextCtrl
	 */
	bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& arrFilenames);
	
private:
	wxTextCtrl *m_Text;
	wxVirtualDirTreeCtrl *m_Tree;
};

#endif
