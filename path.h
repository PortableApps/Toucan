/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include <wx/string.h>
#include <wx/filename.h>

namespace Path{
    wxFileName Normalise(const wxFileName &filename);
    wxString Normalise(const wxString &path);
    void CreateDirectoryPath(const wxFileName &path);
}
