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

    /*
    The various things that show up, like button, text box, etc
    */
    wxButton *m_btn1 = nullptr;
    wxTextCtrl *m_txt1 = nullptr;
    wxListBox *m_list1 = nullptr;

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

