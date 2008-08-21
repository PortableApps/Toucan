/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2007 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SECURE
#define H_SECURE

#include "securedata.h"
#include "rules.h"
#include "frmprogress.h"

bool Secure(SecureData data, Rules rules, frmProgress *window);
bool CryptDir(wxString strPath, SecureData data, Rules rules, frmProgress* window);
bool CryptFile(wxString strFile, SecureData data, Rules rules, frmProgress* window);

#endif 
