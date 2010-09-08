/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include "toucan.h"
#include <wx/string.h>

namespace File{
	int Copy(const wxString& source, const wxString &dest);
	int Rename(const wxString& source, const wxString &dest, bool overwrite);
	int Delete(const wxString&path, bool recycle, bool ignorero);
	void Normalise(wxString *path);
}

#ifdef __WXMSW__
	DWORD CALLBACK CopyProgressRoutine(LARGE_INTEGER TotalFileSize, LARGE_INTEGER TotalBytesTransferred, LARGE_INTEGER StreamSize,
							  LARGE_INTEGER StreamBytesTransferred, DWORD dwStreamNumber, DWORD dwCallbackReason,
							  HANDLE hSourceFile, HANDLE hDestinationFile, LPVOID lpData);
#endif
