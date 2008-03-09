#include <wx/dnd.h>
#include <wx/filename.h>

#include "basicfunctions.h"

/*File droptarget to a wxVirtualDirTreeCtrl, multiple files allowed*/
class DnDFileTree : public wxFileDropTarget
{
public:
	DnDFileTree(wxVirtualDirTreeCtrl *tree) {
		m_Tree = tree;
	}
	
	virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& arrFilenames){
		//Loop through all of the paths adding each one to the wxVirtualDirTreeCtrl		
		for(unsigned int i = 0; i < arrFilenames.GetCount(); i++){
			m_Tree->AddNewPath(arrFilenames.Item(i));
		}
		return true;
	}
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
	
	virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& arrFilenames){
		//Only add the first item as Sync only supports one to one syncing		
		m_Text->SetValue(arrFilenames.Item(0));
		m_Tree->DeleteAllItems();
		m_Tree->AddRoot(_("Hidden root"));
		m_Tree->AddNewPath(arrFilenames.Item(0));
		//Return a message to the user if they have tried to add more than one item
		if(arrFilenames.GetCount() < 1){
			MessageBox(_("Only the first item you dragged was added"), _("Warning"));
		}
		return true;
	}
private:
	wxTextCtrl *m_Text;
	wxVirtualDirTreeCtrl *m_Tree;
};
