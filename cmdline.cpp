/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "toucan.h"
#include <iostream>
#include <wx/cmdline.h>

bool ParseCommandLine(){
	wxMessageOutput::Set(new wxMessageOutputStderr); 
	static const wxCmdLineEntryDesc desc[] =
	{
		{wxCMD_LINE_SWITCH, "g", "showgui", "Show user interface"},
		{wxCMD_LINE_OPTION, "s", "script", "Script to run", wxCMD_LINE_VAL_STRING},
		{wxCMD_LINE_NONE}
	};

	wxCmdLineParser parser(desc, wxGetApp().argc, wxGetApp().argv);
	if(!parser.Parse()){
		return false;
	}
	return true;
}
