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
    btn = new wxButton*[nFieldWidth * nFieldHeight];
    wxGridSizer *grid = new wxGridSizer(nFieldWidth, nFieldHeight, 0, 0); // Sizer that adjusts size of buttons to size of window

    // Create array of mines
    nField = new int[nFieldWidth * nFieldHeight];

    // Make the font easier to read
    wxFont font(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

    // Create buttons and add them to window
    for (int x = 0; x < nFieldWidth; x++) {
        for (int y = 0; y < nFieldHeight; y++) {
            unsigned int loopId = y * nFieldWidth + x;

            btn[loopId] = new wxButton(this, 10000 + loopId); // Creates button with this frame as the parent
            btn[loopId]->SetFont(font); // Set font to the font specified in font
            grid->Add(btn[loopId], 1, wxEXPAND | wxALL); // Add button to grid sizer. Expand as necessary

            // Call bind function for this particular button
            btn[loopId]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::onButtonClicked, this);

            // Set default value of each location in minefield array
            nField[loopId] = 0;
        }
    }

    // Tell parent window which sizer to use
    this->SetSizer(grid);
    grid->Layout();
}

cMain::~cMain() {
    // Note: You have to delete the array of buttons "btn". Otherwise you'll get a memory leak
    delete[]btn;
    delete[]nField;
}

/*
Button action: On clicking the button, take the string from the text control box
and add it to the list box.
*/
void cMain::onButtonClicked(wxCommandEvent &evt) {
    
    // Determine x and y coordinates of button that was clicked
    int x = (evt.GetId() - 10000) % nFieldWidth;
    int y = (evt.GetId() - 10000) / nFieldWidth;

    // Generate minefield after first click
    if (bFirstClick) {
        int mines = 30; // Number of mines
        while (mines) {
            int rx = rand() % nFieldWidth;
            int ry = rand() % nFieldHeight;

            // Place a mine if there isn't one there already and it's not the first button clicked
            int idum = ry * nFieldWidth + rx;
            if (nField[idum] == 0 && rx != x && ry != y) {
                nField[idum] = -1;
                mines--;
            }
        }
        bFirstClick = false;
    }

    // Disable button
    int idum = y * nFieldWidth + x;
    btn[idum]->Enable(false);

    // Check if player hit a mine
    if (nField[idum] == -1) {
        wxMessageBox("Boom! Game Over");

        // Reset game
        bFirstClick = true;
        for (int x = 0; x < nFieldWidth; x++) {
            for (int y = 0; y < nFieldHeight; y++) {
                nField[y*nFieldWidth + x] = 0;
                btn[y*nFieldWidth + x]->SetLabel("");
                btn[y*nFieldWidth + x]->Enable(true);
            }
        }
    }
    else {
        // Indicate how many mines are around the clicked button (count the mines)
        int mine_count = 0;
        for (int i = -1; i < 2; i++) {
            for (int j = -1; j < 2; j++) {
                if (x + i >= 0 && x + i < nFieldWidth && y + j >= 0 && y + j < nFieldHeight) {
                    if (nField[(y + j)*nFieldWidth + (x + i)] == -1) {
                        mine_count++;
                    }
                }
            }
        }

        // Update buttons label to show mine count if > 0
        if (mine_count > 0) {
            btn[y*nFieldWidth + x]->SetLabel(std::to_string(mine_count));
        }
    }

    evt.Skip();
}