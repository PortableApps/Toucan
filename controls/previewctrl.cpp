/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include "previewctrl.h"
#include "../rules.h"

PreviewDirCtrl::PreviewDirCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos,
							   const wxSize& size, long style) 
							   : DirCtrl(parent, id, pos, size, style){
   m_Rules = NULL;
}

PreviewDirCtrl::~PreviewDirCtrl(){
	delete m_Rules;
}
