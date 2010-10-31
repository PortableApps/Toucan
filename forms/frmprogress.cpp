/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007 - 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include "frmmain.h"
#include "frmprogress.h"
#include "../luamanager.h"
#include "../script.h"
#include "../toucan.h"

#include <wx/stdpaths.h>
#include <wx/grid.h>
#include <wx/textfile.h>
#include <wx/wx.h>

#if defined(__WXMSW__) && !defined(__MINGW32__)
	#include <shobjidl.h>
	#undef Yield
#endif

//frmProgress event table
BEGIN_EVENT_TABLE(frmProgress, wxFrame)
	EVT_BUTTON(wxID_OK, frmProgress::OnOkClick)
	EVT_BUTTON(wxID_CANCEL, frmProgress::OnCancelClick)
	EVT_BUTTON(wxID_SAVE, frmProgress::OnSaveClick)
    EVT_BUTTON(ID_PROGRESS_AUTOSCROLL, frmProgress::OnAutoscrollClick)
	EVT_CLOSE(frmProgress::OnClose)
END_EVENT_TABLE()

//Constructor
frmProgress::frmProgress(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style){
	Init();
	wxFrame::Create(parent, id, caption, pos, size, style);
#if defined(__WXMSW__) && !defined(__MINGW32__)
	m_TaskBarId = RegisterWindowMessage(wxT("TaskbarButtonCreated"));
#endif
	CreateControls();
	Centre();
}

//frmProgress initialisation
void frmProgress::Init(){
	ProgressGrid = NULL;
	m_OK = NULL;
	m_Cancel = NULL;
	m_Save = NULL;
	m_Gauge = NULL;
#if defined(__WXMSW__) && !defined(__MINGW32__)
	m_Taskbar = NULL;
#endif
    m_ShouldScroll = true;
}

