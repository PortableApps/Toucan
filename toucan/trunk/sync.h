#include "sync-complete.h"
#include "sync-update.h"
#include "normalise.h"
#include "removedir.h"

bool Sync(wxString strSource, wxString strDest, wxString strFunction, wxArrayString arrExclusion, bool blBox)
{	strSource = Normalise(strSource);
	strDest = Normalise(strDest);
	if(strFunction == _("Copy"))
	{
		CopyDir(strSource, strDest, arrExclusion, blBox);
	}
	else if(strFunction == _("Update"))
	{
		UpdateDir(strSource, strDest, arrExclusion, blBox);
	}
	else if(strFunction == _("Mirror"))
	{
		RemDir(strDest);
		CopyDir(strSource, strDest, arrExclusion, blBox);
	}
	else if(strFunction == _("Equalise"))
	{
		UpdateDir(strSource, strDest, arrExclusion, blBox);
		UpdateDir(strDest, strSource,  arrExclusion, blBox);
	}
	return true;
}