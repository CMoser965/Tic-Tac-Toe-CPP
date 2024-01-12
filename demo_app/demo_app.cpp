// demo_app.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <map>
#include <tuple>
#include <ctype.h>
#include <cstdlib>
using namespace std;

int BOARD_SIZE = 3;
char** BOARD_STATE = new char* [BOARD_SIZE];
bool GAME_IN_PROGRESS = true;
int ASCII_ONE = 48;
map<int, tuple<int, int>> movement_system = {
    {1, {0, 0}},
    {2, {0, 1}},
    {3, {0, 2}},
    {4, {1, 0}},
    {5, {1, 1}},
    {6, {1, 2}},
    {7, {2, 0}},
    {8, {2, 1}},
    {9, {2, 2}},
};
bool PLAYER_WON;

char getBoardObject(int position) {
    int i = get<0>(movement_system[position]);
    int j = get<1>(movement_system[position]);
    return BOARD_STATE[i][j];
}

void checkWinCondition() {
    int winningMoves[8][3] = {
        {1, 2, 3}, 
        {1, 4, 7}, 
        {1, 5, 9},  
        {2, 5, 8}, 
        {3, 5, 7},  
        {3, 6, 9}, 
        {4, 5, 6}, 
        {7, 8, 9}
    };
    char potentialWinner = NULL;
    int spaceCount = 0;
    for (int i = 0; i < 8; i++) {
        spaceCount = 0;
        for (int j = 0; j < 3; j++) {
            char winPos = getBoardObject(winningMoves[i][j]);
            if ((winPos == 'X' or winPos == 'O') && potentialWinner == NULL) {
                potentialWinner = winPos;
            }
            if (winPos == potentialWinner) {
                if (++spaceCount == 3) {
                    GAME_IN_PROGRESS = false;
                    if (potentialWinner == 'X') PLAYER_WON = true;
                    else PLAYER_WON = false;
                }
            } else {
                spaceCount = 0;
                potentialWinner = NULL;
                continue;
            }
        }
    }
}

void printGameScreen(char** board) {
    int i, j;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            cout << board[i][j] << ' ';
        }
        cout << endl;
    }
}

void initBoardState(char** board) {
    int i, j, curr;
    curr = ASCII_ONE;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            curr++; 
            board[get<0>(movement_system[curr - ASCII_ONE])][get<1>(movement_system[curr - ASCII_ONE])] = curr;
        }
    }
}

void allocateBoard() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        BOARD_STATE[i] = new char[BOARD_SIZE];
    }
}

void placeMarker(int position, char marker_symbol = 'X') {
    int i = get<0>(movement_system[position]);
    int j = get<1>(movement_system[position]);
    BOARD_STATE[i][j] = marker_symbol;
}

void aiMove() {
    int move = rand() + 1 % 10;
    cout << "AI move: " << move << endl;
    while(!isdigit(getBoardObject(move))) {
        move = rand() % 10;
        cout << "AI move: " << move << endl;
    }
    placeMarker(move, 'O');
}

int main()
{
    allocateBoard();
    initBoardState(BOARD_STATE);
    printGameScreen(BOARD_STATE);
    int move;
    while (GAME_IN_PROGRESS) {
        cout << "Please choose a marker location:" << endl;
        cin >> move;
        cout << endl;
        if (move < 1 || move > 9 || !isdigit(getBoardObject(move)) ) {
            cout << "Invalid move. Please select a valid move" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        placeMarker(move);
        printGameScreen(BOARD_STATE);
        checkWinCondition();
        if (!GAME_IN_PROGRESS) break;
        aiMove();
        printGameScreen(BOARD_STATE);
        checkWinCondition();
    }
    cout << "Game over!" << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Would you like to play again? (Y/N)" << endl;
    char newGame;
    cin >> newGame;
    if (newGame == 'Y' || newGame == 'y') {
        GAME_IN_PROGRESS = true;
        PLAYER_WON = NULL;
        main();
    }
    return 0;
}