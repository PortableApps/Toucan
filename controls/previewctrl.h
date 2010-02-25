/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include "dirctrl.h"

class Rules;

//A basic previewing control, used by backup and secure
class PreviewDirCtrl : public DirCtrl{

public:
	PreviewDirCtrl(wxWindow* parent, wxWindowID id,
				   const wxPoint& pos = wxDefaultPosition,
				   const wxSize& size = wxDefaultSize,
				   long style = wxTR_HAS_BUTTONS|wxTR_MULTIPLE|wxTR_HIDE_ROOT);
	~PreviewDirCtrl();

	void SetRules(Rules *rules) { m_Rules = rules; }

private:
   Rules *m_Rules;
};
