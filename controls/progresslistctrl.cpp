/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2011 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include "progresslistctrl.h"

ProgressListCtrl::ProgressListCtrl(wxWindow *parent)
{
    wxListCtrl::Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_VIRTUAL|wxLC_SINGLE_SEL|wxLC_VRULES);
    m_red = new wxListItemAttr();
    m_red->SetTextColour(wxColour("Red"));
}

wxString ProgressListCtrl::OnGetItemText(long item, long column) const {
    if(column == 0)
        return m_items[item].column0;
    else if(column == 1)
        return m_items[item].column1;
    else
        return "";
}

wxListItemAttr* ProgressListCtrl::OnGetItemAttr(long item) const {
    if(m_items[item].error)
        return m_red;
    else
        return NULL;
}

void ProgressListCtrl::AddItem(bool error, const wxString& column0, const wxString& column1){
    ProgressItem item;
    item.column0 = column0;
    item.column1 = column1;
    item.error = error;
    m_items.push_back(item);
}