/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_BACKUPPROCESS
#define H_BACKUPPROCESS

#include <wx/process.h>
#include <wx/txtstrm.h>

class PipedProcess : public wxProcess{
	
public:
    DECLARE_CLASS(PipedProcess)

    PipedProcess(){
        Redirect();
    }

    virtual bool HasInput();
	void OnTerminate(int pid, int status);

	void SetRealPid(const long& RealPID) {this->m_RealPID = RealPID;}
	const long& GetRealPid() const {return m_RealPID;}
	
private:
	long m_RealPID;
};

#endif
