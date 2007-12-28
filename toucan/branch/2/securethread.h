#ifndef H_SECURETHREAD
#define H_SECURETHREAD

#include "rules.h"
#include "securedata.h"
#include "secure.h"

class SecureThread : public wxThread
{
public:
//Constructor
SecureThread(SecureData data, Rules rules, frmProgress *window){
	m_Data = data;
	m_Rules = rules;
	m_Window = window;
}
//Main function, all stuff goes in here
virtual void *Entry();

private:
	SecureData m_Data;
	Rules m_Rules;
	frmProgress *m_Window;
};

void *SecureThread::Entry(){
	//wxGetApp().SetStrTemp(wxEmptyString);
	//wxGetApp().SetStrAbort(wxEmptyString);
	//Need to add form creation before thread start
	OutputProgress(_("Starting..."), m_Window);
	//wxGetApp().SetStrAbort(wxEmptyString);
	unsigned int i;
	wxArrayString arrLocation = m_Data.GetLocations();
	//Iterate through the entries in the array
	for(i = 0; i < arrLocation.Count(); i++)
	{
		//if(wxGetApp().GetStrAbort() == wxT("ABORT")){
		//return false;
		//}
		//Need to add normalisation to SecureData
		if(arrLocation.Item(i) != wxEmptyString){
			if(wxDirExists(arrLocation.Item(i))){
				//MessageBox(arrLocation.Item(i), _("Boo"));
				CryptDir(arrLocation.Item(i), m_Data, m_Rules, m_Window);
			}
			else{
				CryptFile(arrLocation.Item(i), m_Data, m_Rules, m_Window);
            			}
		}
	}
	OutputProgress(_("Finished..."), m_Window);
	return NULL;
}
#endif
