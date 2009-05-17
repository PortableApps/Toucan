/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SECURE
#define H_SECURE

class frmProgress;
class Rules;
class SecureData;

bool Secure(SecureData data, Rules rules, frmProgress *window);
bool CryptDir(wxString strPath, SecureData data, Rules rules);
bool CryptFile(wxString strFile, SecureData data, Rules rules);

#endif 
