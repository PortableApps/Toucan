/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "sync.h"
#include "basicfunctions.h"
#include "frmprogress.h"
#include "frmmain.h"
#include "toucan.h"
#include "variables.h"

//Include the hashlib++ library for MD5 support
#include <hashlibpp.h>

#include <wx/dir.h>

void *SyncThread::Entry(){
	//Launch the correct set of loops
	if(m_Data.GetFunction() == _("Copy") || m_Data.GetFunction() == _("Update")){
		SyncLoop(m_Data, m_Rules);
		if(m_Data.GetTimeStamps()){
			FolderTimeLoop(m_Data.GetSource(), m_Data.GetDest());		
		}
	}
	else if(m_Data.GetFunction() ==  _("Mirror (Copy)")){
		m_Data.SetFunction(_("Copy"));
		SyncLoop(m_Data, m_Rules);
		if(m_Data.GetTimeStamps()){
			FolderTimeLoop(m_Data.GetSource(), m_Data.GetDest());
		}
		//Swap the source and dest around for the mirror routine
		wxString strTemp = m_Data.GetSource();
		m_Data.SetSource(m_Data.GetDest());
		m_Data.SetDest(strTemp);
		//Run the mirror loop
		m_Data.SetFunction(_("Mirror (Copy)"));
		SyncLoop(m_Data, m_Rules);
		if(m_Data.GetTimeStamps()){
			FolderTimeLoop(m_Data.GetSource(), m_Data.GetDest());
		}
	}
	else if(m_Data.GetFunction() ==  _("Mirror (Update)")){
		m_Data.SetFunction(_("Update"));
		SyncLoop(m_Data, m_Rules);
		if(m_Data.GetTimeStamps()){
			FolderTimeLoop(m_Data.GetSource(), m_Data.GetDest());
		}
		//Swap the source and dest around for the mirror routine
		wxString strTemp = m_Data.GetSource();
		m_Data.SetSource(m_Data.GetDest());
		m_Data.SetDest(strTemp);
		//Run the mirror loop
		m_Data.SetFunction(_("Mirror (Update)"));
		SyncLoop(m_Data, m_Rules);
		if(m_Data.GetTimeStamps()){
			FolderTimeLoop(m_Data.GetSource(), m_Data.GetDest());
		}
	}
	else if(m_Data.GetFunction() ==  _("Equalise")){
		m_Data.SetFunction(_("Update"));
		SyncLoop(m_Data, m_Rules);
		if(m_Data.GetTimeStamps()){
			FolderTimeLoop(m_Data.GetSource(), m_Data.GetDest());
		}
		wxString strTemp = m_Data.GetSource();
		m_Data.SetSource(m_Data.GetDest());
		m_Data.SetDest(strTemp);
		m_Data.SetFunction(_("Update"));
		SyncLoop(m_Data, m_Rules);
		if(m_Data.GetTimeStamps()){
			FolderTimeLoop(m_Data.GetSource(), m_Data.GetDest());
		}
	}
	if(wxGetApp().blGUI){
		wxMutexGuiEnter();
		m_Main->m_Sync_Source_Tree->DeleteAllItems();
		m_Main->m_Sync_Dest_Tree->DeleteAllItems();
		
		m_Main->m_Sync_Source_Tree->AddRoot(_("Hidden text"));
		m_Main->m_Sync_Dest_Tree->AddRoot(_("Hidden text"));
		
		m_Main->m_Sync_Source_Tree->AddNewPath(Normalise(Normalise(m_Main->m_Sync_Source_Txt->GetValue())));
		m_Main->m_Sync_Dest_Tree->AddNewPath(Normalise(Normalise(m_Main->m_Sync_Dest_Txt->GetValue())));		
		wxMutexGuiLeave();
	}
	wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, ID_SCRIPTFINISH);
	wxPostEvent(wxGetApp().MainWindow, event);
	//As we are finished cancel any aborts
	wxGetApp().SetAbort(false);
	return NULL;

}

bool SyncLoop(SyncData data, Rules rules)
{
	if(wxGetApp().ShouldAbort()){
		return true;
	}
	wxString strTo = data.GetDest();
	wxString strFrom = data.GetSource();
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
			wxMkdir(strTo);
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
			if(wxGetApp().ShouldAbort()){
				return true;
			}
			if(wxDirExists(strFrom + strFilename))
			{
				if(!rules.ShouldExclude(strTo + strFilename, true))
				{ 
					if (wxDirExists(strTo + strFilename)){
						data.SetSource(strFrom + strFilename);
						data.SetDest(strTo + strFilename);
						SyncLoop(data, rules);
					}
					else{
						//wxMessageBox(_("Doesnt exists"));
						if(data.GetFunction() == _("Mirror (Copy)") || data.GetFunction() == _("Mirror (Update)")){
							DirectoryRemove(strFrom + strFilename);
						}
						else{
							wxMkdir(strTo + strFilename);
							data.SetSource(strFrom + strFilename);
							data.SetDest(strTo + strFilename);
							SyncLoop(data, rules);
						}
					}
				}
			}
			else{
				data.SetSource(strFrom + strFilename);
				data.SetDest(strTo + strFilename);
				SyncFile(data, rules);
			}
		}
		while (dir.GetNext(&strFilename) );
	} 
	return true;
	
}

