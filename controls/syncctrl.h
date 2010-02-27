/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SYNCCTRL
#define H_SYNCCTRL

#include "previewctrl.h"

enum SyncType{
	SYNC_SOURCE,
	SYNC_DEST
};

//A more advanced previewing control, used by sync
class SyncPreviewDirCtrl : public PreviewDirCtrl{

public:
	SyncPreviewDirCtrl(wxWindow* parent, wxWindowID id, SyncType type,
					   SyncPreviewDirCtrl *ctrl, 
					   const wxPoint& pos = wxDefaultPosition,
					   const wxSize& size = wxDefaultSize,
					   long style = wxTR_HAS_BUTTONS|wxTR_MULTIPLE|wxTR_HIDE_ROOT);

	//We use our own previewing thread
	//virtual DirTraverserThread* GetThread(const wxString& path, wxEvtHandler* handler);

private:
   SyncPreviewDirCtrl *m_Ctrl;
   SyncType m_Type;
};

#endif
