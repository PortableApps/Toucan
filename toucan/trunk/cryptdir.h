bool CryptDir(wxString strPath, wxString strPass, wxString strFunction, bool blBox)
{
    wxProgressDialog dialog( _("Progress"), _("Working"), 1, NULL , wxPD_AUTO_HIDE);
    if(blBox == false)
    {
        //wxMessageBox(wxT("Boo"));
        dialog.Update(1, _("Working"));
    }
    
    wxDir dir(strPath);
    wxString filename;
    bool bla = dir.GetFirst(&filename);
   
    if (bla)
	{
        do {
            if (wxDirExists(strPath + wxT("\\") + filename) )
            {
                CryptDir(strPath + wxT("\\") + filename, strPass, strFunction, blBox);
                dialog.Update(1, _("Working"));
            }
            else
			{
                CryptFile(strPath + wxT("\\") + filename, strPass, strFunction, blBox);
                dialog.Update(1, _("Working"));
            }

        }

        while (dir.GetNext(&filename) );
    }   
    return true;
   
}
