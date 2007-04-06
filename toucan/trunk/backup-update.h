bool BackupUpdate(wxString strFirst, wxString strSecond, wxString strFormat)
{
    wxArrayString arrOutput;
    wxArrayString arrErrors;
    
    if (strFormat == wxT("Zip"))
    {
        strFormat = wxT("zip");
    }
    else if (strFormat == wxT("7 Zip"))
    {
        strFormat = wxT("7z");
    }
                
    wxString command = wxT("App\\7za.exe  u -t") + strFormat + wxT(" \"") + strSecond + wxT("\" \"") + strFirst + wxT("\\*\"");
    wxExecute(command, arrOutput, arrErrors);
    return true;

}