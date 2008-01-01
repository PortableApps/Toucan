#ifndef H_SYNCTHREAD
#define H_SYNCTHREAD

#include "syncdata.h"
#include "sync.h"

class SyncThread : public wxThread
{
public:
	//Constructor
	SyncThread(SyncData data, Rules rules, frmProgress* window, frmMain *main){
		m_Data = data;
		m_Rules = rules;
		m_Window = window;
		m_Main = main;
	}
	//Main function, all stuff goes in here
	virtual void *Entry();

private:
	SyncData m_Data;
	Rules m_Rules;
	frmProgress* m_Window;
	frmMain* m_Main;
};

void *SyncThread::Entry(){
	wxMutexGuiEnter();
	m_Window->m_OK->Enable(false);
	m_Window->m_Save->Enable(false);
	wxMutexGuiLeave();
	OutputProgress(_("Starting..."), m_Window);
	//Launch the correct set of loops
	if(m_Data.GetFunction() == _("Copy") || m_Data.GetFunction() == _("Update")){
		SyncLoop(m_Data, m_Rules, m_Window);
	}
	else if(m_Data.GetFunction() ==  _("Mirror (Copy)")){
		m_Data.SetFunction(_("Copy"));
		SyncLoop(m_Data, m_Rules, m_Window);
		//Swap the source and dest around for the mirror routine
		wxString strTemp = m_Data.GetSource();
		m_Data.SetSource(m_Data.GetDest());
		m_Data.SetDest(strTemp);
		//Run the mirror loop
		m_Data.SetFunction(_("Mirror"));
		SyncLoop(m_Data, m_Rules, m_Window);
	}
	else if(m_Data.GetFunction() ==  _("Mirror (Copy)")){
		m_Data.SetFunction(_("Update"));
		SyncLoop(m_Data, m_Rules, m_Window);
		//Swap the source and dest around for the mirror routine
		wxString strTemp = m_Data.GetSource();
		m_Data.SetSource(m_Data.GetDest());
		m_Data.SetDest(strTemp);
		//Run the mirror loop
		m_Data.SetFunction(_("Mirror"));
		SyncLoop(m_Data, m_Rules, m_Window);
	}
	else if(m_Data.GetFunction() ==  _("Equalise")){
		m_Data.SetFunction(_("Update"));
		wxString strTemp = m_Data.GetSource();
		m_Data.SetSource(m_Data.GetDest());
		m_Data.SetDest(strTemp);
		m_Data.SetFunction(_("Update"));
		SyncLoop(m_Data, m_Rules, m_Window);
	}
	wxMutexGuiEnter();
	m_Main->m_Sync_Source_Tree->DeleteAllItems();
	m_Main->m_Sync_Dest_Tree->DeleteAllItems();
	AddDirToTree(m_Main->m_Sync_Source_Txt->GetValue(), m_Main->m_Sync_Source_Tree);
	AddDirToTree(m_Main->m_Sync_Dest_Txt->GetValue(), m_Main->m_Sync_Dest_Tree);
	m_Window->m_OK->Enable(true);
	m_Window->m_Save->Enable(true);
	m_Window->m_Cancel->Enable(false);
	wxMutexGuiLeave();
	OutputProgress(_("Finished..."), m_Window);
	
	return NULL;

}

#endif
