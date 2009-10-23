/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "frmmain.h"
#include "frmprogress.h"
#include "../variables.h"
#include "../script.h"
#include "../toucan.h"
#include "../controls/vdtc.h"

#include <wx/wx.h>
#include <wx/stdpaths.h>
#include <wx/listctrl.h>
#include <wx/textfile.h>
#include <algorithm>

//frmProgress event table
BEGIN_EVENT_TABLE(frmProgress, wxFrame)
	EVT_CLOSE(frmProgress::OnCloseWindow)
	EVT_BUTTON(wxID_OK, frmProgress::OnOkClick)
	EVT_BUTTON(wxID_CANCEL, frmProgress::OnCancelClick)
	EVT_BUTTON(wxID_SAVE, frmProgress::OnSaveClick)
	
	//ScriptManager
	EVT_BUTTON(ID_SCRIPTFINISH, frmProgress::OnScriptFinish)
	EVT_BUTTON(ID_SCRIPTTEXT, frmProgress::OnScriptText)
	EVT_BUTTON(ID_SCRIPTTIME, frmProgress::OnScriptTime)
	EVT_BUTTON(ID_SCRIPTERROR, frmProgress::OnScriptError)
	EVT_BUTTON(ID_SCRIPTBLANK, frmProgress::OnScriptBlank)

END_EVENT_TABLE()

//Constructor
frmProgress::frmProgress(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style){
	Init();
	wxFrame::Create( parent, id, caption, pos, size, style );
	CreateControls();
	Centre();
}

//frmProgress initialisation
void frmProgress::Init(){
	m_List = NULL;
	m_OK = NULL;
	m_Cancel = NULL;
	m_Save = NULL;
	m_Gauge = NULL;
}

