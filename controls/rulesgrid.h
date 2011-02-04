/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

class wxComboBox;

#include <wx/grid.h>
#include "../rules.h"

class RulesGrid : public wxGrid{
public:
    RulesGrid(wxWindow* parent, wxWindowID id, wxComboBox* names);
    void AddRows(int count = 1);
    void Clear();
    void OnKeyDown(wxKeyEvent &event);
    void OnKeyUp(wxKeyEvent &event);
    void OnCellLeftClick(wxGridEvent &event);
    void OnCellChanged(wxGridEvent &event);
    void DeleteSelected();
    bool LoadData(const RuleSet &rules);
    RuleSet SaveData() const;
    void ValidateRow(int row);
private:
    wxComboBox* names;
};
