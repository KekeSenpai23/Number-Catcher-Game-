// This program was created to build an efficient and working number catching game

#include <iostream>
#include <windows.h>            // to access windows api
#include <conio.h>             // for getch function
#include <string>
#include <cstdlib>            // for rand function
#include <thread>            // for delay (difficulties)
#include <chrono>           // for timer
using namespace std;


// Game parameters
int robotX = 30;                          // Initial position of the robot
int score = 0;                           // Player's score
const int gridX = 80;                   // Number of columns in the grid
const int gridY = 25;                  // Number of rows in the grid
const int nOfFallingNumbers = 3;      // Number of falling numbers at once
bool isPaused = false;              // Flag to check if the game is paused



// Arrays to store falling numbers' positions and values
int numX[nOfFallingNumbers];                     //X-axis of falling numbers
int numY[nOfFallingNumbers];                    // Y-axis of falling numbers
int numV[nOfFallingNumbers];                   // value of falling numbers


// Buffer to store characters and attributes for console output (the entire grid)
CHAR_INFO consoleBuffer[gridY][gridX];


// Purpose: Function to clear the console buffer and set all characters to spaces
// Global variables accessed: gridX, gridY
void clearBuffer() {
	
    // Loop through each cell in the buffer and set default values
    for (int i = 0; i < gridY; ++i) {
        for (int j = 0; j < gridX; ++j) {
            consoleBuffer[i][j].Char.AsciiChar = ' ';       //Setting a character to a space
            consoleBuffer[i][j].Attributes = 7;            // Default color (7)
        }
    }
}

// Purpose: Function to display the console buffer to the screen
// Global Variables accessed: gridX, gridY
void displayBuffer() {
    // Set up console output parameters
    COORD bufferSize = { static_cast<SHORT>(gridX), static_cast<SHORT>(gridY) };
    COORD bufferCoord = { 0, 0 };
    SMALL_RECT writeRegion = { 0, 0, static_cast<SHORT>(gridX - 1), static_cast<SHORT>(gridY - 1) };

    // Write the console buffer to the screen
    WriteConsoleOutput(GetStdHandle(STD_OUTPUT_HANDLE), &consoleBuffer[0][0], bufferSize, bufferCoord, &writeRegion);
}

// Purpose: Function to update the console buffer with a character and attributes at a specific position
// Input parameters: int x, int y, char value, WORD attributes
void updateBuffer(int x, int y, char value, WORD attributes) {
    consoleBuffer[y][x].Char.AsciiChar = value;
    consoleBuffer[y][x].Attributes = attributes;
}

// Purpose: Function to initialize the falling numbers' positions and values
// Global Variables accessed: nOfFallingNumbers, gridX, gridY, numX, numY, numV
void initializeFallingNumbers() {
    for (int i = 0; i < nOfFallingNumbers; ++i) {
        numX[i] = rand() % gridX; // Random initial X position
        numY[i] = rand() % gridY; // Start from the top of the grid
        numV[i] = rand() % 10;    // Random number value between 0-9
    }
}

// Purpose: Function to move the falling numbers down the grid
// Global variables accessed: nOfFallingNumbers, numY, numX, numV
void moveFallingNumbers() {
    for (int i = 0; i < nOfFallingNumbers; ++i) {
        numY[i]++;
        if (numY[i] >= 24) {
        	
            // Reset if it reaches the bottom
            numY[i] = 0;
            numX[i] = rand() % gridX;
            numV[i] = rand() % 10;
        }
    }
}

// Purpose: Function to check for collisions between falling numbers and the robot
// Global variables accessed: nOfFallingNumbers, numX, numY, robotX
void checkCollision() {
    for (int i = 0; i < nOfFallingNumbers; ++i) {
    	
    	// Check if the falling numbers are in the robot's collision range
        if (numY[i] >= 20 && numY[i] <= 21 && numX[i] >= robotX && numX[i] <= robotX + 4) { 
        
            // Collision detected
            cout << "\a";                           // Beep sound
            if (numV[i] == 0)                       //Set score to 0 if falling num is 0
                score = 0;
            else
                score += numV[i];                // Increase score by the value of the number
            numY[i] = 0;                        // Reset the falling number
            numX[i] = rand() % gridX;
            numV[i] = rand() % 10;
        }
    }
}

// Purpose: Function to display the final score
// Global Variable accessed: score
void finalScore() {
	
    cout << "Your Score: " << score << endl;
}



