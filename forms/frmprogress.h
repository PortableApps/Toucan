/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef FRMPROGRESS_H
#define FRMPROGRESS_H

#include <wx/frame.h>

#ifdef __WXMSW__
	struct ITaskbarList3;
#endif

class wxListCtrl;
class wxButton;
class wxGauge;

enum{
	ID_FRMPROGRESS = wxID_HIGHEST + 1,
	ID_PANEL_PROGRESS,
	ID_PROGRESS_LIST,
	ID_PROGRESS_GAUGE
};

class frmProgress: public wxFrame
{
	DECLARE_EVENT_TABLE()

public:
	//Constructor
	frmProgress(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style);

	//Sets up all of the member vaiables
	void Init();

	//Creates the controls and sizers
	void CreateControls();

#ifdef __WXMSW__
	WXUINT m_TaskBarId;
	ITaskbarList3 *m_Taskbar;
	//We catch windows events so we can support windows 7 taskbar progress
	WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wparam, WXLPARAM lparam);
#endif

	void OnOkClick(wxCommandEvent& event);
	void OnCancelClick(wxCommandEvent& event);
	void OnSaveClick(wxCommandEvent& event);

	void IncrementGauge();
	void FinishGauge();

	wxListCtrl* m_List;
	wxButton* m_OK;
	wxButton* m_Cancel;
	wxButton* m_Save;
	wxGauge* m_Gauge;
};

#endif
