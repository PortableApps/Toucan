#include "sync-complete.h"
#include "sync-update.h"
#include "sync-mirror.h"
#include "normalise.h"
#include "removedir.h"
#include "frmprogress.h"

bool Sync(wxString strSource, wxString strDest, wxString strFunction, wxArrayString arrExclusion, bool blBox, bool blAttributes)
{	
/*
    frmProgress* window = new frmProgress(NULL, ID_FRMPROGRESS, _("Open"));
            window->m_Progress_Text->SetValue(_("Test"));
    window->Show();
    
      window->m_Progress_Text->SetValue(_("Test1"));
Sleep(500);
      window->m_Progress_Text->SetValue(_("Test2"));*/
    
    strSource = Normalise(strSource);
	strDest = Normalise(strDest);
	if(strFunction == _("Copy"))
	{
		//wxMessageBox(strSource + wxT(" ") + strDest);
        CopyDir(strSource, strDest, arrExclusion, blBox, blAttributes);
	}
	else if(strFunction == _("Update"))
	{
		UpdateDir(strSource, strDest, arrExclusion, blBox, blAttributes);
	}
	else if(strFunction == _("Mirror"))
	{
		CopyDir(strSource, strDest, arrExclusion, blBox, blAttributes);
        MirrorDir(strSource,strDest, blBox);
	}
	else if(strFunction == _("Equalise"))
	{
		UpdateDir(strSource, strDest, arrExclusion, blBox, blAttributes);
		UpdateDir(strDest, strSource,  arrExclusion, blBox, blAttributes);
	}
	return true;
    
}