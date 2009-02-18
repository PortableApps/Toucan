/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_DRAGNDROP
#define H_DRAGNDROP

#include "basicfunctions.h"
#include "virtualdirtreectrl.h"
#include <wx/dnd.h>


/*File droptarget to a wxVirtualDirTreeCtrl, multiple files allowed*/
class DnDFileTree : public wxFileDropTarget
{
public:
	DnDFileTree(wxVirtualDirTreeCtrl *tree) {
		m_Tree = tree;
	}

	bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& arrFilenames);
	
private:
	wxVirtualDirTreeCtrl *m_Tree;
};

/*File droptarget to a wxTextCtrl and a wxVirtualDirTreeCtrl, only used in Sync*/
class DnDFileTreeText : public wxFileDropTarget
{
public:
	DnDFileTreeText(wxTextCtrl *text, wxVirtualDirTreeCtrl *tree) {
		m_Text = text; 
		m_Tree = tree;
	}
	
	bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& arrFilenames);
	
private:
	wxTextCtrl *m_Text;
	wxVirtualDirTreeCtrl *m_Tree;
};

#endif
