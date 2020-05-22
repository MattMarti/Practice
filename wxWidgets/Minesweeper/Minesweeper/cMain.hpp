#pragma once

#include <wx/wx.h>

/*
This is a graphical interface component. We're going to make this a "Form", which is
a "Frame" in wxWidget speak

This is the start of the GUI. It implements the event handlers

This class basically defines all the parts of the primary window, like buttons, etc
*/
class cMain : public wxFrame {
public:
    cMain();
    ~cMain();

public:

    // 10x10 array of buttons
    int nFieldWidth = 10;
    int nFieldHeight = 10;
    wxButton **btn;

    // Minesweeper game data
    int *nField = nullptr;
    bool bFirstClick = true; // Populate minefield after first click

    /*
    Let's add some actions that happen when you click the button
    */
    void onButtonClicked(wxCommandEvent &evt);

    /*
    Parent window responsible for handling events. include the ability to handle events
    with this macro
    */
    wxDECLARE_EVENT_TABLE();
};

