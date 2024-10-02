#pragma once
#include "MineCalculator.h"
#include <iostream>
#include "Tiles.h"
#include <string>
#include <SFML/Graphics.hpp>
#include <random>
#include <cmath>
using namespace std;

vector<vector<Tile*>> initializeBoard(int rows, int columns) {
    vector<vector<Tile*>> board(rows, vector<Tile*>(columns));

    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[0].size(); j++) {
            Tile* new_tile = new NumberTile(j * 32.f, i * 32.f);
            board[i][j] = new_tile;
        }
    }
    return board;
}

void EmptyBoard(int columns, int rows, vector<vector<Tile*>>& board) {
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[0].size(); j++) {
            delete board[i][j];
        }
    }

    board = initializeBoard(board.size(), board[0].size());
}


//places mines throughout the board
void mineNumberInitializer(vector<vector<vector<bool>>>& board, int& num_of_mines, int hor, int vert, int& tiles_left, default_random_engine gen) {
    if (!(0 <= hor && hor < board[0].size()))
        return;
    if (!(0 <= vert && vert < board.size()))
        return;
    if (num_of_mines <= 0 || tiles_left <= 0)
        return;

    if (board[vert][hor][1])
        return;



    board[vert][hor][1] = true;
    uniform_int_distribution<int> distribution(1,tiles_left);
    tiles_left--;
    int random = distribution(gen);

    if (random <= num_of_mines) {
        board[vert][hor][0] = true;
        num_of_mines--;
    }




    mineNumberInitializer(board, num_of_mines, hor - 1, vert, tiles_left, gen);
    mineNumberInitializer(board, num_of_mines, hor, vert - 1, tiles_left, gen);
    mineNumberInitializer(board, num_of_mines, hor, vert + 1, tiles_left, gen);
    mineNumberInitializer(board, num_of_mines, hor + 1, vert, tiles_left, gen);
}


void BoardCreator(int num_of_mines, vector<vector<Tile*>>& board, sf::Vector2i& position, int& bombs_left) {
    int rows = board.size();
    int columns = board[0].size();

    //find index of tile that was clicked
    int horizontal = floor(position.x / 32.f);
    int vertical = floor(position.y / 32.f);

    //find which tiles already have red flags on them
    vector<vector<int>> tiles_with_flags;
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[0].size(); j++) {
            if (board[i][j]->getRedFlag()) {
                tiles_with_flags.push_back({i, j});
            }
        }
    }

    //three dimensional vector carrying values of tiles and if they were visited or not
    vector<vector<vector<bool>>> mine_or_num(rows, vector<vector<bool>>(columns, {false, false}));
    int total = columns * rows;

    //places mines throughout board; first tile that is clicked already has been visited and should have no mines nearby
    mine_or_num[vertical][horizontal][1] = true;

    bool top = (vertical - 1 >= 0);
    bool bottom = (vertical + 1 < board.size());
    bool left = (horizontal - 1 >= 0);
    bool right = (horizontal + 1 < board[0].size());
    int counter = 1;
    if (top) {
        mine_or_num[vertical-1][horizontal][1] = true;
        counter++;
        if (left) {
            mine_or_num[vertical-1][horizontal-1][1] = true;
            counter++;
        }
        if (right) {
            mine_or_num[vertical-1][horizontal+1][1] = true;
            counter++;
        }
    }

    if (bottom) {
        mine_or_num[vertical+1][horizontal][1] = true;
        counter++;
        if (left) {
            mine_or_num[vertical+1][horizontal-1][1] = true;
            counter++;
        }
        if (right) {
            mine_or_num[vertical+1][horizontal+1][1] = true;
            counter++;
        }
    }

    if (left) {
        mine_or_num[vertical][horizontal-1][1] = true;
        counter++;
    }
    if (right) {
        mine_or_num[vertical][horizontal+1][1] = true;
        counter++;
    }



    int tiles_left = total - counter;
    default_random_engine generator(random_device{}());

    if (horizontal - 2 >= 0) {
        mineNumberInitializer(mine_or_num, num_of_mines, horizontal - 2, vertical, tiles_left, generator);
    }
    else if (horizontal + 2 < board[0].size()) {
        mineNumberInitializer(mine_or_num, num_of_mines, horizontal + 2, vertical, tiles_left, generator);
    }
    else if (vertical - 2 >= 0) {
        mineNumberInitializer(mine_or_num, num_of_mines, horizontal, vertical - 2, tiles_left, generator);
    }
    else if (vertical + 2 < board.size()) {
        mineNumberInitializer(mine_or_num, num_of_mines, horizontal, vertical + 2, tiles_left, generator);
    }




    //create actual board now
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            float x_position = j * 32.f;
            float y_position = i * 32.f;
            if (mine_or_num[i][j][0]) {
                auto mine_ptr = new BombTile(x_position, y_position);
                delete board[i][j];
                board[i][j] = mine_ptr;
            }
            else {
                auto number_ptr = new NumberTile(x_position, y_position);
                delete board[i][j];
                board[i][j] = number_ptr;
            }
        }
    }

    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[0].size(); j++) {
            //check if tile is not a mine
            NumberTile* num_tile = dynamic_cast<NumberTile*>(board[i][j]);
            if (num_tile) {
                int mines_nearby = MineCalculator(board, i, j);
                board[i][j]->setTileNumber(mines_nearby);
            }
        }
    }

    for (int i = 0; i < tiles_with_flags.size(); i++) {
        board[tiles_with_flags[i][0]][tiles_with_flags[i][1]]->rightClick(bombs_left);
    }
}