//Create controls
void frmProgress::CreateControls(){
	frmProgress* Frame = this;

	wxPanel* Panel = new wxPanel(Frame, ID_PANEL_PROGRESS, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );

	wxBoxSizer* TopSizer = new wxBoxSizer(wxVERTICAL);
	Panel->SetSizer(TopSizer);

	m_Gauge = new wxGauge(Panel, ID_PROGRESS_GAUGE, 100, wxDefaultPosition, wxDefaultSize, wxGA_SMOOTH|wxGA_HORIZONTAL);
	TopSizer->Add(m_Gauge, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxEXPAND, 5);

	m_List = new wxListCtrl(Panel, ID_PROGRESS_LIST, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_VRULES);
	TopSizer->Add(m_List, 1, wxGROW|wxALL, 5);

	wxBoxSizer* ButtonSizer = new wxBoxSizer(wxHORIZONTAL);
	TopSizer->Add(ButtonSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	m_OK = new wxButton(Panel, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	ButtonSizer->Add(m_OK, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_Cancel = new wxButton(Panel, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	ButtonSizer->Add(m_Cancel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_Save = new wxButton(Panel, wxID_SAVE, _("Save"), wxDefaultPosition, wxDefaultSize, 0 );
	ButtonSizer->Add(m_Save, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
	//Add columns
	m_List->InsertColumn(0, _("Time"));
	m_List->InsertColumn(1, _("Status"));
	
	//Set the form icon
	wxString strPath = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH;
	this->SetIcon(wxIcon(strPath + wxT("Toucan.ico"), wxBITMAP_TYPE_ICO));
}

void frmProgress::OnOkClick(wxCommandEvent& WXUNUSED(event)){
	this->MakeModal(false);
	this->Show(false);
}

void frmProgress::OnCancelClick(wxCommandEvent& WXUNUSED(event)){
	wxGetApp().SetAbort(true);
}

void frmProgress::OnSaveClick(wxCommandEvent& WXUNUSED(event)){
	wxString strCaption = _("Save");
	wxString strWildcard;
	strWildcard = wxT("Text Files (*.txt)|*.txt");
	wxString defaultFilename = wxEmptyString;
	wxString defaultDir = wxT("/");
	wxFileDialog dialog(this, strCaption, defaultDir, defaultFilename, strWildcard, wxFD_SAVE);
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
		for(int i = 0; i < m_List->GetItemCount() - 1; i++){
			wxListItem itemcol1, itemcol2;

			itemcol1.m_itemId = i;
			itemcol1.m_col = 0;
			itemcol1.m_mask = wxLIST_MASK_TEXT;
			m_List->GetItem(itemcol1);
			itemcol2.m_itemId = i;
			itemcol2.m_col = 1;
			itemcol2.m_mask = wxLIST_MASK_TEXT;
			m_List->GetItem(itemcol2);
			file.AddLine(itemcol1.m_text + wxT("\t") + itemcol2.m_text);
		}
	file.Write();
	}
}

void frmProgress::OnCloseWindow(wxCloseEvent& WXUNUSED(event)){
	this->MakeModal(false);
	this->Show(false);
}

//ID_SCRIPTFINISH
void frmProgress::OnScriptFinish(wxCommandEvent& event){
	if(wxGetApp().m_Script->GetCommand() < wxGetApp().m_Script->GetCount()){
		wxGetApp().m_Script->SetCommand(wxGetApp().m_Script->GetCommand() + 1);
		wxGetApp().m_Script->ParseCommand(wxGetApp().m_Script->GetCommand() - 1);
	}
	else{
		wxGetApp().m_Script->CleanUp();
		wxGetApp().m_Script->SetCommand(wxGetApp().m_Script->GetCommand() + 1);
		//The int specifies which sort of job has just finished so we can refresh it correctly
		//1 - Sync
		//Others - Not yet implmented
		if(event.GetInt() == 1){
			if(wxGetApp().GetUsesGUI()){
				//Turn off the preview
				wxGetApp().MainWindow->m_Sync_Source_Tree->SetSync(false);
				wxGetApp().MainWindow->m_Sync_Dest_Tree->SetSync(false);
				
				wxGetApp().MainWindow->m_Sync_Source_Tree->DeleteAllItems();
				wxGetApp().MainWindow->m_Sync_Dest_Tree->DeleteAllItems();
				
				wxGetApp().MainWindow->m_Sync_Source_Tree->AddRoot(wxT("Hidden text"));
				wxGetApp().MainWindow->m_Sync_Dest_Tree->AddRoot(wxT("Hidden text"));
				
				wxGetApp().MainWindow->m_Sync_Source_Tree->AddNewPath(Normalise(wxGetApp().MainWindow->m_Sync_Source_Txt->GetValue()));
				wxGetApp().MainWindow->m_Sync_Dest_Tree->AddNewPath(Normalise(wxGetApp().MainWindow->m_Sync_Dest_Txt->GetValue()));		
			}
		}
		//If we are in console then set the global variable to let Toucan know it has finished
		if(!wxGetApp().GetUsesGUI()){
			wxGetApp().SetFinished(true);
		}
		#ifdef __WXMSW__
			FLASHWINFO info;
			info.uCount = 3;
			info.dwTimeout = 0;
			info.hwnd = (HWND) this->GetHWND();
			info.cbSize = sizeof(info);
			if(wxWindow::FindFocus() == this){
				info.dwFlags = FLASHW_ALL|FLASHW_TIMERNOFG;			
			}
			else{
				info.dwFlags = FLASHW_ALL;					
			}
			FlashWindowEx(&info);
		#else
			RequestUserAttention();
		#endif
	}
}

//ID_SCRIPTTEXT
void frmProgress::OnScriptText(wxCommandEvent& event){
	if(event.GetString() == wxEmptyString){
		return;
	}
	long location = std::max(0, m_List->GetItemCount() - 1);
	long index = m_List->InsertItem(location, wxEmptyString);
	m_List->SetItem(index, 1, event.GetString());
	m_List->EnsureVisible(index);
	Update();
}

//ID_SCRIPTTEXT
void frmProgress::OnScriptTime(wxCommandEvent& event){
	long location = std::max(0, m_List->GetItemCount() - 1);
	long index = m_List->InsertItem(location, wxEmptyString);
	m_List->SetItem(index, 1, event.GetString());
	m_List->SetItem(index, 0, event.GetString().Left(event.GetInt()));
	m_List->SetItem(index, 1, event.GetString().Right(event.GetString().Length() - event.GetInt()));
	m_List->EnsureVisible(index);
	Update();
}

//ID_SCRIPTERROR
void frmProgress::OnScriptError(wxCommandEvent& event){
	long location = std::max(0, m_List->GetItemCount() - 1);
	long index = m_List->InsertItem(location, wxEmptyString);
	m_List->SetItem(index, 1, event.GetString());
	m_List->SetItem(index, 0, event.GetString().Left(event.GetInt()));
	m_List->SetItem(index, 1, event.GetString().Right(event.GetString().Length() - event.GetInt()));
	m_List->SetItemTextColour(index, wxColour(wxT("Red")));
	m_List->EnsureVisible(index);
	Update();
}

//ID_SCRIPTBLANK
void frmProgress::OnScriptBlank(wxCommandEvent& WXUNUSED(event)){
	long location = std::max(0, m_List->GetItemCount() - 1);
	long index = m_List->InsertItem(location, wxEmptyString);
	m_List->SetItem(index, 1, wxEmptyString);
}
