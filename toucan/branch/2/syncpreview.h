/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2007 Steven Lamerton
// Licence:     GNU GPL 2 (See readme for more info
/////////////////////////////////////////////////////////////////////////////////

#include "basicfunctions.h"
#include "frmprogress.h"
#include <wx/dir.h>


bool PreviewSyncLoop(SyncData data, Rules rules, frmMain *window, wxTreeItemId idParent));
bool PreviewSyncFile(SyncData data, Rules rules, frmMain *window, wxTreeItemId idParent));


class PreviewSyncThread : public wxThread
{
public:
	//Constructor
	PreviewSyncThread(SyncData data, Rules rules, frmProgress* window, frmMain *main){
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
	m_Window->m_Sync_Dest_Tree->DeleteAllItems();
	wxTreeItemId parent = m_Window->m_Sync_Dest_Tree->AppendItem(m_Window->m_Sync_Dest_Tree->GetRootItem(), m_Data.GetSource().AfterLast(wxFILE_SEP_PATH));
	wxMutexGuiLeave();
	//Launch the correct set of loops
	if(m_Data.GetFunction() == _("Copy") || m_Data.GetFunction() == _("Update")){
		PreviewSyncLoop(m_Data, m_Rules, m_Window, parent);
	}
	else if(m_Data.GetFunction() ==  _("Mirror (Copy)")){
		m_Data.SetFunction(_("Copy"));
		PreviewSyncLoop(m_Data, m_Rules, m_Window, parent);
		//Swap the source and dest around for the mirror routine
		wxString strTemp = m_Data.GetSource();
		m_Data.SetSource(m_Data.GetDest());
		m_Data.SetDest(strTemp);
		//Run the mirror loop
		m_Data.SetFunction(_("Mirror"));
		PreviewSyncLoop(m_Data, m_Rules, m_Window, parent);
	}
	else if(m_Data.GetFunction() ==  _("Mirror (Copy)")){
		m_Data.SetFunction(_("Update"));
		PreviewSyncLoop(m_Data, m_Rules, m_Window, parent);
		//Swap the source and dest around for the mirror routine
		wxString strTemp = m_Data.GetSource();
		m_Data.SetSource(m_Data.GetDest());
		m_Data.SetDest(strTemp);
		//Run the mirror loop
		m_Data.SetFunction(_("Mirror"));
		PreviewSyncLoop(m_Data, m_Rules, m_Window, parent);
	}
	else if(m_Data.GetFunction() ==  _("Equalise")){
		m_Data.SetFunction(_("Update"));
		PreviewSyncLoop(m_Data, m_Rules, m_Window, parent);
		wxString strTemp = m_Data.GetSource();
		m_Data.SetSource(m_Data.GetDest());
		m_Data.SetDest(strTemp);
		m_Data.SetFunction(_("Update"));
		PreviewSyncLoop(m_Data, m_Rules, m_Window, parent);
	}
	/*wxMutexGuiEnter();
	m_Main->m_Sync_Source_Tree->DeleteAllItems();
	m_Main->m_Sync_Dest_Tree->DeleteAllItems();
	AddDirToTree(m_Main->m_Sync_Source_Txt->GetValue(), m_Main->m_Sync_Source_Tree);
	AddDirToTree(m_Main->m_Sync_Dest_Txt->GetValue(), m_Main->m_Sync_Dest_Tree);
	m_Window->m_OK->Enable(true);
	m_Window->m_Save->Enable(true);
	m_Window->m_Cancel->Enable(false);
	wxMutexGuiLeave();
	OutputProgress(_("Finished..."), m_Window);*/
	
	return NULL;

}

bool PreviewSyncLoop(SyncData data, Rules rules, frmMain *window, wxTreeItemId idParent))
{
	//wxMessageBox(_("In the loop"));
	wxString strTo = data.GetDest();
	wxString strFrom = data.GetSource();
	//This section need to be updated for the new data stuff
	//Append a slash in case of an incorrect pass
	if (strTo[strTo.length()-1] != wxFILE_SEP_PATH) {
		strTo += wxFILE_SEP_PATH;       
	}
	// for both dirs
	if (strFrom[strFrom.length()-1] != wxFILE_SEP_PATH) {
		strFrom += wxFILE_SEP_PATH;       
	}
	if (!wxDirExists(strTo)){
            	if(!rules.ShouldExclude(strTo, true)){
            		//wxMkdir(strTo);
		}
		else{
			return false;
		}
	}
	wxDir dir(strFrom);
	wxString strFilename;
	bool blDir = dir.GetFirst(&strFilename);
	if(blDir){
		do {
			if(wxDirExists(strFrom + strFilename))
			{
				if(!rules.ShouldExclude(strTo + strFilename, true))
				{ 
					if (wxDirExists(strTo + strFilename)){
						data.SetSource(strFrom + strFilename);
						data.SetDest(strTo + strFilename);
						wxTreeItemId tempidParent = window->m_Sync_Dest_Tree->AppendItem(idParent, strFilename.AfterLast(wxFILE_SEP_PATH));
						PreviewSyncLoop(data, rules, window, tempidParent);
					}
					else{
						if(data.GetFunction() == _("Mirror")){
							//PreivewRemoveDirectory(strTo + strFilename);
						}
						else{
							//wxMkdir(strTo + strFilename);
							data.SetSource(strFrom + strFilename);
							data.SetDest(strTo + strFilename);
							wxTreeItemId tempidParent = window->m_Sync_Dest_Tree->AppendItem(idParent, strFilename.AfterLast(wxFILE_SEP_PATH));
							PreviewSyncLoop(data, rules, window, tempidParent);
						}
					}
				}
			}
			else{
				data.SetSource(strFrom + strFilename);
				data.SetDest(strTo + strFilename);
				PreviewSyncFile(data, rules, window, idParent);
			}
		}
		while (dir.GetNext(&strFilename) );
	}  
	return true;
	
}

