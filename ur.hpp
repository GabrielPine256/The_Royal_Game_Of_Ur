#ifndef THE_ROYAL_GAME_OF_UR
#define THE_ROYAL_GAME_OF_UR

// The Royal Game Of Ur
// https://www.youtube.com/watch?v=WZskjLq040I


#include"to_curses.hpp" 

#include<vector> 
#include<array>
#include<chrono>    // srand(time(0)); 
#include<algorithm> // upper_bound 

/*#include<iostream>    //debug
#include<string>      //*/

namespace Ur { 
    using namespace To_Curses; 
    using std::pair; 
    using std::make_pair; 
    using std::array; 
    using std::find; 
    using std::count; 
    /*using std::string;    //debug
    using std::to_string;*/ //debug


int roll(const bool color = 0) { 
    attron(COLOR_PAIR(color+1));
    mvaddstr(18, 0, "  +   +   +   =  "); 
    int result = 0; 
    for (int i = 0; i < 4; ++i) { 
        const int d = rand() % 2; 
        mvaddch(18, 4*i, char(d + '0')); // int('0') is 48. 
        result += d; 
    }   
    mvaddch(18, 16, char(result + '0')); 
    attroff(COLOR_PAIR(color+1)); 
    return result; 
}
/*
// Prank version
int roll(const bool color = 0) { 
    attron(COLOR_PAIR(color+1));
    mvaddstr(18, 0, "  +   +   +   =  "); 
    int result = 0; 
    for (int i = 0; i < 4; ++i) { 
        int d = bool(rand() % 4); 
        if (color) d = !d; 
        mvaddch(18, 4*i, char(d + '0')); // int('0') is 48. 
        result += d; 
    }   
    mvaddch(18, 16, char(result + '0')); 
    attroff(COLOR_PAIR(color+1)); 
    return result; 
}
*/

void draw(const int index, const vector<int>& indexes, const bool highlight = false) { 
    
    const bool colored = count(indexes.begin(), indexes.end(), index); 
    int color = index / 16; 
    const int place = index % 16; 
    
    // Selecting the appropriate symbol. 
    constexpr array<char,16> index_to_sym = {' ', '^', '^', '^', '*', 'v', 'v', 'v', '*', 'v', 'v', 'v', ' ', '^', '*', ' '}; 
    char sym = index_to_sym[place]; 
    if (place == 0) {                                             // +  48;
        if (color) { sym = count(indexes.begin(), indexes.end(), 16) + '0'; } 
        else       { sym = count(indexes.begin(), indexes.end(),  0) + '0'; } 
    } else if (place == 15) {
        if (color) { sym = count(indexes.begin(), indexes.end(), 31) + '0'; } 
        else       { sym = count(indexes.begin(), indexes.end(), 15) + '0'; } 
    } else if (place == 12) { 
        if (colored) { sym = color ? '>' : '<';   }
        else         { sym = '-'; }
    }
    
    // Selecting the appropriate coordinate. 
    constexpr array<pair<int,int>, 16> index_to_coord = {
        make_pair(8, 0), make_pair(6, 0), make_pair(4, 0), make_pair(2, 0),
        make_pair(0, 0), make_pair(0, 2), make_pair(2, 2), make_pair(4, 2),
        make_pair(6, 2), make_pair(8, 2), make_pair(10,2), make_pair(12,2),
        make_pair(14,2), make_pair(14,0), make_pair(12,0), make_pair(10,0)}; 
    int x = index_to_coord[place].second;
    if (color && x == 0) { x = 4; }  
    
    // Meshing color from red-blue switch to COLOR_PAIR. 
    if (highlight) { color += 2; } 
    if (!colored && place%16 != 0 && place%16 != 15)  { color = -1; } 
    
    mvaddch(index_to_coord[place].first, x, sym | COLOR_PAIR(color+1)); 
    
    /*mvaddstr(21, 0, "\n");                                //debug
    for (int i = 0; i < 14; ++i) 
        mvaddstr(21, i*3, to_string(indexes[i]).c_str());   */
}

void set_board(const vector<int>& indexes = {0,0,0,0,0,0,0,16,16,16,16,16,16,16}) { for (int i = 0; i < 32; ++i) { draw(i, indexes); } } 


constexpr array<int,3> rosettes = {4,8,14};
 
// Returns legitimate moves (pieces that can move by the rolled value). 
vector<int> get_choices(const int roll, const bool color, const vector<int>& indexes) { 
    if (!roll) { return {}; } // Technically redundant, but more durable. 
    
    vector<int> result = {}; 
    
    // Can only use one piece at start position. 
    // +1 to rejustify the "vec.last() downwards" search to "vec.end() downwards". 
    int start = indexes.size() - (find(indexes.rbegin(), indexes.rend(), (color ? 16 : 0)) - indexes.rbegin()+1); 
    if (start == -1) { start = (color ? 7 : 0); }
    
    for (int i = start; i < (color ? 14 : 7); ++i) { 
        const int prospect = indexes[i]+roll; 
        if (indexes[i]/16 != prospect/16 || indexes[i]%16 == 15) 
            continue; // Can't run off of board.
        if (count(indexes.begin(), indexes.end(), prospect) && prospect%16 != 15) 
            continue; // Can't step on your own piece. 
        if (prospect%16 == 8 && count(indexes.begin(), indexes.end(), (color ? 8 : 24) )) 
            continue; // Can't take other's rosette. 
        result.push_back(i);  
    } 
    
    return result; 
}

int choose(const vector<int>& choices, const vector<int>& indexes) { 
    if (choices.empty()) { return -1; } 
    
    int i = choices.size()-1; // Start from latest available position. 
    
    draw(indexes[choices[i]], indexes, true); 
    for (int c = getch(); c != '\n'; c = getch()) { 
        draw(indexes[choices[i]], indexes, false); 
        if      (c == KEY_RIGHT) { i = (i + 1                 ) % choices.size(); }
        else if (c == KEY_LEFT)  { i = (i - 1 + choices.size()) % choices.size(); }    
        draw(indexes[choices[i]], indexes, true); 
    } 
    draw(indexes[choices[i]], indexes, false); 
    
    return choices[i]; 
} 


// Will resolve one misplaced element, given its index. 
template <typename T> 
int restore_sort(const int i, vector<T>& vec) { 
    const T mis = vec[i]; 
    vec.erase(i + vec.begin()); 
    const auto iter = upper_bound(vec.begin(), vec.end(), mis);
    vec.insert(iter, mis);
    return iter - vec.begin(); 
}


void make_move(int choice, const int roll, vector<int>& indexes) { 
    if (choice == -1) {
        await_enter(); // Let player see they have no moves. 
        return;
    }  
    
    // If a piece will be taken, re-zero it. Overlap only occurs 5 to 12. 
    if (5 <= (indexes[choice]+roll)%16 && (indexes[choice]+roll)%16 <= 12) { 
        const int take = find(indexes.begin(), indexes.end(), indexes[choice]+roll + ((indexes[choice])/16 ? -16 : 16)) - indexes.begin(); 
        if (take != indexes.size()) { 
            indexes[take] = (indexes[take]/16 ? 16 : 0); 
            draw(indexes[take], indexes); // Update start position counter. 
            restore_sort(take, indexes); 
        } 
    } 
    
    indexes[choice] += roll; 
    choice = restore_sort(choice, indexes); 
    draw(indexes[choice] - roll, indexes); // Reset previous position. 
    draw(indexes[choice], indexes); 
} 


class Game {
public: 
    
