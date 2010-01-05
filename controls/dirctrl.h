/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_DIRCTRL
#define H_DIRCTRL

#include <vector>

#include <wx/treectrl.h>
#include <wx/filename.h>
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/imaglist.h>
#include <wx/dir.h>

enum DirCtrlItemType{
	DIRCTRL_DEFAULT,
	DIRCTRL_FILE,
	DIRCTRL_FOLDER,
	DIRCTRL_ROOT
};

class DirCtrlItem : public wxTreeItemData{

public:
	DirCtrlItem(const wxFileName &path){
		m_Path = path;
		if(!wxFileExists(path.GetFullPath())){
			m_Caption = path.GetName();
			if(path.GetVolume().Length() == path.GetFullPath().Length()){
				m_Type = DIRCTRL_ROOT;
				m_Icon = 2;
			}
			else{
				m_Type = DIRCTRL_FOLDER;
				m_Icon = 1;
			}
		}
		else{
			m_Caption = path.GetFullPath().AfterLast(wxFILE_SEP_PATH);
			m_Type = DIRCTRL_FILE;
			m_Icon = 0;
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
	int GetIcon() const { return m_Icon; }

protected:
	wxFileName m_Path;
	wxString m_Caption;
	DirCtrlItemType m_Type;
	int m_Icon;
};

//Typedef a vector for holding
typedef std::vector<DirCtrlItem*> DirCtrlItemArray;

class DirCtrlTraverser : public wxDirTraverser{

public:
	DirCtrlTraverser(DirCtrlItemArray *items) : m_Items(items){ 
	}

	virtual wxDirTraverseResult OnFile(const wxString& filename){
		m_Items->push_back(new DirCtrlItem(filename));
		return wxDIR_CONTINUE;
	}

	virtual wxDirTraverseResult OnDir(const wxString& dirname){
		m_Items->push_back(new DirCtrlItem(dirname));
		return wxDIR_IGNORE;
	}

private:
	DirCtrlItemArray *m_Items;
};

class DirCtrl : public wxTreeCtrl{

public:
	DirCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition,
	                     const wxSize& size = wxDefaultSize, long style = wxTR_HAS_BUTTONS|wxTR_LINES_AT_ROOT|wxTR_HIDE_ROOT|wxTR_SINGLE);
	virtual ~DirCtrl();

	void AddItem(DirCtrlItem *item);
	void AddItem(const wxString &path);
	
	//Event Handlers
	void OnNodeExpand(wxTreeEvent &event);

	//Inline functions
	void SetScanDepth(const int &ScanDepth) {this->m_ScanDepth = ScanDepth;}
	int GetScanDepth() const {return m_ScanDepth;}

private:
	DECLARE_EVENT_TABLE()
	
	void AddDirectory(DirCtrlItem *item, int depth);
	
	int m_ScanDepth;
	wxImageList *m_Image;
};

#endif
