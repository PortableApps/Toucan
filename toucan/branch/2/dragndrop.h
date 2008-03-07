#include <wx/dnd.h>
#include <wx/filename.h>

/*File droptarget to a wxVirtualDirTreeCtrl*/
class DnDFileTree : public wxFileDropTarget
{
public:
	DnDFileTree(wxVirtualDirTreeCtrl *tree) {
		m_Tree = tree;
	}
	
	virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& arrFilenames){
		for(unsigned int i = 0; i < arrFilenames.GetCount(); i++){
			if(wxFileName::DirExists(arrFilenames.Item(i))){
				m_Tree->AddNewPath(arrFilenames.Item(i));
			}
		}
		return true;
	}
private:
	wxVirtualDirTreeCtrl *m_Tree;
};

/*File droptarget to a wxTextCtrl*/
class DnDFileText : public wxFileDropTarget
{
public:
	DnDFileText(wxTextCtrl *text, wxVirtualDirTreeCtrl *tree) {
		m_Text = text; 
		m_Tree = tree;
	}
	
	virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& arrFilenames){
		if(wxFileName::DirExists(arrFilenames.Item(0))){
			m_Text->SetValue(arrFilenames.Item(0));
			m_Tree->DeleteAllItems();
			m_Tree->AddRoot(_("Hidden root"));
			m_Tree->AddNewPath(arrFilenames.Item(0));
		}
		return true;
	}
private:
	wxTextCtrl *m_Text;
	wxVirtualDirTreeCtrl *m_Tree;
};
