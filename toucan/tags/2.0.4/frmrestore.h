/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef _FRMRESTORE_H_
#define _FRMRESTORE_H_

#include <wx/wx.h>
#include <wx/dialog.h>

#define ID_FRMRESTORE 10001
#define ID_FILE 10002
#define ID_TEXT_FILE 10003
#define ID_DESTINATION 10004
#define ID_TEXT_DESTINATION 10005
#define ID_PASS 10006

/*frmProgress, the backup restore window for Toucan*/
class frmRestore: public wxDialog
{    
    DECLARE_EVENT_TABLE()

public:
    //Constructor
    frmRestore(wxWindow* parent, wxWindowID id = ID_FRMRESTORE, const wxString& caption = _("Restore"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(400, 300), long style = wxDEFAULT_DIALOG_STYLE);

	//Sets up all of the member vaiables
    void Init();

	//Creates the controls and sizers
    void CreateControls();
	
    void OnOkClick(wxCommandEvent& event);
    void OnCancelClick(wxCommandEvent& event);
    void OnDestinationClick(wxCommandEvent& event);
    void OnFileClick(wxCommandEvent& event);

    wxTextCtrl* m_File;
    wxTextCtrl* m_Destination;
	wxCheckBox* m_Pass;

};

#endif