//Create controls
void frmProgress::CreateControls(){
	frmProgress* Frame = this;

	wxPanel* Panel = new wxPanel(Frame, ID_PANEL_PROGRESS, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );

	wxBoxSizer* TopSizer = new wxBoxSizer(wxVERTICAL);
	Panel->SetSizer(TopSizer);

	wxBoxSizer* MiddleSizer = new wxBoxSizer(wxHORIZONTAL);
	TopSizer->Add(MiddleSizer, 1, wxGROW|wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* ProgressSizer = new wxBoxSizer(wxVERTICAL);
    MiddleSizer->Add(ProgressSizer, 1, wxGROW);

	m_Gauge = new wxGauge(Panel, ID_PROGRESS_GAUGE, 100);
	ProgressSizer->Add(m_Gauge, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxEXPAND, 5);

	ProgressGrid = new wxGrid(Panel, ID_PROGRESS_GRID, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS | wxBORDER_THEME);
	ProgressSizer->Add(ProgressGrid, 1, wxGROW|wxALL, 5);

    //Create the grid and set the various styles
    ProgressGrid->CreateGrid(1, 2, wxGrid::wxGridSelectRows);
    ProgressGrid->HideRowLabels();
    ProgressGrid->EnableEditing(false);
    ProgressGrid->EnableGridLines(false);
    ProgressGrid->UseNativeColHeader();
    ProgressGrid->SetColLabelValue(0, _("Time"));
    ProgressGrid->SetColLabelValue(1, _("Status"));
    ProgressGrid->SetColLabelAlignment(wxALIGN_LEFT, wxALIGN_CENTRE);
    ProgressGrid->SetColSize(0, 150);
    ProgressGrid->SetColSize(1, 200);

	wxBoxSizer* SmallButtonSizer = new wxBoxSizer(wxVERTICAL);
	MiddleSizer->Add(SmallButtonSizer, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

	m_Save = new wxBitmapButton(Panel, wxID_SAVE, GetBitmapResource(wxT("save.png")));
	SmallButtonSizer->Add(m_Save, 0, wxALL, 5);

	m_Autoscroll = new wxBitmapButton(Panel, ID_PROGRESS_AUTOSCROLL, GetBitmapResource(wxT("autoscroll.png")));
	SmallButtonSizer->Add(m_Autoscroll, 0, wxALL, 5);

	wxBoxSizer* ButtonSizer = new wxBoxSizer(wxHORIZONTAL);
	TopSizer->Add(ButtonSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	m_OK = new wxButton(Panel, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	ButtonSizer->Add(m_OK, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_Cancel = new wxButton(Panel, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	ButtonSizer->Add(m_Cancel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	//Set the form icon
	this->SetIcon(wxIcon(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + "Toucan.ico", wxBITMAP_TYPE_ICO));
}

//Get bitmap resources
wxBitmap frmProgress::GetBitmapResource(const wxString& name){
	wxString path = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("bitmaps") + wxFILE_SEP_PATH;
	wxBitmap bitmap(path + name, wxBITMAP_TYPE_PNG);
	if(bitmap.IsOk()){
		return bitmap;
	}
	return wxNullBitmap;
}

void frmProgress::OnClose(wxCloseEvent& WXUNUSED(event)){
	wxGetApp().m_LuaManager->NullWindow();
	Destroy();
}

void frmProgress::OnOkClick(wxCommandEvent& WXUNUSED(event)){
	Close();
}

void frmProgress::OnCancelClick(wxCommandEvent& WXUNUSED(event)){
	wxGetApp().SetAbort(true);
}

void frmProgress::OnSaveClick(wxCommandEvent& WXUNUSED(event)){
	wxFileDialog dialog(this, _("Save"), wxEmptyString, wxEmptyString, "Text Files (*.txt)|*.txt", wxFD_SAVE);
	if(dialog.ShowModal() == wxID_OK){
		wxTextFile file(dialog.GetPath());
		if(wxFileExists(dialog.GetPath())){
			file.Open();
			file.Clear();
			file.Write();
		}
		else{
			file.Create();
		}
        for(int i = 0; i < ProgressGrid->GetNumberRows(); i++){
            file.AddLine(ProgressGrid->GetCellValue(i, 0) + wxT("\t") + ProgressGrid->GetCellValue(i, 1));
	    }
	    file.Write();
	}
}

#if defined(__WXMSW__) && !defined(__MINGW32__)
WXLRESULT frmProgress::MSWWindowProc(WXUINT message, WXWPARAM wparam, WXLPARAM lparam){
	if(message == m_TaskBarId){
		int major = wxPlatformInfo::Get().GetOSMajorVersion();
		int minor = wxPlatformInfo::Get().GetOSMajorVersion();
		//Only supported on windows 7 and greater
		if(major >= 6 && minor >= 1){
			if(m_Taskbar){
				m_Taskbar->Release();
			}
			
			CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_ALL, IID_ITaskbarList3, (void**)&m_Taskbar);
			m_Taskbar->HrInit();
		}
	}
	return wxFrame::MSWWindowProc(message, wparam, lparam);
}
#endif

void frmProgress::IncrementGauge(){
	m_Gauge->SetValue(m_Gauge->GetValue() + 1);
#if defined(__WXMSW__) && !defined(__MINGW32__)
	if(m_Taskbar){
		m_Taskbar->SetProgressValue (static_cast<HWND>(GetHandle()), m_Gauge->GetValue() + 1, m_Gauge->GetRange());
	}
#endif

}

void frmProgress::FinishGauge(){
	m_Gauge->SetValue(m_Gauge->GetRange());
#if defined(__WXMSW__) && !defined(__MINGW32__)
	if(m_Taskbar){
		m_Taskbar->SetProgressValue (static_cast<HWND>(GetHandle()), m_Gauge->GetRange(), m_Gauge->GetRange());
	}
#endif
}

void frmProgress::OnAutoscrollClick(wxCommandEvent& WXUNUSED(event)){
    m_ShouldScroll = !m_ShouldScroll;
}
