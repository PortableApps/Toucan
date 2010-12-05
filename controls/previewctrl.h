/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_PREVIEWCTRL
#define H_PREVIEWCTRL

#include "dirctrl.h"

class RuleSet;

class PreviewThread : public DirThread{
public:

	PreviewThread(const wxString& path, wxTreeItemId parent, RuleSet *rules, wxEvtHandler* handler) 
		: m_Rules(rules), DirThread(path, parent, handler)
	{}

	virtual void* Entry();

protected:
	RuleSet *m_Rules;
};

//A basic previewing control, used by backup and secure
class PreviewDirCtrl : public DirCtrl{

public:
	PreviewDirCtrl(wxWindow* parent, wxWindowID id,
				   const wxPoint& pos = wxDefaultPosition,
				   const wxSize& size = wxDefaultSize,
				   long style = wxTR_HAS_BUTTONS|wxTR_SINGLE|wxTR_HIDE_ROOT);
	~PreviewDirCtrl();

	void SetRules(RuleSet *rules) { m_Rules = rules; }

	//We use our own previewing thread
	virtual DirThread* GetThread(const wxString& path, wxTreeItemId parent);

protected:
   RuleSet *m_Rules;
};

#endif
