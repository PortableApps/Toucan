/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_DIRCTRL
#define H_DIRCTRL

#include <vector>

#include <wx/dynarray.h>
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
		m_Caption = path.GetName();
		if(path.IsDir()){
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
typedef std::vector<DirCtrlItem> DirCtrlItemArray;

class DirCtrlTraverser : public wxDirTraverser{

public:
	DirCtrlTraverser(wxArrayString *files, wxString filespec = wxEmptyString) : m_Files(files), m_FileSpec(filespec) { }

	virtual wxDirTraverseResult OnFile(const wxString& filename){
		if(!filename.Matches(m_FileSpec)){
			m_Files->Add(filename);
		}
		return wxDIR_IGNORE;
	}

	virtual wxDirTraverseResult OnDir(const wxString& dirname){
		m_Files->Add(dirname);
		return wxDIR_IGNORE;
	}

private:
	wxArrayString *m_Files;
	wxString m_FileSpec;
};

class DirCtrl : public wxTreeCtrl{

public:
	DirCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition,
	                     const wxSize& size = wxDefaultSize, long style = wxTR_HAS_BUTTONS|wxTR_FULL_ROW_HIGHLIGHT|wxTR_HIDE_ROOT|wxTR_MULTIPLE);
	virtual ~DirCtrl();

	void AddItem(DirCtrlItem *item);

	virtual bool OnAddItem(DirCtrlItem *item);
	virtual void OnAddDirectory(DirCtrlItemArray *items);
	
	//EventHandlers
	void OnNodeExpand(wxTreeEvent &event);

	//Inline functions
	void SetScanDepth(const int &ScanDepth) {this->m_ScanDepth = ScanDepth;}
	void SetExtensions(const wxString &Extensions) {this->m_Extensions = Extensions;}
	void SetImageList(wxImageList *Image) {this->m_Image = Image;}
	int GetScanDepth() const {return m_ScanDepth;}
	const wxString& GetExtensions() const {return m_Extensions;} 
	void ShowHidden(const bool &ShowHidden = true) { m_ShowHidden = ShowHidden; }

private:
	DECLARE_EVENT_TABLE()
	
	void AddDirectory(DirCtrlItem *item, int depth);
	
	int m_ScanDepth;
	bool m_ShowHidden;
	wxString m_Extensions;
	wxImageList *m_Image;
};

#endif