bool SyncFile(SyncData data, Rules rules)
{
	if(wxGetApp().ShouldAbort()){
		return true;
	}
	bool ShouldTimeStamp = false;
	#ifdef __WXMSW__
		int iAttributes = FILE_ATTRIBUTE_NORMAL;
	#endif
	if(!rules.ShouldExclude(data.GetDest(), false)){
		if(data.GetIgnoreRO()){
			#ifdef __WXMSW__
				iAttributes = GetFileAttributes(data.GetDest());
				SetFileAttributes(data.GetDest(),FILE_ATTRIBUTE_NORMAL); 
			#endif
		} 
		if(data.GetFunction() == _("Copy")){	
			if(wxCopyFile(data.GetSource(), wxPathOnly(data.GetDest()) + wxFILE_SEP_PATH + wxT("Toucan.tmp"), true)){
				if(wxRenameFile(wxPathOnly(data.GetDest()) + wxFILE_SEP_PATH + wxT("Toucan.tmp"), data.GetDest(), true)){
					OutputProgress(data.GetSource() + _("\t copied to \t") + data.GetDest());
					ShouldTimeStamp = true;
				}
			}
			if(wxFileExists(wxPathOnly(data.GetDest()) + wxFILE_SEP_PATH + wxT("Toucan.tmp"))){
				wxRemoveFile(wxPathOnly(data.GetDest()) + wxFILE_SEP_PATH + wxT("Toucan.tmp"));
			}
		}	
		if(data.GetFunction() == _("Update")){
			/*Check to see if the desination file exists, if it does then a time check is made, if not then 
			the source file is always copied*/
			if(wxFileExists(data.GetDest())){	
				wxDateTime tmTo, tmFrom;
				wxFileName flTo(data.GetDest());
				wxFileName flFrom(data.GetSource());
				
				flTo.GetTimes(NULL, &tmTo, NULL);
				flFrom.GetTimes(NULL, &tmFrom, NULL);		
				
				if(data.GetIgnoreDLS()){
					tmFrom.MakeTimezone(wxDateTime::Local, true);
				}
				//Check to see if the source file is newer than the destination file, subtracts one hour to make up for timestamp errors
				if(tmFrom.IsLaterThan(tmTo)){
					hashwrapper *myWrapper = new md5wrapper();
					std::string sourceHash = myWrapper->getHashFromFile(std::string(data.GetSource().mb_str()));
					std::string desinationHash = myWrapper->getHashFromFile(std::string(data.GetDest().mb_str()));
					delete myWrapper;
					if(sourceHash != desinationHash){
						if(wxCopyFile(data.GetSource(), wxPathOnly(data.GetDest()) + wxFILE_SEP_PATH + wxT("Toucan.tmp"), true)){
							if(wxRenameFile(wxPathOnly(data.GetDest()) + wxFILE_SEP_PATH + wxT("Toucan.tmp"), data.GetDest(), true)){
								OutputProgress(data.GetSource() + _("\t updated \t") + data.GetDest());
								ShouldTimeStamp = true;
							}
						}
						if(wxFileExists(wxPathOnly(data.GetDest()) + wxFILE_SEP_PATH + wxT("Toucan.tmp"))){
							wxRemoveFile(wxPathOnly(data.GetDest()) + wxFILE_SEP_PATH + wxT("Toucan.tmp"));
						}
					}
				}
			}
			else{
				if(wxCopyFile(data.GetSource(), wxPathOnly(data.GetDest()) + wxFILE_SEP_PATH + wxT("Toucan.tmp"), true)){
					if(wxRenameFile(wxPathOnly(data.GetDest()) + wxFILE_SEP_PATH + wxT("Toucan.tmp"), data.GetDest(), true)){
						OutputProgress(data.GetSource() + _("\t copied to \t") + data.GetDest());
						ShouldTimeStamp = true;
					}
				}
				if(wxFileExists(wxPathOnly(data.GetDest()) + wxFILE_SEP_PATH + wxT("Toucan.tmp"))){
					wxRemoveFile(wxPathOnly(data.GetDest()) + wxFILE_SEP_PATH + wxT("Toucan.tmp"));
				}
			}
		}
		if(data.GetFunction() == _("Mirror (Copy)") || data.GetFunction() == _("Mirror (Update)")){	
			if(!wxFileExists(data.GetDest())){
				wxRemoveFile(data.GetSource());
				OutputProgress(_("Removed ") + data.GetSource());
			}
		}
		if(wxGetApp().ShouldAbort()){
			return true;
		}
		//Set the old attrributes back
		if(data.GetIgnoreRO()){
			#ifdef __WXMSW__
				SetFileAttributes(data.GetDest(), iAttributes); 
			#endif
		} 
	
		if(data.GetAttributes() == true){
			#ifdef __WXMSW__
				int filearrtibs = GetFileAttributes(data.GetSource());
				SetFileAttributes(data.GetDest(),FILE_ATTRIBUTE_NORMAL);                       
				SetFileAttributes(data.GetDest(),filearrtibs);
			#endif
		}
		if(data.GetTimeStamps() && ShouldTimeStamp){
			wxFileName from(data.GetSource());
			wxFileName to(data.GetDest());
			wxDateTime access, mod, created;
			from.GetTimes(&access ,&mod ,&created );
			to.SetTimes(&access ,&mod , &created); 
		}			
	}
	return true;
}


