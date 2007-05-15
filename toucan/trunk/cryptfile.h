#include <wx/tokenzr.h>
#include <wx/filename.h>

bool CryptFile(wxString strFile, wxString strPass, wxString strFunction, bool blBox)
{

	wxProgressDialog dialog( _("Progress"), _("Working"), 1, NULL , wxPD_AUTO_HIDE);
	if(blBox == false)
	{
		//wxMessageBox(wxT("Boo"));
		dialog.Update(1, _("Working"));
	}
    wxFileName filename(strFile);
    if(filename.IsOk() == true)
    {
    wxString size = filename.GetHumanReadableSize();
    if(size == wxT("Not available"))
    {
        return false;
    }
    }
	wxArrayString arrErrors;
	wxArrayString arrOutput;

	if(strFunction == _("Encrypt"))
	{
		wxString command = wxT("App\\burp -eo -k\"") + strPass + wxT("\" \"") + strFile + wxT("\"");
		wxExecute(command, arrErrors,arrOutput);
		dialog.Update(1, _("Working"));
	}
	else if(strFunction == _("Decrypt"))
	{
		wxString command = wxT("App\\burp -d -k\"") + strPass + wxT("\" \"") + strFile + wxT("\" \"") + wxPathOnly(strFile) + wxT("\\") + wxT("1.tmp\"");
		wxExecute(command, arrErrors, arrOutput, wxEXEC_SYNC);
		wxCopyFile(wxPathOnly(strFile) + wxT("\\1.tmp"), strFile, true);
		wxRemoveFile(wxPathOnly(strFile) + wxT("\\1.tmp"));
		dialog.Update(1, _("Working"));
	}
	return true;

}
