#include "cApp.hpp"
#include "cMain.hpp"

/*
This tells wxWidgets to generate a "main" function for us. We're developing a 
program that is different from a usual C++ program. This is an "Operating 
System Compliant" program, which uses a "_WinMain@16" main function instead
of "int main()".
*/
wxIMPLEMENT_APP(cApp); // Creates main

/*
The usual constructors and destructor
*/
cApp::cApp() {
}
cApp::~cApp() {
}

/*
Implement the function
This is basically main()
*/
bool cApp::OnInit() {
    m_frame1 = new cMain();
    m_frame1->Show();

    return true;
}
