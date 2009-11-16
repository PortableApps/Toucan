/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SECURE
#define H_SECURE

class SecureData;

bool Secure(SecureData *data);
bool CryptDir(wxString strPath, SecureData *data);
bool CryptFile(const wxString &path, SecureData *data);

#endif 
