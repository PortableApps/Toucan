#ifndef H_TREEFUNC
#define H_TREEFUNC

#include <wx/treectrl.h>
#include <wx/dir.h>
#include "toucan.h"

bool AddTreeChildren(wxTreeItemId idParent, wxTreeItemIdValue cookie, wxTreeCtrl *from, wxTreeCtrl *to, wxTreeItemId toparent, frmMain *window){
	wxTreeItemId id;

	if (!cookie){
		id = from->GetFirstChild(idParent, cookie);
	}
	else{
		id = from->GetNextChild(idParent, cookie);
	}

	if (!id.IsOk()){
		return false;
	}


	wxString text = from->GetItemText(id);
	//wxMessageBox(text);
	wxTreeItemId toid = to->AppendItem(toparent, from->GetItemText(id));

	if (from->ItemHasChildren(id))
	AddTreeChildren(id, 0, from, to, toid, window);

	AddTreeChildren(idParent, cookie, from, to, toparent, window);
	return true;
}

bool AddTreeToTree(wxTreeCtrl *from,wxTreeCtrl *to, frmMain *window){
	wxBusyCursor wait;
	//window->m_Notebook->Freeze();

	wxTreeItemId parent = to->AppendItem(to->GetRootItem(), from->GetItemText(from->GetSelection()));
	
	from->ExpandAllChildren(from->GetSelection());
	AddTreeChildren(from->GetSelection(),  0,from, to, parent, window);
	from->CollapseAllChildren(from->GetSelection());

	//window->m_Notebook->Thaw();
	return true;
}

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
				wxTreeItemId toid = to->AppendItem(idParent, strFilename.AfterLast(wxFILE_SEP_PATH));
				AddDirChildren(strFrom + strFilename, toid, to);
			}
			else{
				to->AppendItem(idParent, strFilename);
			}	
		}
		while (dir.GetNext(&strFilename));
	}  
	return true;	
}

bool AddDirToTree(wxString strFilepath, wxTreeCtrl* to){
	wxTreeItemId parent = to->AppendItem(to->GetRootItem(), strFilepath.AfterLast(wxFILE_SEP_PATH));
	AddDirChildren(strFilepath, parent,  to);
	return true;
}


#endif
