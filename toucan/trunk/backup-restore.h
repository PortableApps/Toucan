bool BackupRestore(wxString strFirst, wxString strSecond, wxString strFormat)
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
    wxString command = wxT("App\\7za.exe  x  \"") + strFirst + wxT("\" -o\"") + strSecond + wxT("\" * -r");
    wxExecute(command, arrOutput, arrErrors);
    return true;

}