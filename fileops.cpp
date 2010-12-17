/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include "fileops.h"

int File::Copy(const wxFileName &source, const wxFileName &dest){
    wxString longsource = GetLongPath(source), longdest = GetLongPath(dest);
#ifdef __WXMSW__
	return CopyFileEx(longsource.fn_str(), longdest.fn_str(), &CopyProgressRoutine, NULL, NULL, 0);
#else
	return wxCopyFile(longsource, longdest, true);
#endif
}

int File::Rename(const wxFileName &source, const wxFileName &dest, bool overwrite){
    wxString longsource = GetLongPath(source), longdest = GetLongPath(dest);
#ifdef __WXMSW__
	DWORD flags = overwrite ? MOVEFILE_REPLACE_EXISTING : 0;
	return MoveFileWithProgress(longsource.fn_str(), longdest.fn_str(), &CopyProgressRoutine, NULL, flags);
#else
	return wxRenameFile(longsource, longdest, overwrite);
#endif
}

int File::Delete(const wxFileName &path, bool recycle, bool ignorero){
    wxString longpath = GetLongPath(path);
#ifdef __WXMSW__
	//If we want to recycle then we must use a shfileop but it doesn't support
	//long paths
    if(ignorero){
        SetFileAttributes(longpath.fn_str(), FILE_ATTRIBUTE_NORMAL);
    }
	if(recycle){
		wxString tmppath(path.GetFullPath());
		tmppath += wxT('\0');

		SHFILEOPSTRUCT opstruct;
		ZeroMemory(&opstruct, sizeof(opstruct));
		opstruct.wFunc = FO_DELETE;
		opstruct.pFrom = longpath.fn_str() + '\0';
		opstruct.fFlags = FOF_ALLOWUNDO | FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI;
		return SHFileOperation(&opstruct);
	}
	else{
		return DeleteFile(longpath.fn_str());
	}
#else
	return wxRemoveFile(longpath);
#endif
}

wxString File::GetLongPath(const wxFileName &path){
#ifdef __WXMSW__
    if(path.GetFullPath().Left(2) == "\\\\")
        return "\\\\?\\UNC\\" + path.GetVolume() + path.GetPath(wxPATH_GET_SEPARATOR) + path.GetFullName();
    else
        return "\\\\?\\" + path.GetFullPath();
#else
    return path.GetFullPath();
#endif
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

