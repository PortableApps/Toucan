/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_PREVIEWCTRL
#define H_PREVIEWCTRL

#include "dirctrl.h"

class RuleSet;

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
	virtual void AddThread(const wxString& path, wxTreeItemId parent, boost::threadpool::pool* pool);

protected:
   RuleSet *m_Rules;
};

#endif
