#include "frmprogress.h"

bool Backup(wxString strFirst, wxString strSecond, wxString strType, wxString strFormat, wxString strRatio, bool blBox)
{	
    wxGetApp().SetStrTemp(_("Backup"));
    wxGetApp().SetStrType(strType);
    wxGetApp().SetStrFormat(strFormat);
    wxGetApp().SetStrRatio(strRatio);
    wxGetApp().SetStrSecond(strSecond);
    wxGetApp().SetStrFirst(strFirst);
    
    frmProgress* window = new frmProgress(NULL, ID_FRMPROGRESS, _("Progress"));
    window->m_OK->Enable(false);
    window->m_Save->Enable(false);
    window->m_Progress_Text->SetValue(_("Starting..."));
    if(blBox)
    {
    window->ShowModal();
    }
    window->Update();
    return true;

}
