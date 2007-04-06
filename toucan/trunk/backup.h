#include "backup-complete.h"
#include "backup-update.h"
#include "backup-restore.h"
#include "normalise.h"

bool Backup(wxString strFirst, wxString strSecond, wxString strType, wxString strFormat, wxString strRatio, bool blBox)
{	
	wxProgressDialog dialog( _("Progress"), _("Working"), 2, NULL , wxPD_AUTO_HIDE);
	if(blBox == false)
	{
		dialog.Update(2, _("Working"));
	}
	strFirst = Normalise(strFirst);
	strSecond = Normalise(strSecond);
	if(strType == wxT("Complete"))
	{
		dialog.Update(0, _("Working - Backing Up Files"));
		BackupComplete(strFirst, strSecond, strFormat, strRatio);
		dialog.Update(2, _("Working - Finished"));
		
	}
	else if(strType == wxT("Update"))
	{
		dialog.Update(0, _("Working"));
		BackupUpdate(strFirst, strSecond, strFormat);
		dialog.Update(0, _("Working"));
	}
	else if(strType == wxT("Restore"))
	{
		dialog.Update(0, _("Working"));
		BackupRestore(strFirst, strSecond, strFormat);
		dialog.Update(0, _("Working"));
	}
		return true;

}
