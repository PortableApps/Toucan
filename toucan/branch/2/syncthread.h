#ifndef H_SYNCTHREAD
#define H_SYNCTHREAD

class SyncThread : public wxThread
{
public:
	//Constructor
	SyncThread(SyncData* data, Rules* rules, frmProgress* window){
		m_Data = data;
		m_Rules = rules;
		m_Window = window;
	}
	//Main function, all stuff goes in here
	virtual void *Entry();

private:
	SyncData* m_Data;
	Rules* m_Rules;
	frmProgress* m_Window;
}

void *SyncThread::Entry(){
	//Launch the correct set of loops
	if(data.GetFunction()== _("Copy") || data.GetFunction() == _("Update")){
		SyncLoop(data, rules);
	}
	else if(data.GetFunction() ==  _("Mirror (Copy)")){
		data.SetFunction(_("Copy"));
		SyncLoop(data, rules);
		//Swap the source and dest around for the mirror routine
		wxString strTemp = data.GetSource();
		data.SetSource(data.GetDest());
		data.SetDest(strTemp);
		//Run the mirror loop
		data.SetFunction(_("Mirror"));
		SyncLoop(data, rules);
	}
	else if(data.GetFunction() ==  _("Mirror (Copy)")){
		data.SetFunction(_("Update"));
		SyncLoop(data, rules);
		//Swap the source and dest around for the mirror routine
		wxString strTemp = data.GetSource();
		data.SetSource(data.GetDest());
		data.SetDest(strTemp);
		//Run the mirror loop
		data.SetFunction(_("Mirror"));
		SyncLoop(data, rules);
	}
	else if(data.GetFunction() ==  _("Equalise")){
		data.SetFunction(_("Update"));
		wxString strTemp = data.GetSource();
		data.SetSource(data.GetDest());
		data.SetDest(strTemp);
		data.SetFunction(_("Update"));
		SyncLoop(data, rules);
	}
	return true;
}

#endif
