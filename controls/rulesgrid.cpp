/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2010 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include "rulesgrid.h"
#include "../rules.h"

RulesGrid::RulesGrid(wxWindow* parent, wxWindowID id) : wxGrid(parent, id, wxDefaultPosition,
                                                        wxDefaultSize, wxWANTS_CHARS|wxBORDER_THEME){
    //Bind the events we need
    Bind(wxEVT_KEY_DOWN, &RulesGrid::OnKeyDown, this, wxID_ANY);
    Bind(wxEVT_KEY_UP, &RulesGrid::OnKeyUp, this, wxID_ANY);
    Bind(wxEVT_GRID_CELL_LEFT_CLICK, &RulesGrid::OnCellLeftClick, this, wxID_ANY);
    Bind(wxEVT_GRID_CELL_CHANGED, &RulesGrid::OnCellChanged, this, wxID_ANY);
    //Set up the grid itself
    CreateGrid(0, 3);
    EnableGridLines(false);
    HideRowLabels();
    UseNativeColHeader();
    SetColLabelValue(0, _("Include / Exclude"));
    SetColLabelValue(1, _("Type"));
    SetColLabelValue(2, _("Rule"));
    SetColLabelAlignment(wxALIGN_LEFT, wxALIGN_CENTRE);
    SetColSize(0, 150);
    SetColSize(1, 150);
    SetColSize(2, 150);
}

void RulesGrid::Clear(){
    if(GetNumberRows() > 0)
        DeleteRows(0, GetNumberRows());
}

//This ensures that the event doesn't get eaten by an editor, we don't handle
//the deletion here as there are multiple downs for one up
void RulesGrid::OnKeyDown(wxKeyEvent &event){
    if(event.GetKeyCode() != WXK_DELETE)
        event.Skip();
}

void RulesGrid::OnKeyUp(wxKeyEvent &event){
    if(event.GetKeyCode() == WXK_DELETE)
        DeleteSelected();
}

void RulesGrid::OnCellLeftClick(wxGridEvent &event){
    SetGridCursor(event.GetRow(), event.GetCol());
    EnableCellEditControl(true);
    wxGridCellEditor *edit = GetCellEditor(event.GetRow(), event.GetCol());
    if(edit){
        edit->Show(true);
        edit->DecRef();
    }
    event.Skip();
}

void RulesGrid::OnCellChanged(wxGridEvent &event){
    ValidateRow(event.GetRow());
}

void RulesGrid::ValidateRow(int row){
    Rule rule(GetCellValue(row, 2), 
              functionmap.left.at(GetCellValue(row, 0)),
              typemap.left.at(GetCellValue(row, 1)));
    wxColour colour;
    if(!rule.IsValid())
        colour = wxColour("red");
    else
        colour = wxColour("white");
    SetCellBackgroundColour(row, 0, colour);
    SetCellBackgroundColour(row, 1, colour);
    SetCellBackgroundColour(row, 2, colour);
    Refresh();
}

void RulesGrid::DeleteSelected(){
    //Get the bounds for the selected blocks
    wxGridCellCoordsArray topleft = GetSelectionBlockTopLeft();
    wxGridCellCoordsArray bottomright = GetSelectionBlockBottomRight();

    //Make sure that for every end there is a start
    if(topleft.Count() != bottomright.Count())
        return;

    //Loop through each block, this is reverse order so we start at the bottom
    for(unsigned int i = 0; i < bottomright.Count(); i++){
        int top = topleft.Item(i).GetRow();
        int bottom = bottomright.Item(i).GetRow();
        //Remove the lines from the bottom up so they don't invalidate the other positions
        do{
            DeleteRows(bottom);
            bottom--;
        }while(top <= bottom);
    }

    //Delete any rows with a singly selected cell
    wxGridCellCoordsArray cells = GetSelectedCells();
    for(unsigned int i = 0; i < cells.Count(); i++){
        DeleteRows(cells.Item(i).GetRow());
    }

    //Deselct everything as otherwise the selections will look wrong
    ClearSelection();
}
