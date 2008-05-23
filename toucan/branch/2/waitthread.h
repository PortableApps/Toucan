
class WaitThread : public wxThread
{
public:
	//Constructor
	WaitThread(long pid, PipedProcess *process):wxThread(wxTHREAD_JOINABLE){
		m_PID = pid;
		m_Process = process;
	}
	//Main function, all stuff goes in here
	virtual void *Entry();

private:
	long m_PID;
	PipedProcess *m_Process;
};

void *WaitThread::Entry(){
	HANDLE hProcess=OpenProcess(PROCESS_QUERY_INFORMATION,FALSE,m_PID);
	DWORD lgReturn;
	GetExitCodeProcess(hProcess, &lgReturn);
	while(lgReturn == STILL_ACTIVE){
	//	wxMutexGuiEnter();
		m_Process->HasInput();
	//	wxMutexGuiLeave();
		GetExitCodeProcess(hProcess, &lgReturn);
	}
	return NULL;
}
