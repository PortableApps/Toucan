#include <wx/dnd.h>

/*File droptarget to a wxListCtrl*/
class DnDFileList : public wxFileDropTarget
{
    public:
        DnDFileList(wxListBox *parent) {m_Parent = parent;}
        
        virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& arrFilenames){
            size_t sFiles = arrFilenames.GetCount();
            for(size_t n = 0; n < sFiles; n++){
                m_Parent->Append(arrFilenames.Item(n));
            }
            return true;
        }
    private:
        wxListBox *m_Parent;
};

/*File droptarget to a wxTextCtrl*/
class DnDFileText : public wxFileDropTarget
{
    public:
        DnDFileText(wxTextCtrl *parent) {m_Parent = parent;}
        
        virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& arrFilenames){
                m_Parent->WriteText(arrFilenames.Item(0));
                return true;
        }
    private:
        wxTextCtrl *m_Parent;
};