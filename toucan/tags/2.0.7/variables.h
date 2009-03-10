/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2007 Steven Lamerton
// Licence:     GNU GPL 2 (See readme for more info
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_NORMALISE
#define H_NORMALISE

/*This code creates the correct filenames from ones including Portable Variables
if the name to be returned is not valid then the function returns an empty string
and this is dealt with by the calling function*/
wxString Normalise(wxString strFilePath);

#endif
