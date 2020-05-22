#include "cMain.hpp"

/*
Implement what the event table is

Takes name of class that it produces events for, and requires base class

Bind any function with the id 10001 to "OnButtonClicked" function
*/
wxBEGIN_EVENT_TABLE(cMain, wxFrame)
    EVT_BUTTON(10001, onButtonClicked)
wxEND_EVENT_TABLE()


/*
- This is the base class and doesn't have a parent, so it's parent is nullptr
- All windows require an id. We don't need a specific on though, so wxID_ANY is good
- Title
*/
cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Minesweeper", wxPoint(10, 70), wxSize(800, 600)) {

    // Create a button, a list box, and a text box
    m_btn1 = new wxButton(this, 10001, "Click Me", wxPoint(10, 10), wxSize(150, 50));
    m_txt1 = new wxTextCtrl(this, wxID_ANY, "", wxPoint(10, 70), wxSize(300, 30));
    m_list1 = new wxListBox(this, wxID_ANY, wxPoint(10, 110), wxSize(300, 300));

}

cMain::~cMain() {

}

/*
Button action: On clicking the button, take the string from the text control box
and add it to the list box.
*/
void cMain::onButtonClicked(wxCommandEvent &evt) {
    
    // Handle the string
    auto textbox_string = m_txt1->GetValue();
    m_list1->AppendString(textbox_string);

    // Tell the sytem that the event has been handled. This skips making the parent
    // frames handle the button clicked event
    evt.Skip();
}