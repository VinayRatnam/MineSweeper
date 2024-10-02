#pragma once
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
using namespace std;


class Tile {
    sf::Vector2f position;
    bool curr_display = false;
    bool red_flag = false;
    bool functionality = true; //only turns false when game is paused

public:
    Tile(float x, float y) {
        position.x = x;
        position.y = y;

    }

    virtual void leftClick() {
        curr_display = true;
    }

    virtual void rightClick(int& remaining) = 0;

    bool getCurrDisplay() {
        return curr_display;
    }

    void setCurrDisplay(bool t) {
        curr_display = t;
    }

    virtual void setTileNumber(int num) = 0;


    float getXPosition() {
        return position.x;
    }

    float getYPosition() {
        return position.y;
    }

    bool getRedFlag() {
        return red_flag;
    }

    void setRedFlag(bool t) {
        red_flag = t;
    }

    virtual ~Tile() = default;

};


class NumberTile : public Tile {
    int tile_number;

public:
    NumberTile(float x, float y) : Tile(x, y) {}

    void setTileNumber(int num) {
        tile_number = num;
    }

    int getTileNumber() {
        return tile_number;
    }


    void leftClick() {
        if (!getRedFlag()) {
            setCurrDisplay(true);
        }
    }

    void rightClick(int& remaining) {
        if (!getCurrDisplay()) {
            setRedFlag(!getRedFlag());
            if (getRedFlag()) {
                remaining--;
            }
            else {
                remaining++;
            }
        }
    }

    ~NumberTile() = default;
};


class BombTile : public Tile {

public:
    BombTile(float x, float y) : Tile(x, y) {}

    void setTileNumber(int num) {
        cout << "Do not use" << endl;
    }

    void leftClick() {
        setCurrDisplay(true);
    }

    void rightClick(int &remaining) {
        if (!getCurrDisplay()) {
            setRedFlag(!getRedFlag());
            if (getRedFlag()) {
                remaining--;
            }
            else {
                remaining++;
            }
        }
    }

    ~BombTile() = default;
};


//checks if number tiles nearby have a value of 0 and reveals them if so
//ONLY INPUT NUMBER TILES WITH NO MINES NEAR IT TO THIS FUNCTION
void NoneNearby(vector<vector<Tile*>>& board, int i, int j, vector<vector<bool>>& visited) {
    //Bounds checking
    if (i < 0 || i >= board.size() || j < 0 || j >= board[0].size()) return;
    if (visited[i][j]) return;  // Check if already visited

    //Attempt to cast and check for zero tiles
    NumberTile* curr_tile = dynamic_cast<NumberTile*>(board[i][j]);
    if (!curr_tile) return;
    else {
        curr_tile->leftClick();
        if (curr_tile->getTileNumber() != 0) {
            return;
        }

    }


    visited[i][j] = true;  // Mark this tile as visited

    //Recursive calls for all 8 adjacent tiles
    NoneNearby(board, i - 1, j, visited);  // Up
    NoneNearby(board, i + 1, j, visited);  // Down
    NoneNearby(board, i, j - 1, visited);  // Left
    NoneNearby(board, i, j + 1, visited);  // Right
    NoneNearby(board, i - 1, j - 1, visited);  // Upper Left
    NoneNearby(board, i - 1, j + 1, visited);  // Upper Right
    NoneNearby(board, i + 1, j - 1, visited);  // Lower Left
    NoneNearby(board, i + 1, j + 1, visited);  // Lower Right
}