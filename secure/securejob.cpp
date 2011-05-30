/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 - 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include "securejob.h"
#include "../toucan.h"
#include "../rules.h"
#include "../path.h"
#include "../basicfunctions.h"
#include "../data/securedata.h"
#include "../forms/frmmain.h"

#include <wx/filefn.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>

SecureJob::SecureJob(SecureData *Data) : Job(Data){
	;
}

void* SecureJob::Entry(){
	SecureData *data = static_cast<SecureData*>(GetData());

	wxArrayString locations = data->GetLocations();

	for(unsigned int i = 0; i < locations.GetCount(); i++){
        data->SetLocation(i, Path::Normalise(data->GetLocation(i)));
	}

	//Iterate through the entries in the array
	for(unsigned int i = 0; i < locations.Count(); i++){
        Crypt(locations.Item(i), data);
	}
	return NULL;
}

bool SecureJob::Crypt(const wxString &path, SecureData *data)
{   
	if(wxGetApp().GetAbort()){
		return true;
	}
    if(wxDirExists(path)){
	    wxDir dir(path);
	    wxString filename;
	    if(dir.GetFirst(&filename)){
		    do{
                wxFileName location = wxFileName(path + filename);
                RuleResult result = data->GetRules()->Matches(location);

                if((result == Excluded && location.IsDir()) || 
                  ((result == Included || result == NoMatch))){
                    //We recurse into subdirectories or to crypt files
                    Crypt(path + wxFILE_SEP_PATH + filename, data);
                }
                else{
                    //Do nothing as we are either an excluded file or an 
                    //absolutely excluded folder
                }
		    }
		    while (dir.GetNext(&filename) );
	    }
    }
    else{
        RuleResult res = data->GetRules()->Matches(wxFileName::FileName(path));
        if(res != Excluded && res != AbsoluteExcluded){
            CryptFile(path, data);
        }
    }
	return true;
}


bool SecureJob::CryptFile(const wxString &path, SecureData *data){
    if(wxGetApp().GetAbort())
        return false;

    //Make sure that it is a 'real' file
    wxFileName filename(path);
    if(filename.IsOk() && filename.GetSize() == 0)
        return false;

	//Ensure that we are not encrypting an already encrypted file or decrypting a non encrypted file
	if((filename.GetExt() == wxT("cpt") && data->GetFunction() == _("Encrypt"))
	||(filename.GetExt() != wxT("cpt") && data->GetFunction() == _("Decrypt"))
    ||(wxFileExists(path + wxT(".cpt")) && data->GetFunction() == _("Encrypt"))
    ||(wxFileExists(path.Left(path.length() - 4)) && data->GetFunction() == _("Decrypt"))){
        OutputProgress(_("Failed to encrypt ") + path, Error);
		return false;
    }

	//Set the file attributes to normal
#ifdef __WXMSW__   
		int filearrtibs = GetFileAttributes(path);
		SetFileAttributes(path, FILE_ATTRIBUTE_NORMAL);  
#endif

	wxString command;
	const wxString exepath = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + "ccrypt";

	if(data->GetFunction() == _("Encrypt"))
		command = exepath + wxT(" -f -e -K\"") + data->GetPassword() + wxT("\" \"") + path + wxT("\"");
	else
		command = exepath + wxT(" -f -d -K\"") + data->GetPassword() + wxT("\" \"") + path + wxT("\"");

	int id = wxDateTime::Now().GetTicks();
	wxCommandEvent *event = new wxCommandEvent(wxEVT_COMMAND_BUTTON_CLICKED, ID_SECUREPROCESS);
	event->SetInt(id);
	event->SetString(command);
	wxGetApp().QueueEvent(event);
	while(wxGetApp().m_StatusMap[id] != true){
		wxMilliSleep(100);
	}
	long lgReturn = wxGetApp().m_ProcessStatusMap[id];

	//Put the old attributed back
#ifdef __WXMSW__
	SetFileAttributes(path, filearrtibs);
#endif

	if(data->GetFunction() == _("Encrypt")){
		if(lgReturn == 0)
            OutputProgress(_("Encrypted ") + path, Message);
		else
			OutputProgress(_("Failed to encrypt ") + path + wxString::Format(wxT(" : %i"), lgReturn), Error);
	}
	else{
		if(lgReturn == 0)
 			OutputProgress(_("Decrypted ") + path, Message);
		else
 			OutputProgress(_("Failed to decrypt ") + path + wxString::Format(wxT(" : %i"), lgReturn), Error);
	}
	return true;
}
