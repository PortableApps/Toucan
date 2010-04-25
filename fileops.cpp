/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include "fileops.h"

int File::Copy(const wxString& source, const wxString &dest){
#ifdef __WXMSW__
	wxString sourcemsw(source), destmsw(dest);
	File::Normalise(&sourcemsw);
	File::Normalise(&destmsw);
	return CopyFileEx(sourcemsw.fn_str(), destmsw.fn_str(), &CopyProgressRoutine, NULL, NULL, 0);
#else
	return wxCopyFile(source, dest, true);
#endif
}

int File::Rename(const wxString& source, const wxString &dest, bool overwrite){
#ifdef __WXMSW__
	wxString sourcemsw(source), destmsw(dest);
	File::Normalise(&sourcemsw);
	File::Normalise(&destmsw);
	DWORD flags = overwrite ? MOVEFILE_REPLACE_EXISTING : 0;
	return MoveFileWithProgress(sourcemsw.fn_str(), destmsw.fn_str(), &CopyProgressRoutine, NULL, flags);
#else
	return wxRenameFile(source, dest, overwrite);
#endif
}

int File::Delete(const wxString& path, bool recycle){
#ifdef __WXMSW__
	//If we want to recycle then we must use a shfileop but it doesn't support
	//long paths
	if(recycle){
		wxString tmppath(path);
		tmppath += wxT('\0');

		SHFILEOPSTRUCT opstruct;
		ZeroMemory(&opstruct, sizeof(opstruct));
		opstruct.wFunc = FO_DELETE;
		opstruct.pFrom = tmppath.fn_str();
		opstruct.fFlags = FOF_ALLOWUNDO | FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI;
		return SHFileOperation(&opstruct);
	}
	else{
		wxString pathmsw(path);
		File::Normalise(&pathmsw);
		return DeleteFile(pathmsw.fn_str());
	}
#else
	return wxRemoveFile(path);
#endif
}

void File::Normalise(wxString *path){
	path->Replace("/", "\\");
	path->Prepend("\\\\?\\");
}

#ifdef __WXMSW__
DWORD CALLBACK CopyProgressRoutine(LARGE_INTEGER WXUNUSED(TotalFileSize), LARGE_INTEGER WXUNUSED(TotalBytesTransferred), 
									LARGE_INTEGER WXUNUSED(StreamSize), LARGE_INTEGER WXUNUSED(StreamBytesTransferred), 
									DWORD WXUNUSED(dwStreamNumber), DWORD WXUNUSED(dwCallbackReason),
									HANDLE WXUNUSED(hSourceFile), HANDLE WXUNUSED(hDestinationFile), 
									LPVOID WXUNUSED(lpData)){
	if(wxGetApp().GetAbort()){
		return PROGRESS_CANCEL;
	}
	else{
		return PROGRESS_CONTINUE;
	}
}
#endif

