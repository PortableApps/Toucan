/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include "syncctrl.h"

SyncPreviewDirCtrl::SyncPreviewDirCtrl(wxWindow* parent, wxWindowID id, SyncType type,
									   SyncPreviewDirCtrl *ctrl, const wxPoint& pos, 
									   const wxSize& size, long style)
									   :m_Type(type), m_Ctrl(ctrl), PreviewDirCtrl(parent, id, pos, size, style){
   
}