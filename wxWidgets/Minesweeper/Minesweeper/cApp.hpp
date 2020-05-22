#pragma once

#include <wx/wx.h>
#include "cMain.hpp"

/*
This is the launcher for the application
*/

/*
To make a GUI, we start with a driver class. It must extend the wxApp class.
*/
class cApp : public wxApp {
public:
    cApp();
    ~cApp();

private:
    cMain* m_frame1 = nullptr;

public:

    /*
    The wxApp class has an OnInit() function that must be implemented.
    */
    virtual bool OnInit();
};

