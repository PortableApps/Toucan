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

//The base process
class BaseProcess : public wxProcess
{
    DECLARE_CLASS(BaseProcess)
public:
    BaseProcess(wxWindow* win): wxProcess(win) {}
	
};

//The actual piped process, this could be merged with the base process
class PipedProcess : public BaseProcess
{
public:
    DECLARE_CLASS(PipedProcess)

    PipedProcess(frmProgress* window): BaseProcess(window), m_Window(window)
    {
	//Redirect the process input and output
        Redirect();
    }

    virtual bool HasInput();
	
	void SetRealPid(long pid);
	
	long GetRealPid() {return m_PID;}
	

protected:
    frmProgress*   m_Window;
	long m_PID;
};


#endif
