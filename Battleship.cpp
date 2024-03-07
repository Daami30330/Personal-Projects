#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

// Constants and globals
const char OCEAN = 'O';
const char FIRE = 'X';
const char HIT = '*';
const int SIZE = 10;
const vector<int> SHIPS = {5, 4, 3, 3, 2};

// Globals
int orientation = -1; // Stores the hit ship orientation. Determined on second hit
vector<int> total_hits; // Stores the ship number every time AI hits a ship while ship is afloat
int miss = 1; // Stores whether last AI shot was a miss

// Player variables
int player_alive = 17; // -1 every time a ship is hit
vector<vector<char>> player_radar(SIZE, vector<char>(SIZE, OCEAN));
vector<vector<char>> player_board(SIZE, vector<char>(SIZE, OCEAN));

// AI variables
int ai_alive = 17;
vector<vector<char>> ai_radar(SIZE, vector<char>(SIZE, OCEAN));
vector<vector<char>> ai_board(SIZE, vector<char>(SIZE, OCEAN));
vector<vector<int>> number_board(SIZE, vector<int>(SIZE, -1)); // Added number_board
vector<int> ship_position; // Stores the first hit of ships which will be eliminated [row, col]
vector<int> ship_length; // Stores the length of ship on first hit

// Function prototypes
void print_board();
int random_row(bool is_vertical, int size);
int random_col(bool is_vertical, int size);
bool is_ocean(int row, int col, vector<vector<char>>& b, vector<vector<int>>& number_board); // Updated function prototype
vector<vector<char>> make_ship(int size, vector<vector<char>>& board, int set_ship = -1);
int ship_number(int r, int c, vector<vector<int>>& number_board);
bool ship_sunk();
void ai_turn();

// Function definitions
void print_board() {
    cout << "  0 1 2 3 4 5 6 7 8 9 || 0 1 2 3 4 5 6 7 8 9" << endl;
    for (int i = 0; i < SIZE; ++i) {
        cout << i << " ";
        for (int j = 0; j < SIZE; ++j) {
            cout << player_radar[i][j] << " ";
        }
        cout << "|| ";
        for (int j = 0; j < SIZE; ++j) {
            cout << player_board[i][j] << " ";
        }
        cout << endl;
    }
}

int random_row(bool is_vertical, int size) {
    if (is_vertical) {
        return rand() % (SIZE - size + 1);
    } else {
        return rand() % SIZE;
    }
}

int random_col(bool is_vertical, int size) {
    if (is_vertical) {
        return rand() % SIZE;
    } else {
        return rand() % (SIZE - size + 1) + size - 1;
    }
}

bool is_ocean(int row, int col, vector<vector<char>>& b, vector<vector<int>>& number_board) { // Updated function definition
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) {
        return false;
    }
    return b[row][col] == OCEAN && number_board[row][col] == -1;
}

vector<vector<char>> make_ship(int size, vector<vector<char>>& board, int set_ship) {
    bool is_vertical = rand() % 2 == 0; // Vertical ship if true
    bool occupied = true;
    int ship_row, ship_col; // Moved declaration outside the loop
    while (occupied) {
        occupied = false;
        ship_row = random_row(is_vertical, size); // Corrected variable name
        ship_col = random_col(is_vertical, size); // Corrected variable name
        if (is_vertical) {
            for (int p = 0; p < size; ++p) {
                if (!is_ocean(ship_row + p, ship_col, board, number_board)) {
                    occupied = true;
                    break;
                }
            }
        } else {
            for (int p = 0; p < size; ++p) {
                if (!is_ocean(ship_row, ship_col - p, board, number_board)) {
                    occupied = true;
                    break;
                }
            }
        }
    }
    if (is_vertical) {
        board[ship_row][ship_col] = '^';
        board[ship_row + size - 1][ship_col] = 'v';
        if (set_ship != -1) {
            number_board[ship_row][ship_col] = set_ship; // Set ship number on number board if set_ship
            number_board[ship_row + size - 1][ship_col] = set_ship; // Set ship number on number board if set_ship
        }
        for (int p = 1; p < size - 1; ++p) {
            board[ship_row + p][ship_col] = '+';
            if (set_ship != -1) {
                number_board[ship_row + p][ship_col] = set_ship; // Set ship number on number board if set_ship
            }
        }
    } else {
        board[ship_row][ship_col] = '>';
        board[ship_row][ship_col - size + 1] = '<';
        if (set_ship != -1) {
            number_board[ship_row][ship_col] = set_ship; // Set ship number on number board if set_ship
            number_board[ship_row][ship_col - size + 1] = set_ship; // Set ship number on number board if set_ship
        }
        for (int p = 1; p < size - 1; ++p) {
            board[ship_row][ship_col - p] = '+';
            if (set_ship != -1) {
                number_board[ship_row][ship_col - p] = set_ship; // Set ship number on number board if set_ship
            }
        }
    }
    return board;
}

int ship_number(int r, int c, vector<vector<int>>& number_board) {
    if (is_ocean(r, c, player_board, number_board)) {
        return -1;
    }
    return SHIPS[number_board[r][c]];
}

bool ship_sunk() {
    if (count(total_hits.begin(), total_hits.end(), total_hits[0]) == ship_length[0]) {
        return true;
    }
    return false;
}

