/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include "toucan.h"
#include <wx/string.h>
#include <wx/filename.h>

namespace File{
	int Copy(const wxFileName &source, const wxFileName &dest);
	int Rename(const wxFileName &source, const wxFileName &dest, bool overwrite);
	int Delete(const wxFileName &path, bool recycle, bool ignorero);
    //In wxMSW we get the full path and then preprend \\?\ to avoid filename limits
    wxString GetLongPath(const wxFileName &path);
}

#ifdef __WXMSW__
    DWORD CALLBACK CopyProgressRoutine(LARGE_INTEGER TotalFileSize, LARGE_INTEGER TotalBytesTransferred, LARGE_INTEGER StreamSize,
                                       LARGE_INTEGER StreamBytesTransferred, DWORD dwStreamNumber, DWORD dwCallbackReason,
                                       HANDLE hSourceFile, HANDLE hDestinationFile, LPVOID lpData);
#endif
