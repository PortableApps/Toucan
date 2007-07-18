#include "frmmain.h"

#include <wx/textfile.h>
#include <wx/file.h>

bool PrepareTextFile(wxString strFileName)
{
    wxTextFile file;
    if(wxFile::Exists(strFileName))
    {	
        file.Open(strFileName);
        file.Clear();
        file.Write();
    }
    else
    {
        file.Create(strFileName);
    }
    file.Close();
    return true;
}
