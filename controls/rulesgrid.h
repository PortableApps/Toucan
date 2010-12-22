/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include <wx/grid.h>

class RulesGrid : public wxGrid{
public:
    RulesGrid(wxWindow* parent, wxWindowID id);
    void Clear();
    void OnKeyDown(wxKeyEvent &event);
    void OnKeyUp(wxKeyEvent &event);
    void OnCellLeftClick(wxGridEvent &event);
    void OnCellChanged(wxGridEvent &event);
    void DeleteSelected();
    void ValidateRow(int row);
};
