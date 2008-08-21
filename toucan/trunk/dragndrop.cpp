/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "dragndrop.h"
#include "basicfunctions.h"

bool DnDFileTree::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& arrFilenames){
	//Loop through all of the paths adding each one to the wxVirtualDirTreeCtrl		
	for(unsigned int i = 0; i < arrFilenames.GetCount(); i++){
		m_Tree->AddNewPath(arrFilenames.Item(i));
	}
	return true;
}

bool DnDFileTreeText::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& arrFilenames){
	//Only add the first item as Sync only supports one to one syncing		
	m_Text->SetValue(arrFilenames.Item(0));
	m_Tree->DeleteAllItems();
	m_Tree->AddRoot(wxT("Hidden root"));
	m_Tree->AddNewPath(arrFilenames.Item(0));
	//Return a message to the user if they have tried to add more than one item
	if(arrFilenames.GetCount() < 1){
		//MessageBox(_("Only the first item you dragged was added"), _("Warning"));
	}
	return true;
}
