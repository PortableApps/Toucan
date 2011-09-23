/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2011 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_PROGRESSLISTCTRL
#define H_PROGRESSLISTCTRL

#include <vector>
#include <wx/listctrl.h>

struct ProgressItem{
    bool error;
    wxString column0, column1; 
};

class ProgressListCtrl : public wxListCtrl{
public:
    ProgressListCtrl(wxWindow *parent);
    virtual wxString OnGetItemText(long item, long column) const;
    virtual wxListItemAttr* OnGetItemAttr(long item) const;
    void AddItem(bool error, const wxString& column0, const wxString& column1);

private:
    std::vector<ProgressItem> m_items;
    wxListItemAttr* m_red;
};

#endif
