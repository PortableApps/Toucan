bool BackupComplete(wxString strFirst, wxString strSecond, wxString strFormat, wxString strRatio)
{

    wxArrayString arrOutput;
    wxArrayString arrErrors;
    
    if (strFormat == _("Zip"))
    {
        strFormat = wxT("zip");
    }
    else if (strFormat == _("7 Zip"))
    {
        strFormat = wxT("7z");
    }
    
    if (strRatio == _("Normal"))
    {
    
        strRatio = wxT("5");
    
    }
    else if (strRatio == _("Maximum"))
    {
    
        strRatio = wxT("9");
    
    }
    
    wxString command = wxT("App\\7za.exe a -t") + strFormat + wxT(" -mx") + strRatio + wxT(" \"") + strSecond + wxT("\" \"") + strFirst + wxT("\\*\"") + wxT(" -x@\"") + wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("App") + wxFILE_SEP_PATH + wxT("temp-exclusions.txt") + wxT("\"");    
    //wxMessageBox(command);
    wxExecute(command, arrOutput, arrErrors);
    return true;

}
