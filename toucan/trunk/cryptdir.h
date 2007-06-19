#include "frmprogress.h"

bool CryptDir(wxString strPath, wxString strPass, wxString strFunction, frmProgress* window)
{   
    if(wxGetApp().GetStrAbort() == wxT("ABORT"))
    {
    
        return false;
    
    }
    
    wxDir dir(strPath);
    wxString filename;
    bool bla = dir.GetFirst(&filename);
   
    if (bla)
	{
        do {
            if (wxDirExists(strPath + wxT("\\") + filename) )
            {
                CryptDir(strPath + wxT("\\") + filename, strPass, strFunction, window);
            }
            else
			{
                if(wxGetApp().GetStrAbort() == wxT("ABORT"))
                {
    
                return false;
    
                }
                CryptFile(strPath + wxT("\\") + filename, strPass, strFunction, window);
            }

        }

        while (dir.GetNext(&filename) );
    }   
    return true;
   
}
