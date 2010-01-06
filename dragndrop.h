/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_DRAGNDROP
#define H_DRAGNDROP

class DirCtrl;
class wxTextCtrl;

#include <wx/dnd.h>

class DnDFileTree : public wxFileDropTarget
{
public:
	DnDFileTree(DirCtrl *tree) {
		m_Tree = tree;
	}

	bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& arrFilenames);

private:
	DirCtrl *m_Tree;
};

class DnDFileTreeText : public wxFileDropTarget
{
public:
	DnDFileTreeText(wxTextCtrl *text, DirCtrl *tree) {
		m_Text = text; 
		m_Tree = tree;
	}

	bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& arrFilenames);
	
private:
	wxTextCtrl *m_Text;
	DirCtrl *m_Tree;
};

#endif
