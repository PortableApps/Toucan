#ifndef H_TREEFUNC
#define H_TREEFUNC

#include <wx/treectrl.h>
#include <wx/dir.h>
#include "toucan.h"

bool AddDirChildren(wxString strFrom, wxTreeItemId idParent, wxTreeCtrl* to){
	if (strFrom[strFrom.length()-1] != wxFILE_SEP_PATH) {
		strFrom += wxFILE_SEP_PATH;       
	}
	wxDir dir(strFrom);
	wxString strFilename;
	bool blDir = dir.GetFirst(&strFilename);

	if(blDir){
		do {
			//wxMessageBox(strFrom);
	//wxMessageBox(strFilename);
			if(wxDirExists(strFrom + strFilename)){
				wxMutexGuiEnter();
				wxTreeItemId toid = to->AppendItem(idParent, strFilename.AfterLast(wxFILE_SEP_PATH));
				wxMutexGuiLeave();
				AddDirChildren(strFrom + strFilename, toid, to);
			}
			else{
				wxMutexGuiEnter();
				to->AppendItem(idParent, strFilename);
				wxMutexGuiLeave();
			}	
		}
		while (dir.GetNext(&strFilename));
	}  
	return true;	
}

bool AddDirToTree(wxString strFilepath, wxTreeCtrl* to){
	wxMutexGuiEnter();
	wxTreeItemId parent = to->AppendItem(to->GetRootItem(), strFilepath.AfterLast(wxFILE_SEP_PATH));
	wxMutexGuiLeave();
	AddDirChildren(strFilepath, parent,  to);
	return true;
}

class AddDirectoryThread : public wxThread
{
public:
	//Constructor
	AddDirectoryThread(wxString strFilePath, wxTreeCtrl* to){
		m_FilePath = strFilePath;
		m_Tree = to;
	}
	//Main function, all stuff goes in here
	virtual void *Entry();

private:
	wxString m_FilePath;
	wxTreeCtrl* m_Tree;
};

void *AddDirectoryThread::Entry(){
	AddDirToTree(m_FilePath, m_Tree);
	return NULL;
}

#endif
