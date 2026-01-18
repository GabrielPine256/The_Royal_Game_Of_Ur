#ifndef TO_CURSES
#define TO_CURSES


#include<curses.h>

#include<iostream> 
#include<vector>
#include<string>


namespace To_Curses {
    using std::vector;
    using std::string;
    
/*
#ifdef _WIN32 // pdcurses 
const vector<string> color_name = {"black", "blue", "green", "cyan", "red", "magenta", "yellow", "white"};
#else         // ncurses 
const vector<string> color_name = {"black", "red", "green", "yellow", "blue", "magenta", "cyan", "white"};
#endif 
*/

// A single function to call to prepare curses. 
void start_curses() { 
    initscr();                // start curses
    if (!has_colors()) {
        endwin();
        std::cerr << "This terminal does not support color.\n";
        exit(1);              // terminate the program 
    }
    curs_set(0);              // Make cursor tranparent.
    noecho();                 // Don't display keyboard input. 
    cbreak();                 // "Disable line buffering." 
    intrflush(stdscr, FALSE); // Prepare to enable keypad input. 
    keypad(stdscr, TRUE);     //            Enable keypad input. 
    start_color();            // Enable colored text. 
    
    // init_pair(0, COLOR_WHITE, COLOR_BLACK); // Redundant declaration. 
    init_pair(1,  COLOR_RED,     COLOR_BLACK); 
    init_pair(2,  COLOR_BLUE,    COLOR_BLACK); 
    init_pair(3,  COLOR_RED,     COLOR_YELLOW); 
    init_pair(4,  COLOR_BLUE,    COLOR_YELLOW); 
    init_pair(5,  COLOR_YELLOW,  COLOR_BLACK); 
    /*
    #ifdef _WIN32 // pdcurses 
    // COLOR_PAIR(0) is white on black
    init_pair(1,  COLOR_BLUE,    COLOR_BLACK); 
    init_pair(2,  COLOR_GREEN,   COLOR_BLACK); 
    init_pair(3,  COLOR_CYAN,    COLOR_BLACK); 
    init_pair(4,  COLOR_RED,     COLOR_BLACK); 
    init_pair(5,  COLOR_MAGENTA, COLOR_BLACK); 
    init_pair(6,  COLOR_YELLOW,  COLOR_BLACK); 
    init_pair(7,  COLOR_WHITE,   COLOR_BLACK);
    init_pair(8,  COLOR_BLACK,   COLOR_WHITE); 
    init_pair(9,  COLOR_BLUE,    COLOR_WHITE); 
    init_pair(10, COLOR_GREEN,   COLOR_WHITE); 
    init_pair(11, COLOR_CYAN,    COLOR_WHITE); 
    init_pair(12, COLOR_RED,     COLOR_WHITE); 
    init_pair(13, COLOR_MAGENTA, COLOR_WHITE); 
    init_pair(14, COLOR_YELLOW,  COLOR_WHITE); 
    init_pair(15, COLOR_WHITE,   COLOR_YELLOW);
    #else         // ncurses 
    // COLOR_PAIR(0) is white on black
    init_pair(1,  COLOR_RED,     COLOR_BLACK); 
    init_pair(2,  COLOR_GREEN,   COLOR_BLACK); 
    init_pair(3,  COLOR_YELLOW,  COLOR_BLACK); 
    init_pair(4,  COLOR_BLUE,    COLOR_BLACK); 
    init_pair(5,  COLOR_MAGENTA, COLOR_BLACK); 
    init_pair(6,  COLOR_CYAN,    COLOR_BLACK); 
    init_pair(7,  COLOR_WHITE,   COLOR_BLACK);
    init_pair(8,  COLOR_BLACK,   COLOR_WHITE); 
    init_pair(9,  COLOR_RED,     COLOR_WHITE); 
    init_pair(10, COLOR_GREEN,   COLOR_WHITE); 
    init_pair(11, COLOR_YELLOW,  COLOR_WHITE); 
    init_pair(12, COLOR_BLUE,    COLOR_WHITE); 
    init_pair(13, COLOR_MAGENTA, COLOR_WHITE); 
    init_pair(14, COLOR_CYAN,    COLOR_WHITE); 
    init_pair(15, COLOR_WHITE,   COLOR_YELLOW);
    #endif
    */
} // void start_curses()


// A single function to call to close curses. 
void stop_curses() { 
    curs_set(1); // Return cursor to normal. 
    endwin();    // Exit ncurses. 
}


// This function looks cleaner than writing out the for-loop inline. 
void await_enter() { for (char c = '\0'; c != '\n'; c = getch()); } 


// Show the user non-interactive menu text. 
void static_menu(const string& text) {  
    
    stop_curses(); // Don't call static_menu() without having first called start_curses(). 
    std::cout << text; 
    for (char c = '\0'; c != '\n'; c = getchar()); 
    start_curses(); 
    
    /* 
    // Fails if text is larger than existing window. 
    clear();
    mvaddstr(0, 0, text.c_str()); 
    await_enter();
    clear();
    */
}


} // namespace To_Curses

#endif
