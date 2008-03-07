#include <wx/dnd.h>
#include <wx/filename.h>


/*File droptarget to a wxTextCtrl needs code to add */
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
			m_Tree->SetRootPath(arrFilenames.Item(0));
		}
		return true;
	}
private:
	wxTextCtrl *m_Text;
	wxVirtualDirTreeCtrl *m_Tree;
};
