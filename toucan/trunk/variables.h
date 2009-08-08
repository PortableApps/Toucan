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
 * The Normalise funtion takes a path and attempts to expand out any Variables
 * that are within it. It is recursive until no more changes are detected allowing
 * deeply nested variables. If the given path is not valid a wxEmptyString is
 * returned.
 * \return The result if successful, otherwise wxEmptyString
 */
wxString Normalise(const wxString &path);

#endif
