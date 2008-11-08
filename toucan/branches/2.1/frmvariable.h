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

/*frmVariable, the window used for inserting variables into Toucan*/
class frmVariable: public wxDialog
{    
    DECLARE_EVENT_TABLE()

public:
    //Constructor
    frmVariable( wxWindow* parent, wxWindowID id = ID_FRMVARIABLE, const wxString& caption = _("Insert Variable"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(400, 300), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);

	//Sets up all of the member vaiables
    void Init();

	//Creates the controls and sizers
    void CreateControls();

    void OnPathClick(wxCommandEvent& event);
    void OnVarsAddClick(wxCommandEvent& event);
    void OnOkClick(wxCommandEvent& event);
    void OnCancelClick(wxCommandEvent& event);
	void OnTextChange(wxCommandEvent& event);

    wxBitmap GetBitmapResource( const wxString& name );

    wxTextCtrl* m_Text;
    wxComboBox* m_Variables;
    wxTextCtrl* m_Preview;
};

#endif