bool DirectoryRemove(wxString strLocation){
	if(wxGetApp().ShouldAbort()){
		return true;
	}
	// if it's a possible root directory
	if (strLocation.length() <= 3) {
		wxLogError(_("Toucan tried to delete a root directory. This has been forbidden for your own safety"));
		return false;
	}
	//Make sure that the correct ending is appended
	if (strLocation[strLocation.length()-1] != wxFILE_SEP_PATH) {
		strLocation += wxFILE_SEP_PATH;       
	}
	
	wxDir* dir = new wxDir(strLocation);
	wxString strFilename;
	bool blDir = dir->GetFirst(&strFilename);
	if(blDir){
		do {
			if(wxGetApp().ShouldAbort()){
				return true;
			}
			if(wxDirExists(strLocation + strFilename)){
				DirectoryRemove(strLocation + strFilename);
			}
			else{
				if(wxRemoveFile(strLocation + strFilename)){
                    OutputProgress(_("Removed ") + strLocation + strFilename);
                }
            }
	
		}
		while (dir->GetNext(&strFilename) );
	} 
	delete dir;
  	wxLogNull log;
	if(wxFileName::Rmdir(strLocation)){
        OutputProgress(_("Removed ") + strLocation);
    }
	return true;
}


bool FolderTimeLoop(wxString strFrom, wxString strTo){
	if(wxGetApp().ShouldAbort()){
		return false;
	}
	//Make sure that the correct ending is appended
	if (strTo[strTo.length()-1] != wxFILE_SEP_PATH) {
		strTo += wxFILE_SEP_PATH;       
	}
	// for both dirs
	if (strFrom[strFrom.length()-1] != wxFILE_SEP_PATH) {
		strFrom += wxFILE_SEP_PATH;       
	}
	
	wxDir* dir = new wxDir(strFrom);
	wxString strFilename;
	bool blDir = dir->GetFirst(&strFilename);
	if(blDir){
		do {
			if(wxGetApp().ShouldAbort()){
				return false;	
			}
			if(wxDirExists(strTo + strFilename)){
				FolderTimeLoop(strFrom + strFilename, strTo + strFilename);
			}
		}
		while (dir->GetNext(&strFilename) );
	} 
	delete dir;
	if (wxDirExists(strTo)){
		#ifdef __WXMSW__
			//Need to tidy up this code and submit as a patch to wxWidgets
			FILETIME ftCreated,ftAccessed,ftModified;
			HANDLE hFrom = CreateFile(strFrom, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
			if(hFrom == INVALID_HANDLE_VALUE){
			  return false;
			}  
			
			GetFileTime(hFrom,&ftCreated,&ftAccessed,&ftModified);
			CloseHandle(hFrom);
			HANDLE hTo = CreateFile(strTo, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
			if(hTo == INVALID_HANDLE_VALUE){
			  return false;
			}  
			SetFileTime(hTo,&ftCreated,&ftAccessed,&ftModified);
			CloseHandle(hTo);
		#elif defined(__WXGTK__)
			wxFileName from(strTo);
			wxFileName to(strFrom);
			wxDateTime access, mod, created;
			from.GetTimes(&access ,&mod ,&created );
			to.SetTimes(&access ,&mod , &created); 
		#endif
		OutputProgress(_("Set folder timestamps for ") + strTo);
	}	
	return true;
}