    void play(); 

private: 
    
    void step(const bool color); 
    
    vector<int> indexes = {0,0,0,0,0,0,0,16,16,16,16,16,16,16}; 
    
}; // class Game 


void Game::step(const bool color) { 
    const int rolled = roll(color);
    const int choice = choose(get_choices(rolled, color, indexes), indexes);  
    make_move(choice, rolled, indexes);  
    if (count(rosettes.begin(), rosettes.end(), indexes[choice]%16)) { step(color); } 
}

                                                                             
/*
 ,-.                  ,-.        .  .     
/                    /   \  ,-   |  |     
| -. ,-: ;-.-. ,-.   |   |  |    |  | ;-. 
\  | | | | | | |-'   \   /  |-   |  | |   
 `-' `-` ' ' ' `-'    `-'   |    `--` '   
                           -'             
*/
const string tutorial_text = {
 " ,-.                  ,-.        .  .                                         \
\n/                    /   \\  ,-   |  |                                        \
\n| -. ,-: ;-.-. ,-.   |   |  |    |  | ;-.                                     \
\n\\  | | | | | | |-'   \\   /  |-   |  | |                                     \
\n `-' `-` ' ' ' `-'    `-'   |    `--` '                                       \
\n                           -'                                                 \
\n                                                                              \
\nA long time ago, this game was as common as chess. You could go anywhere and  \
\npeople would know how to play it.                                             \
\n                                                                              \
\nEach turn, you roll 4 dice with 4 sides. 2 sides each would be marked, making \
\na 50/50 chance of a die coming up marked. You count the marked dice to get the\
\namount of steps you'll move a piece this turn.                                \
\n                                                                              \
\nEach player has 7 pieces to get across the board. The first to all 7 wins.    \
\nYou can reset an opponent piece by landing on it. If you land on a * then you \
\nget another placement. A piece on a * cannot be taken. An exact roll is needed\
\nto get a piece onto the final position.                                       \
\n                                                                              \
\nUse left and right arrow keys and ENTER.                                      \
\n                                                                              \
\nFor a full playthrough:  https://www.youtube.com/watch?v=WZskjLq040I "};


void Game::play() { 

    srand(time(0));     
    
    start_curses();
    static_menu(tutorial_text);  
    set_board(); 
    
    for (int turn = 0; count(indexes.begin(), indexes.end(), 15) < 7 && count(indexes.begin(), indexes.end(), 31) < 7; ++turn)  
        step(turn % 2); 
    
    attron(COLOR_PAIR(5));
    mvaddstr(18, 0, "\n");
    mvaddstr(18, 0, "Game Finished"); 
    attroff(COLOR_PAIR(5));
    
    await_enter(); 
    await_enter();
    
    stop_curses(); 
}

    
} // namespace Ur

#endif