// Purpose: Function to display information including score, timer, and game mode
// Input parameters: choice, elapsed_time
// Global variables accessed: score
void displayInfo(int choice, int elapsed_time) {
	
	
    // Display the score
    COORD scorePos = { 0, 0 };                                                                       // Coordinates for score display
    DWORD charsWritten;                                                                            // To store the number of char written
    FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', 15, scorePos, &charsWritten);  // Clear previous score display
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), scorePos);                           // Set cursor position to the score coordinates
    cout << "Score: " << score;                                                                   // Output the current score



    // Display the timer
    COORD timerPos = { 70, 0 };                                                                      // Coordinates for timer display
    FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', 10, timerPos, &charsWritten);  // Clear previous timer display
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), timerPos);                          // Set cursor position to the timer coordinates
    cout << "Timer: " << elapsed_time;                                                            // Output the elapsed time



    // Display game mode based on the chosen difficulty
    COORD gamemodePos = { 0, 1 };                                                                       // Coordinates for game mode display
    FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', 15, gamemodePos, &charsWritten);  // Clear previous game mode display
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), gamemodePos);                          // Set cursor position to the game mode coordinates
 
 
    // Switch statement to determine and display the game mode
    
    switch (choice) {
        case 1:
            cout << "Gamemode: Easy ";
            break;
        case 2:
            cout << "Gamemode: Medium ";
            break;
        case 3:
            cout << "Gamemode: Hard ";
            break;
    }
}





// Function to display the current score on the screen
//void displayScore() {
//    COORD scorePos = { 0, 0 };  // Adjust the coordinates to the desired position
//    DWORD charsWritten;
//    FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', 10, scorePos, &charsWritten);
//    // Display the score
//    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), scorePos);
//    cout << "Score: " << score;
//}






// Purpose: Function to display the game grid and the robot
// Global variables accessed: gridY, gridX, robotX, numX, numY, nOfFallingNumbers
void dGrid() {
    int grid[gridY][gridX];                                                           // Initialize the grid's array

                                                                                    // Adding color to the grid
    
    for (int i = 0; i < gridY; ++i) {
        for (int j = 0; j < gridX; ++j) {
            if (i < 20 && j < 80)                                                // Blue color for the top of the grid
                grid[i][j] = FOREGROUND_RED | BACKGROUND_BLUE;
            else if (i >= 20 && i < 24 && j < 80)                              // Green color for the middle of the grid
                grid[i][j] = FOREGROUND_GREEN | BACKGROUND_GREEN;
            else if (i >= 24 && j < 80)                                       // Yellow color for the bottom section of the grid
                grid[i][j] = FOREGROUND_RED | FOREGROUND_GREEN | BACKGROUND_RED | BACKGROUND_GREEN;
        }
    }

    // Update the buffer with the grid and the robot
    for (int i = 0; i < gridY; ++i) {
        for (int j = 0; j < gridX; ++j) {
        	
            // Robot positioning for display
            
            if (i >= 20 && i < 21 && j == robotX) {
                updateBuffer(j, i, '\\', BACKGROUND_GREEN);
            } 
			else if (i >= 20 && i < 21 && (j == robotX + 1 || j == robotX + 2 || j == robotX + 3)) {
                updateBuffer(j, i, '_', BACKGROUND_GREEN);
            } 
			else if (i == 20 && j == robotX + 4) {
                updateBuffer(j, i, '/', BACKGROUND_GREEN);
            } 
			else if (i == 21 && j == robotX + 2) {
                char a = 1;
                updateBuffer(j, i, a, BACKGROUND_GREEN);
            } 
			else if (i == 21 && j == robotX + 1) {
                updateBuffer(j, i, '\\', BACKGROUND_GREEN);
            } 
			else if (i == 21 && j == robotX + 3) {
                updateBuffer(j, i, '/', BACKGROUND_GREEN);
            } 
			else if (i == 22 && j == robotX + 2) {
                updateBuffer(j, i, '|', BACKGROUND_GREEN);
            }
			else if (i == 22 && (j == robotX + 1 || j == robotX + 3)) {
                updateBuffer(j, i, '_', BACKGROUND_GREEN);
            } 
			else if (i == 23 && (j == robotX + 1 || j == robotX + 3)) {
                updateBuffer(j, i, '|', BACKGROUND_GREEN);
            } 
			else {                                                       // Creating the rest of the 25x80 grid
                updateBuffer(j, i, ' ', grid[i][j]);                    // Adding colored spaces
            }
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);    // Reset text attribute
    }

                                                                       // Display the falling numbers in the buffer
    for (int i = 0; i < nOfFallingNumbers; ++i) {
        updateBuffer(numX[i], numY[i], '0' + numV[i], 7);             // Display falling numbers
    }

    // Display the buffer to the console
    displayBuffer();
}

