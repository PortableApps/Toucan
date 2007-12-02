#ifndef H_SYNCTHREAD
#define H_SYNCTHREAD

class SyncThread : public wxThread
{
public:
	//Constructor
	SyncThread(SyncData* data, Rules* rules){
		m_Data = data;
		m_Rules = rules;
	}
	//Main function, all stuff goes in here
	virtual void *Entry();

private:
	SyncData* m_Data;
	Rules* m_Rules;
}

void *SyncThread::Entry(){
	

#endif
