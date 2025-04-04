#include <iostream>
#include "clui.h"
using namespace std;

struct User {
    char username[30];
    int sudoku[9][9];
    int original[9][9];
    int savedSudoku[9][9];
    int score;
    int timeLeft;
};

void addUser(User users[], int &userCount, const char* newUsername);
bool usernameExists(User users[], int userCount, const char* username, int &index);
void entry();
void menu(User &currentUser, User users[], int userCount);
void copySudoku(int source[9][9], int dest[9][9]);
void zeroRandomCells(int sudoku[9][9]);
void printSudoku(int sudoku[9][9]);
bool checkWin(int sudoku[9][9], int original[9][9]);
bool timer(User &user);
void mode_menu(User &player);
void sudoku(User &player);
void load_game(User &user);
void leader_board(User users[], int userCount);



void addUser(User users[], int &userCount, const char* newUsername) {
    for (int i = 0; i < 30; i++) {
        users[userCount].username[i] = newUsername[i];
        if (newUsername[i] == '\0') {
            break;
        }
    }
    
    users[userCount].score = 0;
    userCount++;
}

bool usernameExists(User users[], int userCount, const char* username, int &index) {
    for (int i = 0; i < userCount; i++) {
        bool isEqual = true;
        for (int j = 0; j < 30; j++) {
            if (users[i].username[j] != username[j]) {
                isEqual = false;
                break; 
            }
            if (users[i].username[j] == '\0' && username[j] == '\0') {
                break;
            }
        }
        if (isEqual) {
            index = i;
            return true;
        }
    }
    return false;
}

void entry() {
    User users[30];
    int userCount = 0;
    int currentUserIndex = -1;

    while (true) {
        cout << "Welcome to Sudoku Game!" << endl;
        cout << "1. New user" << endl;
        cout << "2. Already has an account" << endl;
        cout << "3. Exit" << endl;
        cout << "Please enter a number from 1 to 3: ";

        int command;
        cin >> command;

        char username[30];

        switch (command) {
            case 1:
                clear_screen();
                cout << "Enter your username: ";
                cin.getline(username, 30);

                if (usernameExists(users, userCount, username, currentUserIndex)) {
                    cout << "Username " << username << " already exists. Please try again." << endl;
                } else {
                    addUser(users, userCount, username);
                    currentUserIndex = userCount - 1;
                    cout << "User " << username << " registered successfully!" << endl;
                    clear_screen();
                    menu(users[currentUserIndex], users, userCount);
                }
                break;

            case 2:
                clear_screen();
                cout << "Enter your username: ";
                cin.getline(username, 30);

                if (usernameExists(users, userCount, username, currentUserIndex)) {
                    cout << "Welcome back, " << username << "!" << endl;
                    menu(users[currentUserIndex], users, userCount);
                    clear_screen();
                } else {
                    cout << "Username " << username << " not found. Please try again." << endl;
                }
                break;

            case 3:
                return;

            default:
                cout << "Invalid choice. Please enter a number between 1 and 3." << endl;
        }
    }
}

void menu(User &currentUser, User users[], int userCount){
    cout << "You logged in as " << currentUser.username << endl;
    cout << "1. New Game" << endl;
    cout << "2. Load Game" << endl;
    cout << "3. Leader Board" << endl;
    cout << "4. Exit" << endl;
    cout << "please enter a number from 1 to 4: " << endl;
    int command;
    while(true){
    cin >> command;
switch (command)
{
case 1:
    clear_screen();
    mode_menu(currentUser);
    break;
case 2:
    clear_screen();
    load_game(currentUser);
    break;
case 3:
    clear_screen(); 
    leader_board(users, userCount);;
    break;
case 4:
    entry();
    break;
default:
    break;
}
}
}

void copySudoku(int source[9][9], int dest[9][9]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            dest[i][j] = source[i][j];
        }
    }
}

void zeroRandomCells(int sudoku[9][9]) {
    srand(time(0));
    for (int k = 0; k < 20; k++) {
        int i = rand() % 9;
        int j = rand() % 9;
        sudoku[i][j] = 0;
    }
}

void printSudoku(int sudoku[9][9]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            cout << sudoku[i][j] << " ";
        }
        cout << endl;
    }
}

bool checkWin(int sudoku[9][9], int original[9][9]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (sudoku[i][j] != original[i][j]) {
                return false;
            }
        }
    }
    return true;
}

bool timer(User &user) {
    for (int i = user.timeLeft - 1; i >= 0; i--) {
        for (int j = 59; j >= 0; j--) {
            if (checkWin(user.sudoku, user.original)) {
                cout << "You win!" << endl;
                user.score += 10;
                return true;
            }
            if (i == 0 && j == 0) {
                cout << "Time is up. You lose!" << endl;
                return false;
            }
            delay(1000);
            cout << i << ":" << j << "  ";
            if (is_keyboard_hit()) {
                char key = getch();
                if (key == 27) {
                    cout << "Paused. Press ESC to resume..." << endl;
                    while (getch() != 27);
                } else if (key == 's') {
                    copySudoku(user.sudoku, user.savedSudoku);
                    cout << "Game saved!" << endl;
                }
            }
        }
    }
    return false;
}


void mode_menu(User &player) {
    cout << "1. Easy" << endl;
    cout << "2. Normal" << endl;
    cout << "3. Hard" << endl;
    cout << "Please enter a number from 1 to 3: ";
    int command;
    cin >> command;

    switch (command) {
        case 1:
            player.timeLeft = 7;
            break;
        case 2:
            player.timeLeft = 5;
            break;
        case 3:
            player.timeLeft = 2;
            break;
        default:
            cout << "Invalid choice!" << endl;
            return;
    }

    clear_screen();
    sudoku(player);
}

void sudoku(User &player) {
    int defaultBoard[9][9] = {
        {5, 8, 3, 6, 9, 4, 7, 2, 1},
        {7, 1, 6, 8, 3, 2, 5, 4, 9},
        {2, 9, 4, 1, 7, 5, 3, 8, 6},
        {6, 7, 1, 5, 2, 8, 4, 9, 3},
        {8, 2, 9, 7, 4, 3, 1, 6, 5},
        {4, 3, 5, 9, 1, 6, 8, 7, 2},
        {1, 5, 8, 2, 6, 7, 9, 3, 4},
        {3, 6, 7, 4, 5, 9, 2, 1, 8},
        {9, 4, 2, 3, 8, 1, 6, 5, 7}
    };

    copySudoku(defaultBoard, player.sudoku);
    copySudoku(defaultBoard, player.original);
    zeroRandomCells(player.sudoku);
    printSudoku(player.sudoku);
    timer(player);
}

void load_game(User &user) {
    if (user.savedSudoku[0][0] == 0) {
        cout << "No saved game found!" << endl;
        return;
    }
    copySudoku(user.savedSudoku, user.sudoku);
    cout << "Game loaded!" << endl;
    printSudoku(user.sudoku);
    timer(user);
}
 
void leader_board(User users[], int userCount) {
    for (int i = 0; i < userCount - 1; i++) {
        for (int j = 0; j < userCount - i - 1; j++) {
            if (users[j].score < users[j + 1].score) {
                User temp = users[j];
                users[j] = users[j + 1];
                users[j + 1] = temp;
            }
        }
    }

    cout << "Leaderboard:\n";
    cout << "Rank | Username | Score\n";
    for (int i = 0; i < userCount; i++) {
        cout << i + 1 << "    | " << users[i].username << " | " << users[i].score << endl;
    }
    cout << "----------------------------\n";
}


int main() {
    entry();
    return 0;
}