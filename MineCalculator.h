#pragma once
#include <iostream>
#include "Tiles.h"
#include <vector>
using namespace std;


//only tiles without mines will be inputed into function
int MineCalculator(vector<vector<Tile*>>& board, float i, float j) {
    int num_of_mines = 0;

    bool top = (i >= 1);
    bool bottom = (i < board.size() - 1);


    if (j >= 1) {
        //check if there is mine on left side
        BombTile* left_tile = dynamic_cast<BombTile*>(board[i][j - 1]);
        if (left_tile) {
            num_of_mines++;
        }

        //check for mine on top left
        if (top) {
            BombTile* top_left = dynamic_cast<BombTile*>(board[i - 1][j - 1]);
            if (top_left) {
                num_of_mines++;
            }
        }

        //check for mine on bottom left
        if (bottom) {
            BombTile* bottom_left = dynamic_cast<BombTile*>(board[i + 1][j - 1]);
            if (bottom_left) {
                num_of_mines++;
            }
        }
    }

    if (j < board[0].size() - 1) {
        //check if there is a mine on the right side
        BombTile* right_tile = dynamic_cast<BombTile*>(board[i][j + 1]);
        if (right_tile) {
            num_of_mines++;
        }

        //check if there is a mine on top right
        if (top) {
            BombTile* top_right = dynamic_cast<BombTile*>(board[i - 1][j + 1]);
            if (top_right) {
                num_of_mines++;
            }
        }

        //check if there is a mine on bottom right
        if (bottom) {
            BombTile* bottom_right = dynamic_cast<BombTile*>(board[i + 1][j + 1]);
            if (bottom_right) {
                num_of_mines++;
            }
        }
    }

    //check for mine on top
    if (top) {
        BombTile* upper = dynamic_cast<BombTile*>(board[i - 1][j]);
        if (upper) {
            num_of_mines++;
        }
    }

    //check for mine on bottom
    if (bottom) {
        BombTile* down = dynamic_cast<BombTile*>(board[i + 1][j]);
        if (down) {
            num_of_mines++;
        }
    }

    return num_of_mines;
}

bool winCondition(vector<vector<Tile*>>& board) {
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[0].size(); j++) {
            auto curr_tile = dynamic_cast<NumberTile*>(board[i][j]);
            if (curr_tile) {
                if (!curr_tile->getCurrDisplay()) {
                    return false;
                }
            }
        }
    }
    return true;
}


//use to check if tiles are working properly before making GUI
void Display(vector<vector<Tile*>>& board) {
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[0].size(); j++) {
            BombTile* bomb_tile = dynamic_cast<BombTile*>(board[i][j]);
            if (bomb_tile) {
                cout << -1 << " ";
            }
            else {
                NumberTile* num_tile = dynamic_cast<NumberTile*>(board[i][j]);
                cout << num_tile->getTileNumber() << " ";
            }
        }
        cout << endl;
    }
}
