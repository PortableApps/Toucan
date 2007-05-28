#include "frmprogress.h"

bool CryptDir(wxString strPath, wxString strPass, wxString strFunction, frmProgress* window)
{   
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
                CryptFile(strPath + wxT("\\") + filename, strPass, strFunction, window);
            }

        }

        while (dir.GetNext(&filename) );
    }   
    return true;
   
}
