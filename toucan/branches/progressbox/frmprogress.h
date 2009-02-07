/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef _FRMPROGRESS_H_
#define _FRMPROGRESS_H_

#include <wx/wx.h>
#include <wx/frame.h>

//Window 10000 - 10049
#define ID_FRMPROGRESS 10000
#define ID_PANEL_PROGRESS 10001
#define ID_PROGRESS_TEXT 10002

//Script 10050 - 10099
#define ID_SCRIPTFINISH 10050
#define ID_SCRIPTPROGRESS 10051

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
	void OnCloseWindow(wxCloseEvent& event);
	
	//Script functions
	void OnScriptFinish(wxCommandEvent& event);
	void OnScriptProgress(wxCommandEvent& event);

	wxTextCtrl* m_Text;
	wxButton* m_OK;
	wxButton* m_Cancel;
	wxButton* m_Save;
};

#endif
