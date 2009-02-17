/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SYNCDATA
#define H_SYNCDATA

#include "frmmain.h"
#include "rootdata.h"
#include <wx/string.h>


class SyncData : public RootData{

public:

	//Functions
	bool TransferToFile();
	bool TransferFromFile();
	bool TransferToForm();
	bool TransferFromForm();
	
	void Output();
	
	bool Execute(Rules rules);
	
	bool NeedsPassword() { return false; }
	
	bool NeededFieldsFilled();

	//Inline functions
	void SetSource(wxString source) { strSource = source; }
	wxString GetSource() { return strSource; }

	void SetDest(wxString dest) { strDest = dest; }
	wxString GetDest() { return strDest; }

	void SetFunction(wxString function) { strFunction = function; }
	wxString GetFunction() { return strFunction; }

	void SetPreText(wxString text) { strPreText = text; }
	wxString GetPreText() { return strPreText; }

	void SetTimeStamps(bool timestamps) { blTimeStamps = timestamps; }
	bool GetTimeStamps() { return blTimeStamps; }

	void SetAttributes(bool attributes) { blAttributes = attributes; }
	bool GetAttributes() { return blAttributes; }

	void SetIgnoreRO(bool ignorero) { blIgnoreRO = ignorero; }
	bool  GetIgnoreRO() { return blIgnoreRO; }

	void SetIgnoreDLS(bool ignoredls) { blIgnoreDLS = ignoredls; }
	bool  GetIgnoreDLS() { return blIgnoreDLS; }
	

private:
	wxString strSource;
	wxString strDest;
	wxString strFunction;
	//To put before  the output text
	wxString strPreText;
	bool blTimeStamps;
	bool blAttributes;
	bool blIgnoreRO;
	bool blIgnoreDLS;

};

#endif