// Purpose: Function to display the quit screen
// Global variables accessed: gridY, gridX
void quitScreen() {
    int grid[gridY][gridX];

    for (int i = 0; i < gridY; ++i) {
        for (int j = 0; j < gridX; ++j) {
            if (i < 20 && j < 80)                                                       // Blue color for the top of the grid
                grid[i][j] = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_BLUE;
            else if (i >= 20 && i < 24 && j < 80)                                      // Green color for the middle of the grid
                grid[i][j] = FOREGROUND_GREEN | BACKGROUND_GREEN;
            else if (i >= 24 && j < 80)                                               // Yellow color for the bottom section of the grid
                grid[i][j] = FOREGROUND_RED | FOREGROUND_GREEN | BACKGROUND_RED | BACKGROUND_GREEN;
        }
    }

    // Display the buffer with the quit screen information
    for (int i = 0; i < gridY; ++i) {
        for (int j = 0; j < gridX; ++j) {
            // Display the player's score at a specific position
            if (i == 13 && j == 40) {
                COORD coord = { i + 18, j - 28 };
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                cout << score;
            } 
			else {
                updateBuffer(j, i, ' ', grid[i][j]); // Display colored spaces if the above case is false
            }
        }
    }

    // Display the buffer to the console
    displayBuffer();      // buffer for the quit screen
    getch();             // Wait for user input
}




// main function

int main() {
	
	// Main Menu of the game with instructions
	
	cout << endl << "\t\t\t\t\t\t Number Catcher ^^" << endl;
	cout<<"Objective:\nCatch falling numbers with your robot to increase your score. Avoid missing numbers and 0s to maintain or increase your score!"<<endl<<endl;
	cout<<"Controls:\nMove your robot to the left: Use the 'A' key or the left arrow key.\nMove your robot to the right: Use the 'D' key or the right arrow key.\nPause/Play the game: Press the 'P' key.\nQuit the game: Press the 'ESC' key or wait for the game to automatically end after 180 seconds."<<endl<<endl;
    cout<<"Let's get started!"<<endl<<endl;
    
    
    string name;                                   // Declare a variable for user's name
    int choice;                                   // Variable for difficulty choice
    cout << "Enter your name : " << endl;
    cin >> name;                                // Get user input and read it
    
    
    // Asking user to input difficulty level 
    cout << name << ", select the difficulty level (1-3) : " << endl;
    cout << "1. Easy" << endl;
    cout << "2. Medium" << endl;
    cout << "3. Hard" << endl;
    cin >> choice;                           // Get user input for difficulty level



    int delay = 0;                          // Default delay for easy mode

    // Delay based on difficulty level
    
    switch (choice) {
        case 1:
            delay = 100;     // Adjust this value for your preferred delay
            break;
        case 2:
            delay = 75;     // Faster than easy mode
            break;
        case 3:
            delay = 50;   // Fastest
            break;
        default:
            cout << "Invalid difficulty level. Going for the default mode :D" << endl;
    }

    initializeFallingNumbers();
    system("cls");
    int elapsed_time = 0;                                                        // Setting elapsed time to 0

    auto start_time = std::chrono::steady_clock::now();

    while (true) {
        dGrid();                                                                // Display the game grid
        moveFallingNumbers();
        checkCollision();
        displayInfo(choice, elapsed_time);                                       // Display score, gamemode, and timer

         

        this_thread::sleep_for(chrono::milliseconds(delay));                     // Adding a delay for better visibility

        // Input handling for moving the robot, pausing and ending game
        
        if ((GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A')) && robotX > 0 )                 // A and left arrow key
            robotX -= 3;                                                                       // For boundary check
            
        else if ((GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D')) && robotX < gridX - 5) // D and right arrow key
            robotX += 3;                                                                    // For boundary check
            
        else if (GetAsyncKeyState('P') & 0x8000) {                                         // Check if 'P' key is pressed
        
            system("cls");
            displayInfo(choice, elapsed_time);
            isPaused = !isPaused;                                            // Toggle the pause state
            if (isPaused) {
            	
				// Display a message indicating the game is paused
                system("cls");
                displayInfo(choice, elapsed_time);                         // Display score, gamemode, and time
                displayBuffer();
                

                while (true) {
                    if (GetAsyncKeyState('P')) {
                        break;                                                  // Break the pause loop when 'P' is pressed again
                    }
                }
            }
        } else if (GetAsyncKeyState(VK_ESCAPE) || elapsed_time == 180){       // Escape key or game time limit
            system("cls");
            quitScreen();                                                   // Display the quit screen
            getch();                                                       // Wait for user input
            break;
        }
             auto current_time = std::chrono::steady_clock::now();        
             elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count();
    }


    finalScore();                                                  // Display the final score
    
    
    //While loop used to remove the default console ending screen
    while (true)
        getch(); 
    return 0;
}
