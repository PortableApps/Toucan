/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include "dirctrl.h"

//A class to show all of the files and folders availiable to to local pc
class LocalDirCtrl : public DirCtrl{

public:
	LocalDirCtrl(wxWindow* parent, wxWindowID id,
					const wxPoint& pos = wxDefaultPosition,
					const wxSize& size = wxDefaultSize,
					long style = wxTR_HAS_BUTTONS|wxTR_MULTIPLE|wxTR_HIDE_ROOT);

	void ReCreateTree();
};
