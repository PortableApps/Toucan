bool CutStart(wxString strBasePath, bool blBox)
{
   
   wxString strPath = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("App") + wxFILE_SEP_PATH + wxT("temp-exclusions.txt");
    wxTextFile file;
    file.Open(strPath);
    unsigned int i;
    wxProgressDialog dialog( _("Progress"), _("Working - Generating Exceptions"), file.GetLineCount(), NULL , wxPD_AUTO_HIDE);
    if(blBox == false)
    {
        //wxMessageBox(wxT("Boo"));
        dialog.Update(file.GetLineCount(), _("Working"));
    }
    for(i = 0; i < file.GetLineCount(); i++)
    {
    
        wxString strLine = file.GetLine(i);
        //wxMessageBox(strLine);
        wxString strTemp = strLine.Right(strLine.Length() - strBasePath.Length() - 1);
        //wxMessageBox(strTemp);
        file.RemoveLine(i);
        file.InsertLine(strTemp, i);
        file.Write();
        dialog.Update(i, _("Working - Saving Exceptions"));
    }
    return true;
}