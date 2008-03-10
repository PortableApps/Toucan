/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2007 Steven Lamerton
// Licence:     GNU GPL 2 (See readme for more info
/////////////////////////////////////////////////////////////////////////////////

#include "basicfunctions.h"
#include "frmprogress.h"

#include <wx/dir.h>
#include <wx/textfile.h>

bool PreviewSyncLoop(wxString strPath, wxVirtualDirTreeCtrl *window, Rules rules);

class PreviewSyncThread : public wxThread
{
public:
	//Constructor
	PreviewSyncThread(SyncData data, Rules rules, frmMain *main){
		m_Data = data;
		m_Rules = rules;
		m_Main = main;
	}
	//Main function, all stuff goes in here
	virtual void *Entry();

private:
	SyncData m_Data;
	Rules m_Rules;
	frmMain* m_Main;
};

void *PreviewSyncThread::Entry(){
	//Launch the correct set of loops
	if(m_Data.GetFunction() == _("Copy") || m_Data.GetFunction() == _("Update")){
		//m_Main->m_Sync_Dest_Tree->DeleteAllItems();
		//wxTreeItemId parent = m_Main->m_Sync_Dest_Tree->AppendItem(m_Main->m_Sync_Dest_Tree->GetRootItem(), m_Data.GetDest().AfterLast(wxFILE_SEP_PATH));
		//PreviewSyncLoop(m_Data, m_Rules, m_Main->m_Sync_Source_Tree, m_Main->m_Sync_Dest_Tree,m_Main->m_Sync_Source_Tree->GetRootItem(),m_Main->m_Sync_Dest_Tree->GetRootItem(), 0);
		//m_Main->m_Sync_Dest_Tree->Refresh();
		//PreviewExistingList(m_Main->m_Sync_Dest_Tree);
		//PreviewCheckAll(m_Data.GetSource() ,m_Data);
		//m_Main->m_Sync_Dest_Tree->DeleteAllItems();
		//m_Main->m_Sync_Dest_Tree->AppendItem(m_Main->m_Sync_Dest_Tree->GetRootItem(), m_Data.GetDest());
		//PreviewReAdd(m_Data, m_Main->m_Sync_Dest_Tree, m_Data.GetDest().Length());
		//wxMessageBox(_("Done"));
		PreviewSyncLoop(m_Data.GetSource(), m_Main->m_Sync_Dest_Tree, m_Rules);
	}/*
	else if(m_Data.GetFunction() ==  _("Mirror (Copy)")){
		m_Data.SetFunction(_("Copy"));
		//PreviewSyncLoop(m_Data, m_Rules, m_Window, parent);
		//Swap the source and dest around for the mirror routine
		wxString strTemp = m_Data.GetSource();
		m_Data.SetSource(m_Data.GetDest());
		m_Data.SetDest(strTemp);
		//Run the mirror loop
		m_Data.SetFunction(_("Mirror"));
		//PreviewSyncLoop(m_Data, m_Rules, m_Window, parent);
	}
	else if(m_Data.GetFunction() ==  _("Mirror (Copy)")){
		m_Data.SetFunction(_("Update"));
		//PreviewSyncLoop(m_Data, m_Rules, m_Window, parent);
		//Swap the source and dest around for the mirror routine
		wxString strTemp = m_Data.GetSource();
		m_Data.SetSource(m_Data.GetDest());
		m_Data.SetDest(strTemp);
		//Run the mirror loop
		m_Data.SetFunction(_("Mirror"));
		//PreviewSyncLoop(m_Data, m_Rules, m_Window, parent);
	}
	else if(m_Data.GetFunction() ==  _("Equalise")){
		m_Data.SetFunction(_("Update"));
		//PreviewSyncLoop(m_Data, m_Rules, m_Window, parent);
		wxString strTemp = m_Data.GetSource();
		m_Data.SetSource(m_Data.GetDest());
		m_Data.SetDest(strTemp);
		m_Data.SetFunction(_("Update"));
		//PreviewSyncLoop(m_Data, m_Rules, m_Window, parent);
	}*/
	return NULL;

}

bool PreviewSyncLoop(wxString strPath, wxVirtualDirTreeCtrl *window, Rules rules){
	if (strPath[strPath.length()-1] != wxFILE_SEP_PATH) {
		strPath += wxFILE_SEP_PATH;       
	}
	wxDir dir(strPath);
	wxString strFilename;
	bool blDir = dir.GetFirst(&strFilename);
	//If the path is ok
	if(blDir){
		//Loop through all of the files and folders in the directory
		do {
			//If it is a directory
			if(wxDirExists(strPath + strFilename))
			{
				//It the folder is not excluded then add it to the file
				if(!rules.ShouldExclude(strPath + strFilename, true)){
					//file->AddLne(strPath + strFilename);
					window->AddFileItem(strPath + strFilename);
				}
				//Always call the function again to ensure that ALL files and folders are processed
				PreviewSyncLoop(strPath + strFilename, window, rules);
			}
			//If it is a file
			else{
				if(!rules.ShouldExclude(strPath + strFilename, true)){
					//file->AddLine(strPath + strFilename);
				}
			}
		}
		while (dir.GetNext(&strFilename) );
	}  
	return true;
}
