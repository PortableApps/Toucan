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

	SyncPreviewThread(const wxString& path, const wxString &opppath, SyncType type, SyncData *data, wxTreeItemId parent, wxEvtHandler* handler) 
		: m_Data(data), m_OppPath(opppath), m_Type(type), DirThread(path, parent, handler)
	{}

	virtual void operator()();

private:
	SyncData *m_Data;
	wxString m_OppPath;
	SyncType m_Type;
};

//A more advanced previewing control, used by sync
class SyncPreviewDirCtrl : public PreviewDirCtrl{

public:
	SyncPreviewDirCtrl(wxWindow* parent, wxWindowID id, SyncType type, 
					   const wxPoint& pos = wxDefaultPosition,
					   const wxSize& size = wxDefaultSize,
					   long style = wxTR_HAS_BUTTONS|wxTR_SINGLE|wxTR_HIDE_ROOT);

	//We use our own previewing thread
	virtual DirThread* GetThread(const wxString& path, wxTreeItemId parent);

    void SetOppCtrl(SyncPreviewDirCtrl *ctrl) { m_Ctrl = ctrl; }

	//We override additem so only one item is added at a time
	virtual void AddItem(DirCtrlItem *item);
	virtual void AddItem(const wxString &path);

	void SetPreview(bool preview) { m_Preview = preview; }
    bool GetPreview() { return m_Preview; }

	void SetPreviewChanges(bool previewchanges) { m_PreviewChanges = previewchanges; }
    bool GetPreviewChanges() { return m_PreviewChanges; }

	const wxString& GetRoot() { return m_Root; }

protected:
	virtual void OnTraversed(wxTreeEvent &event);

private:
	SyncPreviewDirCtrl* m_Ctrl;
	SyncType m_Type;
	bool m_Preview;
    bool m_PreviewChanges;
	wxString m_Root;
};

#endif