bool PreviewSyncFile(SyncData data, Rules rules, frmMain *window, wxTreeItemId idParent))
{
	//int iAttributes = FILE_ATTRIBUTE_NORMAL;
	if(!rules.ShouldExclude(data.GetDest(), false)){
		/*if(data.GetIgnoreRO()){
			iAttributes = GetFileAttributes(data.GetDest());
			SetFileAttributes(data.GetDest(),FILE_ATTRIBUTE_NORMAL); 
		} */
		if(data.GetFunction() == _("Copy")){	
			//if(wxCopyFile(data.GetSource(), data.GetDest(), true)){
				OutputProgress(_("Copied ") + data.GetSource().AfterLast(wxFILE_SEP_PATH) + _(" to ") + data.GetSource(), window);
			//}
		}	
		if(data.GetFunction() == _("Update")){
			/*Check to see if the desination file exists, if it does then a time check is made, if not then 
			the source file is always copied*/
			if(wxFileExists(data.GetDest())){	
				wxDateTime tmTo = wxFileModificationTime(data.GetDest());
				wxDateTime tmFrom = wxFileModificationTime(data.GetSource());
				if(data.GetIgnoreDLS()){
					tmFrom.MakeTimezone(wxDateTime::Local, true);
				}
				//I.E. strFrom is newer
				if(tmFrom.IsLaterThan(tmTo)){
					//wxCopyFile(data.GetSource(), data.GetDest(), true);
					OutputProgress(_("Updated ") + data.GetSource().AfterLast(wxFILE_SEP_PATH) + _(" in ") + data.GetSource(), window);
				}
			}
			else{
				//wxCopyFile(data.GetSource(), data.GetDest(), true);
				OutputProgress(_("Copied ") + data.GetSource().AfterLast(wxFILE_SEP_PATH)+  _(" to ") + data.GetSource(), window);
			}
		}
		if(data.GetFunction() == _("Mirror")){	
			if(!wxFileExists(data.GetDest())){
				//wxRemoveFile(data.GetSource());
				OutputProgress(_("Removed ") + data.GetSource(), window);
			}
		}
		//Set the old attrributes back
		/*if(data.GetIgnoreRO()){
			SetFileAttributes(data.GetDest(), iAttributes); 
		} 
		//Code needs to be added for Linux, Mac also needs to be researched
		if(data.GetAttributes() == true){
			//#ifdef(__WXMSW__)
			int filearrtibs = GetFileAttributes(data.GetSource());
			SetFileAttributes(data.GetDest(),FILE_ATTRIBUTE_NORMAL);                       
			SetFileAttributes(data.GetDest(),filearrtibs);
			//#endif
		}
		if(data.GetTimeStamps()){
			wxFileName from(data.GetSource());
			wxFileName to(data.GetDest());
			wxDateTime access, mod, created;
			from.GetTimes(&access ,&mod ,&created );
			to.SetTimes(&access ,&mod , &created); 
		}	*/		
	}
	return true;
}