// AI turn function
void ai_turn() {
    int ai_guess_row, ai_guess_col;
    if (ship_length.empty()) { // No current targets
        ai_guess_row = rand() % SIZE;
        ai_guess_col = rand() % SIZE;
        while (!is_ocean(ai_guess_row, ai_guess_col, ai_radar, number_board)) {
            ai_guess_row = rand() % SIZE;
            ai_guess_col = rand() % SIZE;
        }
        if (ai_board[ai_guess_row][ai_guess_col] != OCEAN) { // AI hit
            miss = 0;
            player_alive--;
            ship_length.push_back(ship_number(ai_guess_row, ai_guess_col, number_board));
            ship_position.push_back(ai_guess_row);
            ship_position.push_back(ai_guess_col);
            orientation = -1;
            ai_board[ai_guess_row][ai_guess_col] = HIT;
            ai_radar[ai_guess_row][ai_guess_col] = HIT;
            total_hits.push_back(number_board[ai_guess_row][ai_guess_col]);
            cout << "Attenton Admiral! You have been hit!" << endl;
        } else {
            miss = 1;
            ai_board[ai_guess_row][ai_guess_col] = FIRE;
            ai_radar[ai_guess_row][ai_guess_col] = FIRE;
            cout << "Good news! They've missed!" << endl;
        }
    } else { // Find next spot to shoot on ship
        if (orientation == -1) { // Test for orientation of hit ship
            if (is_ocean(ship_position[0] + 1, ship_position[1], ai_radar, number_board)) {
                ai_guess_row = ship_position[0] + 1;
                ai_guess_col = ship_position[1];
            } else if (is_ocean(ship_position[0] - 1, ship_position[1], ai_radar, number_board)) {
                ai_guess_row = ship_position[0] - 1;
                ai_guess_col = ship_position[1];
            } else if (is_ocean(ship_position[0], ship_position[1] - 1, ai_radar, number_board)) {
                ai_guess_row = ship_position[0];
                ai_guess_col = ship_position[1] - 1;
            } else {
                ai_guess_row = ship_position[0];
                ai_guess_col = ship_position[1] + 1;
            }
        } else if (orientation) { // Shoot at vertical ship
            if (is_ocean(ai_guess_row + 1, ai_guess_col, ai_radar, number_board) && !miss) {
                ai_guess_row++;
            } else {
                ai_guess_row--;
                while (!is_ocean(ai_guess_row, ai_guess_col, ai_radar, number_board)) {
                    ai_guess_row--;
                }
            }
        } else { // Shoot at horizontal ship
            if (is_ocean(ai_guess_row, ai_guess_col - 1, ai_radar, number_board) && !miss) {
                ai_guess_col--;
            } else {
                ai_guess_col++;
                while (!is_ocean(ai_guess_row, ai_guess_col, ai_radar, number_board)) {
                    ai_guess_col++;
                }
            }
        }
        if (ai_board[ai_guess_row][ai_guess_col] != OCEAN) {
            player_alive--;
            ai_board[ai_guess_row][ai_guess_col] = HIT;
            ai_radar[ai_guess_row][ai_guess_col] = HIT;
            total_hits.push_back(number_board[ai_guess_row][ai_guess_col]);
            if (count(total_hits.begin(), total_hits.end(), total_hits[0]) == 2 &&
                ship_number(ai_guess_row, ai_guess_col, number_board) ==
                    ship_number(ship_position[0], ship_position[1], number_board)) {
                if (ai_guess_col != ship_position[1]) {
                    orientation = 0;
                } else {
                    orientation = 1;
                }
            } else if (total_hits[0] != number_board[ai_guess_row][ai_guess_col]) {
                ship_length.push_back(ship_number(ai_guess_row, ai_guess_col, number_board));
                ship_position.push_back(ai_guess_row);
                ship_position.push_back(ai_guess_col);
            }
            if (player_alive) {
                miss = 0;
                cout << "Attenton Admiral! You have been hit!" << endl;
            } else {
                cout << "I'm sorry sir, but we're going down" << endl;
                print_board();
                return;
            }
        } else { // AI missed
            miss = 1;
            ai_board[ai_guess_row][ai_guess_col] = FIRE;
            ai_radar[ai_guess_row][ai_guess_col] = FIRE;
            cout << "Good news! They've missed!" << endl;
        }
        if (ship_sunk()) { // Reset variables
            orientation = -1;
            ship_position.erase(ship_position.begin(), ship_position.begin() + 2);
            ship_length.erase(ship_length.begin());
            int t = total_hits[0];
            total_hits.erase(remove(total_hits.begin(), total_hits.end(), t), total_hits.end());
            if (!ship_length.empty()) {
                miss = 0;
            } else {
                miss = 1;
            }
        }
    }
}

// Main function
int main() {
    srand(time(0)); // Seed random number generator

    // Make the boards
    for (int x = 0; x < SHIPS.size(); ++x) {
        make_ship(SHIPS[x], player_board, x);
        make_ship(SHIPS[x], ai_board);
    }

    cout << "Let's play Battleship!" << endl;
    print_board();

    // Game loop
    while (player_alive && ai_alive) {
        // Player turn
        int guess_row, guess_col;
        do {
            cout << "Guess Row:";
            cin >> guess_row;
            cout << "Guess Col:";
            cin >> guess_col;
        } while (!is_ocean(guess_row, guess_col, player_radar, number_board));
        
        if (ai_board[guess_row][guess_col] != OCEAN) {
            ai_alive--;
            if (ai_alive) {
                cout << "Admiral, we've hit the enemy ship!" << endl;
                player_radar[guess_row][guess_col] = HIT;
            } else {
                player_radar[guess_row][guess_col] = HIT;
                cout << "Congratulations! You sunk my battleship!" << endl;
                break;
            }
        } else {
            cout << "Admiral, we've missed the enemy battleship!" << endl;
            player_radar[guess_row][guess_col] = FIRE;
        }

        // AI turn
        ai_turn(); // Call the AI turn function
        print_board();
    }

    return 0;
}
