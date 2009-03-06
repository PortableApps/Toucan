/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "filecounter.h"
#include "toucan.h"
#include <wx/dir.h>

FileCounter::FileCounter(){
	m_Count = 0;
}

void FileCounter::AddPath(wxString path){
	m_Paths.Add(path);
}

void FileCounter::AddPaths(wxArrayString paths){
	for(unsigned int i = 0; i < paths.Count(); i++){
		m_Paths.Add(paths.Item(i));
	}
}

long FileCounter::GetCount(){
	return m_Count;
}

bool FileCounter::Count(){
	for(unsigned int i = 0; i < m_Paths.Count(); i++){
		CountFolder(m_Paths.Item(i));
	}
	return true;
}

bool FileCounter::CountFolder(wxString path){
	if(wxGetApp().ShouldAbort()){
		return true;
	}
	wxGetApp().Yield();
	if (path[path.length()-1] != wxFILE_SEP_PATH) {
		path += wxFILE_SEP_PATH;       
	}
	wxDir dir(path);
	wxString strFilename;
	bool blDir = dir.GetFirst(&strFilename);
	if(blDir){
		do {
			if(wxDirExists(path + strFilename)){
				CountFolder(path + strFilename);
			}
			else{
				m_Count++;
			}
		}
		while (dir.GetNext(&strFilename) );
	} 
	return true;
}

bool FileCounter::Clear(){
	m_Paths.Clear();
	m_Count = 0;
	return false;
}
