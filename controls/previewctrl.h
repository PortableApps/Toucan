/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_PREVIEWCTRL
#define H_PREVIEWCTRL

#include "dirctrl.h"

class Rules;

class PreviewThread : public DirThread{
public:

	PreviewThread(const wxString& path, wxTreeItemId parent, Rules *rules, wxEvtHandler* handler) 
		: m_Rules(rules), DirThread(path, parent, handler)
	{}

	virtual void operator()();

protected:
	Rules *m_Rules;
};

//A basic previewing control, used by backup and secure
class PreviewDirCtrl : public DirCtrl{

public:
	PreviewDirCtrl(wxWindow* parent, wxWindowID id,
				   const wxPoint& pos = wxDefaultPosition,
				   const wxSize& size = wxDefaultSize,
				   long style = wxTR_HAS_BUTTONS|wxTR_SINGLE|wxTR_HIDE_ROOT);
	~PreviewDirCtrl();

	void SetRules(Rules *rules) { m_Rules = rules; }

protected:
   Rules *m_Rules;

	//Runs thread needed to add a directory
	virtual void AddDirectory(DirCtrlItem *item);
};

#endif
