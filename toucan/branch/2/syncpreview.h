/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2007 Steven Lamerton
// Licence:     GNU GPL 2 (See readme for more info
/////////////////////////////////////////////////////////////////////////////////

#include "basicfunctions.h"
#include "frmprogress.h"

#include <wx/dir.h>
#include <wx/textfile.h>

bool PreviewExistingList(wxTreeCtrl *treeSource);
bool PreviewExistingListLoop(wxTextFile *file, wxTreeCtrl *treeSource,  wxTreeItemId idParent, wxTreeItemIdValue cookie);
bool PreviewCheckAll(wxString strPath, SyncData data);
bool PreviewCheckAllLoop(wxString strPath, wxTextFile *file, SyncData data);
bool PreviewCompareFiles(wxString strA, wxString strB, SyncData data);
bool PreviewReAdd(SyncData data, wxTreeCtrl *treeDest, int iLength);

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
		m_Main->m_Sync_Dest_Tree->Refresh();
		PreviewExistingList(m_Main->m_Sync_Dest_Tree);
		PreviewCheckAll(m_Data.GetSource() ,m_Data);
		m_Main->m_Sync_Dest_Tree->DeleteAllItems();
		m_Main->m_Sync_Dest_Tree->AppendItem(m_Main->m_Sync_Dest_Tree->GetRootItem(), m_Data.GetDest());
		PreviewReAdd(m_Data, m_Main->m_Sync_Dest_Tree, m_Data.GetDest().Length());
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
	return NULL;

}

bool PreviewExistingList(wxTreeCtrl *treeSource){
	wxMessageBox(_("Launching loop"));
	wxArrayString arrList;
	wxTextFile *file = new wxTextFile(_("C:\\test.txt"));
	file->Create();
	PreviewExistingListLoop(file, treeSource, treeSource->GetRootItem(), 0);
	file->Write();
	file->Close();
	delete file;
	return true;
}

bool PreviewExistingListLoop(wxTextFile *file, wxTreeCtrl *treeSource,  wxTreeItemId idParent, wxTreeItemIdValue cookie){
	wxTreeItemId tempidParent, loopidParent;

    if (!cookie){
        tempidParent = treeSource->GetFirstChild(idParent, cookie);
	}
    else{
        tempidParent = treeSource->GetNextChild(idParent, cookie);
	}
    if (!tempidParent.IsOk())
        return true;
	
	wxString strPath = treeSource->GetItemText(tempidParent);
	loopidParent = tempidParent;
	while(treeSource->GetItemParent(loopidParent) != treeSource->GetRootItem()){
		loopidParent = treeSource->GetItemParent(loopidParent);
		strPath = treeSource->GetItemText(loopidParent) + wxFILE_SEP_PATH + strPath;
	}
	strPath = treeSource->GetItemText(treeSource->GetRootItem()) + wxFILE_SEP_PATH + strPath;
	file->AddLine(strPath);

    if (treeSource->ItemHasChildren(tempidParent)){
        PreviewExistingListLoop(file, treeSource, tempidParent, 0);
	}

    PreviewExistingListLoop(file, treeSource, idParent, cookie);
	return true;
}

bool PreviewCheckAll(wxString strPath, SyncData data){
	wxTextFile *file = new wxTextFile(_("C:\\test.txt"));
	file->Open();
	PreviewCheckAllLoop(strPath, file, data);
	file->Write();
	file->Close();
	delete file;
	
}

bool PreviewCheckAllLoop(wxString strPath, wxTextFile *file, SyncData data){
	if (strPath[strPath.length()-1] != wxFILE_SEP_PATH) {
		strPath += wxFILE_SEP_PATH;       
	}
	wxDir dir(strPath);
	wxString strFilename;
	bool blDir = dir.GetFirst(&strFilename);
	if(blDir){
		do {
			if(wxDirExists(strPath + strFilename))
			{
				PreviewCheckAllLoop(strPath + strFilename, file, data);
			}
			else{
				for(unsigned int i = 0; i < file->GetLineCount(); i++){
					if(strPath + strFilename == file->GetLine(i)){
						if(PreviewCompareFiles(strPath + strFilename, file->GetLine(i), data)){
							wxMessageBox(_("appending"));
							wxString strTemp = file->GetLine(i);
							wxMessageBox(strTemp);
							file->RemoveLine(i);
							file->AddLine(strTemp + wxT("#"));
							//wxMessageBox(arrList.Item(i));
						}
					}
				}
			}
		}
		while (dir.GetNext(&strFilename) );
	}  
	return true;
}

bool PreviewCompareFiles(wxString strA, wxString strB, SyncData data){
	wxFileName flA(strA);
	wxFileName flB(strB);
	return true;
	
}

bool PreviewReAdd(SyncData data, wxTreeCtrl *treeDest, int iLength){
	wxArrayString arrList;
	wxTextFile *file = new wxTextFile(_("C:\\test.txt"));
	file->Open();
	for(unsigned int i = 0; i < file->GetLineCount(); i++){
		arrList.Add(file->GetLine(i));
	}
	for(unsigned int j = 0; j < arrList.GetCount(); j++){
		arrList.Item(j) = arrList.Item(j).Right(arrList.Item(j).Length() - iLength);
	}
	for(unsigned int k = 0; k < arrList.GetCount(); k++){
		wxStringTokenizer tkz(arrList.Item(k), wxFILE_SEP_PATH, wxTOKEN_STRTOK);
		wxTreeItemId idParent;
		idParent = treeDest->GetRootItem();
		while (tkz.HasMoreTokens())
		{
			wxString strToken = tkz.GetNextToken();
			wxTreeItemIdValue cookie;
			wxTreeItemId tempidParent, idFinal;
			bool blFound;
			tempidParent = treeDest->GetFirstChild(idParent, cookie);
			if(treeDest->GetItemText(tempidParent) == strToken){
				blFound = true;
				idFinal = tempidParent;
			}
			while(tempidParent = treeDest->GetNextChild(idParent, cookie)){
				if(treeDest->GetItemText(tempidParent) == strToken){
					blFound = true;
					idFinal = tempidParent;
				}	
			}
			if(!blFound){
				idFinal = treeDest->AppendItem(idParent, strToken);
			}
			idParent = idFinal;		
		}
	}
	return true;
}
