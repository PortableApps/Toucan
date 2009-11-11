/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef FRMPROGRESS_H
#define FRMPROGRESS_H

#include <wx/frame.h>
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

	void OnOkClick(wxCommandEvent& event);
	void OnCancelClick(wxCommandEvent& event);
	void OnSaveClick(wxCommandEvent& event);

	//Script functions
	void OnScriptFinish(wxCommandEvent& event);
	void OnScriptText(wxCommandEvent& event);
	void OnScriptTime(wxCommandEvent& event);
	void OnScriptError(wxCommandEvent& event);
	void OnScriptBlank(wxCommandEvent& event);

	//wxTextCtrl* m_Text;
	wxListCtrl* m_List;
	wxButton* m_OK;
	wxButton* m_Cancel;
	wxButton* m_Save;
	wxGauge* m_Gauge;
};

#endif
