/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_BACKUPPROCESS
#define H_BACKUPPROCESS

#include "../signalprocess.h"

class BackupProcess : public SignalProcess 
{
public:
	BackupProcess(int id) : SignalProcess(id){
        Redirect();
    }

    virtual bool HasInput();

	void SetRealPid(const long& RealPID) {this->m_RealPID = RealPID;}
	const long& GetRealPid() const {return m_RealPID;}
	
private:
	long m_RealPID;
};

#endif
