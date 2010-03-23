/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SYNCCTRL
#define H_SYNCCTRL

#include "previewctrl.h"

class Rules;
class SyncData;

enum SyncType{
	SYNC_SOURCE,
	SYNC_DEST
};

class SyncPreviewThread : public DirThread{
public:

	SyncPreviewThread(const wxString& path, const wxString &opppath, SyncType type, SyncData *data, wxEvtHandler* handler) 
		: m_Data(data), m_OppPath(opppath), m_Type(type), DirThread(path, handler)
	{}

	virtual void* Entry();

private:
	SyncData *m_Data;
	wxString m_OppPath;
	SyncType m_Type;
};

//A more advanced previewing control, used by sync
class SyncPreviewDirCtrl : public PreviewDirCtrl{

public:
	SyncPreviewDirCtrl(wxWindow* parent, wxWindowID id, SyncType type,
					   SyncPreviewDirCtrl *ctrl, 
					   const wxPoint& pos = wxDefaultPosition,
					   const wxSize& size = wxDefaultSize,
					   long style = wxTR_HAS_BUTTONS|wxTR_SINGLE|wxTR_HIDE_ROOT);

	//We use our own previewing thread
	virtual DirThread* GetThread(const wxString& path);

	//We override additem so only one item is added at a time
	virtual void AddItem(DirCtrlItem *item);
	virtual void AddItem(const wxString &path);

	void SetPreview(bool preview) { m_Preview = preview; }

	const wxString& GetRoot() { return m_Root; }

private:
	SyncPreviewDirCtrl* m_Ctrl;
	SyncType m_Type;
	bool m_Preview;
	wxString m_Root;
};

#endif
