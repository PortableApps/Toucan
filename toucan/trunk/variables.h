/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2007 Steven Lamerton
// Licence:     GNU GPL 2 (See readme for more info
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_NORMALISE
#define H_NORMALISE

#include <wx/string.h>

/*!
 * \file variables.h
 */
 
/*!
 * Expands any Variables in /a path. It is a recursive function allowing
 * variables to be nested.
 * \return The result if successful, otherwise wxEmptyString
 */
wxString Normalise(const wxString &path);

#endif
