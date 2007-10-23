/////////////////////////////////////////////////////////////////////////////
// Name:        frmmain.h
// Purpose:     
// Author:      Steven Lamerton
// Modified by: 
// Created:     19/10/2007 11:57:09
// RCS-ID:      
// Copyright:   Copyright (c) Steven Lamerton 2006-2007
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _FRMMAIN_H_
#define _FRMMAIN_H_


/*!
* Includes
*/

////@begin includes
#include "wx/aui/framemanager.h"
#include "wx/frame.h"
#include "wx/toolbar.h"
#include "wx/aui/auibook.h"
#include "wx/treectrl.h"
#include "wx/dirctrl.h"
////@end includes

/*!
* Forward declarations
*/

////@begin forward declarations
class wxTreeCtrl;
class wxGenericDirCtrl;
////@end forward declarations

/*!
* Control identifiers
*/

////@begin control identifiers
#define ID_AUIFRAME 10000
#define ID_TOOLBAR 10001
#define ID_TOOL1 10004
#define ID_TOOL2 10042
#define ID_AUINOTEBOOK 10003
#define ID_PANEL_SYNC 10009
#define ID_SYNC_JOB_SELECT 10019
#define ID_SYNC_JOB_OPEN 10020
#define ID_SYNC_JOB_SAVE 10021
#define ID_SYNC_RULES 10022
#define ID_SYNC_SOURCE_TXT 10039
#define ID_SYNC_SOURCE_BTN 10040
#define ID_SYNC_SOURCE_TREE 10041
#define ID_SYNC_DEST_TXT 10043
#define ID_SYNC_DEST_BTN 10044
#define ID_SYNC_DEST_TREE 10045
#define ID_SYNC_FUNCTION 10046
#define ID_SYNC_TIMESTAMP 10047
#define ID_SYNC_ATTRIB 10048
#define ID_SYNC_IGNORERO 10049
#define ID_SYNC_IGNOREDS 10063
#define ID_PANEL_BACKUP 10010
#define ID_BACKUP_JOB_SELECT 10008
#define ID_BACKUP_JOB_OPEN 10011
#define ID_BACKUP_JOB_SAVE 10012
#define ID_BACKUP_RULES 10013
#define ID_BACKUP_DIRCTRL 10002
#define ID_BACKUP_ADD 10005
#define ID_BACKUP_REMOVE 10006
#define ID_BACKUP_LISTBOX 10007
#define ID_BACKUP_ADDVAR 10038
#define ID_BACKUP_FUNCTION 10014
#define ID_BACKUP_FORMAT 10015
#define ID_BACKUP_RATIO 10016
#define ID_BACKUP_PASS 10017
#define ID_BACKUP_REPASS 10018
#define ID_PANEL_SECURE 10023
#define ID_SECURE_JOB_SELECT 10024
#define ID_SECURE_JOB_OPEN 10025
#define ID_SECURE_JOB_SAVE 10026
#define ID_SECURE_RULES 10027
#define ID_SECURE_DIRCTRL 10028
#define ID_SECURE_ADD 10005
#define ID_SECURE_REMOVE 10006
#define ID_SECURE_LISTBOX 10007
#define ID_SECURE_ADDVAR 10038
#define ID_SECURE_MAKERELATIVE 10037
#define ID_SECURE_FUNCTION 10039
#define ID_SECURE_FORMAT 10040
#define ID_SECURE_PASS 10041
#define ID_SECURE_REPASS 10042
#define ID_PANEL2 10050
#define ID_LISTBOX2 10051
#define ID_BITMAPBUTTON13 10052
#define ID_BITMAPBUTTON14 10053
#define ID_LISTBOX3 10054
#define ID_BITMAPBUTTON15 10055
#define ID_BITMAPBUTTON16 10056
#define ID_LISTBOX4 10057
#define ID_BITMAPBUTTON17 10058
#define ID_BITMAPBUTTON18 10059
#define ID_LISTBOX5 10060
#define ID_BITMAPBUTTON19 10061
#define ID_BITMAPBUTTON20 10062
#define ID_PANEL4 10064
#define ID_PANEL6 10066
#define ID_PANEL5 10065
#define SYMBOL_FRMMAIN_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxMAXIMIZE|wxMINIMIZE_BOX|wxMAXIMIZE_BOX|wxCLOSE_BOX
#define SYMBOL_FRMMAIN_TITLE _("Toucan")
#define SYMBOL_FRMMAIN_IDNAME ID_AUIFRAME
#define SYMBOL_FRMMAIN_SIZE wxSize(800, 800)
#define SYMBOL_FRMMAIN_POSITION wxDefaultPosition
////@end control identifiers


