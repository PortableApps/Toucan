/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_BACKUPFUNCTIONS
#define H_BACKUPFUNCTIONS

#include <wx/textfile.h>
#include "rules.h"

bool CreateList(wxTextFile *file, Rules rules, wxString strPath, int iRootLength);

#endif
