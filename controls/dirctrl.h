/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009-2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_DIRCTRL
#define H_DIRCTRL

#include <vector>
#include <map>

#include <wx/treectrl.h>
#include <wx/filename.h>
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/imaglist.h>
#include <wx/dir.h>
#include <wx/colour.h>

#ifdef __WXMSW__
	#include <windows.h>
	#include <wx/msw/winundef.h>
#endif

//Saves which nodes are expanded on creation and then re-expands them on deletion
//Useful if you are refreshing a wxGenericDirCtrl for example
class TreeStateSaver{
public:
	TreeStateSaver(wxTreeCtrl *tree);
	~TreeStateSaver();

private:
	wxArrayString SaveChildren(const wxString &path, wxTreeItemId parent);
	void LoadChildren(wxString path, wxTreeItemId parent);
	wxArrayString m_Paths;
	wxTreeCtrl *m_Tree;
};

//IDs for event handling
enum{
	ID_TRAVERSED
};

//Types of node in the treectrl
enum DirCtrlItemType{
	DIRCTRL_FILE,
	DIRCTRL_FOLDER,
	DIRCTRL_ROOT
};

//The item used in DirCtrl, four main properties, filename, caption, type, icon
class DirCtrlItem : public wxTreeItemData{
public:
	DirCtrlItem(const wxFileName &path);
	
	DirCtrlItem(const wxFileName &path, const wxString &caption, const DirCtrlItemType &type, const int &icon)
				:m_Path(path),
				 m_Caption(caption),
				 m_Type(type),
				 m_Icon(icon)
				{};
	
	~DirCtrlItem() {};
	
	wxString GetFullPath() const { return m_Path.GetFullPath(); }
	wxString GetCaption() const { return m_Caption; }
	DirCtrlItemType GetType() const { return m_Type; }
	wxColour GetColour() const { return m_Colour; }
	int GetIcon() const { return m_Icon; }
	void SetColour(wxColour colour) { m_Colour = colour; }

protected:
	wxFileName m_Path;
	wxString m_Caption;
	DirCtrlItemType m_Type;
	wxColour m_Colour;
	int m_Icon;
};

//Comparison function for std::sort
inline bool DirCtrlItemComparison(DirCtrlItem *a, DirCtrlItem *b){
	if(a->GetType() == DIRCTRL_FOLDER && b->GetType() == DIRCTRL_FILE){
		return true;
	}
	else if(a->GetType() == DIRCTRL_FILE && b->GetType() == DIRCTRL_FOLDER){
		return false;
	}
	else{
		if(a->GetFullPath().CmpNoCase(b->GetFullPath()) >= 0){
			return false;
		}
		else{
			return true;
		}
	}
}

//Typedef a more convenient vector
typedef std::vector<DirCtrlItem*> DirCtrlItemArray;

//The thread that actually traverses the directories, posts back its results
//in a DirThreadEvent
class DirThread : public wxThread{
public:

	DirThread(const wxString& path, wxEvtHandler* handler) 
		: m_Handler(handler), m_Path(path), wxThread(wxTHREAD_DETACHED)
	{}

	virtual void* Entry();
	
protected:
	wxString m_Path;
	int m_Id;
	wxEvtHandler* m_Handler;
};

//Declare our own event type
wxDECLARE_EVENT(TRAVERSER_FINISHED, wxCommandEvent);

//The dirctrl itself
class DirCtrl : public wxTreeCtrl{

public:
	DirCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition,
	                     const wxSize& size = wxDefaultSize, long style = wxTR_HAS_BUTTONS|wxTR_MULTIPLE|wxTR_HIDE_ROOT);
	virtual ~DirCtrl();

	wxArrayString GetSelectedPaths();

	void AddItem(DirCtrlItem *item);
	void AddItem(const wxString &path);

	//A neater version that uses freeze and thaw
	void ExpandAll();

	//Get the correct
	virtual DirThread* GetThread(const wxString& path, wxEvtHandler* handler);

private:
	DECLARE_EVENT_TABLE()

	//Event Handlers
	void OnNodeExpand(wxTreeEvent &event);
	void OnTraversed(wxCommandEvent &event);

	//Runs thread needed to add a directory
	void AddDirectory(DirCtrlItem *item);

	wxImageList *m_Image;
	std::map<int, wxTreeItemId> m_IdMap;
};

#endif
