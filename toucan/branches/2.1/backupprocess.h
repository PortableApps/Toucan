/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_BACKUPPROCESS
#define H_BACKUPPROCESS

#include <wx/process.h>
#include <wx/txtstrm.h>
#include "frmprogress.h"
#include "toucan.h"

class PipedProcess : public wxProcess
{
public:
    DECLARE_CLASS(PipedProcess)

    PipedProcess(frmProgress* window){
        Redirect();
    }

    virtual bool HasInput();
	
	void OnTerminate(int pid, int status);
	
	void SetRealPid(long pid);
	long GetRealPid() {return m_PID;}
	
protected:
    frmProgress*   m_Window;
	long m_PID;
};

#endif
