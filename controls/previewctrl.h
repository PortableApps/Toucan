/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_PREVIEWCTRL
#define H_PREVIEWCTRL

#include "dirctrl.h"

class Rules;

class PreviewTraverserThread : public DirTraverserThread{
public:

	PreviewTraverserThread(const wxString& path, Rules *rules, wxEvtHandler* handler) 
		: m_Rules(rules), DirTraverserThread(path, handler)
	{}

	virtual void* Entry();

private:
	Rules *m_Rules;
};

//A basic previewing control, used by backup and secure
class PreviewDirCtrl : public DirCtrl{

public:
	PreviewDirCtrl(wxWindow* parent, wxWindowID id,
				   const wxPoint& pos = wxDefaultPosition,
				   const wxSize& size = wxDefaultSize,
				   long style = wxTR_HAS_BUTTONS|wxTR_MULTIPLE|wxTR_HIDE_ROOT);
	~PreviewDirCtrl();

	void SetRules(Rules *rules) { m_Rules = rules; }

	//We use our own previewing thread
	virtual DirTraverserThread* GetThread(const wxString& path, wxEvtHandler* handler);

private:
   Rules *m_Rules;
};

#endif
