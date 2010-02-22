/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

//A more advanced previewing control, used by sync
class SyncPreviewDirCtrl : public DirCtrl{

public:
	SyncPreviewDirCtrl(wxWindow* parent, wxWindowID id,
					const wxPoint& pos = wxDefaultPosition,
					const wxSize& size = wxDefaultSize,
					long style = wxTR_HAS_BUTTONS|wxTR_MULTIPLE|wxTR_HIDE_ROOT);
	~SyncPreviewDirCtrl();

	void SetPartner(SyncPreviewDirCtrl *opptree) { m_OppTree = opptree; }
	void SetSource(bool source) { m_source = source; }
	void OnTraversed(wxCommandEvent &event);
	void AddDirectory(DirCtrlItem *item, int depth);

private:
   SyncPreviewDirCtrl *m_OppTree;
   bool m_Source;
};
