/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef _FRMPROGRESS_H_
#define _FRMPROGRESS_H_

#include "wx/frame.h"
#include "wx/wx.h"

#define ID_FRMPROGRESS 10007
#define ID_PANEL_PROGRESS 10028
#define ID_PROGRESS_TEXT 10067

/*frmProgress, the main progress window for Toucan*/
class frmProgress: public wxFrame
{
	DECLARE_EVENT_TABLE()

public:
	//Constructor
	frmProgress(wxWindow* parent, wxWindowID id = ID_FRMPROGRESS, const wxString& caption = _("Progress"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(640, 480), long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxMAXIMIZE_BOX|wxMINIMIZE_BOX);

	//Sets up all of the member vaiables
	void Init();

	//Creates the controls and sizers
	void CreateControls();

	void OnOkClick(wxCommandEvent& event);
	void OnCancelClick(wxCommandEvent& event);
	void OnSaveClick(wxCommandEvent& event);
	void OnCloseWindow(wxCloseEvent& event);

	wxTextCtrl* m_Text;
	wxButton* m_OK;
	wxButton* m_Cancel;
	wxButton* m_Save;
};

#endif
