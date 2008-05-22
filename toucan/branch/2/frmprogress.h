/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef _FRMPROGRESS_H_
#define _FRMPROGRESS_H_

/*!
 * Includes
 */

#include "wx/frame.h"
#include "wx/wx.h"

/*!
 * Control identifiers
 */

#define ID_FRMPROGRESS 10007
#define ID_PANEL 10028
#define ID_TEXTCTRL 10067
#define SYMBOL_FRMPROGRESS_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_FRMPROGRESS_TITLE _("Progress")
#define SYMBOL_FRMPROGRESS_IDNAME ID_FRMPROGRESS
#define SYMBOL_FRMPROGRESS_SIZE wxSize(400, 300)
#define SYMBOL_FRMPROGRESS_POSITION wxDefaultPosition

/*!
 * frmProgress class declaration
 */

class frmProgress: public wxFrame
{
	DECLARE_CLASS( frmProgress )
	DECLARE_EVENT_TABLE()

public:
	/// Constructors
	frmProgress();
	frmProgress( wxWindow* parent, wxWindowID id = SYMBOL_FRMPROGRESS_IDNAME, const wxString& caption = SYMBOL_FRMPROGRESS_TITLE, const wxPoint& pos = SYMBOL_FRMPROGRESS_POSITION, const wxSize& size = SYMBOL_FRMPROGRESS_SIZE, long style = SYMBOL_FRMPROGRESS_STYLE );

	bool Create( wxWindow* parent, wxWindowID id = SYMBOL_FRMPROGRESS_IDNAME, const wxString& caption = SYMBOL_FRMPROGRESS_TITLE, const wxPoint& pos = SYMBOL_FRMPROGRESS_POSITION, const wxSize& size = SYMBOL_FRMPROGRESS_SIZE, long style = SYMBOL_FRMPROGRESS_STYLE );

	/// Destructor
	~frmProgress();

	/// Initialises member variables
	void Init();

	/// Creates the controls and sizers
	void CreateControls();

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
	void OnOkClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
	void OnCancelClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_SAVE
	void OnSaveClick( wxCommandEvent& event );
	
	/// wxEVT_CLOSE_WINDOW event handler for ID_PROGRESS
	void OnCloseWindow( wxCloseEvent& event );

	wxTextCtrl* m_Text;
	wxButton* m_OK;
	wxButton* m_Cancel;
	wxButton* m_Save;
};

#endif
