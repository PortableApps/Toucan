/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef _FRMVARIABLE_H_
#define _FRMVARIABLE_H_


#define ID_FRMVARIABLE 10009
#define ID_TEXT 10010
#define ID_PATH 10011
#define ID_VARS 10012
#define ID_VARS_ADD 10013
#define ID_PREVIEW 10014
#define SYMBOL_FRMVARIABLE_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_FRMVARIABLE_TITLE _("Insert Variable")
#define SYMBOL_FRMVARIABLE_IDNAME ID_FRMVARIABLE
#define SYMBOL_FRMVARIABLE_SIZE wxSize(400, 300)
#define SYMBOL_FRMVARIABLE_POSITION wxDefaultPosition

class frmVariable: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( frmVariable )
    DECLARE_EVENT_TABLE()

public:

    /// Constructors
	frmVariable();

    frmVariable( wxWindow* parent, wxWindowID id = SYMBOL_FRMVARIABLE_IDNAME, const wxString& caption = SYMBOL_FRMVARIABLE_TITLE, const wxPoint& pos = SYMBOL_FRMVARIABLE_POSITION, const wxSize& size = SYMBOL_FRMVARIABLE_SIZE, long style = SYMBOL_FRMVARIABLE_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_FRMVARIABLE_IDNAME, const wxString& caption = SYMBOL_FRMVARIABLE_TITLE, const wxPoint& pos = SYMBOL_FRMVARIABLE_POSITION, const wxSize& size = SYMBOL_FRMVARIABLE_SIZE, long style = SYMBOL_FRMVARIABLE_STYLE );


    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_PATH
    void OnPathClick(wxCommandEvent& event);

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_VARS_ADD
    void OnVarsAddClick(wxCommandEvent& event);

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
    void OnOkClick(wxCommandEvent& event);

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
    void OnCancelClick(wxCommandEvent& event);
	
	void OnTextChange(wxCommandEvent& event);

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );

    wxTextCtrl* m_Text;
    wxComboBox* m_Variables;
    wxTextCtrl* m_Preview;
};

#endif
