/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "dragndrop.h"
#include "toucan.h"
#include "forms/frmmain.h"

bool DnDFileTree::OnDropFiles(wxCoord WXUNUSED(x), wxCoord WXUNUSED(y), const wxArrayString& arrFilenames){
	//Loop through all of the paths adding each one to the wxVirtualDirTreeCtrl		
	for(unsigned int i = 0; i < arrFilenames.GetCount(); i++){
		m_Tree->AddNewPath(arrFilenames.Item(i));
		if(m_Tree->GetId() == ID_BACKUP_TREECTRL){
			wxGetApp().MainWindow->m_BackupLocations->Add(arrFilenames.Item(i));
		}
		else if(m_Tree->GetId() == ID_SECURE_TREECTRL){
			wxGetApp().MainWindow->m_SecureLocations->Add(arrFilenames.Item(i));
		}
	}

	return true;
}

bool DnDFileTreeText::OnDropFiles(wxCoord WXUNUSED(x), wxCoord WXUNUSED(y), const wxArrayString& arrFilenames){
	//Only add the first item as Sync only supports one to one syncing		
	m_Text->SetValue(arrFilenames.Item(0));
	m_Tree->DeleteAllItems();
	m_Tree->AddRoot(wxT("Hidden root"));
	m_Tree->AddNewPath(arrFilenames.Item(0));
	return true;
}
