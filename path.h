/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#ifndef PATH_H
#define PATH_H

#include <wx/string.h>
#include <wx/filename.h>

#include <map>

namespace Path{
    wxFileName Normalise(const wxFileName &filename);
    wxString Normalise(const wxString &path);
    void CreateDirectoryPath(const wxFileName &path);

    //<drive label, drive letter> Used by variables to resolve drive labels
	static std::map<wxString, wxString> DriveLabels;
}

#endif