/*!
* frmMain class declaration
*/

class frmMain: public wxFrame
{    
	DECLARE_CLASS( frmMain )
	DECLARE_EVENT_TABLE()

public:
	/// Constructors
	frmMain();
	frmMain( wxWindow* parent, wxWindowID id = SYMBOL_FRMMAIN_IDNAME, const wxString& caption = SYMBOL_FRMMAIN_TITLE, const wxPoint& pos = SYMBOL_FRMMAIN_POSITION, const wxSize& size = SYMBOL_FRMMAIN_SIZE, long style = SYMBOL_FRMMAIN_STYLE );

	bool Create( wxWindow* parent, wxWindowID id = SYMBOL_FRMMAIN_IDNAME, const wxString& caption = SYMBOL_FRMMAIN_TITLE, const wxPoint& pos = SYMBOL_FRMMAIN_POSITION, const wxSize& size = SYMBOL_FRMMAIN_SIZE, long style = SYMBOL_FRMMAIN_STYLE );

	/// Destructor
	~frmMain();

	/// Initialises member variables
	void Init();

	/// Creates the controls and sizers
	void CreateControls();

	////@begin frmMain event handler declarations

	////@end frmMain event handler declarations

	////@begin frmMain member function declarations

 /// Returns the AUI manager object
 wxAuiManager& GetAuiManager() { return m_auiManager; }

 /// Retrieves bitmap resources
 wxBitmap GetBitmapResource( const wxString& name );

 /// Retrieves icon resources
 wxIcon GetIconResource( const wxString& name );
	////@end frmMain member function declarations

	/// Should we show tooltips?
	static bool ShowToolTips();

	////@begin frmMain member variables
 wxAuiManager m_auiManager;
 wxComboBox* m_Sync_Job_Select;
 wxComboBox* m_Sync_Rules;
 wxTextCtrl* m_Sync_Source_Txt;
 wxTreeCtrl* m_Sync_Source_Tree;
 wxTextCtrl* m_Sync_Dest_Txt;
 wxTreeCtrl* m_Sync_Dest_Tree;
 wxRadioBox* m_Sync_Function;
 wxCheckBox* m_Sync_Timestamp;
 wxCheckBox* m_Sync_Attributes;
 wxCheckBox* m_Sync_Ignore_Readonly;
 wxCheckBox* m_Sync_Ignore_DaylightS;
 wxComboBox* m_Backup_Job_Select;
 wxComboBox* m_Backup_Rules;
 wxGenericDirCtrl* m_Backup_DirCtrl;
 wxListBox* m_Backup_List;
 wxRadioBox* m_Backup_Function;
 wxRadioBox* m_Backup_Format;
 wxRadioBox* m_Backup_Ratio;
 wxTextCtrl* m_Backup_Pass;
 wxTextCtrl* m_Backup_Repass;
 wxComboBox* m_Secure_Job_Select;
 wxComboBox* m_Secure_Rules;
 wxGenericDirCtrl* m_Secure_DirCtrl;
 wxListBox* m_Secure_List;
 wxRadioBox* m_Secure_Function;
 wxRadioBox* m_Secure_Format;
 wxTextCtrl* m_Secure_Pass;
 wxTextCtrl* m_Secure_Repass;
	////@end frmMain member variables
};

#endif
// _FRMMAIN_H_
