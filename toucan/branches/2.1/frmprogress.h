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
#define ID_PANEL 10028
#define ID_TEXTCTRL 10067
#define SYMBOL_FRMPROGRESS_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxMAXIMIZE_BOX|wxMINIMIZE_BOX
#define SYMBOL_FRMPROGRESS_TITLE _("Progress")
#define SYMBOL_FRMPROGRESS_IDNAME ID_FRMPROGRESS
#define SYMBOL_FRMPROGRESS_SIZE wxSize(640, 480)
#define SYMBOL_FRMPROGRESS_POSITION wxDefaultPosition

/*frmProgress, the main progress window for Toucan*/
class frmProgress: public wxFrame
{
	DECLARE_CLASS(frmProgress)
	DECLARE_EVENT_TABLE()

public:
	// Constructor
	frmProgress( wxWindow* parent, wxWindowID id = SYMBOL_FRMPROGRESS_IDNAME, const wxString& caption = SYMBOL_FRMPROGRESS_TITLE, const wxPoint& pos = SYMBOL_FRMPROGRESS_POSITION, const wxSize& size = SYMBOL_FRMPROGRESS_SIZE, long style = SYMBOL_FRMPROGRESS_STYLE );

	//Sets up all of the member vaiables
	void Init();

	// Creates the controls and sizers
	void CreateControls();

	void OnOkClick( wxCommandEvent& event );
	void OnCancelClick( wxCommandEvent& event );
	void OnSaveClick( wxCommandEvent& event );
	void OnCloseWindow( wxCloseEvent& event );

	wxTextCtrl* m_Text;
	wxButton* m_OK;
	wxButton* m_Cancel;
	wxButton* m_Save;
};

#endif
