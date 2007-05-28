#include "sync-complete.h"
#include "sync-update.h"
#include "sync-mirror.h"
#include "normalise.h"
#include "removedir.h"
#include "frmprogress.h"

bool Sync(wxString strSource, wxString strDest, wxString strFunction, wxArrayString arrExclusion, bool blBox, bool blAttributes)
{	

    frmProgress* window = new frmProgress(NULL, ID_FRMPROGRESS, _("Progress"));
    window->m_OK->Enable(false);
    window->m_Save->Enable(false);
    window->m_Progress_Text->SetValue(_("Starting..."));
    if(blBox)
    {
    window->Show();
    }
    window->Update();
    int length1 = strSource.Length();
    int length2 = strDest.Length();   
    
    strSource = Normalise(strSource);
    strSource = Normalise(strSource);
	strDest = Normalise(strDest);
	strDest = Normalise(strDest);
	
    if(strFunction == _("Copy"))
	{
        CopyDir(strSource, strDest, arrExclusion,  blAttributes, window,length1, length2);
	}
    
	else if(strFunction == _("Update"))
	{
		UpdateDir(strSource, strDest, arrExclusion, blAttributes, window,length1, length2);
	}
    
	else if(strFunction == _("Mirror"))
	{
		CopyDir(strSource, strDest, arrExclusion, blAttributes, window,length1, length2);
        MirrorDir(strSource,strDest, window,length1, length2);
	}
	else if(strFunction == _("Equalise"))
	{
		UpdateDir(strSource, strDest, arrExclusion, blAttributes, window,length1, length2);
		UpdateDir(strDest, strSource, arrExclusion, blAttributes, window,length1, length2);
	}
    window->m_Progress_Text->AppendText(_("Finished..."));
    window->m_OK->Enable();
    window->m_Save->Enable();
    
	return true;
    
}