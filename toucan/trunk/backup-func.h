#include <wx/filefn.h>
#include <wx/dir.h>
#include <wx/filename.h>

bool GenerateExclusions(wxString strFrom, wxArrayString arrExclusions, bool blBox)
{
    #ifdef __WXMSW__
            wxString SLASH = wxT("\\");
    #else
            wxString SLASH = wxT("/");
    #endif

    if (strFrom[strFrom.length()-1] != SLASH) {
            strFrom += SLASH;       
    }

    wxDir dir(strFrom);
    wxString strFilename;
    bool bla = dir.GetFirst(&strFilename);
   
    if (bla){
        do {
            if (wxDirExists(strFrom + strFilename))
            {
                unsigned int i;
                bool blEqual = false;
                for(i = 0; i <arrExclusions.GetCount(); i++)
                {
                    if(strFilename == arrExclusions.Item(i)){
                        blEqual = true;
                    }
                }
                if(blEqual == false){
                    GenerateExclusions(strFrom + strFilename, arrExclusions, blBox);
                }
                else{
                    wxString strPath = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("App") + wxFILE_SEP_PATH + wxT("toucan") + wxFILE_SEP_PATH + wxT("temp-exclusions.txt");
                    wxTextFile file;
                    file.Open(strPath);
                    file.AddLine(strFrom + strFilename);
                    file.Write();
					file.Close();
                }
            }
            else{
                wxFileName flName(strFrom + strFilename);
                unsigned int i;
                bool blEqual = false;
                for(i = 0; i <arrExclusions.GetCount(); i++)
                {
                    if(wxT(".") + flName.GetExt() == arrExclusions.Item(i)){
                        blEqual = true;
                    }
                }
                if(blEqual == true)
                {
                    wxString strPath = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("App") + wxFILE_SEP_PATH+ wxT("toucan") + wxFILE_SEP_PATH + wxT("temp-exclusions.txt");
                    wxTextFile file;
                    file.Open(strPath);
                    file.AddLine(strFrom + strFilename);
                    file.Write();
					file.Close();
                }
            }
        }
        while (dir.GetNext(&strFilename) );
    }  
    return true;
   
}

bool CutStart(wxString strBasePath, bool blBox)
{
	wxString strPath = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("App") + wxFILE_SEP_PATH+ wxT("toucan") + wxFILE_SEP_PATH + wxT("temp-exclusions.txt");
    wxTextFile file;
    file.Open(strPath);
    unsigned int i;
    for(i = 0; i < file.GetLineCount(); i++)
    {
        wxString strLine = file.GetLine(i);
        wxString strTemp = strLine.Right(strLine.Length() - strBasePath.Length() - 1);
        file.RemoveLine(i);
        file.InsertLine(strTemp, i);
        file.Write();
    }
    return true;
}