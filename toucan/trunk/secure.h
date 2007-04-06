#include <wx/file.h>
#include <wx/filefn.h>
#include <wx/dir.h>
#include <wx/progdlg.h>
#include "cryptfile.h"
#include "cryptdir.h"
#include "normalise.h"

bool Secure(wxArrayString arrLocation, wxString strFunction, wxString strPass, bool blBox)
{  
    unsigned int i;
    for(i = 0; i < arrLocation.Count(); i++)
    {
    arrLocation.Item(i) = Normalise(arrLocation.Item(i));
    arrLocation.Item(i) = Normalise(arrLocation.Item(i));
      //wxMessageBox(arrLocation.Item(i));
       if(wxDirExists(arrLocation.Item(i)))
        {

            //wxMessageBox(wxT("Crypt Dir ") + arrLocation.Item(i));
            CryptDir(arrLocation.Item(i), strPass, strFunction, blBox);
        }
        else
        {


            //wxMessageBox(wxT("Crypt File ") + arrLocation.Item(i));
            CryptFile(arrLocation.Item(i), strPass, strFunction, blBox);
        }
    }

return true;

}