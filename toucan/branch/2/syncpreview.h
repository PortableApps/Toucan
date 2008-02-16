/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2007 Steven Lamerton
// Licence:     GNU GPL 2 (See readme for more info
/////////////////////////////////////////////////////////////////////////////////

#include "basicfunctions.h"
#include "frmprogress.h"
#include <wx/dir.h>


bool PreviewSyncLoop(SyncData data, Rules rules, wxTreeCtrl * treeSource, wxTreeCtrl *treeDest, wxTreeItemId idSourceParent, wxTreeItemId idDestParent, wxTreeItemIdValue cookie);
bool PreviewSyncFile(SyncData data, Rules rules, wxTreeCtrl * treeSource, wxTreeCtrl *treeDest, wxTreeItemId idSourceParent, wxTreeItemId idDestParent);


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
		PreviewSyncLoop(m_Data, m_Rules, m_Main->m_Sync_Source_Tree, m_Main->m_Sync_Dest_Tree,m_Main->m_Sync_Source_Tree->GetRootItem(),m_Main->m_Sync_Dest_Tree->GetRootItem(), 0);
		m_Main->m_Sync_Dest_Tree->Refresh();
	}
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
	}
	/*wxMutexGuiEnter();
	m_Main->m_Sync_Source_Tree->DeleteAllItems();
	m_Main->m_Sync_Dest_Tree->DeleteAllItems();
	AddDirToTree(m_Main->m_Sync_Source_Txt->GetValue(), m_Main->m_Sync_Source_Tree);
	AddDirToTree(m_Main->m_Sync_Dest_Txt->GetValue(), m_Main->m_Sync_Dest_Tree);
	m_Window->m_OK->Enable(true);
	m_Window->m_Save->Enable(true);
	m_Window->m_Cancel->Enable(false);
	wxMutexGuiLeave();
	OutputProgress(_("Finished..."), m_Window);*/
	
	return NULL;

}

bool PreviewSyncLoop(SyncData data, Rules rules, wxTreeCtrl * treeSource, wxTreeCtrl *treeDest, wxTreeItemId idSource, wxTreeItemId idDest, wxTreeItemIdValue cookie)
{
	wxMessageBox(treeDest->GetItemText(idDest), _("ID Dest"));
	wxTreeItemId tempidSource, tempidDest;
    if (!cookie){
        tempidSource = treeSource->GetFirstChild(idSource, cookie);
	}
    else{
        tempidSource = treeSource->GetNextChild(idSource, cookie);
	}
    if (!tempidSource.IsOk()){
        return false;
	}
	//wxMessageBox(treeSource->GetItemText(tempidSource), _("Source text"));
    if (treeSource->ItemHasChildren(tempidSource)){
		//wxMessageBox(_("Has children"));
		bool blExists = false;
		wxTreeItemIdValue destcookie;
		tempidDest = treeDest->GetFirstChild(idDest, destcookie);
		//wxMessageBox(treeDest->GetItemText(tempidDest), _("Dest loop text"));
		if(treeSource->GetItemText(tempidSource) == treeDest->GetItemText(tempidDest)){
			//There is already a directory in the destination
			blExists = true;
			wxMessageBox(treeDest->GetItemText(tempidDest), _("Calling with"));
			PreviewSyncLoop(data, rules, treeSource, treeDest, tempidSource, tempidDest, 0);
		}
		else{
			do{
				tempidDest = treeDest->GetNextChild(idDest, destcookie);
				//wxMessageBox(treeDest->GetItemText(tempidDest), _("Dest loop text"));
				if(treeSource->GetItemText(tempidSource) == treeDest->GetItemText(tempidDest)){
					//There is already a directory in the destination
					blExists = true;
					wxMessageBox(treeDest->GetItemText(tempidDest), _("Calling with"));
					PreviewSyncLoop(data, rules, treeSource, treeDest, tempidSource, tempidDest, 0);
					break;
				}
			}while(tempidDest.IsOk());
		}
		if(!blExists){
			//wxMessageBox(_("Appending folder"));
			//Need to add a folder to the dest tree and call the loop again
			tempidDest = treeDest->AppendItem(idDest, treeSource->GetItemText(tempidSource));
			PreviewSyncLoop(data, rules, treeSource, treeDest, tempidSource, idDest, 0);			
		}
	}
	else{
		//It is a file
		//Preview sync file
		PreviewSyncFile(data, rules, treeSource, treeDest, tempidSource, idDest);
	}
	PreviewSyncLoop(data, rules, treeSource, treeDest, idSource, idDest, cookie);	
}

bool PreviewSyncFile(SyncData data, Rules rules, wxTreeCtrl * treeSource, wxTreeCtrl *treeDest, wxTreeItemId idSource, wxTreeItemId idDest)
{
	wxMessageBox(treeDest->GetItemText(idDest), _("appending too"));
	treeDest->AppendItem(idDest, treeSource->GetItemText(idSource));
	//wxMessageBox(_("At file"));
}
