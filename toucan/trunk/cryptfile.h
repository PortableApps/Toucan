#include <wx/tokenzr.h>
#include <wx/filename.h>
#include "frmprogress.h"

bool CryptFile(wxString strFile, wxString strPass, wxString strFunction, frmProgress* window)
{

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
        window->m_Progress_Text->AppendText(_("\nEncrypted") + strFile);
	}
	else if(strFunction == _("Decrypt"))
	{
		wxString command = wxT("App\\burp -d -k\"") + strPass + wxT("\" \"") + strFile + wxT("\" \"") + wxPathOnly(strFile) + wxT("\\") + wxT("1.tmp\"");
		wxExecute(command, arrErrors, arrOutput, wxEXEC_SYNC);
		wxCopyFile(wxPathOnly(strFile) + wxT("\\1.tmp"), strFile, true);
		wxRemoveFile(wxPathOnly(strFile) + wxT("\\1.tmp"));
        window->m_Progress_Text->AppendText(_("\nDecrypted") + strFile);
	}
	return true;

}
