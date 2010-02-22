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

enum{
	ID_TRAVERSED
};

enum DirCtrlItemType{
	DIRCTRL_DEFAULT,
	DIRCTRL_FILE,
	DIRCTRL_FOLDER,
	DIRCTRL_ROOT
};

//The item used in DirCtrl, four main properties, filename, caption, type, icon
class DirCtrlItem : public wxTreeItemData{

public:
	DirCtrlItem(const wxFileName &path){
		m_Colour = wxColour("Black");
		m_Path = path;
		if(!wxFileExists(path.GetFullPath())){
			/*We add 2 because GetVolume returns C and we expect C:\ */
			if(path.GetVolume().Length() + 2 == path.GetFullPath().Length()){
				m_Caption = path.GetFullPath();
				m_Type = DIRCTRL_ROOT;
#ifdef __WXMSW__
				unsigned int type = GetDriveType(path.GetFullPath());
				if(type == DRIVE_REMOVABLE){
					m_Icon = 5;
				}
				else if(type == DRIVE_CDROM){
					m_Icon = 4;
				}
				else{
					//Default to using the harddisk icon
					m_Icon = 3;
				}
#else
				m_Icon = 3;
#endif
			}
			else{
				m_Caption = path.GetName();
				m_Type = DIRCTRL_FOLDER;
				m_Icon = 0;
			}
		}
		else{
			m_Caption = path.GetFullName();
			m_Type = DIRCTRL_FILE;
			m_Icon = (path.GetExt() == "exe") ? 2 : 1;
		}
	};
	
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

//Typedef a more convenient vector
typedef std::vector<DirCtrlItem*> DirCtrlItemArray;

//A custom version of wxDirTraverser that only iterates through one directory
//and adds all items to a DirCtrlItemArray
class DirCtrlTraverser : public wxDirTraverser{
public:
	DirCtrlTraverser(DirCtrlItemArray* items) : m_Items(items)
	{}

	virtual wxDirTraverseResult OnFile(const wxString& filename){
		m_Items->push_back(new DirCtrlItem(filename));
		return wxDIR_CONTINUE;
	}

	virtual wxDirTraverseResult OnDir(const wxString& dirname){
		m_Items->push_back(new DirCtrlItem(dirname));
		return wxDIR_IGNORE;
	}

	DirCtrlItemArray* GetItems() { return m_Items; }

private:
	DirCtrlItemArray* m_Items;
};

//The thread that actually traverses the directories, posts back its results
//in a DirTraverserThreadEvent
class DirTraverserThread : public wxThread{
public:

	DirTraverserThread(const wxString& path, int depth, wxEvtHandler* handler) 
		: m_Handler(handler), m_Path(path), m_Depth(depth), wxThread(wxTHREAD_DETACHED)
	{}

	virtual DirCtrlTraverser* GetTraverser();
	
private:
	virtual void* Entry();
	wxString m_Path;
	int m_Depth;
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
	
	//Event Handlers
	void OnNodeExpand(wxTreeEvent &event);
	virtual void OnTraversed(wxCommandEvent &event);

	//Inline functions
	void SetScanDepth(const int &ScanDepth) {this->m_ScanDepth = ScanDepth;}
	int GetScanDepth() const {return m_ScanDepth;}

protected:
	virtual void AddDirectory(DirCtrlItem *item, int depth);

private:
	DECLARE_EVENT_TABLE()

	int m_ScanDepth;
	wxImageList *m_Image;
	std::map<int, wxTreeItemId> m_IdMap;
};

#endif
