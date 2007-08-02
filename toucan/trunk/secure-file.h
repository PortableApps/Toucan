bool CryptFile(wxString strFile, wxString strPass, wxString strFunction, frmProgress* window)
{

    if(wxGetApp().GetStrAbort() == wxT("ABORT"))
    {
    
        return false;
    
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
        int filearrtibs = GetFileAttributes(strFile);
        SetFileAttributes(strFile,FILE_ATTRIBUTE_NORMAL);           
        wxString command = wxT("App\\toucan\\burp -eo -k\"") + strPass + wxT("\" \"") + strFile + wxT("\"");
		wxExecute(command, arrErrors,arrOutput, wxEXEC_NODISABLE);
        SetFileAttributes(strFile,filearrtibs);
        window->m_Progress_Text->AppendText(_("\nEncrypted ") + strFile);
        window->Update();
        wxGetApp().Yield(true);
        //wxMilliSleep(100);

	}
	else if(strFunction == _("Decrypt"))
	{
        int filearrtibs = GetFileAttributes(strFile);
        SetFileAttributes(strFile,FILE_ATTRIBUTE_NORMAL); 
        wxString command = wxT("App\\toucan\\burp -d -k\"") + strPass + wxT("\" \"") + strFile + wxT("\" \"") + wxPathOnly(strFile) + wxT("\\") + wxT("1.tmp\"");
		wxExecute(command, arrErrors, arrOutput, wxEXEC_NODISABLE);
		wxCopyFile(wxPathOnly(strFile) + wxT("\\1.tmp"), strFile, true);
		wxRemoveFile(wxPathOnly(strFile) + wxT("\\1.tmp"));
        SetFileAttributes(strFile,filearrtibs);
        window->m_Progress_Text->AppendText(_("\nDecrypted ") + strFile);
        window->Update();
        //wxMilliSleep(100);
        wxGetApp().Yield(true);
	}
	return true;

}