/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef _FRMPROGRESS_H_
#define _FRMPROGRESS_H_

#include <wx/wx.h>

class wxFrame;
class wxListCtrl;

//Window 10000 - 10049
#define ID_FRMPROGRESS 10000
#define ID_PANEL_PROGRESS 10001
#define ID_PROGRESS_LIST 10002
#define ID_PROGRESS_GAUGE 10003

//Script 10050 - 10099
#define ID_SCRIPTFINISH 10050
#define ID_SCRIPTTEXT 10051
#define ID_SCRIPTTIME 10052
#define ID_SCRIPTERROR 10053
#define ID_SCRIPTBLANK 10054

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
